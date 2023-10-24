#include "QM.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]){
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	QM *qm = new QM();
    qm->Parser(fin);
    qm->Run();
    qm->Dump(fout);
	delete qm;
	return 0;
}