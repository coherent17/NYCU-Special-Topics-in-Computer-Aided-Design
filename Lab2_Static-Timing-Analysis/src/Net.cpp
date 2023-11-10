#include "Net.h"

// Constructor & Destructor
Net::Net(string Name, Net_Type Type):
    Name(Name),
    Type(Type)
{
    ;
}

Net::~Net(){
    ;
}

// Friend function to output net info
ostream &operator<<(ostream &out, const Net &net) {
    out << "Net Name: " << net.Name << endl;
    out << "\tNet Type: ";
    if(net.Type == input) cout << "input" << endl;
    else if(net.Type == output) cout << "output" << endl;
    else if(net.Type == wire) cout << "wire" << endl;
    else abort();

    if (net.Input_Cells.second) {
        out << "\tInput Cells: " << net.Input_Cells.second->Name << "->" << net.Input_Cells.first << endl;
    } else {
        out << "\tInput Cells: None" << endl;
    }
    out << "\tOutput Cells: ";
    for (const auto &connection : net.Output_Cells) {
        out << connection.second->Name << "->" << connection.first << " ";
    }
    if(net.Output_Cells.size() == 0){
        out << "None";
    }
    out << endl;
    return out;
}