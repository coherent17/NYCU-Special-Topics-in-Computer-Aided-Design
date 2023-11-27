#include <bits/stdc++.h>
#include "STA.h"
using namespace std;

int main(int argc, char *argv[]){
    STA sta_manager;
    sta_manager.Parse_Library(argv[3]);
    sta_manager.Parse_Pattern(argv[2]);
    sta_manager.Parse_Netlist(argv[1]);
    //sta_manager.Print_Library();
    //sta_manager.Print_Netlist();
    //sta_manager.Print_Pattern();

    // Step 1
    sta_manager.Calculate_Output_Loading();
    sta_manager.Dump_Output_Loading();

    // Step2
    sta_manager.Calculate_Cell_Delay();

    return 0;
}