#include "Library.h"

// Constructor & Destructor
Look_Up_Table::Look_Up_Table(string Cell_Name):Cell_Name(Cell_Name){
    ;
}

Look_Up_Table::~Look_Up_Table(){
    ;
}

// Constructor & Destructor
Library::Library(){
    ;
}

Library::~Library(){
    for(const auto &lut : LUT){
        delete lut.second;
    }
    LUT.clear();
}

ostream &operator<<(ostream &out, Look_Up_Table &Lut){
    out << "Cell: "<< Lut.Cell_Name << endl;
    out << "\tPin Cap:" << endl;
    for(const auto &pin_cap : Lut.Pin_Cap){
        out << "\t\t" << pin_cap.first << ": " << pin_cap.second << endl;
    }
    out << "\tRise Power:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["rise_power"].size(); i++){
        out << Lut.Table["rise_power"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;

    out << "\tFall Power:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["fall_power"].size(); i++){
        out << Lut.Table["fall_power"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;

    out << "\tCell Rise:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["cell_rise"].size(); i++){
        out << Lut.Table["cell_rise"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;

    out << "\tCell Fall:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["cell_fall"].size(); i++){
        out << Lut.Table["cell_fall"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;

    out << "\tRise Transition:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["rise_transition"].size(); i++){
        out << Lut.Table["rise_transition"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;

    out << "\tFall Transition:" << endl << "\t\t";
    for(size_t i = 0; i < Lut.Table["fall_transition"].size(); i++){
        out << Lut.Table["fall_transition"][i] << " ";
        if((i + 1) % 7 == 0) out << endl << "\t\t";
    }
    out << endl;
    return out;
}

ostream &operator<<(ostream &out, const Library &Lib){
    // Output index1 & index2
    out << "index_1(Output Cap):" << endl << "\t";
    for(size_t i = 0; i < Lib.index_1.size(); i++){
        out << Lib.index_1[i] << " ";
    }
    out << endl;
    out << "index_2(Input Slew):" << endl << "\t";
    for(size_t i = 0; i < Lib.index_2.size(); i++){
        out << Lib.index_2[i] << " ";
    }
    out << endl;

    // Output Pin Cap & LUT
    out << *(Lib.LUT.at("NOR2X1"));
    out << *(Lib.LUT.at("INVX1"));
    out << *(Lib.LUT.at("NANDX1"));

    return out;
}