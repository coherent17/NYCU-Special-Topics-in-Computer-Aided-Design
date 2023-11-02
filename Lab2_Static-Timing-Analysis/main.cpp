#include <bits/stdc++.h>
#include "STA.h"
using namespace std;

int main(int argc, char *argv[]){
    STA *sta_manager = new STA();
    sta_manager->Parse_Library(argv[3]);
    sta_manager->Parse_Netlist(argv[1]);

    // Step 1
    sta_manager->Calculate_Output_Loading();
    sta_manager->Dump_Output_Loading();

    // Step 2
    sta_manager->Calculate_Cell_Delay();
    //sta_manager->Print_Netlist();
    delete sta_manager;
    return 0;
}