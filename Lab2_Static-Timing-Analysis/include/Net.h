#ifndef _NET_H_
#define _NET_H_

#include <bits/stdc++.h>
#include "Cell.h"
using namespace std;

struct Cell;

struct Net{
    string Net_Name;
    Net_Type Type;
    vector<Cell *> Cell_In;
    vector<Cell *> Cell_Out;

    Net(string, Net_Type);
    ~Net();
};

#endif