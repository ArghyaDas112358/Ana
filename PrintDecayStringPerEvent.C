#include "TChain.h"
#include "TFile.h"



void PrintDecayStringPerEvent() 
{
	TFile *file = TFile::Open("../../data/v5/BkgDstarDsstar.root", "READ"); 
	TTree *tree = static_cast<TTree*>(file->Get("ntuplizer/tree")); 

	std::string *decaystring1 = new std::string(); 
	std::string *decaystring2 = new std::string(); 
	std::string *decaystring3 = new std::string(); 

	tree->SetBranchAddress("pttau_tau_gen1str", &decaystring1); 
	tree->SetBranchAddress("pttau_tau_gen2str", &decaystring2); 
	tree->SetBranchAddress("pttau_tau_gen3str", &decaystring3); 

	int max = std::max(10, static_cast<int>(tree->GetEntries())); 
	for (int i=0; i<max; i++) 
	{
		tree->GetEntry(i);

		for (unsigned int i=0; i<45; i++) 
		{
			std::cout << "-"; 
		}
		std::cout << endl; 

		std::cout << *decaystring1 << std::endl; 
		std::cout << *decaystring2 << std::endl; 
		std::cout << *decaystring3 << std::endl; 
	}


	file->Close(); 
}

