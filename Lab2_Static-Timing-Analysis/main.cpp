#include <bits/stdc++.h>
#include "Static_Timing_Analyer.h"
using namespace std;

int main(int argc, char *argv[]){
    ifstream Netlist_File(argv[1]);
    ifstream Library_File(argv[3]);

    Static_Timing_Analyer *STA = new Static_Timing_Analyer();
    STA->Netlist_Parser(Netlist_File);
    STA->Library_Parser(Library_File);
    delete STA;
    return 0;
}