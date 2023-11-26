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

    public:
        // Constructor & Destructor
        STA();
        ~STA();

        // Parser
        void Parse_Library(const char *library_filename);
        void Parse_Netlist(const char *netlist_filename);
        void Print_Library();
        void Print_Netlist();

        // Step 1
        void Calculate_Output_Loading();
        void Dump_Output_Loading();
};

#endif