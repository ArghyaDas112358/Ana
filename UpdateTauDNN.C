#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "/Users/mhuwiler/coding/plugins/FileManager/CFileManager.C"
ClassImp(FileManager)
#include "TLorentzVector.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TLegend.h"
#include <iostream>
#include "DrawTMVAHistogram.C"
#include "GetSeparation.C"
//#include "Python.h"
//#include "TPython.h"
#include "Tau.h"


using namespace ROOT; 


// To run this macro, the python class TFEvaluation.py needs to be loaded into root prior to execution. e.g.:
// root -e 'TPython::LoadMacro("TFEvaluation.py");' ApplyTFweight.C


void PauseUntilAnyKey() 
{
	std::cout << "Press any key to continue... " << std::endl; 
	std::cin.get(); 
}

void PauseUntilEnter() 
{
	std::cout << "Press 'enter' to continue..." << std::endl;
	std::cin.ignore(); 
	//std::cin.ignore(std::numeric_limits<streamsize>::max(),'\n'); // #include <limits>
}

void Pause(Int_t timeInSec) 
{
	// Better way, taken from: https://stackoverflow.com/questions/23609507/pause-program-execution-for-5-seconds-in-c
	#include <chrono>
	#include <thread>
	//std::this_thread::sleep_for(static_cast<std::chrono::seconds>(timeInSec));
	sleep(timeInSec); 
}

TLorentzVector LV(double pt, double eta, double phi, double m) 
{
	TLorentzVector V; 
	V.SetPtEtaPhiM(pt, eta, phi, m); 
	return V; 
}

std::vector<float> EvaluateTFresponse(std::vector<float> pt, std::vector<float> eta, std::vector<float> phi, std::vector<float> q, std::vector<float> DOCA2D, std::vector<float> DOCA2DErr, std::vector<float> DOCA3D, std::vector<float> DOCA3DErr, std::vector<float> dzToPV, std::vector<float> dzToClosest, std::vector<float> isAssociate, std::vector<float> assocQualityToPV, std::vector<int> genmatch) 
{
	std::vector<float> response; 


	response.push_back(-999.); 
	return response; 
}

std::vector<float>  extendArray(const std::vector<float>& array, const int dim) 
{
	std::vector<float> result; 
	result.reserve(dim); 
	if (array.size() > dim) 
	{
		result = std::vector<float>(array.begin(), array.begin()+dim); 
	}
	else 
	{
		result = array; 
		while(result.size() < dim)
		{
			result.push_back(0.); 
		}
	}
	return result; 
}

// TODO: function that does the same inplace 

template<typename T>
void PrintArray(const std::vector<T>& vec) 
{
	std::cout << "Vector content: "; 
	for (auto i : vec) 
	{
		std::cout << i << ", "; 
	}
	std::cout << std::endl; 
}

std::vector<float> concatenateVectors(const std::vector<std::vector<float>* > vectors) 
{
	std::vector<float> result; // TODO: reserve the size 
	for (auto vec : vectors) 
	{
		result.insert(result.end(), vec->begin(), vec->end()); 
	}
	return result; 
}

void extendArray(std::vector<float>*& array, const int dim, std::vector<std::vector<float>* >& garbageCollector) 
{
	if (array->size() > dim) 
	{
		array = new std::vector<float>(array->begin(), array->begin()+dim); 
		garbageCollector.push_back(array); 
	}
	else 
	{
		array->insert(array->end(), dim - array->size(), 0.); 
	}
}

std::vector<double> castVector(std::vector<float> vec) 
{
	std::vector<double> result; 
	result.reserve(vec.size()); 
	for (auto element : vec)
	{
		result.push_back(static_cast<double>(element)); 
	}
	return result; 
}


ROOT::VecOps::RVec<float> FillTauDNNscore(ROOT::VecOps::RVec<int> trackIndices, std::vector<float> trackScores) 
{
	std::vector<float> responses; 
	for (auto index : trackIndices) 
	{
		float dnnScore = -1; 
		if (index < trackScores.size()) 
		{
			dnnScore = trackScores.at(index); 
		}
		else 
		{
			std::cout << "WARNING: index out of range! " << std::endl; 
		}
		responses.push_back(dnnScore); 
	}
	assert(responses.size() == trackIndices.size()); 
	return responses; 
}


std::vector<float> FillSumDNN(ROOT::VecOps::RVec<float> pion1_dnn, ROOT::VecOps::RVec<float> pion2_dnn, ROOT::VecOps::RVec<float> pion3_dnn) 
{
	std::vector<float> response; 
	const int dim = pion1_dnn.size(); 
	assert(pion2_dnn.size() == dim); 
	assert(pion3_dnn.size() == dim); 
	for (unsigned int i=0; i<dim; i++) 
	{
		response.push_back(pion1_dnn.at(i)+pion2_dnn.at(i)+pion3_dnn.at(i)); 
	}
	return response; 
}


struct Basictau 
{
	float pt; 
	float eta; 
	float phi; 
	int q; 
	float m; 
	int idx1;
	int idx2; 
	int idx3; 
	float sumdnn; 
	float dnn1; 
	float dnn2; 
	float dnn3; 

}; 


std::vector<Tau> SelectTauCandidate(ROOT::VecOps::RVec<float> taupt, ROOT::VecOps::RVec<float> taueta, ROOT::VecOps::RVec<float> tauphi) 
{
	std::vector<Tau> mytaus; 
	for (unsigned int i=0; i<taupt.size(); i++) 
	{
		Tau tau; 
		tau.pt = taupt.at(i); 

		mytaus.push_back(tau); 
	}
	return mytaus; 
}

 

void UpdateTauDNN(TString campaignName = "ApplyTFweight/") 
{
	//ROOT::EnableImplicitMT(); //ROOT::DisableImplicitMT(); 
	//gROOT->LoadMacro("/Users/mhuwiler/coding/plugins/FileManager/CFileManager.C"); 
	//gROOT->LoadMacro("/Users/mhuwiler/coding/plugins/FileManager/CFileManager.C+");
	//gSystem->Load("/Users/mhuwiler/coding/plugins/FileManager/CFileManager.so"); 

	FileManager filemanager; 


	filemanager.AddItem("prodlatest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/flatTupleDataLatest.root", "ntuplizer/tree"); 
	filemanager.AddItem("MCgenmatched", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/flatTupleGenmatchedAllSingleTauLatest.root", "ntuplizer/tree"); 

	filemanager.AddItem("MCOfficialSample", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/all.root", "ntuplizer/tree"); 
	filemanager.AddItem("DstarDsMCfirst", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/bkgDstarDsFirst.root", "ntuplizer/tree"); 
	filemanager.AddItem("DataWS", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/DataWS.root", "ntuplizer/tree"); 
	filemanager.AddItem("DataLatest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/DataLast.root", "ntuplizer/tree"); 
	filemanager.AddItem("MCofficial", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/MCFirstSubmission.root", "ntuplizer/tree"); 
	filemanager.AddItem("DataLarge", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/DataVeryLarge.root", "ntuplizer/tree"); 

	// For ABCD estimation
	filemanager.AddItem("DataLargeMVAfirst", "/eos/home-m/mhuwiler/data/Analysis/v9/DataVeryLarge_mva.root", "tree"); 
	filemanager.AddItem("DataLargeMVA", "/eos/home-m/mhuwiler/data/Analysis/v9/DataVeryLarge_mvaxgb.root", "tree"); 
	filemanager.AddItem("DataLargeMVASimple", "/eos/home-m/mhuwiler/data/Analysis/v9/DataVeryLarge_converted_mvaxgbsimple.root", "tree"); 
	filemanager.AddItem("MCvalidation", "/eos/home-m/mhuwiler/data/Analysis/v9/TauCutflowSample_mva.root", "tree"); 
	filemanager.AddItem("signalTrain", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/trainingLarge/plots.root", "signalTrain"); 
	filemanager.AddItem("signalTest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/trainingLarge/plots.root", "signalTest"); 
	filemanager.AddItem("backgroundTrain", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/trainingLarge/plots.root", "backgroundTrain"); 
	filemanager.AddItem("backgroundTest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/trainingLarge/plots.root", "backgroundTest"); 
	filemanager.AddItem("DataBackground", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/DataVeryLarge_converted.root", "tree"); 
	filemanager.AddItem("MCSignal", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/PrivateProductionGenDstar_converted.root", "tree"); 
	filemanager.AddItem("SignalOfficialMC50M", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M.root", "ntuplizer/tree"); 
	filemanager.AddItem("ParkingBPHAllRun2018B", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted.root", "tree"); 
	filemanager.AddItem("MCSignalMMultipleTau", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstAllTau.root", "ntuplizer/tree"); 
	filemanager.AddItem("MCSignalMMultipleTauWithResponse", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_withTFweight.root", "ntuplizer/tree"); 



	filemanager.OpenAllItems(); 

	gStyle->SetOptStat(0); 


	auto dataframe = RDataFrame(*filemanager.GetItem<TTree*>("MCSignalMMultipleTauWithResponse")); // tree100k
	

	// Defining the delta
	auto P = [](double pt, double eta, double phi, double m) {
		TLorentzVector V; 
		V.SetPtEtaPhiM(pt, eta, phi, m); 
		return V; 
	}; 

	auto P_v = [](std::vector<float>& pt, std::vector<float>& eta, std::vector<float>& phi, std::vector<float>& m) {
		size_t vecSize = pt.size(); 
		assert(eta.size() == vecSize and phi.size() == vecSize and m.size() == vecSize); 
		std::vector<TLorentzVector> result; 
		for (unsigned int i=0; i<vecSize; i++) 
		{
			TLorentzVector V; 
			V.SetPtEtaPhiM(pt.at(i), eta.at(i), phi.at(i), m.at(i)); 
			result.push_back(V); 
		}
		return result; 
	}; 

	auto invMass = [](TLorentzVector V1, TLorentzVector V2) {
		auto V = V1 + V2; 
		return V.M(); 
	}; 

	auto invMass_v = [](std::vector<TLorentzVector>& V1, std::vector<TLorentzVector>& V2) {
		assert(V1.size() == V2.size()); 
		std::vector<float> result; 
		std::cout << "Size of branch (vector): " << V1.size() << std::endl; 
		for (unsigned int i=0; i<V1.size(); i++) 
		{
			auto V = V1.at(i) + V2.at(i); 
			result.push_back(V.M()); 
		}
		return result; 
	}; 

	//auto invMass_v2 = [invMass](std::vector<TLorentzVector>& V1, std::vector<TLorentzVector>& V2) {
	//	assert(V1.size() == V2.size()); 
	//	std::vector<float> result; 
	//	for (unsigned int i=0; i<V1.size(); i++) 
	//	{
	//		result.push_back(std::invoke(invMass, V1, V2)); 
	//	}
	//	return result; 
	//}; 

	auto mass = [](TLorentzVector& V) {
		return V.M(); 
	}; 

	// overloading lambdas: if constexpr (std::is_same_v<T, int>) 

	//auto frame2 = dataframe.Define("P_D0", P_v, {"BsDstarTauNu_D0_pt", "BsDstarTauNu_D0_eta", "BsDstarTauNu_D0_phi", "BsDstarTauNu_D0_mass"}) // auto frame2 = dataframe.Define("LV_D0", "TLorentzVector LV_D0; LV_D0.SetPtEtaPhiM(BsDstarTauNu_D0_pt, BsDstarTauNu_D0_eta, BsDstarTauNu_D0_phi, BsDstarTauNu_D0_mass); return LV_D0"); 
	//				.Define("P_Ds", P_v, {"BsDstarTauNu_Ds_pt", "BsDstarTauNu_Ds_eta", "BsDstarTauNu_Ds_phi", "BsDstarTauNu_Ds_mass"})
	//				.Define("P_tau", P_v, {"BsDstarTauNu_tau_pt", "BsDstarTauNu_tau_eta", "BsDstarTauNu_tau_phi", "BsDstarTauNu_tau_mass"})
	//				.Define("B_mass", invMass_v, {"P_Ds", "P_tau"}); 


	//auto histo1 = frame2.Histo1D("B_mass"); 

	//auto histo2 = frame2.Histo2D({"Bmass_vs_Dmass", "Correlation plot between B and D masses", 100, 0., 7000., 100, 0., 5000.}, "BsDstarTauNu_B_mass", "BsDstarTauNu_D0_unfit_mass"); 

	auto TFresponse = [](std::vector<float> Dstarpt, std::vector<float> Dstareta, std::vector<float> Dstarphi, std::vector<float> Dstarcharge, std::vector<float> pt, std::vector<float> eta, std::vector<float> phi, std::vector<float> q, std::vector<float> DOCA2D, std::vector<float> DOCA2DErr, std::vector<float> DOCA3D, std::vector<float> DOCA3DErr, std::vector<float> dzToPV, std::vector<float> dzToClosest, std::vector<float> isAssociate, std::vector<float> assocQualityToPV, std::vector<int> genmatch) 
	{
		std::vector<float> response; 

		return response; 
	};

	auto withDNN = dataframe.Define("b_tau_dnn_1", FillTauDNNscore, {"BsDstarTauNu_tau_pfidx1", "TFscore"}).Define("b_tau_dnn_2", FillTauDNNscore, {"BsDstarTauNu_tau_pfidx2", "TFscore"}).Define("b_tau_dnn_3", FillTauDNNscore, {"BsDstarTauNu_tau_pfidx3", "TFscore"}); 

	withDNN = withDNN.Define("b_tau_sumdnn", FillSumDNN, {"b_tau_dnn_1", "b_tau_dnn_2", "b_tau_dnn_3"}); 

	withDNN = withDNN.Define("b_taucandidates", SelectTauCandidate, {"BsDstarTauNu_tau_pt", "BsDstarTauNu_tau_eta", "BsDstarTauNu_tau_phi"}); 

	withDNN.Snapshot("ntuplizer/tree", "../../data/SignalOfficialMC50M_tauDNN.root"); 

	//Pause(5); 

	//PauseUntilEnter(); //system("pause"); 

	filemanager.CloseAll(); 


}

