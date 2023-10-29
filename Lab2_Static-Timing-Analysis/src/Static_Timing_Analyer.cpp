#include "Static_Timing_Analyer.h"

// Constructor & Destructor
Static_Timing_Analyer::Static_Timing_Analyer(){
    Lib = new Library();
}

Static_Timing_Analyer::~Static_Timing_Analyer(){
    delete Lib;
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

    string line;
    while(getline(fin, line)){
        // Remove comment and the multiple spaces
        line = regex_replace(line, Comment_Front_Space_Pattern, "");
        line = regex_replace(line, Multi_Space_Pattern, " ");

        // Continue for empty line
        if(line.length() <= 1) continue;


        if(regex_search(line, Net_Input_Pattern)){
            smatch match;
            while(regex_search(line, match, Net_Name_Pattern)){
                if(match.str() != "input"){
                    cell = new Cell(match.str(), Primary_Input);
                    Primary_Input_Cells[match.str()] = cell;
                    net = new Net(match.str(), input);
                    Nets[match.str()] = net;
                }
                line = match.suffix().str();
            }
        }

        else if(regex_search(line, Net_Output_Pattern)){
            smatch match;
            while(regex_search(line, match, Net_Name_Pattern)){
                if(match.str() != "output"){
                    cell = new Cell(match.str(), Primary_Output);
                    Primary_Output_Cells[match.str()] = cell;
                    net = new Net(match.str(), output);
                    Nets[match.str()] = net;
                }
                line = match.suffix().str();
            }
        }

        else if(regex_search(line, Net_Wire_Pattern)){
            smatch match;
            while(regex_search(line, match, Net_Name_Pattern)){
                if(match.str() != "wire"){
                    net = new Net(match.str(), wire);
                    Nets[match.str()] = net;
                }
                line = match.suffix().str();
            }
        }

        // Read gate connection
        else if(regex_search(line, Cell_Type_Pattern)){
            cout << line << endl;
            smatch match;
            Cell_Type type;
            regex_search(line, match, Cell_Type_Pattern);
            if(match.str() == "NOR2X1" || match.str() == "INVX1" || match.str() == "NANDX1"){
                if(match.str() == "NOR2X1") type = NOR2X1;
                else if(match.str() == "INVX1") type = INVX1;
                else if(match.str() == "NANDX1") type = NANDX1;
            }

            string Cell_Name = "";
            while(regex_search(line, match, Pin_Name_Pattern)){
                if(match.str() == "NOR2X1" || match.str() == "INVX1" || match.str() == "NANDX1"){
                    line = match.suffix().str();
                    continue;
                }
                else if(Cell_Name == ""){
                    Cell_Name = match.str();
                    cell = new Cell(Cell_Name, type);
                    Cells[Cell_Name] = cell;
                }
                // Pin connection
                else{
                    string Pin_Name = match.str();
                    if(Pin_Name == "ZN"){
                        line = match.suffix().str();
                        regex_search(line, match, Pin_Name_Pattern);
                        string Net_Name = match.str();
                        cell->Output = Nets[Net_Name];
                    }
                    else if(Pin_Name == "A1" || Pin_Name == "A2" || Pin_Name == "I"){
                        line = match.suffix().str();
                        regex_search(line, match, Pin_Name_Pattern);
                        string Net_Name = match.str();
                        cell->Input.emplace_back(Nets[Net_Name]);
                    }

                }
                line = match.suffix().str();
            }
        }
    }
    fin.close();
}