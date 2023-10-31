#include "STA.h"

// Constructor & Destructor
STA::STA(){
    library = new Library();
}

STA::~STA(){
    delete library;
    for(const auto &cell: Cells){
        delete cell.second;
    }
    for(const auto &net : Nets){
        delete net.second;
    }
}

// Parser
void STA::Parse_Library(ifstream &library_file){
    string Cell_Name, Pin_Name, Table_Name, line;
    smatch match;
    while(getline(library_file, line)){
        // Remove the comment and the space in the string
        line = regex_replace(line, Comment_Space_Pattern, "");
        
        // Continue for only "}" or " " in the line
        if(line.length() <= 1) continue;

        // Extract index1 & index2 info
        if(regex_search(line, Index_1_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                library->Index_1.emplace_back(stod(match.str()));
                Extract_Values = match.suffix().str();
            }
        }
        else if(regex_search(line, Index_2_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                library->Index_2.emplace_back(stod(match.str()));
                Extract_Values = match.suffix().str();
            }
        }

        // Extract Cell info
        else if(regex_search(line, Cell_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Cell_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            library->Cell_LUT_Map[Cell_Name] = new Cell_LUT(Cell_Name);
        }

        else if(regex_search(line, Pin_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Pin_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
        }

        // Extract Pin Cap
        else if(regex_search(line, Cap_Pattern)){
            size_t Start_Index = line.find(":");
            size_t End_Index = line.rfind(";");
            double Capacitance = stod(line.substr(Start_Index + 1, End_Index - Start_Index - 1));
            library->Cell_LUT_Map[Cell_Name]->Pin_Cap[Pin_Name] = Capacitance;
        }

        // Extract the 2-Dim LUT
        else if(regex_search(line, Table_Name_Pattern)){
            size_t End_Index = line.rfind("(");
            Table_Name = line.substr(0, End_Index);
        }

        else if(regex_search(line, Table_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                double Delay = stod(match.str());
                library->Cell_LUT_Map[Cell_Name]->Look_Up_Tables[Table_Name].emplace_back(Delay);
                Extract_Values = match.suffix().str();
            }
        }
    }
    library_file.close();
}

void STA::Parse_Netlist(ifstream &netlist_file){
    // Remove comment in verilog code
    string Verilog_Code, line;
    while(getline(netlist_file, line)){
        Verilog_Code += line + "\n";
    }
    netlist_file.close();
    string Clean_Verilog_Code = regex_replace(Verilog_Code, Verilog_Comment_Pattern, "");

    // Add proper new line to the clean verilog code
    size_t found = 0;
    // Find and insert '\n' after each ';'
    while((found = Clean_Verilog_Code.find(';', found + 1)) != std::string::npos){
        Clean_Verilog_Code.insert(found + 1, "\n");
        found += 2; // Move past the inserted '\n' to avoid infinite loop
    }

    // Start to parse the Netlist
    Cell *cell;
    Net *net;
    Cell_Type cell_type;
    Net_Type net_type;
    smatch match;
    stringstream ss(Clean_Verilog_Code);
    while(getline(ss, line)){
        // Remove the extra space
        line = regex_replace(line, Multi_Space_Pattern, " ");

        // Skip empty line
        if(line.length() < 1) continue;

        // Extract the net info
        if(regex_search(line, match, Net_Type_Pattern)){
            // Find the net type
            if(match.str() == "input") net_type = input;
            else if(match.str() == "output") net_type = output;
            else if(match.str() == "wire") net_type = wire;
            else abort();
            line = match.suffix().str();

            // Extract the net name that belongs to this net type
            while(regex_search(line, match, Word_Pattern)){
                string net_name = match.str();
                net = new Net(net_name, net_type);
                Nets[net_name] = net;
                line = match.suffix().str();
            }
        }

        // Extract the cell info
        else if(regex_search(line, match, Cell_Type_Pattern)){
            if(match.str() == "NOR2X1") cell_type = NOR2X1;
            else if(match.str() == "NANDX1") cell_type = NANDX1;
            else if(match.str() == "INVX1") cell_type = INVX1;
            else abort();
            line = match.suffix().str();

            // Extract the cell name
            regex_search(line, match, Word_Pattern);
            string cell_name = match.str();
            cell = new Cell(cell_name, cell_type);
            Cells[cell_name] = cell;
            line = match.suffix().str();

            // Extract the pin connection
            while(regex_search(line, match, Word_Pattern)){
                string pin_name = match.str();
                line = match.suffix().str();
                regex_search(line, match, Word_Pattern);
                string pin_connect_net_name = match.str();
                line = match.suffix().str();
                if(pin_name == "ZN"){
                    Nets[pin_connect_net_name]->Input_Cell_Connections[pin_name] = cell;
                    cell->Output_Net = Nets[pin_connect_net_name];
                    // The loading of primary output of the design is 0.03
                    if(cell->Output_Net->Type == output) cell->Output_Loading = 0.03;
                }
                else if(pin_name == "A1" || pin_name == "A2" || pin_name == "I"){
                    Nets[pin_connect_net_name]->Output_Cell_Connections[pin_name] = cell;
                    cell->Input_Nets.emplace_back(Nets[pin_connect_net_name]);
                }
                else{
                    abort();
                }
            }
        }
    }
}

// Step 1
void STA::Calculate_Output_Loading(){
    for(const auto &pair1 : Cells){
        Cell *cell = pair1.second;
        Net *Output_Net = cell->Output_Net;
        double Output_Loading = cell->Output_Loading;
        for(const auto & pair2: Output_Net->Output_Cell_Connections){
            string pin_name = pair2.first;
            Cell_Type type = pair2.second->Type;
            string cell_type_str;
            if(type == NANDX1)  cell_type_str = "NANDX1";
            else if(type == INVX1) cell_type_str = "INVX1";
            else if(type == NOR2X1) cell_type_str = "NOR2X1";
            else abort();
            Output_Loading += library->Cell_LUT_Map[cell_type_str]->Pin_Cap[pin_name];
        }
        cell->Output_Loading = Output_Loading;
        cout << cell->Name << " " << Output_Loading << endl;
    }
}