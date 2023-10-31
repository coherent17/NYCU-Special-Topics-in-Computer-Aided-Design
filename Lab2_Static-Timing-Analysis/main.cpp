#include <bits/stdc++.h>
#include "STA.h"
using namespace std;

int main(int argc, char *argv[]){
    ifstream Netlist_File(argv[1]);
    ifstream Library_File(argv[3]);

    STA *sta_manager = new STA();
    sta_manager->Parse_Library(Library_File);
    sta_manager->Parse_Netlist(Netlist_File);

    sta_manager->Calculate_Output_Loading();
    delete sta_manager;
    return 0;
}