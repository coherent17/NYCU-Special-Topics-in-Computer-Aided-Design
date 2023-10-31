#ifndef _NET_H_
#define _NET_H_

#include <bits/stdc++.h>
#include "Cell.h"
using namespace std;

enum Net_Type{
    input,
    output,
    wire
};

class Cell;

class Net{
    public:
        string Name;
        Net_Type Type;
        unordered_map<string, Cell *> Input_Cell_Connections;
        unordered_map<string, Cell *> Output_Cell_Connections;

    public:
        // Constructor & Destructor
        Net(string, Net_Type);
        ~Net();
};

#endif