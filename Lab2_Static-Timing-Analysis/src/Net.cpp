#include "Net.h"

// Constructor & Destructor
Net::Net(string Name, Net_Type Type){
    this->Name = Name;
    this->Type = Type;
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

    if (net.Input_Cell_Connections.second) {
        out << "\tInput Cell Connection: " << net.Input_Cell_Connections.second->Name << "->" << net.Input_Cell_Connections.first << endl;
    } else {
        out << "\tInput Cell Connection: None" << endl;
    }
    out << "\tOutput Cell Connections: ";
    for (const auto &connection : net.Output_Cell_Connections) {
        out << connection.second->Name << "->" << connection.first << " ";
    }
    out << endl;
    return out;
}