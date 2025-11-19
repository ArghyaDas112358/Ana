#ifndef HHbbtautauAnaElements_hxx
#define HHbbtautauAnaElements_hxx
#include <string>
#include <vector>
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"
#include "TLorentzVector.h"
#include <thread>


constexpr double Pion_Mass = 0.13957; // The pion mass from the PDG (used as default mass hypothesis)
constexpr double Muon_Mass = -1.; // TODO: set value from PDG, perhaps also for electron and Kaon


namespace Ana 
{

	TLorentzVector computeP4(double pt, double eta, double phi, double m = Pion_Mass)   
	{
		TLorentzVector P; 
		P.SetPtEtaPhiM(pt, eta, phi, m); 
		return P; 
	}


	ROOT::VecOps::RVec<TLorentzVector> computeP4Vec(ROOT::VecOps::RVec<double> pt, ROOT::VecOps::RVec<double> eta, ROOT::VecOps::RVec<double> phi, ROOT::VecOps::RVec<double> m)   
	{
		// Return a vector of P4 for collections with vector branches
		ROOT::VecOps::RVec<TLorentzVector> P; 
		int n = pt.size(); 
		P.reserve(n); 

		assert(eta.size() == n); 
		assert(phi.size() == n); 
		assert(m.size() == n); 

		// Loop over the elements
		for (unsigned int i=0; i<n; i++) 
		{
			P[i].SetPtEtaPhiM(pt[i], eta[i], phi[i], m[i]); 
		}

		return P; 
	}


	ROOT::RDF::RNode* GetP4(ROOT::RDF::RNode *frame, TString prefix, TString name = "") 
	{
		//TLorentzVector P4; 
		std::string pfx = prefix.Data(); 
		//ROOT::RDF::RNode *extended 
		*frame = frame->Define(pfx+"_P4", computeP4Vec, {pfx+"_pt", pfx+"_eta", pfx+"_phi", pfx+"_mass"});
		return frame; 
	}

	void AddColumn(ROOT::RDF::RNode* df, const std::string &newColName) {
    	*df = df->Define(newColName, [](){ return 42; });
	}

}

#endif


