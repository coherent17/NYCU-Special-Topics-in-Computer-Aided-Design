#include "Library.h"

// Constructor & Destructor
Library::Library(){
    ;
}

Library::~Library(){
    for(const auto &cell_lut : Cell_LUT_Map){
        delete cell_lut.second;
    }
}

// Getter
const vector<double> &Library::Get_Cell_Table(const string &Cell_Name, const string &Table_Name){
    assert(Cell_LUT_Map.find(Cell_Name) != Cell_LUT_Map.end() && "Cell name not found");
    assert(Cell_LUT_Map[Cell_Name]->Look_Up_Tables.find(Table_Name) != Cell_LUT_Map[Cell_Name]->Look_Up_Tables.end() && "Table name not found");
    return Cell_LUT_Map[Cell_Name]->Look_Up_Tables[Table_Name];
}

// cout Library
ostream &operator<<(ostream &out, const Library &library){
    // Output index1 & index2
    out << "index_1(Output Cap):" << endl << "\t";
    for(size_t i = 0; i < library.Index_1.size(); i++){
        out << library.Index_1[i] << " ";
    }
    out << endl;
    out << "index_2(Input Slew):" << endl << "\t";
    for(size_t i = 0; i < library.Index_2.size(); i++){
        out << library.Index_2[i] << " ";
    }
    out << endl;

    // Output Cell_LUTs
    out << "Cell LUTs:" << endl;
    for (const auto &cell_lut_entry : library.Cell_LUT_Map) {
        out << *(cell_lut_entry.second) << endl;
    }
    return out;
    
}