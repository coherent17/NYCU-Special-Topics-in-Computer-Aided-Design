#ifndef _CELL_H_
#define _CELL_H_

#include <bits/stdc++.h>
#include "Util.h"
#include "Net.h"
using namespace std;

class Net;

class Cell{
    public:
        string Name;
        int Instance_Number;
        Cell_Type Type;
        vector<Net *> Input_Nets;
        Net *Output_Net;
        size_t In_Degree;
        double Output_Loading;
        double Input_Transition_Time;
        double Output_Transition_Time;
        bool Worst_Case_Output;
        double Propagation_Delay;
        double Arrival_Time;
        double Longest_Delay;   // Arrival Time + Propagation Delay
        Cell *Prev_Cell;
    
    public:
        // Constructor & Destructor
        Cell(string, Cell_Type);
        ~Cell();
        
        // cout Cell
        friend ostream &operator<<(ostream &out, const Cell &cell); 
};

#endif