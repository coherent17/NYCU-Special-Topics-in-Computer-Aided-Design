#ifndef _STATIC_TIMING_ANALYER_H_
#define _STATIC_TIMING_ANALYER_H_

#include <bits/stdc++.h>
#include "Library.h"
using namespace std;

struct Static_Timing_Analyer{
    Library *Lib;

    // Constructor & Destructor
    Static_Timing_Analyer();
    ~Static_Timing_Analyer();

    // Parser (Netlist & Library)
    void Library_Parser(ifstream &);
    void Netlist_Parser(ifstream &);

    
};

#endif