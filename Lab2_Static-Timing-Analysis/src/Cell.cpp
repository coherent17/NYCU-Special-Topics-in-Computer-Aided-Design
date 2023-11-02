#include "Cell.h"

// Constructor & Destructor
Cell::Cell(string Name, Cell_Type Type){
    this->Name = Name;
    this->Type = Type;
    this->Output_Loading = 0;
    smatch match;
    regex_search(Name, match, Number_Pattern);
    this->Instance_Number = stoi(match.str());
    this->In_Degree = 0;
    this->Input_Transition_Time = UNSET;
    this->Output_Transition_Time = UNSET;
}

Cell::~Cell(){
    ;
}

ostream &operator<<(ostream &out, const Cell &cell) {
    out << "Cell Name: " << cell.Name << endl;
    out << "\tInstance Number: " << cell.Instance_Number << endl;
    out << "\tCell Type: ";
    if(cell.Type == NOR2X1) cout << "NOR2X1" << endl;
    else if(cell.Type == INVX1) cout << "INVX1" << endl;
    else if(cell.Type == NANDX1) cout << "NANDX1" << endl;
    else abort();
    out << "\tInput Nets: ";
    for (auto net : cell.Input_Nets) {
        out << net->Name << " ";
    }
    out << endl;
    if (cell.Output_Net) {
        out << "\tOutput Net: " << cell.Output_Net->Name << endl;
    } else {
        out << "\tOutput Net: None" << endl;
    }
    out << "\tOutput Loading: " << fixed << setprecision(6) << cell.Output_Loading << endl;
    return out;
}