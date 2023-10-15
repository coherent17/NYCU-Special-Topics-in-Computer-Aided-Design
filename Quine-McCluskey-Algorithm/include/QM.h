#ifndef _QM_H_
#define _QM_H_

#include <bits/stdc++.h>
#include "Binary_Bits.h"
#include "Util.h"
using namespace std;

class QM{
    private:
        int Num_Vars;
        vector<int>     ON_Set_Decimal;
        vector<int>     DC_Set_Decimal;
        vector<string>  ON_Set_Binary;
        vector<string>  DC_Set_Binary;
        vector<string>  Implicants;
        vector<string>  Prime_Implicants;


    public:
        // Constructor & Destructor
        QM();
        ~QM();

        // Parse the input file
        void Parser(ifstream &);
        void Dump(ofstream &);

        // Main algorithm
        void Run();

    private:
        bool Has_One_Bit_Different(const string &, const string &);
        string Merge_Implicant(const string &, const string &);
        void Find_Prime_Implicants();

};

#endif