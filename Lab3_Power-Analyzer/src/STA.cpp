#include "STA.h"

// Constructor & Destructor
STA::STA():
    Design_Name(""),
    Num_Cells(0),
    Num_Nets(0)
{
    this->library = new Library();
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
void STA::Parse_Library(const char *library_filename){
    this->library_filename = library_filename;
    ifstream library_file(library_filename);
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

void STA::Parse_Netlist(const char *netlist_filename){
    // Find the Design Name
    this->netlist_filename = netlist_filename;
    string filename(netlist_filename);
    size_t Start_Index = filename.find_last_of("/");
    size_t End_Index = filename.find_last_of(".");
    Design_Name = filename.substr(Start_Index + 1, End_Index - Start_Index - 1);
    ifstream netlist_file(netlist_filename);
    
    // concat all of the verilog code together
    string Verilog_Code, line;
    while(getline(netlist_file, line)){
        Verilog_Code += line;
    }
    netlist_file.close();
    size_t found = 0;
    while((found = Verilog_Code.find(';', found + 1)) != std::string::npos){
        Verilog_Code.insert(found + 1, "\n");
        found += 2; // Move past the inserted '\n' to avoid infinite loop
    }
    
    //Start to parse the Netlist
    Cell *cell;
    Net *net;
    Cell_Type cell_type;
    Net_Type net_type;
    smatch match;
    stringstream ss(Verilog_Code);
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
                    Nets[pin_connect_net_name]->Input_Cells = make_pair(pin_name, cell);
                    cell->Output_Net = Nets[pin_connect_net_name];
                    if(cell->Output_Net->Type == output){
                        cell->Output_Loading = PRIMARY_OUTPUT_LOADING;
                        Primary_Output_Cells.emplace_back(cell);
                    }
                }
                else if(pin_name == "A1" || pin_name == "A2" || pin_name == "I"){
                    Nets[pin_connect_net_name]->Output_Cells.emplace_back(make_pair(pin_name, cell));
                    cell->Input_Nets.emplace_back(Nets[pin_connect_net_name]);
                }
                else{
                    abort();
                }
            }
        }
    }
    Num_Cells = Cells.size();
    Num_Nets = Nets.size();
}

void STA::Print_Library(){
    cout << *library << endl;
}

void STA::Print_Netlist(){
    cout << "Design Name: " << Design_Name << endl;
    cout << "Num Cells: " << Num_Cells << endl;
    cout << "Num Nets: " << Num_Nets << endl;
    cout << "Cells: " << endl;
    for (const auto &cell : Cells) {
        cout << *(cell.second) << endl;
    }
    cout << "Nets: " << endl;
    for (const auto &net : Nets){
        cout << *(net.second) << endl;
    }
}

// Step 1
void STA::Calculate_Output_Loading(){
    for(const auto &pair1 : Cells){
        Cell *cell = pair1.second;
        Net *Output_Net = cell->Output_Net;
        double Output_Loading = cell->Output_Loading;
        for(const auto & pair2: Output_Net->Output_Cells){
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
    }
}

void STA::Dump_Output_Loading(){
    ofstream fout(string(STUDENT_ID) + "_" + Design_Name + "_load.txt");
    auto cell_cmp = [](const Cell *c1, const Cell *c2){
        return c1->Instance_Number < c2->Instance_Number;
    };
    vector<Cell *> Sorted_Cells;
    for(const auto &cell : Cells){
        Sorted_Cells.emplace_back(cell.second);
    }
    sort(Sorted_Cells.begin(), Sorted_Cells.end(), cell_cmp);
    for(const auto &cell: Sorted_Cells){
        fout << cell->Name << " " << fixed << setprecision(6) << cell->Output_Loading << endl;
    }
    fout.close();
}