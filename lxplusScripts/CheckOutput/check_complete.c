#include <TFile.h>
#include <TString.h>
#include <iostream>
#include <TChain.h>
#include <TKey.h>
#include <sys/stat.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv){
	TString exampleFN;
	vector<TChain*> chains;
	bool error = false;

	if(argc!=4){
		cout << "bad number of arguments : ExampleFile File Expected" << endl;
		exit(0);
	}
	exampleFN = argv[1];
	int expected = atoi(argv[3]);
	
	//1) Open 1 example root file and build tree structures
	TFile *ex = TFile::Open(exampleFN, "READ");
	
	TKey *curr_key;
	TList *keys = ex->GetListOfKeys();
	for(int i=0; i<keys->GetEntries(); i++){
		curr_key = (TKey*)keys->At(i);
		cout << "Found Key : " << curr_key->GetName() << endl;
		if(TString(curr_key->GetClassName()).CompareTo("TTree")==0) chains.push_back(new TChain(curr_key->GetName()));
	}

	//3) Chain each tree

	cout << "Adding file : " << argv[2] << endl;
	for(int i=0; i<chains.size(); i++){	
        //MCTruthTree->AddFile(inputFileName);
        //filesList.push_back(inputFileName);
		if(chains[i]->AddFile(argv[2])==0) error = true;
	}

	//4) Verify that we have the right number on each tree
	for(int i=0; i<chains.size() && !error; i++){
		cout << "Found " << chains[i]->GetEntries() << " entries for tree " << chains[i]->GetName() << endl;
		if(TString(chains[i]->GetName()).CompareTo("RUNS") == 0){
			if(chains[i]->GetEntries() != 1) error = true;
		}else{
			if(chains[i]->GetEntries() != expected && chains[i]->GetEntries() != expected/2.) error = true;
		}
	}

	if(error){
		cout << "__ERROR__" << endl;
		return -1;
	}
	return 0;
}

