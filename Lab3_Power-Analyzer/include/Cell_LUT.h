#ifndef _CELL_LUT_H_
#define _CELL_LUT_H_

#include <bits/stdc++.h>
using namespace std;

class Cell_LUT{
    public:
        string Cell_Name;
        unordered_map<string, double> Pin_Cap;
        unordered_map<string, vector<double>> Look_Up_Tables;

    public:
        // Constructor & Destructor
        Cell_LUT(string);
        ~Cell_LUT();
        
        // Getter
        double Get_Pin_Cap(const string &Pin_Name);
        const vector<double> &Get_Look_Up_Table(const string &Table_Name);

        // cout Cell_LUT
        friend ostream &operator<<(ostream &out, const Cell_LUT &cell_lut);
};

#endif