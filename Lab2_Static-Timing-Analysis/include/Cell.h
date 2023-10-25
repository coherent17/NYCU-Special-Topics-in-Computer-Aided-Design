#ifndef _CELL_H_
#define _CELL_H_

#include <bits/stdc++.h>
#include "Util.h"
using namespace std;

struct Cell{
    string Name;                    // The name of the gate
    Cell_Type Type;                 // START, END, NAND, NOR, INV
    double Gate_Delay;              // From .lib LUT
    double Shortest_Path_Delay;     // Contamination Delay
    double Longest_Path_Delay;      // Propagation Delay
    

    // Constructor & Destructor
    Cell(string Name, Cell_Type Type);
    ~Cell();
};

#endif