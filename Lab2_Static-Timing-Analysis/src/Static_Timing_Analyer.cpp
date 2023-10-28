#include "Static_Timing_Analyer.h"

// Constructor & Destructor
Static_Timing_Analyer::Static_Timing_Analyer(){
    Lib = new Library();
}

Static_Timing_Analyer::~Static_Timing_Analyer(){
    ;
}

// Parser (Netlist & Library)
void Static_Timing_Analyer::Library_Parser(ifstream &fin){
    
    // The regular expression used to extract the pattern in the text
    regex Comment_Space_Pattern("(\\/\\/.*)|(\\/\\*.*\\*\\/)|(\\s+)");
    regex Index_Head_Pattern("index\\w*");
    regex Float_Pattern(R"(-?\d+(\.\d+)?)");
    regex Cell_Pattern("^cell\\(");
    regex Pin_Pattern("^pin\\(");
    regex Cap_Pattern("^capacitance");
    regex Table_Name("(^rise)")
    regex Table_Pattern("(^values)|(^\")");

    string Cell_Name;
    string Pin_Name;
    string Table_Name;

    string line;
    bool For_Index1 = true;
    while(getline(fin, line)){
        // Remove the comment and the space in the string
        line = regex_replace(line, Comment_Space_Pattern, "");

        // Continue for only "}" in the line
        if(line.length() == 1) continue;

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
            double capacitance = stod(line.substr(Start_Index + 1, End_Index - Start_Index - 1));
            Lib->LUT[Cell_Name]->Pin_Cap[Pin_Name] = capacitance;
            //cout << Cell_Name << " " << Pin_Name << " " << capacitance << endl;
        }
        else{
            cout << line << endl;
        }
    }


    fin.close();
}

void Static_Timing_Analyer::Netlist_Parser(ifstream &fin){
    


    fin.close();
}