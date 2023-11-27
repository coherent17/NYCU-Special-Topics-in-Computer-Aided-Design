#include "Cell.h"

// Constructor & Destructor
Cell::Cell(string Name, Cell_Type Type):
    Name(Name),
    Type(Type),
    In_Degree(0),
    Output_Loading(0),
    Input_Transition_Time(0),
    Output_Transition_Time(0),
    Propagation_Delay(0),
    Arrival_Time(0)
{
    smatch match;
    regex_search(Name, match, Number_Pattern);
    this->Instance_Number = stoi(match.str());
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
    out << "\tInput Transition Time: " << fixed << setprecision(6) << cell.Input_Transition_Time << endl;
    out << "\tOutput Transition Time: " << fixed << setprecision(6) << cell.Output_Transition_Time << endl;
    out << "\tArrival Time: " << fixed << setprecision(6) << cell.Arrival_Time << endl;
    out << "\tPropagation Delay: " << fixed << setprecision(6) << cell.Propagation_Delay << endl;
    return out;
}