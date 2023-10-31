#include "Cell_LUT.h"

// Constructor & Destructor
Cell_LUT::Cell_LUT(string Cell_Name){
    this->Cell_Name = Cell_Name;
}

Cell_LUT::~Cell_LUT(){
    ;
}

// Getter
double Cell_LUT::Get_Pin_Cap(const string &Pin_Name){
    assert(Pin_Cap.find(Pin_Name) != Pin_Cap.end() && "Pin name not found");
    return Pin_Cap[Pin_Name];
}

const vector<double> &Cell_LUT::Get_Look_Up_Table(const string &Table_Name){
    assert(Look_Up_Tables.find(Table_Name) != Look_Up_Tables.end() && "Table name not found");
    return Look_Up_Tables[Table_Name];
}

// cout Cell_LUT
ostream &operator<<(ostream &out, const Cell_LUT &cell_lut){
    out << "Cell Name: "<< cell_lut.Cell_Name << endl;
    out << "\tPin Cap:" << endl;
    for(const auto &entry : cell_lut.Pin_Cap) {
        out << "\t\tPin Name: " << entry.first << ", Capacitance: " << entry.second << endl;
    }
    out << "\tLUT: " << endl;
    for (const auto &entry : cell_lut.Look_Up_Tables) {
        out << "\t\tTable Name: " << entry.first << endl << "\t\t";
        for(size_t i = 0; i < entry.second.size(); i++){
            out << entry.second[i] << " ";
            if((i + 1) % 7 == 0) out << endl << "\t\t";
        }
        out << endl;
    }
    return out;
}