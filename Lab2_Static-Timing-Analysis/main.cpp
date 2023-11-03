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
    sta_manager->Dump_Cell_Delay();
    
    // Step 3
    sta_manager->Find_Longest_Delay_And_Path();
    sta_manager->Find_Shortest_Delay_And_Path();
    sta_manager->Dump_Longest_And_Shortest_Delay_And_Path();
    delete sta_manager;
    return 0;
}