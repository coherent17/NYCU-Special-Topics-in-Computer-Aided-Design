#ifndef _STATIC_TIMING_ANALYER_H_
#define _STATIC_TIMING_ANALYER_H_

#include <bits/stdc++.h>
#include "Library.h"
#include "Util.h"
#include "Cell.h"
#include "Net.h"
using namespace std;

struct Static_Timing_Analyer{
    Library *Lib;
    unordered_map<string, Cell *> Primary_Input_Cells;
    unordered_map<string, Cell *> Cells;
    unordered_map<string, Cell *> Primary_Output_Cells;
    unordered_map<string, Net *> Nets;

    // Constructor & Destructor
    Static_Timing_Analyer();
    ~Static_Timing_Analyer();

    // Parser (Netlist & Library)
    void Library_Parser(ifstream &);
    void Netlist_Parser(ifstream &);
};

#endif