#include "QM.h"
// Constructor & Destructor
QM::QM():Num_Vars(0), Min_Literal(INT_MAX){
    ;
}

QM::~QM(){
    ;
}

// I/O
void QM::Parser(ifstream &fin){
    string line;
    while(getline(fin, line)){
        if(line == ".i"){
            getline(fin, line);
            stringstream ss(line);
            ss >> Num_Vars;
        }
        else if(line == ".m"){
            getline(fin, line);
            stringstream ss(line);
            int on_set;
            while(ss >> on_set){
                ON_Set_Decimal.emplace_back(on_set);
                ON_Set_Binary.emplace_back(binary_strings[Num_Vars][on_set]);
                Implicants.emplace_back(binary_strings[Num_Vars][on_set]);
            }
        }
        else if(line == ".d"){
            getline(fin, line);
            stringstream ss(line);
            int dc_set;
            while(ss >> dc_set){
                DC_Set_Decimal.emplace_back(dc_set);
                DC_Set_Binary.emplace_back(binary_strings[Num_Vars][dc_set]);
                Implicants.emplace_back(binary_strings[Num_Vars][dc_set]);
            }
        }
        else{
            // should never reach here
            abort();
        }
    }
    fin.close();

    // Sort the implicants by number of 1
    auto Implicants_cmp = [](const string &Implicant1, const string &Implicant2){
        return count(Implicant1.begin(), Implicant1.end(), '1') < count(Implicant2.begin(), Implicant2.end(), '1');
    };
    sort(Implicants.begin(), Implicants.end());
    sort(Implicants.begin(), Implicants.end(), Implicants_cmp);

    if(PRINT_PARSER_SUMMARY){
        cout << "################### Parser Summary #####################" << endl;
        cout << "Num Vars: " << Num_Vars << endl;
        cout << "ON Set: " << endl;
        for(size_t i = 0; i < ON_Set_Decimal.size(); i++){
            cout << "\t" << setw(3) << ON_Set_Decimal[i] << " (" << ON_Set_Binary[i] << ")" << endl;
        }
        cout << endl;
        cout << "DC Set: " << endl;
        for(size_t i = 0; i < DC_Set_Decimal.size(); i++){
            cout << "\t" << setw(3) << DC_Set_Decimal[i] << " (" << DC_Set_Binary[i] << ")" << endl;
        }
        cout << endl;
        cout << "Implicants: " << endl;
        for(size_t i = 0; i < Implicants.size(); i++){
            cout << "\t" << Implicants[i] << endl;
        }
        cout << "################ End Parser Summary ####################" << endl << endl;
    }
}

void QM::Dump(ofstream &fout){
    if(DUMP_ALPHABETS){
        auto Prime_Implicants_cmp = [](const string &Prime_Implicant1, const string &Prime_Implicant2){
            string order = "10-";
            for(size_t i = 0; i < Prime_Implicant1.length(); i++){
                size_t posA = order.find(Prime_Implicant1[i]);
                size_t posB = order.find(Prime_Implicant2[i]);
                if(posA != posB) return posA < posB;
            }
            return false;
        };
        sort(Prime_Implicants.begin(), Prime_Implicants.end(), Prime_Implicants_cmp);
        sort(Min_Cover_Prime_Implicants.begin(), Min_Cover_Prime_Implicants.end(), Prime_Implicants_cmp);
        
        // Dump Prime Implicants
        fout << ".p " << Prime_Implicants.size() << endl;
        for(size_t i = 0; i < Prime_Implicants.size(); i++){
            string Alphabet_Literal = "";
            for(size_t j = 0; j < Prime_Implicants[i].length(); j++){
                if(Prime_Implicants[i][j] == '-') continue;
                if(Prime_Implicants[i][j] == '0') Alphabet_Literal+= Alphabets_Negative[j];
                else if(Prime_Implicants[i][j] == '1') Alphabet_Literal+= Alphabets_Positive[j];
            }
            fout << Alphabet_Literal << endl;
            if(i == 14) break;
        }
        fout << endl;

        // Dump Min Cover Prime Implicants
        fout << ".mc " << Min_Cover_Prime_Implicants.size() << endl;
        for(size_t i = 0; i < Min_Cover_Prime_Implicants.size(); i++){
            string Alphabet_Literal = "";
            for(size_t j = 0; j < Min_Cover_Prime_Implicants[i].length(); j++){
                if(Min_Cover_Prime_Implicants[i][j] == '-') continue;
                if(Min_Cover_Prime_Implicants[i][j] == '0') Alphabet_Literal+= Alphabets_Negative[j];
                else if(Min_Cover_Prime_Implicants[i][j] == '1') Alphabet_Literal+= Alphabets_Positive[j];
            }
            fout << Alphabet_Literal << endl;
        }
        fout << "literal=" << Min_Literal;

    }
    else{
        auto Prime_Implicants_cmp = [](const string &Implicant1, const string &Implicant2){
            string order = "-01";
            for(size_t i = 0; i < Implicant1.length(); i++){
                size_t pos1 = order.find(Implicant1[i]);
                size_t pos2 = order.find(Implicant2[i]);
                if(pos1 != pos2) return pos1 < pos2;
            }
            return false;
        };
        sort(Prime_Implicants.begin(), Prime_Implicants.end(), Prime_Implicants_cmp);
        sort(Min_Cover_Prime_Implicants.begin(), Min_Cover_Prime_Implicants.end(), Prime_Implicants_cmp);

        // Dump Prime Implicants
        fout << ".p " << Prime_Implicants.size() << endl;
        for(size_t i = 0; i < Prime_Implicants.size(); i++){
            fout << Prime_Implicants[i] << endl;
            if(i == 14) break;
        }
        fout << endl;

        // Dump Min Cover Prime Implicants
        fout << ".mc " << Min_Cover_Prime_Implicants.size() << endl;
        for(size_t i = 0; i < Min_Cover_Prime_Implicants.size(); i++){
            fout << Min_Cover_Prime_Implicants[i] << endl;
        }
        fout << "literal=" << Min_Literal;
    }
    fout.close();
}

void QM::Run(){
    //  Run Quine-McCluskey
    while(!Implicants.empty()){
        Find_Prime_Implicants();
    }
    if(PRINT_PRIME_IMPLICANTS){
        vector<string> Prime_Implicants_Copy = Prime_Implicants;
        // Sort the implicants by '-01'
        auto Prime_Implicant_cmp = [](const string &Implicant1, const string &Implicant2){
            string order = "-01";
            for(size_t i = 0; i < Implicant1.length(); i++){
                size_t pos1 = order.find(Implicant1[i]);
                size_t pos2 = order.find(Implicant2[i]);
                if(pos1 != pos2) return pos1 < pos2;
            }
            return false;
        };
        sort(Prime_Implicants.begin(), Prime_Implicants.end(), Prime_Implicant_cmp);
        cout << "################## Prime Implicant #####################" << endl;
        cout << "Prime Implicants : " << Prime_Implicants.size() << endl;
        for(const auto &prime_implicant : Prime_Implicants){
		    cout << "\t" << prime_implicant << endl;
	    }
        cout << "################## Prime Implicant #####################" << endl << endl;;
    }

    // Find Essential Prime Implicants & Prime Implicant Candidate
    Classify_Prime_Implicats();
    if(PRINT_UNCOVER_ON_SET){
        cout << "################## Uncover ON Set ######################" << endl;
        cout << "Uncover ON Set: " << endl;
        for(size_t i = 0; i < Uncover_ON_Set_Decimal.size(); i++){
            cout << "\t" << setw(3) << Uncover_ON_Set_Decimal[i] << " (" << Uncover_ON_Set_Binary[i] << ")" << endl;
        }
        cout << "################## Uncover ON Set ######################" << endl << endl;
    }

    if(PRINT_ESSENTIAL_PRIME_IMPLICANTS){
        cout << "################ Essential Prime Implicant #############" << endl;
        cout << "Essential Prime Implicants : " << Essential_Prime_Implicants.size() << endl;
        for(const auto &essential_prime_implicant : Essential_Prime_Implicants){
		    cout << "\t" << essential_prime_implicant << endl;
	    }
        cout << "################ Essential Prime Implicant #############" << endl << endl;
    }

    if(PRINT_CANDIDATE_PRIME_IMPLICANTS){
        cout << "################ Candidate Prime Implicant #############" << endl;
        cout << "Candidate Prime Implicants : " << Candidate_Prime_Implicants.size() << endl;
        for(const auto &candidate_prime_implicant : Candidate_Prime_Implicants){
		    cout << "\t" << candidate_prime_implicant << endl;
	    }
        cout << "################ Candidate Prime Implicant #############" << endl << endl;
    }

    // For rest of the candidate prime implicants, run Petrick's method => SAT problem
    Build_Product_Of_Sum();
    if(PRINT_PRODUCT_OF_SUM){
        cout << "##################### Product Of Sum ###################" << endl;
        cout << "Product of Sum (POS):" << endl;
        for(size_t i = 0; i < Product_Of_Sum.size(); i++){
            cout << "\t(";
            for(size_t j = 0; j < Product_Of_Sum[i].size(); j++){
                cout << Product_Of_Sum[i][j];
                if(j != Product_Of_Sum[i].size() - 1) cout << " + ";
            }
            cout << ")" << endl;
        }
        cout << "##################### Product Of Sum ###################" << endl << endl;
    }

    Build_Sum_Of_Product();
    if(PRINT_SUM_OF_PRODUCT){
        cout << "##################### Sum Of Product ###################" << endl;
        cout << "Sum of Product (SOP):" << endl;
        for(const vector<string> &Product_Term : Sum_Of_Product){
            cout << "\t(";
            for(const string &Term : Product_Term){
                if(Term != *Product_Term.begin()){
                    cout << " * ";
                }
                cout << Term;
            }
            cout << ")" << endl;
        }
        cout << "##################### Sum Of Product ###################" << endl << endl;
    }

    Find_Minimum_Cover();
    if(PRINT_MIN_COVER_PRIME_IMPLICANTS){
        vector<string> Min_Cover_Prime_Implicants_Copy = Min_Cover_Prime_Implicants;
        // Sort the implicants by '-01'
        auto Prime_Implicant_cmp = [](const string &Implicant1, const string &Implicant2){
            string order = "-01";
            for(size_t i = 0; i < Implicant1.length(); i++){
                size_t pos1 = order.find(Implicant1[i]);
                size_t pos2 = order.find(Implicant2[i]);
                if(pos1 != pos2) return pos1 < pos2;
            }
            return false;
        };
        sort(Min_Cover_Prime_Implicants_Copy.begin(), Min_Cover_Prime_Implicants_Copy.end(), Prime_Implicant_cmp);
        cout << "################ Min Cover Prime Implicant #############" << endl;
        cout << "Min Cover Prime Implicants : " << Min_Cover_Prime_Implicants_Copy.size()  << ", Literal = " << Min_Literal << endl;
        for(const auto &min_cover_prime_implicant : Min_Cover_Prime_Implicants_Copy){
		    cout << "\t" << min_cover_prime_implicant << endl;
	    }
        cout << "################ Min Cover Prime Implicant #############" << endl << endl;
    }
}


// Quine-McCluskey private function
bool QM::Has_One_Bit_Different(const string &Implicant1, const string &Implicant2){
    assert(Implicant1.size() == Implicant2.size());
    int num_bit_different = 0;
	for(size_t i = 0; i < Implicant1.length(); i++){
		if(Implicant1[i] != Implicant2[i]){
			num_bit_different++;
		}
        if(num_bit_different > 1) return false;
	}
	return num_bit_different == 1;
}

string QM::Merge_Implicant(const string &Implicant1, const string &Implicant2){
    assert(Implicant1.size() == Implicant2.size());
    assert(Has_One_Bit_Different(Implicant1, Implicant2));
    string Reduced_Implicant = Implicant1;
    for(size_t i = 0; i < Implicant1.length(); i++){
		if(Implicant1[i] != Implicant2[i]){
			Reduced_Implicant[i] = '-';
			break;
		}
	}
    return Reduced_Implicant;
}

void QM::Find_Prime_Implicants(){
    vector<bool> Is_Prime_Implicants(Implicants.size(), true);
    set<string> Reduced_Implicants;
    for(size_t i = 0; i < Implicants.size() - 1; i++){
        for(size_t j = i + 1; j < Implicants.size(); j++){
            string Implicant1 = Implicants[i];
            string Implicant2 = Implicants[j];
            if(Has_One_Bit_Different(Implicant1, Implicant2)){
                Reduced_Implicants.insert(Merge_Implicant(Implicant1, Implicant2));
                Is_Prime_Implicants[i] = false;
                Is_Prime_Implicants[j] = false;
            }
        }
    }

    // Store the Prime Implicants
    for(size_t i = 0; i < Is_Prime_Implicants.size(); i++){
        if(Is_Prime_Implicants[i]){
            Prime_Implicants.emplace_back(Implicants[i]);
        }
    }

    // Restore the Implicants with the reduced implicants
    Implicants.clear();
    for(const auto &Reduced_Implicant : Reduced_Implicants){
        Implicants.emplace_back(Reduced_Implicant);
    }
}

// Petrick's Method private function
bool QM::Is_Covered(const string &ON_Binary, const string &Prime_Implicant){
    assert(ON_Binary.length() == Prime_Implicant.length());
    for(size_t i = 0; i < ON_Binary.size(); i++){
        if(Prime_Implicant[i] == '-') continue;
        if(Prime_Implicant[i] != ON_Binary[i]){
            return false;
        }
    }
    return true;    // The Prime Implicant covers the On Set
}

// Check whether the prime implicats from QM is essential prime implicants or candidate prime implicants
void QM::Classify_Prime_Implicats(){
    vector<int> ON_Set_Cover_Count(ON_Set_Binary.size(), 0);
    vector<vector<string>> ON_Set_Cover_Prime_Implicants(ON_Set_Binary.size());

    // Count ON set covers and populate cover over prime implicants
    for(size_t i = 0; i < ON_Set_Binary.size(); i++){
        for(size_t j = 0; j < Prime_Implicants.size(); j++){
            if(Is_Covered(ON_Set_Binary[i], Prime_Implicants[j])){
                ON_Set_Cover_Count[i]++;
                ON_Set_Cover_Prime_Implicants[i].emplace_back(Prime_Implicants[j]);
            }
        }
    }

    // Find essential prime implicants
    for(size_t i = 0; i < ON_Set_Cover_Count.size(); i++){
        if(ON_Set_Cover_Count[i] == 1){
            Essential_Prime_Implicants.emplace_back(ON_Set_Cover_Prime_Implicants[i][0]);
        }
    }

    // Make it unique (The essential prime implicants can cover multi-on set)
    sort(Essential_Prime_Implicants.begin(), Essential_Prime_Implicants.end());
    Essential_Prime_Implicants.erase(unique(Essential_Prime_Implicants.begin(), Essential_Prime_Implicants.end()), Essential_Prime_Implicants.end());

    // Find uncovered on sets
    for(size_t i = 0; i < ON_Set_Binary.size(); i++){
        bool isCovered = false;
        for(size_t j = 0; j < Essential_Prime_Implicants.size(); j++){
            if(Is_Covered(ON_Set_Binary[i], Essential_Prime_Implicants[j])){
                isCovered = true;
                break;
            }
        }
        if(!isCovered){
            Uncover_ON_Set_Decimal.emplace_back(ON_Set_Decimal[i]);
            Uncover_ON_Set_Binary.emplace_back(ON_Set_Binary[i]);
        }
    }

    // Find remaining candidate prime implicants
    for(size_t i = 0; i < Prime_Implicants.size(); i++){
        for(size_t j = 0; j < Uncover_ON_Set_Binary.size(); j++){
            if(Is_Covered(Uncover_ON_Set_Binary[j], Prime_Implicants[i])){
                Candidate_Prime_Implicants.emplace_back(Prime_Implicants[i]);
                break;
            }
        }
    }
}

void QM::Build_Product_Of_Sum(){
    // check candidate prime implicants contains which on set
    for(const auto &on_set_binary : Uncover_ON_Set_Binary){
        vector<string> Sum_Literal;
        for(const auto &candidate_prime_implicant : Candidate_Prime_Implicants){
            if(Is_Covered(on_set_binary, candidate_prime_implicant)){
                Sum_Literal.emplace_back(candidate_prime_implicant);
            }
        }
        Product_Of_Sum.emplace_back(Sum_Literal);
    }
}

void QM::Build_Sum_Of_Product(){
    vector<string> visited_terms;
    size_t Current_Min_Term_Count = INT_MAX;
    Multiply_Terms(visited_terms, 0, Current_Min_Term_Count);
}

void QM::Multiply_Terms(vector<string> &visited_terms, size_t visited_index, size_t &Current_Min_Term_Count){
    // Recursion terminate condition
    if(visited_index == Product_Of_Sum.size()){
        vector<string> visited_terms_copy(visited_terms);
        sort(visited_terms_copy.begin(), visited_terms_copy.end());
        visited_terms_copy.erase(unique(visited_terms_copy.begin(), visited_terms_copy.end()), visited_terms_copy.end());
        if(visited_terms_copy.size() < Current_Min_Term_Count) Current_Min_Term_Count = visited_terms_copy.size();
        Sum_Of_Product.emplace_back(visited_terms_copy);
        return;
    }

    // Choose the next terms in Product Of Sum(POS)
    for(size_t i = 0; i < Product_Of_Sum[visited_index].size(); i++){
        vector<string> visited_terms_copy(visited_terms);
        sort(visited_terms_copy.begin(), visited_terms_copy.end());
        visited_terms_copy.erase(unique(visited_terms_copy.begin(), visited_terms_copy.end()), visited_terms_copy.end());
        // Branch & Bound the recursion depth, +1 for larger term but has lower literals cost
        if(Current_Min_Term_Count + 1 < visited_terms_copy.size()) break;
        string Candidate_Prime_Implicant = Product_Of_Sum[visited_index][i];
        visited_terms.emplace_back(Candidate_Prime_Implicant);
        Multiply_Terms(visited_terms, visited_index + 1, Current_Min_Term_Count);
        visited_terms.pop_back();
    }
}

void QM::Find_Minimum_Cover(){
    size_t Min_Index = -1;
    for(size_t i = 0; i < Sum_Of_Product.size(); i++){
        size_t Literal_Count = 0;
        for(size_t j = 0; j < Sum_Of_Product[i].size(); j++){
            for(size_t k = 0; k < Sum_Of_Product[i][j].length(); k++){
                if(Sum_Of_Product[i][j][k] != '-') Literal_Count++;
            }
        }
        if(Literal_Count < Min_Literal){
            Min_Literal = Literal_Count;
            Min_Index = i;
        }
    }

    for(size_t i = 0; i < Essential_Prime_Implicants.size(); i++){
        for(size_t j = 0; j < Essential_Prime_Implicants[i].length(); j++){
            if(Essential_Prime_Implicants[i][j] != '-') Min_Literal++;
        }
    }

    // Add essential prime implicants & candidate prime implicants in SOP to Min Cover Prime Implicants
    for(size_t i = 0; i < Essential_Prime_Implicants.size(); i++){
        Min_Cover_Prime_Implicants.emplace_back(Essential_Prime_Implicants[i]);
    }
    for(size_t i = 0; i < Sum_Of_Product[Min_Index].size(); i++){
        Min_Cover_Prime_Implicants.emplace_back(Sum_Of_Product[Min_Index][i]);
    }
}