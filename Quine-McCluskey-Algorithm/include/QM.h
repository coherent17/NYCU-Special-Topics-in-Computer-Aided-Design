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
        size_t Min_Cover;
        size_t Min_Literal;
        vector<string> Essential_Prime_Implicants;
        vector<string> Candidate_Prime_Implicants;
        vector<string> Min_Cover_Prime_Implicants;
        vector<int> Uncover_ON_Set_Decimal;
        vector<string> Uncover_ON_Set_Binary;
        vector<vector<string>> Product_Of_Sum;
        vector<vector<string>> Sum_Of_Product;

    public:
        // Constructor & Destructor
        QM();
        ~QM();

        // I/O
        void Parser(ifstream &);
        void Dump(ofstream &);

        // Main algorithm
        void Run();

    private:
        // For Quine-McCluskey
        inline bool Has_One_Bit_Different(const string &Implicant1, const string &Implicant2);
        string Merge_Implicant(const string &Implicant1, const string &Implicant2);
        void Find_Prime_Implicants();

        // For Petrick's Method
        inline bool Is_Covered(const string &ON_Binary, const string &Prime_Implicant);
        void Classify_Prime_Implicats();
        void Build_Product_Of_Sum();
        void Build_Sum_Of_Product();
        void Multiply_Terms(vector<string> &visited_terms, size_t visited_index);
        void Find_Minimum_Cover();
        
};

#endif