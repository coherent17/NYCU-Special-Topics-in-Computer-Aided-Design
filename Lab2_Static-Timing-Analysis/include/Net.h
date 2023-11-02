#ifndef _NET_H_
#define _NET_H_

#include <bits/stdc++.h>
#include "Cell.h"
using namespace std;

class Cell;

class Net{
    public:
        string Name;
        Net_Type Type;
        pair<string, Cell *> Input_Cell_Connections;
        vector<pair<string, Cell *>> Output_Cell_Connections;

    public:
        // Constructor & Destructor
        Net(string, Net_Type);
        ~Net();

        // cout Net
        friend ostream &operator<<(ostream &out, const Net &net);
};

#endif