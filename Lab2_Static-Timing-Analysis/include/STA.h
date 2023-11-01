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
};

#endif