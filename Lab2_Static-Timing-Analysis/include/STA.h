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
        Library *library;
        unordered_map<string, Cell *> Cells;
        unordered_map<string, Net *> Nets;

    public:
        // Constructor & Destructor
        STA();
        ~STA();

        // Parser
        void Parse_Library(ifstream &library_file);
        void Parse_Netlist(ifstream &netlist_file);

        // Step 1
        void Calculate_Output_Loading();
};

#endif