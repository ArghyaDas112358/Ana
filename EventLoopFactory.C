#include "TChain.h"
#include "TFile.h"



class EventLoopFactory 
{
	public: 

	EventLoopFactory(TTree *tree, bool fulldecay = false, int maxnum = -1) : fTree(tree), fFullDecay(fulldecay), fMaxNum(maxnum) { Initialise(); }; 

	EventLoopFactory() = delete; 

	void Initialise() 
	{
		
		// Event string printing
		fTree->SetBranchAddress("pttau_tau_gen1str", &decaystring1); 
		fTree->SetBranchAddress("pttau_tau_gen2str", &decaystring2); 
		fTree->SetBranchAddress("pttau_tau_gen3str", &decaystring3); 
		fTree->SetBranchAddress("D0_genkstr", &decaystringK); 
		fTree->SetBranchAddress("D0_genpistr", &decaystringpi); 
		fTree->SetBranchAddress("Dstar_genpistr", &decaystringspi); 	

		fTree->SetBranchAddress("genstring", &decaystring); 	
	}

	void PrintEvents() // Event loop 
	{	
		int max = static_cast<int>(fTree->GetEntries()); 
		if (fMaxNum > 0) 
		{
			max = min(max, fMaxNum); 
		}
		for (int i=0; i<max; i++) 
		{
			fTree->GetEntry(i);	

			for (unsigned int i=0; i<45; i++) 
			{
				std::cout << "-"; 
			}
			std::cout << endl; 	

			// Start doing stuff per event
			PrintDecayString(); 
		}
		std::cout << std::endl << "Analyzed " << max << " events. " << std::endl; 
	}

	void PrintDecayString() 
	{
		if (fFullDecay) std::cout << *decaystring << std::endl; 	

		std::cout << "K: " << *decaystringK << std::endl; 
		std::cout << "pi: " << *decaystringpi << std::endl; 
		std::cout << "spi: " << *decaystringspi << std::endl; 
		std::cout << "pi1: " << *decaystring1 << std::endl; 
		std::cout << "pi2: " << *decaystring2 << std::endl; 
		std::cout << "pi3: " << *decaystring3 << std::endl; 
	}


	void Test() 
	{
		TFile *file = TFile::Open("../../data/v7/BkgDstarDsPart.root", "READ"); 
		fTree = static_cast<TTree*>(file->Get("ntuplizer/tree")); 	

		PrintDecayString(); 	
	

		file->Close(); 
	}


	TTree* fTree = nullptr; 
	bool fFullDecay = false; 
	int fMaxNum = -1; 

	// Variables for decay string printing
	std::string *decaystring = new std::string(); 
	std::string *decaystring1 = new std::string(); 
	std::string *decaystring2 = new std::string(); 
	std::string *decaystring3 = new std::string(); 
	std::string *decaystringK = new std::string(); 
	std::string *decaystringpi = new std::string(); 
	std::string *decaystringspi = new std::string(); 	

};

