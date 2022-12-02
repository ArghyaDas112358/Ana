#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include "FileFlow.h"
#include <TRandom3.h>
#include <TSystem.h>
#include <TPRegexp.h>
#include <TMVA/Reader.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


using namespace Ana; 



struct TMVAWeightfileVariables 
{
    std::vector<std::pair<TString, TString> > variables; 

    void load(const std::string& filename) 
    {
        boost::property_tree::ptree propertyTree; 

        boost::property_tree::read_xml(filename, propertyTree); 

        std::cout << "File name: " << filename << std::endl; 


        for (auto variable : propertyTree.get_child("MethodSetup.Variables")) 
        {
            if (variable.first == "<xmlattr>") continue; 
            // Loading the xml attributes Expression (variable name) and Type into the vector of pairs 
            variables.push_back(std::make_pair<TString, TString>(variable.second.get("<xmlattr>.Label", "").data(), variable.second.get("<xmlattr>.Type", "kNoType").data())); 
        }


    }
};


void AddMVAVariableSimple(const TString& inIdentifier, const TString& outIndentifier, const TString& weightfile = "newtest/model_optimized/weights.xml", const TString& branchName = "mvaScore", const TString& suffix = "_mva") 
{
	Init(); 

	filemanager.OpenItem(inIdentifier); 

	//TFile *inFile = TFile::Open(infile.Data(), "READ"); 

	TTree *tree = filemanager.GetItem<TTree*>(inIdentifier); 


	std::vector<TString> variables = {"b_D0_pt/F", "b_D0_eta/F", "b_D0_phi/F", "b_D0_vprob/F", "b_D0_fl/F", "b_D0_fls/F", "b_Ds_pt/F", "b_Ds_eta/F", "b_Ds_phi/F", "b_Ds_vprob/F", "b_Ds_fl/F", "b_Ds_fls/F", "b_D0_lip/F", "b_D0_lips/F", "b_D0_pvip/F", "b_Ds_lip/F", "b_Ds_lips/F", "b_Ds_pvip/F", "b_tau_pt/F", "b_tau_eta/F", "b_tau_phi/F", "b_tau_fl/F", "b_tau_fsig/F", "b_tau_vprob/F", "b_tau_lip/F", "b_tau_pvip/F", "b_tau_pvips/F", "b_tau_alpha/F", "b_tau_maxdr/F", "b_tau_pi1pt/F", "b_tau_pi1eta/F", "b_tau_pi1phi/F", "b_tau_pi2pt/F", "b_tau_pi2eta/F", "b_tau_pi2phi/F", "b_tau_pi3pt/F", "b_tau_pi3eta/F", "b_tau_pi3phi/F", "b_tau_sumdnn/F"}; 

	std::unique_ptr<TMVA::Reader> reader = std::make_unique<TMVA::Reader>("!Color:Silent"); 

	Int_t numVars = variables.size(); 

	Float_t floatVars[numVars]; 
    Int_t intVars[numVars]; 

    Float_t readerVars[numVars]; 

    float floats[numVars]; 
    Int_t ints[numVars]; 


    std::vector<int> varsToCast; 

	for (Long64_t i=0; i<numVars; i++) //auto variable : variables 
	{
		auto variable = variables.at(i); 

		auto components = variable.Tokenize("/"); 

		assert(components.GetEntries() == 2); 

		TString name = static_cast<TObjString*>(components->At(0))->GetString(); 
		TString type = static_cast<TObjString*>(components->At(1))->GetString(); 

		std::cout << "Variable: " << name << " of type: " << type << std::endl; 

		if (type == "F") 
		{ 
			tree->SetBranchAddress(name, &floats[i]); //&floatVars[i] &readerVars[i]
		}
		else if (type == "I") 
		{
			//auto vec = new std::vector<Int_t>(); 
			//ints.push_back(vec); 
			tree->SetBranchAddress(name, &ints[i]); //&intVars[i]
			varsToCast.push_back(i); 
		}
		else 
		{
			std::cerr << "ERROR: Unsupported type: " << type << " for variable " << name << std::endl; 
			return; 
		}



		reader->AddVariable(name, &readerVars[i]); 

	}

	reader->BookMVA("BDT", weightfile); 

	TFile *outFile = TFile::Open(filemanager.GetFile(outIndentifier).data(), "RECREATE"); // TODO: create directory structure 

	tree->SetBranchStatus("*", 1); 

	TTree *newTree = tree->CloneTree(0); 

	//newTree->SetDirectory(0); // Leaving it in memory 

    Float_t weightVar = 0.; 
    TBranch *branch = newTree->Branch(branchName, &weightVar, branchName+"/F"); 

	for (Long64_t i=0; i<tree->GetEntries(); i++) 
	{
		tree->GetEntry(i); 
		//for (auto index : varsToCast) 
		//{
		//	readerVars[i] = static_cast<Float_t>(intVars[i]); 
		//}
		for (auto index : varsToCast) 
		{
			floats[index] = static_cast<float>(ints[index]); 
		}
		//std::cout << "Size of vector: " << var1->size() << std::endl; 
		//std::cout << "Var 1: " << var1->at(0) << std::endl; 

		//std::cout << "Size of branch vector: " << floats.at(0)->size() << std::endl; 
		//std::cout << "Value: " << floats.at(0)->at(0) << std::endl; 

		/*for (int idx=0; idx<varsToCast.size(); idx++) 
		{
			auto index = varsToCast.at(idx); 
			auto vec = ints.at(idx); 
			//delete floats[index]; 
			//floats[index] = std::vector<Float_t>(vec->begin(), vec->end()); // Cast the vector of int to vector of float 
			std::cout << "Size of int vector: " << vec->size() << std::endl; 
		}*/
		//readerVars[i] = floatVars[i]; 

		// CRAZYYYYY hack fix this bullcrap! 
		//assert(floats.size() == numVars); 
		//std::cout << "Vector size: " << floats.size() << " num float: " << numVars << std::endl; 
		for (int i=0; i < numVars; i++) 
		{
			//assert(floats.at(i)->size()>0); 
			//std::cout << "Size of variable vector: " << floats.at(i)->size() << std::endl; 
			readerVars[i] = floats[i]; 
		}

		weightVar = reader->EvaluateMVA("BDT"); 

		newTree->Fill(); 

	}

	outFile->Write(); 
	outFile->Close(); 

	//inFile->Close(); 

	// Clean up memory 
}

