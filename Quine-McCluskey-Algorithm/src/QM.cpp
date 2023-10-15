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
        cout << "########################################################" << endl;
    }
}

void QM::Dump(ofstream &fout){

}

void QM::Run(){
    while(Implicants.size() != 0){
        Find_Prime_Implicants();
    }

    // sort the prime implicants
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

    for(const auto &Prime_Implicant : Prime_Implicants){
        cout << Prime_Implicant << endl;
    }
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
            int Implicant1_1_count = count(Implicant1.begin(), Implicant1.end(), '1');
            int Implicant2_1_count = count(Implicant2.begin(), Implicant2.end(), '1');
            if(Implicant2_1_count - Implicant1_1_count > 1) break;
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