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

// Parser
void Library::Parser(ifstream &fin){
    
}