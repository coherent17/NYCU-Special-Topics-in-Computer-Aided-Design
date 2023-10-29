#ifndef _NET_H_
#define _NET_H_

#include <bits/stdc++.h>
#include "Cell.h"
using namespace std;

struct Cell;

struct Net{
    string Net_Name;
    Net_Type Type;
    unordered_map<string, Cell *> Cell_In;
    unordered_map<string, Cell *> Cell_Out;

    // Constructor & Destructor
    Net(string, Net_Type);
    ~Net();
};

#endif