#ifndef HHbbtautauAnaElements_hxx
#define HHbbtautauAnaElements_hxx
#include <string>
#include <vector>
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"
#include "TLorentzVector.h"
#include "Math/Vector4D.h"
#include <thread>
#include "Particle.h"
#include "FileFlow.h"


constexpr double Pion_Mass = 0.13957; // The pion mass from the PDG (used as default mass hypothesis)
constexpr double Muon_Mass = -1.; // TODO: set value from PDG, perhaps also for electron and Kaon

// TODO: 
// - implement autoblacklist
// - do not hardcode names, write all initial names and final names for collections
// - test templated P4 TLorentzVector functions
// - separate into NanoBuildingBlocks.C and HHbbtautauAnaElements.C


//typedef ROOT::Math::PtEtaPhiM4D<double> R4Vec; 
template <typename T>
using R4Vec = ROOT::Math::PtEtaPhiM4D<T>; 


namespace Ana 
{
	extern std::unordered_map<std::string, int> autoblacklist; 

	std::unordered_map<std::string, int> PDGid = { 
		{ "Muon", 13 }, 
		{ "Electron", 11 }, 
		{ "Tau", 15 }, 
		{ "Higgs", 25 }, 
		{ "b", 5 }, 
		{"Pi", 211}, 
	}; 

	template<typename T>
	int VecSize(ROOT::VecOps::RVec<T> vec)
	{
		return vec.size(); 
	}


	Particle computeP4(const double pt, const double eta, const double phi, const double m = Pion_Mass)   
	{
		Particle P(pt, eta, phi, m, 15); 
		return P; 
	}


	ROOT::VecOps::RVec<Particle> computeP4Vec(const ROOT::VecOps::RVec<float>& pt, const ROOT::VecOps::RVec<float>& eta, const ROOT::VecOps::RVec<float>& phi, const ROOT::VecOps::RVec<float>& m, const ROOT::VecOps::RVec<int>& pdgid = {-999})   
	{
		// Return a vector of P4 for collections with vector branches
		ROOT::VecOps::RVec<Particle> P; 
		int n = pt.size(); 
		P.reserve(n); 

		assert(eta.size() == n); 
		assert(phi.size() == n); 
		assert(m.size() == n); 

		// Loop over the elements
		for (unsigned int i=0; i<n; i++) 
		{
			P.emplace_back(pt[i], eta[i], phi[i], m[i], pdgid[i]); 
		}

		return P; 
	}


	//template<typename T>
	ROOT::RDF::RNode GetP4(ROOT::RDF::RNode frame, TString prefix, TString name = "") 
	{
		//TLorentzVector P4; 
		std::string pfx = prefix.Data(); 
		//ROOT::RDF::RNode *extended 
		frame = frame.Define(pfx+"_P4", computeP4Vec, {pfx+"_pt", pfx+"_eta", pfx+"_phi", pfx+"_mass", pfx+"_pdgId"});
		//*frame = frame->Define(pfx+"_P4", computeP4Vec<T>, {pfx+"_pt", pfx+"_eta", pfx+"_phi", pfx+"_mass"});
		return frame; 
	}


	/*std::vector<std::string>& purgeColumns(std::vector<std::string> &&columns, const std::vector<std::string>& blacklist)
	{
   			// a lambda that checks if `s` is in the blacklist
   			auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };

   			// removing elements from std::vectors is not pretty, see https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
   			columns.erase(std::remove_if(columns.begin(), columns.end(), is_blacklisted), columns.end());

   			return columns; 
	}*/

	
	ROOT::RDF::RNode GetGenParticleCollection(ROOT::RDF::RNode frame, const std::string genprefix = "GenPart") 
	{
		auto columnNames = frame.GetColumnNames(); 
		if (std::find(columnNames.begin(), columnNames.end(), genprefix+"_Particle") == columnNames.end()) 
		frame = frame.Define(genprefix+"_Particle", computeP4Vec, {genprefix+"_pt", genprefix+"_eta", genprefix+"_phi", genprefix+"_mass", genprefix+"_pdgId"}); 
		autoblacklist[genprefix+"_Particle"]++; 
		return frame; 
	}


	ROOT::RDF::RNode GetGenParticles(ROOT::RDF::RNode frame, const int id, const std::string name, const std::string genprefix = "GenPart") 
	{
		frame = GetGenParticleCollection(frame, genprefix); 

		auto FilterParticles = [id](ROOT::VecOps::RVec<Particle> particles) 
		{
			ROOT::VecOps::RVec<Particle> result; 
			result.reserve(particles.size()); 
			std::copy_if(particles.begin(), particles.end(), std::back_inserter(result), [&](const Particle& particle){ return abs(particle.pdgid) == id; }); // Copy particles where the pdgid matches the requirement
			return result; 
		}; 

		frame = frame.Define(name, FilterParticles, {genprefix+"_Particle"}); 
		return frame; 
	}


	ROOT::RDF::RNode GetGenParticles(ROOT::RDF::RNode frame, const std::string prefix, const std::string genprefix = "GenPart") 
	{
		const int id = PDGid[prefix]; 
		const std::string name = "Gen"+prefix; 
		autoblacklist[name]++; 
		return GetGenParticles(frame, id, name, genprefix); 
	}

	Particle IdentifyGenMuon(ROOT::RDF::RNode *frame, const std::string value = "") 
	{
		/*for (auto particle : genParticles) 
		{
			if (particle.pdgid == PDGid["Muon"]) 
			{
				// Could be the gen muon from Taumu

			}
		}*/
	}


	int getMother(const ROOT::VecOps::RVec<float>& mother, int particle) 
	{
		return mother[particle]; 
	}


	bool isDescendantOf(int descendant, int ancestor, const ROOT::VecOps::RVec<float>& mothers) 
	{

		bool particleFound = false; 

		int particle = descendant; 

		while (particle != ancestor)
		{
			particle = mothers[particle]; 

			if (particle < 0) return false; 
		}

		return true; 
	}


	bool isAncestor(const int ancestor, const int descendant, const ROOT::VecOps::RVec<float>& mothers)
	{
	  	return isDescendantOf(descendant, ancestor, mothers); 
	}


	bool isLastCopy(int flags) 
	{
    	return flags & (1u << 13);
	}

	bool isHardProcess(int flag) 
	{
		return flag & (1u << 7); 
	}

	bool fromHardProcess(int flag) 
	{
		return flag & (1u << 8); 
	}


	std::vector<int> findMothers(int particle, int motherId, const ROOT::VecOps::RVec<float>& id, const ROOT::VecOps::RVec<float>& mothers, const ROOT::VecOps::RVec<int>& statusFlags, const int flag = -999.) 
	{
		int currentId = 0; 

		std::vector<int> results; 

		while (particle > 0 ) // put here >= 0 ?
		{
			particle = mothers[particle]; 

			bool flagOK = isLastCopy(statusFlags[particle]); 
			if (flag > 0 ) flagOK = (statusFlags[particle] & flag); 
			if ((abs(id[particle]) == motherId) && flagOK) results.push_back(particle); 
		}

		return results; 
	}

	std::vector<int> findMothers(int particle, std::string motherType, const ROOT::VecOps::RVec<float>& id, const ROOT::VecOps::RVec<float>& mothers, const ROOT::VecOps::RVec<int>& statusFlags, const int flag = -999.) 
	{
		int motherId = PDGid[motherType]; 

		return findMothers(particle, motherId, id, mothers, statusFlags, flag); 
	}


	std::vector<int> findDescendants(int particle, std::string descendantType, const ROOT::VecOps::RVec<float>& id, const ROOT::VecOps::RVec<float>& mothers, const ROOT::VecOps::RVec<int>& statusFlags, const int flag = -999.) 
	{
		int descendantId = PDGid[descendantType]; 
		std::vector<int> descendants; 

		//std::cout << "Sizes: " << id.size() << " " << mothers.size() << " " << statusFlags.size() << std::endl; 
		assert(id.size() == mothers.size()); 
		assert(id.size() == statusFlags.size()); 
		for (unsigned int i=0; i<id.size(); i++) 
		{
			//std::cout << "mother: " << mothers.at(i) << std::endl; 
			if (abs(id[i]) != PDGid[descendantType]) continue; 
			// check flag
			auto possibleMothers = findMothers(i, id[particle], id, mothers, statusFlags); 
			if (std::find(possibleMothers.begin(), possibleMothers.end(), particle) != possibleMothers.end()) 
			{
				descendants.push_back(i); 
			}
		}

		return descendants; 
	}


	/* ChatGPT on status flags
	0  : isPrompt
	1  : isDecayedLeptonHadron
	2  : isTauDecayProduct
	3  : isPromptTauDecayProduct
	4  : isDirectTauDecayProduct
	5  : isDirectPromptTauDecayProduct
	6  : isDirectHadronDecayProduct
	7  : isHardProcess
	8  : fromHardProcess
	9  : isHardProcessTauDecayProduct
	10 : isDirectHardProcessTauDecayProduct
	11 : fromHardProcessBeforeFSR
	12 : isFirstCopy
	13 : isLastCopy     ← THIS ONE
	14 : isFirstCopyBeforeFSR
	*/


	int DecayGenMatching(const ROOT::VecOps::RVec<float>& id, const ROOT::VecOps::RVec<float>& mother, const ROOT::VecOps::RVec<int>& statusFlag) 
	{

		// statusFlags bit helpers (bit numbers, zero-indexed)
    	//const unsigned int BIT_isLastCopy = (1u << 13);         // 13 => isLastCopy
    	const unsigned int BIT_isDirectTauDecayProduct = (1u << 5); // 5 => isDirectTauDecayProduct (useful)


    	std::vector<int> muons; 
    	muons.reserve(2); 
    	std::vector<int> taus; 
    	taus.reserve(2); 
    	std::vector<int> Higgses; 
    	Higgses.reserve(2); 
    	std::vector<int> bs; 
    	bs.reserve(2); 
    	std::vector<int> electrons; 
    	electrons.reserve(2); 

    	// find the H->bb candidate

    	std::cout << "Sizes: " << id.size() << " " << mother.size() << " " << statusFlag.size() << std::endl; 

		for (unsigned int i=0; i<id.size(); i++) 
		{
			if ((abs(id[i]) == PDGid["Muon"]) && (isLastCopy(statusFlag[i]))) 
			{
				// Might be the muon
				auto localtaus = findMothers(i, "Tau", id, mother, statusFlag); 

				if (localtaus.size() < 1 ) continue; 

				const unsigned int hardProcess = (1u << 7); 
				auto localHiggses = findMothers(i, "Higgs", id, mother, statusFlag, hardProcess); 

				std::cout << "N taus: " << taus.size() << ", N Higgses: " << Higgses.size() << std::endl; 

				if (localHiggses.size() < 1) continue; 


				std::vector<int> otherTaus = findDescendants(localHiggses[0], "Tau", id, mother, statusFlag); 
				for (auto element : otherTaus) 
				{
					std::string text = ""; 
					for (auto it = PDGid.begin(); it != PDGid.end(); it++) 
					{
						if (it->second == element) text = it->first; 
					}
					std::cout << text << ": " << id[element] << " (id), " << mother[element] << " (mother), " << statusFlag[element] << " (status)" << std::endl; 
				}
				//otherTaus.erase(std::remove(otherTaus.begin(), otherTaus.end(), taus.at(0)), otherTaus.end()); // Remove the muonic tau
				/*bool notTauh = false; 
				for (unsigned int j=0; j<otherTaus.size(); j++) // Make sure the other tau decay is not electronic
				{
					if (findDescendants(otherTaus[j], "Electron", id, mother, statusFlag).size() > 0) // If we find an electron in the other tau decay
					{
						notTauh = true; 
					}
				}

				if (notTauh) continue; */



			}
		}

		return 1; 
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


