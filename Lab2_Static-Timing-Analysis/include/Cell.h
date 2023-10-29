#ifndef _CELL_H_
#define _CELL_H_

#include <bits/stdc++.h>
#include "Util.h"
#include "Net.h"
using namespace std;

struct Net;

struct Cell{
    string Cell_Name;
    Cell_Type Type;
    vector<Net *> Input;    // Fan In
    Net *Output;            // No Fan Out

    Cell(string, Cell_Type);
    ~Cell();
};

#endif