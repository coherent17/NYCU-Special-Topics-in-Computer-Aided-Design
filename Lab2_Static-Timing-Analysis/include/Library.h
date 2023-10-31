#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <bits/stdc++.h>
#include "Cell_LUT.h"
using namespace std;

class Library{
    public:
        vector<double> Index_1;
        vector<double> Index_2;
        unordered_map<string, Cell_LUT *> Cell_LUT_Map;

    public:
        // Constructor & Destructor
        Library();
        ~Library();

        // Getter
        const Cell_LUT *Get_Cell_LUT(const string &Cell_Name);

        // cout Library
        friend ostream &operator<<(ostream &out, const Library &library);
};

#endif