#ifndef _STA_H_
#define _STA_H_

#include <bits/stdc++.h>
#include "Library.h"
#include "Cell.h"
#include "Net.h"
using namespace std;

class STA{
    public:
        const char *netlist_filename;
        const char *library_filename;
        string Design_Name;
        Library *library;
        unordered_map<string, Cell *> Cells;
        unordered_map<string, Net *> Nets;
        size_t Num_Cells;
        size_t Num_Nets;
        vector<Cell *> Primary_Output_Cells;
        vector<Cell *> Cells_In_Topological_Order;
        vector<string> Input_Order;
        vector<unordered_map<string, int>> Patterns;

    public:
        // Constructor & Destructor
        STA();
        ~STA();

        // Parser
        void Parse_Library(const char *library_filename);
        void Parse_Netlist(const char *netlist_filename);
        void Parse_Pattern(const char *pattern_filename);

        void Print_Library();
        void Print_Netlist();
        void Print_Pattern();

        // Step 1
        void Calculate_Output_Loading();
        void Dump_Output_Loading();

        // Step 2
        void Calculate_Cell_Delay();


    private:
        void Cell_Topological_Sort();
        void Set_Cell_Input_Transition_Time(Cell *cell);
        double Table_Interpolation(double C_req, double S_req, double C1, double C2, double S1, double S2, double P0, double P1, double P2, double P3);
        double Table_Look_Up(Cell *cell, const string &Table_Name);
};

#endif