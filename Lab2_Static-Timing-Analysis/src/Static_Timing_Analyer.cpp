#include "Static_Timing_Analyer.h"

// Constructor & Destructor
Static_Timing_Analyer::Static_Timing_Analyer(){
    Lib = new Library();
}

Static_Timing_Analyer::~Static_Timing_Analyer(){
    delete Lib;
    for(const auto &pair : Cells) {
        delete pair.second;
    }
    for(const auto &pair : Nets){
        delete pair.second;
    }
}

// Parser (Netlist & Library)
void Static_Timing_Analyer::Library_Parser(ifstream &fin){
    string Cell_Name;
    string Pin_Name;
    string Table_Name;

    string line;
    bool For_Index1 = true;
    while(getline(fin, line)){
        // Remove the comment and the space in the string
        line = regex_replace(line, Comment_Space_Pattern, "");

        // Continue for only "}" or " " in the line
        if(line.length() <= 1) continue;

        // Extract the index1 & index2 infomation
        if(regex_search(line, Index_Head_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            smatch match;
            while(regex_search(Extract_Values, match, Float_Pattern)){
                if(For_Index1){
                    Lib->index_1.emplace_back(stod(match.str()));
                }
                else{
                    Lib->index_2.emplace_back(stod(match.str()));
                }
                Extract_Values = match.suffix().str();
            }
            For_Index1 = false;
        }

        // Extract the cell info
        else if(regex_search(line, Cell_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Cell_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            Lib->LUT[Cell_Name] = new Look_Up_Table(Cell_Name);
        }

        else if(regex_search(line, Pin_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Pin_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
        }

        else if(regex_search(line, Cap_Pattern)){
            size_t Start_Index = line.find(":");
            size_t End_Index = line.rfind(";");
            double Capacitance = stod(line.substr(Start_Index + 1, End_Index - Start_Index - 1));
            Lib->LUT[Cell_Name]->Pin_Cap[Pin_Name] = Capacitance;
        }

        // Extract the 2D LUT (input slew vs output loading)
        else if(regex_search(line, Table_Name_Pattern)){
            size_t End_Index = line.rfind("(");
            Table_Name = line.substr(0, End_Index);
        }
        else if(regex_search(line, Table_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            smatch match;
            while(regex_search(Extract_Values, match, Float_Pattern)){
                double Delay = stod(match.str());
                Lib->LUT[Cell_Name]->Table[Table_Name].emplace_back(Delay);
                Extract_Values = match.suffix().str();
            }
        }
    }
    fin.close();
}

void Static_Timing_Analyer::Netlist_Parser(ifstream &fin){
    Cell *cell;
    Net *net;
    Cell_Type cell_type;
    Net_Type net_type;
    smatch match;

    string line;
    while(getline(fin, line)){
        // Remove comment and the multiple spaces
        line = regex_replace(line, Comment_Front_Space_Pattern, "");
        line = regex_replace(line, Multi_Space_Pattern, " ");

        // Continue for empty line
        if(line.length() <= 1) continue;

        // Extract the net info (Net name, Net type)
        if(regex_search(line, match, Net_Pattern)){
            // Find the Net type
            string net_type_s = match.str();
            if(net_type_s == "input") net_type = input;
            else if(net_type_s == "output") net_type = output;
            else if(net_type_s == "wire") net_type = wire;
            line = match.suffix().str();

            // Extract the Net name that belongs to this net type
            while(regex_search(line, match, Net_Name_Pattern)){
                string net_name = match.str();
                net = new Net(net_name, net_type);
                Nets[net_name] = net;
                line = match.suffix().str();
            }
        }

        // Read Gate-Level netlist
        if(regex_search(line, match, Cell_Type_Pattern)){
            // Extract Cell Type
            string cell_type_s = match.str();
            if(cell_type_s == "NOR2X1") cell_type = NOR2X1;
            else if(cell_type_s == "INVX1") cell_type = INVX1;
            else if(cell_type_s == "NANDX1") cell_type = NANDX1;
            line = match.suffix().str();

            // Extract Cell Name
            string cell_name;
            regex_search(line, match, Pin_Name_Pattern);
            cell_name = match.str();
            line = match.suffix().str();
            cell = new Cell(cell_name, cell_type);
            Cells[cell_name] = cell;

            // Extract the pin connection
            while(regex_search(line, match, Pin_Name_Pattern)){
                string pin_name = match.str();
                line = match.suffix().str();
                regex_search(line, match, Pin_Name_Pattern);
                string pin_connect_net_name = match.str();
                line = match.suffix().str();
                if(pin_name == "ZN"){
                    Nets[pin_connect_net_name]->Cell_In[pin_name] = cell;
                    cell->Output = Nets[pin_connect_net_name];
                    if(cell->Output->Type == output) cell->Output_Loading = 0.03;
                }
                else if(pin_name == "A1" || pin_name == "A2" || pin_name == "I"){
                    Nets[pin_connect_net_name]->Cell_Out[pin_name] = cell;
                    cell->Input.emplace_back(Nets[pin_connect_net_name]);
                }
            }
        }
    }   
    fin.close();
}

void Static_Timing_Analyer::Calculate_Output_Loading(){
    for(const auto &pair : Cells) {
        Cell *cell = pair.second;
        Net *Output_Net = cell->Output;
        double Output_Loading = cell->Output_Loading;
        for(const auto &cell_out : Output_Net->Cell_Out){
            string pin_name = cell_out.first;
            string cell_type_s;
            Cell_Type type = cell_out.second->Type;
            if(type == NANDX1) cell_type_s = "NANDX1";
            else if(type == INVX1) cell_type_s = "INVX1";
            else if(type == NOR2X1) cell_type_s = "NOR2X1";
            Output_Loading += Lib->LUT[cell_type_s]->Pin_Cap[pin_name];
        }
        cell->Output_Loading = Output_Loading;
        cout << cell->Cell_Name << " " << Output_Loading << endl;
    }
}