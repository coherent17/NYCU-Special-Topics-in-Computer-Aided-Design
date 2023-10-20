#include "QM.h"

// Constructor & Destructor
QM::QM():Num_Vars(0){

}

QM::~QM(){

}

// Parser
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
            int ON_Set;
            while(ss >> ON_Set){
                ON_Set_Decimal.emplace_back(ON_Set);
                ON_Set_Binary.emplace_back(binary_strings[Num_Vars][ON_Set]);
                Implicants.emplace_back(binary_strings[Num_Vars][ON_Set]);
            }
        }
        else if(line == ".d"){
            getline(fin, line);
            stringstream ss(line);
            int DC_Set;
            while(ss >> DC_Set){
                DC_Set_Decimal.emplace_back(DC_Set);
                DC_Set_Binary.emplace_back(binary_strings[Num_Vars][DC_Set]);
                Implicants.emplace_back(binary_strings[Num_Vars][DC_Set]);
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
        cout << "########################################################" << endl << endl;
    }
}

void QM::Dump(ofstream &fout){
    // dump prime implicant
    auto Prime_Implicants_cmp = [](const string &Prime_Implicant1, const string &Prime_Implicant2){
        string order = "10-";
        for(size_t i = 0; i < Prime_Implicant1.length(); i++){
            size_t posA = order.find(Prime_Implicant1[i]);
            size_t posB = order.find(Prime_Implicant2[i]);
            if(posA != posB) return posA < posB;
	    }
	    return false;
    };

    vector<string> Prime_Implicants_Copy = Prime_Implicants;
    sort(Prime_Implicants_Copy.begin(), Prime_Implicants_Copy.end(), Prime_Implicants_cmp);

    cout << ".p " << Prime_Implicants.size() << endl;
    for(size_t i = 0; i < Prime_Implicants_Copy.size(); i++){
        string Alphabet_Literal = "";
        for(size_t j = 0; j < Prime_Implicants_Copy[i].length(); j++){
            if(Prime_Implicants_Copy[i][j] == '-') continue;
            if(Prime_Implicants_Copy[i][j] == '0') Alphabet_Literal+= Alphabets_Negative[j];
            else if(Prime_Implicants_Copy[i][j] == '1') Alphabet_Literal+= Alphabets_Positive[j];
        }
        cout << Alphabet_Literal << endl;
        if(i == 14) break;
    }
    cout << endl;

    // dump prime implicant that has min cover
    int Min_Literal = INT_MAX;
    size_t Min_Index = -1;
    for(size_t i = 0; i < Sum_Of_Product.size(); i++){
        int Literal_Count = 0;
        for(size_t j = 0; j < Sum_Of_Product[i].size(); j++){
            for(size_t k = 0; k < Sum_Of_Product[i][j].size(); k++){
                if(Sum_Of_Product[i][j][k] != '-') Literal_Count++;
            }
        }
        if(Literal_Count < Min_Literal){
            Min_Literal = Literal_Count;
            Min_Index = i;
        }
    }
    cout << ".mc " << Sum_Of_Product[Min_Index].size() << endl;
    sort(Sum_Of_Product[Min_Index].begin(), Sum_Of_Product[Min_Index].end(), Prime_Implicants_cmp);
    for(size_t i = 0; i < Sum_Of_Product[Min_Index].size(); i++){
        string Alphabet_Literal = "";
        for(size_t j = 0; j < Sum_Of_Product[Min_Index][i].length(); j++){
            if(Sum_Of_Product[Min_Index][i][j] == '-') continue;
            if(Sum_Of_Product[Min_Index][i][j] == '0') Alphabet_Literal+= Alphabets_Negative[j];
            else if(Sum_Of_Product[Min_Index][i][j] == '1') Alphabet_Literal+= Alphabets_Positive[j];
        }
        cout << Alphabet_Literal << endl;
    }
    cout << "literal=" << Min_Literal << endl;
}

void QM::Run(){
    cout << "####################### Quine McCluskey #####################" << endl;
    while(!Implicants.empty()){
        Find_Prime_Implicants();
    }
    cout << "#############################################################" << endl << endl;
    // Petrick's Method to find the min-cover
    Build_POS();
    Build_SOP();

}

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
                if(PRINT_QM_MERGE_PROCESS){
                    cout << Implicant1 << " " << Implicant2 << " " << Merge_Implicant(Implicant1, Implicant2) << endl;
                }
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
    cout << endl;
}

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

void QM::Build_POS(){
    // Check Prime Implicants contains which ON Set
    for(const auto &on_set_binary : ON_Set_Binary){
        vector<string> Sum_Literal;
        for(const auto &prime_implicant : Prime_Implicants){
            if(Is_Covered(on_set_binary, prime_implicant)){
                Sum_Literal.emplace_back(prime_implicant);
            }
        }
        Product_Of_Sum.emplace_back(Sum_Literal);
    }

    //Dump POS
    if(PRINT_POS){
        cout << "Product of Sum (POS):" << endl;
        for(size_t i = 0; i < Product_Of_Sum.size(); i++){
            cout << "\t(";
            for(size_t j = 0; j < Product_Of_Sum[i].size(); j++){
                cout << Product_Of_Sum[i][j];
                if(j != Product_Of_Sum[i].size() - 1) cout << " + ";
            }
            cout << ")" << endl;
        }
        cout << endl;
    }
}

void QM::Build_SOP(){
    vector<string> visited_terms;
    int Current_Min_Term_Count = INT_MAX;
    Multiply_Terms(visited_terms, 0, Current_Min_Term_Count);
    // Sort the implicants by number of 1
    auto SOP_cmp = [](const vector<string> &Sum_Of_Product_1, const vector<string> &Sum_Of_Product_2){
        return Sum_Of_Product_1.size() < Sum_Of_Product_2.size();
    };
    sort(Sum_Of_Product.begin(), Sum_Of_Product.end(), SOP_cmp);
    if(PRINT_SOP){
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
    }
}

void QM::Multiply_Terms(vector<string> &visited_terms, size_t visited_index, int &Current_Min_Term_Count){

    // Recursion terminate condition
    if(visited_index == Product_Of_Sum.size()){
        vector<string> visited_terms_copy(visited_terms);
        sort(visited_terms_copy.begin(), visited_terms_copy.end());
        visited_terms_copy.erase(unique(visited_terms_copy.begin(), visited_terms_copy.end()), visited_terms_copy.end());
        if(visited_terms_copy.size() < Current_Min_Term_Count) Current_Min_Term_Count = visited_terms_copy.size();
        Sum_Of_Product.emplace_back(visited_terms_copy);
        return;
    }

    // Choose the next terms in POS
    for(size_t i = 0; i < Product_Of_Sum[visited_index].size(); i++){
        vector<string> visited_terms_copy(visited_terms);
        sort(visited_terms_copy.begin(), visited_terms_copy.end());
        visited_terms_copy.erase(unique(visited_terms_copy.begin(), visited_terms_copy.end()), visited_terms_copy.end());
        if(Current_Min_Term_Count < visited_terms_copy.size()) break;
        string Prime_Implicant = Product_Of_Sum[visited_index][i];
        visited_terms.emplace_back(Prime_Implicant);
        // Move to the next term in POS
        Multiply_Terms(visited_terms, visited_index + 1, Current_Min_Term_Count);

        // Go back and choose another terms in POS
        visited_terms.pop_back();
    }
}