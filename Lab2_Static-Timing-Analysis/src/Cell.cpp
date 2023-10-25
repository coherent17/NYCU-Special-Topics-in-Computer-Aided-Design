#include "Cell.h"

Cell::Cell(string Name, Cell_Type Type):
    Name(Name),
    Type(Type),
    Gate_Delay(0),
    Shortest_Path_Delay(INT_MAX),
    Longest_Path_Delay(INT_MIN)
{
    ;
}

Cell::~Cell(){

}