#ifndef _QM_H_
#define _QM_H_

#include <bits/stdc++.h>
#include "Binary_Bits.h"
#include "Util.h"
using namespace std;

class QM{
    private:
        // For Quine-McCluskey
        int Num_Vars;
        vector<int>     ON_Set_Decimal;
        vector<int>     DC_Set_Decimal;
        vector<string>  ON_Set_Binary;
        vector<string>  DC_Set_Binary;
        vector<string>  Implicants;
        vector<string>  Prime_Implicants;

        // For Petrick's Method
        vector<vector<string>> Product_Of_Sum;


    public:
        // Constructor & Destructor
        QM();
        ~QM();

        // Input & Output
        void Parser(ifstream &);
        void Dump(ofstream &);

        // Main algorithm
        void Run();

    private:
        // For Quine-McCluskey
        inline bool Has_One_Bit_Different(const string &, const string &);
        string Merge_Implicant(const string &, const string &);
        void Find_Prime_Implicants();

        // For Petrick's Method
        inline bool Is_Covered(const string &, const string &);
        void Build_POS();
};

#endif