#ifndef HHbbtautauAnaElements_hxx
#define HHbbtautauAnaElements_hxx
#include <string>
#include <vector>
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"
#include "TLorentzVector.h"
#include "Math/Vector4D.h"
#include <thread>


constexpr double Pion_Mass = 0.13957; // The pion mass from the PDG (used as default mass hypothesis)
constexpr double Muon_Mass = -1.; // TODO: set value from PDG, perhaps also for electron and Kaon


//typedef ROOT::Math::PtEtaPhiM4D<double> R4Vec; 
template <typename T>
using R4Vec = ROOT::Math::PtEtaPhiM4D<T>; 


namespace Ana 
{

	template <typename T>
	inline R4Vec<T> computeP4(const T pt, const T eta, const T phi, const T m = Pion_Mass)   
	{
		R4Vec<T> P(pt, eta, phi, m); 
		return P; 
	}


	template <typename T>
	inline ROOT::VecOps::RVec<R4Vec<T> > computeP4Vec(const ROOT::VecOps::RVec<T>& pt, const ROOT::VecOps::RVec<T>& eta, const ROOT::VecOps::RVec<T>& phi, const ROOT::VecOps::RVec<T>& m)   
	{
		// Return a vector of P4 for collections with vector branches
		ROOT::VecOps::RVec<R4Vec<T> > P; 
		int n = pt.size(); 
		P.reserve(n); 

		assert(eta.size() == n); 
		assert(phi.size() == n); 
		assert(m.size() == n); 

		// Loop over the elements
		for (unsigned int i=0; i<n; i++) 
		{
			P.emplace_back(pt[i], eta[i], phi[i], m[i]); 
		}

		return P; 
	}


	template<typename T>
	inline ROOT::RDF::RNode* GetP4(ROOT::RDF::RNode *frame, TString prefix, TString name = "") 
	{
		//TLorentzVector P4; 
		std::string pfx = prefix.Data(); 
		//ROOT::RDF::RNode *extended 
		*frame = frame->Define(pfx+"_P4", computeP4Vec<T>, {pfx+"_pt", pfx+"_eta", pfx+"_phi", pfx+"_mass"});
		return frame; 
	}


	void AddColumn(ROOT::RDF::RNode* df, const std::string &newColName) {
    	*df = df->Define(newColName, [](){ return 42; });
	}


	// Making non-templated wrappwrs
  	//ROOT::VecOps::RVec< R4Vec<float> > computeP4Vec_f(const ROOT::VecOps::RVec<float>& pt, ...)
  	//{ return computeP4Vec<float>(pt, ...); }

  	//ROOT::VecOps::RVec< R4Vec<double> > computeP4Vec_d(const ROOT::VecOps::RVec<double>& pt, ...)
  	//{ return computeP4Vec<double>(pt, ...); }


}


// // --- Explicit instantiations for the types you will use ---
// // This ensures the symbol for computeP4Vec<float> and computeP4Vec<double> exists
// // (only necessary if you compile this into a shared library; harmless otherwise)	

// template ROOT::VecOps::RVec< R4Vec<float> > Ana::computeP4Vec<float>(const ROOT::VecOps::RVec<float>&,
// const ROOT::VecOps::RVec<float>&,
// const ROOT::VecOps::RVec<float>&,
// const ROOT::VecOps::RVec<float>&);	

// template ROOT::VecOps::RVec< R4Vec<double> > Ana::computeP4Vec<double>(const ROOT::VecOps::RVec<double>&,
// const ROOT::VecOps::RVec<double>&,
// const ROOT::VecOps::RVec<double>&,
// const ROOT::VecOps::RVec<double>&);	

// // Also instantiate GetP4 for float/double
// template ROOT::RDF::RNode* Ana::GetP4<float>(ROOT::RDF::RNode*, TString, TString);
// template ROOT::RDF::RNode* Ana::GetP4<double>(ROOT::RDF::RNode*, TString, TString);



#endif


