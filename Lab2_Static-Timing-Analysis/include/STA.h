#ifndef _STA_H_
#define _STA_H_

#include <bits/stdc++.h>
#include "Library.h"
#include "Cell.h"
#include "Net.h"
#include "Util.h"
using namespace std;

class STA{
    public:
        const char *netlist_filename;
        const char *library_filename;
        string Design_Name;
        Library *library;
        unordered_map<string, Cell *> Cells;
        unordered_map<string, Net *> Nets;
        size_t Num_Cell;
        size_t Num_Net;
        vector<Cell *> Primary_Output_Cells;
        vector<Cell *> Cells_In_Topological_Order;
        double Longest_Delay;
        double Shortest_Delay;
        vector<string> Longest_Path;
        vector<string> Shortest_Path;

    public:
        // Constructor & Destructor
        STA();
        ~STA();

        // Parser
        void Parse_Library(const char *library_filename);
        void Parse_Netlist(const char *netlist_filename);
        void Print_Netlist();

        // Step 1
        void Calculate_Output_Loading();
        void Dump_Output_Loading();

        // Step 2
        void Calculate_Cell_Delay();
        void Dump_Cell_Delay();

        // Step 3
        void Find_Longest_Delay_And_Path();
        void Find_Shortest_Delay_And_Path();
        void Dump_Longest_And_Shortest_Delay_And_Path();

    private:
        void Cell_Topological_Sort();
        void Set_Cell_Input_Transition_Time(Cell *cell);
        double Table_Interpolation(double C_req, double S_req, double C1, double C2, double S1, double S2, double P0, double P1, double P2, double P3);
        double Table_Look_Up(Cell *cell, const string &Table_Name);

};

#endif