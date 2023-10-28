#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <bits/stdc++.h>
using namespace std;

struct Look_Up_Table{
    string Cell_Name;
    unordered_map<string, double> Pin_Cap;
    unordered_map<string, vector<double>> Table;

    // Constructor & Destructor
    Look_Up_Table(string);
    ~Look_Up_Table();
    friend ostream &operator<<(ostream &, Look_Up_Table &);
};

struct Library{
    vector<double> index_1;     // Output Cap
    vector<double> index_2;     // Input Slew
    unordered_map<string, Look_Up_Table *> LUT;

    // Constructor & Destructor
    Library();
    ~Library();

    friend ostream &operator<<(ostream &, const Library &);
};

#endif