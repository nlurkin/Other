

int testFile(TString fileName){
	TFile *fd = TFile::Open(fileName);

	TH1D* cuts = (TH1D*)fd->Get("Cuts");
	TH1D* x = (TH1D*)fd->Get("xDistrib");
	cout << (int)cuts->GetEntries() << ":" << (int)x->GetEntries() << endl;

	return 0;
}
