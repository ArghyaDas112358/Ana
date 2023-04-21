#include "TChain.h"
#include "TFile.h"



void PrintDecayStringPerEvent() 
{
	TFile *file = TFile::Open("../../data/v5/BkgDstarDsstar.root", "READ"); 
	TTree *tree = static_cast<TTree*>(file->Get("ntuplizer/tree")); 

	std::string *decaystring = new std::string(); 

	tree->SetBranchAddress("pttau_tau_gen1str", &decaystring); 

	int max = std::max(10, static_cast<int>(tree->GetEntries())); 
	for (int i=0; i<max; i++) 
	{
		tree->GetEntry(i);

		std::cout << *decaystring << std::endl; 
	}


	file->Close(); 
}

