#include "Cell.h"

// Constructor & Destructor
Cell::Cell(string Name, Cell_Type Type){
    this->Name = Name;
    this->Type = Type;
    this->Output_Loading = 0;
}

Cell::~Cell(){
    ;
}