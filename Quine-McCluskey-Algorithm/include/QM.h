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
        vector<string>  Essential_Prime_Implicants;
        vector<string>  Remain_Prime_Implicants;
        vector<int>     Uncover_ON_Set_Decimal;
        vector<string>  Uncover_ON_Set_Binary;

        vector<vector<string>> Product_Of_Sum;
        vector<vector<string>> Sum_Of_Product;

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
        void Find_Essential_Prime_Implicants();
        void Build_POS();
        void Build_SOP();
        void Multiply_Terms(vector<string> &, size_t, int &);
};

#endif