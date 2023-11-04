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
        double Output_Loading;
        size_t In_Degree;
        double Input_Transition_Time;
        double Output_Transition_Time;
        bool Worst_Case_Output;
        double Propagation_Delay;
        double Delay;   // The delay from input to this cell (without propagation delay of this cell)

        double Longest_Delay;
        Cell *Longest_Delay_Prev_Cell;
    
    public:
        // Constructor & Destructor
        Cell(string, Cell_Type);
        ~Cell();
        
        // cout Cell
        friend ostream &operator<<(ostream &out, const Cell &cell); 
};

#endif