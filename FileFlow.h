#ifndef FileFlow_h
#define FileFlow_h
#include "plugins/FileManager/CFileManager.C"
#include <string>
#include "TString.h"
#include "TObjString.h"
#include "TCut.h"
#include <unordered_map>
#include "SampleData.C"
#include "ROOT/RDataFrame.hxx"
//#include <json/value.h>
//#include <json/json.h>
//R__ADD_LIBRARY_PATH($FOODIR) // if needed
//R__LOAD_LIBRARY(/opt/local/lib/libjsoncpp.dylib) // Load the library
#include "external/jsoncpp/dist/jsoncpp.cpp"
#include <fstream>
#include "AnaBuildingBlocks.C"


namespace Ana 
{

	FileManager filemanager; 

	TString folder = "";

	std::map<std::string, TCut> cut; //std::map<std::string, std::map<std::sting, TCut> > cuts; 

	//std::vector<TColor*> colors; 

	//std::vector<Int_t> mycolors; 

	//std::unordered_map<std::string, Int_t> color; 

	std::unordered_map<std::string, Int_t> colorold; 

	std::unordered_map<std::string, std::string> model; 

	std::unordered_map<std::string, std::string> MVA; 

	std::unordered_map<std::string, std::string> FinalBDT; 

	//std::unordered_map<std::string, std::string> legends; 

	std::unordered_map<std::string, ROOT::RDF::TH1DModel> binning; 

	//std::unordered_map<std::string, std::string> labels; 

	RedirectingMap<std::string, SampleData> samples; 


	std::map<std::string, TCut> cutstandalone;


	// Constants being defined centrally 
	Double_t mvaCutSR = 0.7; // TODO: make cuts per version 
	Double_t mvaCutSB = -0.1; 
	Double_t mvaCutCR = -0.5; 


	void CommonInitialisation(const bool denominator)
	{
	
		colorold = {{"Sig", 2}, {"BkgDstarDs", 3}, {"BkgDstarDsstar", 8}, {"BkgDstara1", 4}, {"dataD2WS", 6}, {"dataD2TauWS", 7}, {"other", 9}, {"yetanother", 1}}; // Legacy color scheme 


		Int_t nBins = 20; 

		if (!denominator) 
		{
			binning = {{"b_tau_rhomass1", {"", "#rho_{12} mass;Invariant m_{#rho} [GeV];Counts", nBins, 0., 1.5}},
				{"b_tau_rhomass2", {"", "rho_{23} mass;Invariant m_{#rho} [GeV];Counts", nBins, 0., 1.5}},
				{"b_B_m", {"", "B mass;Reconstructed m_{B} [GeV];Counts", nBins, 2., 6.}},
				{"b_B_q2", {"", "q2;q^{2} [GeV];Counts", nBins, 0., 12.}},
				//{"B_m", {"", ";B mass [GeV];Counts", nBins, 0., 6.}},
				//{"B_q2", {"", ";B mass [GeV];Counts", nBins, 0., 12.}},
				//{"tau_rhomass1", {"", ";#rho_{12} mass [GeV];Counts", nBins, 0., 1.5}},
				//{"tau_rhomass2", {"", ";#rho_{12} mass [GeV];Counts", nBins, 0., 1.5}},
				{"b_B_proper_xi_rho1", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_B_proper_xi_rho2", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_alpha_rho1_pi", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_alpha_rho2_pi", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_theta_rho1", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_theta_rho2", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
			}; 
		}
		else // denominator
		{
			nBins = 50;
			std::vector<double> bins = {4.5, 4.6, 4.7, 4.8, 4.9, 5., 5.07, 5.15, 5.2, 5.21, 5.22, 5.23, 5.24, 5.25, 5.26, 5.27, 5.28, 5.29, 5.3, 5.31, 5.32, 5.33, 5.34, 5.35, 5.36, 5.37, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.};
			binning = {{"b_tau_rhomass1", {"", "#rho_{12} mass;Invariant m_{#rho} [GeV];Counts", nBins, 0., 1.5}},
				{"b_tau_rhomass2", {"", "rho_{23} mass;Invariant m_{#rho} [GeV];Counts", nBins, 0., 1.5}},
				//{"b_B_m", {"", "B mass;Reconstructed m_{B} [GeV];Counts", nBins, 4.5, 6.}},
				{"b_B_m", {"", "B mass;Reconstructed m_{B} [GeV];Counts", static_cast<int>(bins.size()-1), bins.data()}},
				{"b_B_q2", {"", "q2;q^{2} [GeV];Counts", nBins, 0., 12.}},
				{"b_tau_m", {"", "\tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, 1., 3.}},
				//{"B_m", {"", ";B mass [GeV];Counts", nBins, 0., 6.}},
				//{"B_q2", {"", ";B mass [GeV];Counts", nBins, 0., 12.}},
				//{"tau_rhomass1", {"", ";#rho_{12} mass [GeV];Counts", nBins, 0., 1.5}},
				//{"tau_rhomass2", {"", ";#rho_{12} mass [GeV];Counts", nBins, 0., 1.5}},
				{"b_B_proper_xi_rho1", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_B_proper_xi_rho2", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_alpha_rho1_pi", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_alpha_rho2_pi", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_theta_rho1", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
				{"b_tau_proper_theta_rho2", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", nBins, -1.1, 1.1}},
			}; 
		}

		// Complete binning with default
		std::unordered_map<std::string, ROOT::RDF::TH1DModel> defaultbinning = { 
			#include "VariableDefinitions.ccf" 
		}; 

		std::vector<std::string> excluded; 
		if (denominator) 
		{
			excluded = {"b_tau_dnn1", "b_tau_dnn2","b_tau_dnn3","b_tau_sumdnn"};
		}

		for (auto item : defaultbinning) 
		{
			if ((binning.find(item.first) == binning.end()) && (std::find(excluded.begin(), excluded.end(), item.first) == excluded.end())) 
			{
				binning[item.first] = item.second; 
			}
		}

		samples = InitSamples(); 

		// Filling the versions of final MVA discriminator
		FinalBDT = {	
			{"v6.9", "./anaMVA/MVAinSBdata/model_optimized/weights.xml"}, //LatestVsData
			{"v6.95", "./anaMVA/MVAinSBdata/model_optimized/weights.xml"} //LatestVsData
		}; 

		//if (regions != "") LoadRegions(regions); 

	}


	void Init(const TString& cycle = "", const bool denominator = false) 
	{
		if (cycle != "") // For legacy purpose 
		{

			folder = "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/"+cycle+"/"; 

			filemanager.AddItem("Sig_ntuple", folder+"Sig.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig_tf", folder+"Sig_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig_DNN", folder+"Sig_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig", folder+"Sig_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigTrain_ntuple", folder+"SigTrain.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain_tf", folder+"SigTrain_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain_DNN", folder+"SigTrain_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain", folder+"SigTrain_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigTest_ntuple", folder+"SigTest.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTest_tf", folder+"SigTest_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTest_DNN", folder+"SigTest_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTest", folder+"SigTest_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigPart_ntuple", folder+"SignalOfficialMC100MTestNoGenMatch.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart_tf", folder+"SignalOfficialMC100MTestNoGenMatch_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart_DNN", folder+"SignalOfficialMC100MTestNoGenMatch_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart", folder+"SignalOfficialMC100MTestNoGenMatch_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstarDs_ntuple", folder+"BkgDstarDs.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs_tf", folder+"BkgDstarDs_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs_DNN", folder+"BkgDstarDs_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs", folder+"BkgDstarDs_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstarDsPart_ntuple", folder+"BkgDstarDsPart.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsPart_tf", folder+"BkgDstarDsPart_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsPart_DNN", folder+"BkgDstarDsPart_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsPart", folder+"BkgDstarDsPart_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstarDsstar_ntuple", folder+"BkgDstarDsstar.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar_tf", folder+"BkgDstarDsstar_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar_DNN", folder+"BkgDstarDsstar_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar", folder+"BkgDstarDsstar_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDs_ntuple", folder+"B0toDstarDs.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs_tf", folder+"B0toDstarDs_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs_DNN", folder+"B0toDstarDs_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs", folder+"B0toDstarDs_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDsstar_ntuple", folder+"B0toDstarDsstar.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsstar_tf", folder+"B0toDstarDsstar_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsstar_DNN", folder+"B0toDstarDsstar_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsstar", folder+"B0toDstarDsstar_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarD_ntuple", folder+"B0toDstarD.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD_tf", folder+"B0toDstarD_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD_DNN", folder+"B0toDstarD_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD", folder+"B0toDstarD_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDs1_ntuple", folder+"B0toDstarDs1.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs1_tf", folder+"B0toDstarDs1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs1_DNN", folder+"B0toDstarDs1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs1", folder+"B0toDstarDs1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDs0star_ntuple", folder+"B0toDstarDs0star.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs0star_tf", folder+"B0toDstarDs0star_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs0star_DNN", folder+"B0toDstarDs0star_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDs0star", folder+"B0toDstarDs0star_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarD0K_ntuple", folder+"B0toDstarD0K.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0K_tf", folder+"B0toDstarD0K_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0K_DNN", folder+"B0toDstarD0K_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0K", folder+"B0toDstarD0K_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarD0Kstar_ntuple", folder+"B0toDstarD0Kstar.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0Kstar_tf", folder+"B0toDstarD0Kstar_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0Kstar_DNN", folder+"B0toDstarD0Kstar_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarD0Kstar", folder+"B0toDstarD0Kstar_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDDtoTau_ntuple", folder+"B0toDDtoTau.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDDtoTau_tf", folder+"B0toDDtoTau_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDDtoTau_DNN", folder+"B0toDDtoTau_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDDtoTau", folder+"B0toDDtoTau_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstara1_ntuple", folder+"B0toDstara1.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstara1_tf", folder+"B0toDstara1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstara1_DNN", folder+"B0toDstara1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstara1", folder+"B0toDstara1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstar3pi_ntuple", folder+"B0toDstar3pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pi_tf", folder+"B0toDstar3pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pi_DNN", folder+"B0toDstar3pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pi", folder+"B0toDstar3pi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarrho0pi_ntuple", folder+"B0toDstarrho0pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarrho0pi_tf", folder+"B0toDstarrho0pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarrho0pi_DNN", folder+"B0toDstarrho0pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarrho0pi", folder+"B0toDstarrho0pi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstar3pipi0_ntuple", folder+"B0toDstar3pipi0.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pipi0_tf", folder+"B0toDstar3pipi0_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pipi0_DNN", folder+"B0toDstar3pipi0_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar3pipi0", folder+"B0toDstar3pipi0_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstar5pi_ntuple", folder+"B0toDstar5pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi_tf", folder+"B0toDstar5pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi_DNN", folder+"B0toDstar5pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi", folder+"B0toDstar5pi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarKpipi_ntuple", folder+"B0toDstarKpipi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKpipi_tf", folder+"B0toDstarKpipi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKpipi_DNN", folder+"B0toDstarKpipi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKpipi", folder+"B0toDstarKpipi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarKKstar_ntuple", folder+"B0toDstarKKstar.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKKstar_tf", folder+"B0toDstarKKstar_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKKstar_DNN", folder+"B0toDstarKKstar_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarKKstar", folder+"B0toDstarKKstar_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDsX_ntuple", folder+"B0toDstarDsX.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsX_tf", folder+"B0toDstarDsX_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsX_DNN", folder+"B0toDstarDsX_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsX", folder+"B0toDstarDsX_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarRho0pi_ntuple", folder+"B0toDstarRho0pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi_tf", folder+"B0toDstarRho0pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi_DNN", folder+"B0toDstarRho0pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi", folder+"B0toDstarRho0pi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDsPart_ntuple", folder+"B0toDstarDsTestPart.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsPart_tf", folder+"B0toDstarDsTestPart_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsPart_DNN", folder+"B0toDstarDsTestPart_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsPart", folder+"B0toDstarDsTestPart_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarDsDist_ntuple", folder+"B0toDstarDsTestDefinition.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsDist_tf", folder+"B0toDstarDsTestDefinition_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsDist_DNN", folder+"B0toDstarDsTestDefinition_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarDsDist", folder+"B0toDstarDsTestDefinition_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstar3pi_ntuple", folder+"BkgDstar3piNonres.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi_tf", folder+"BkgDstar3piNonres_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi_DNN", folder+"BkgDstar3piNonres_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi", folder+"BkgDstar3piNonres_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstara1_ntuple", folder+"BkgDstara1.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1_tf", folder+"BkgDstara1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1_DNN", folder+"BkgDstara1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1", folder+"BkgDstara1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstara1Part_ntuple", folder+"BkgDstara1Part.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1Part_tf", folder+"BkgDstara1Part_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1Part_DNN", folder+"BkgDstara1Part_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstara1Part", folder+"BkgDstara1Part_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgB0DD_ntuple", folder+"BkgB0DD.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgB0DD_tf", folder+"BkgB0DD_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgB0DD_DNN", folder+"BkgB0DD_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgB0DD", folder+"BkgB0DD_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("ButoDstarDK_ntuple", folder+"ButoDstarDK.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarDK_tf", folder+"ButoDstarDK_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarDK_DNN", folder+"ButoDstarDK_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarDK", folder+"ButoDstarDK_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("ButoDstarXc_ntuple", folder+"ButoDstarXc.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarXc_tf", folder+"ButoDstarXc_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarXc_DNN", folder+"ButoDstarXc_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("ButoDstarXc", folder+"ButoDstarXc_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgBuDXc_ntuple", folder+"BkgBuDXc.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBuDXc_tf", folder+"BkgBuDXc_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBuDXc_DNN", folder+"BkgBuDXc_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBuDXc", folder+"BkgBuDXc_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BstoDD_ntuple", folder+"BstoDD.root", "ntuplizer/tree"); 
			filemanager.AddItem("BstoDD_tf", folder+"BstoDD_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BstoDD_DNN", folder+"BstoDD_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BstoDD", folder+"BstoDD_tauDNN_mva.root", "tree"); 

			// data from D era
			filemanager.AddItem("dataD1_ntuple", folder+"dataD1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1_tf", folder+"dataD1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1_DNN", folder+"dataD1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1", folder+"dataD1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2_ntuple", folder+"dataD2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_tf", folder+"dataD2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_DNN", folder+"dataD2_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2", folder+"dataD2_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("dataD2_SB", folder+"dataD2_tauDNN_mva_SB.root", "tree"); 

			filemanager.AddItem("dataD3_ntuple", folder+"dataD3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3_tf", folder+"dataD3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3_DNN", folder+"dataD3_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3", folder+"dataD3_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD4_ntuple", folder+"dataD4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4_tf", folder+"dataD4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4_DNN", folder+"dataD4_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4", folder+"dataD4_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD5_ntuple", folder+"dataD5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5_tf", folder+"dataD5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5_DNN", folder+"dataD5_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5", folder+"dataD5_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD1WS_ntuple", folder+"dataD1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_tf", folder+"dataD1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_DNN", folder+"dataD1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS", folder+"dataD1WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2WS_ntuple", folder+"dataD2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_tf", folder+"dataD2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_DNN", folder+"dataD2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS", folder+"dataD2WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD3WS_ntuple", folder+"dataD3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3WS_tf", folder+"dataD3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3WS_DNN", folder+"dataD3WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD3WS", folder+"dataD3WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD4WS_ntuple", folder+"dataD4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4WS_tf", folder+"dataD4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4WS_DNN", folder+"dataD4WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD4WS", folder+"dataD4WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD5WS_ntuple", folder+"dataD5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5WS_tf", folder+"dataD5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5WS_DNN", folder+"dataD5WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD5WS", folder+"dataD5WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD_ntuple", folder+"dataD.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD_tf", folder+"dataD_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD_DNN", folder+"dataD_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD", folder+"dataD_tauDNN_mva.root", "tree"); 

			// data from A era
			filemanager.AddItem("dataA1_ntuple", folder+"dataA1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1_tf", folder+"dataA1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1_DNN", folder+"dataA1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1", folder+"dataA1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA2_ntuple", folder+"dataA2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2_tf", folder+"dataA2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2_DNN", folder+"dataA2_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2", folder+"dataA2_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("dataA2_SB", folder+"dataA2_tauDNN_mva_SB.root", "tree"); 

			filemanager.AddItem("dataA3_ntuple", folder+"dataA3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3_tf", folder+"dataA3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3_DNN", folder+"dataA3_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3", folder+"dataA3_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA4_ntuple", folder+"dataA4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4_tf", folder+"dataA4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4_DNN", folder+"dataA4_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4", folder+"dataA4_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA5_ntuple", folder+"dataA5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5_tf", folder+"dataA5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5_DNN", folder+"dataA5_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5", folder+"dataA5_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA1WS_ntuple", folder+"dataA1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1WS_tf", folder+"dataA1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1WS_DNN", folder+"dataA1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA1WS", folder+"dataA1WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA2WS_ntuple", folder+"dataA2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS_tf", folder+"dataA2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS_DNN", folder+"dataA2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS", folder+"dataA2WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA3WS_ntuple", folder+"dataA3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3WS_tf", folder+"dataA3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3WS_DNN", folder+"dataA3WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA3WS", folder+"dataA3WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA4WS_ntuple", folder+"dataA4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4WS_tf", folder+"dataA4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4WS_DNN", folder+"dataA4WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA4WS", folder+"dataA4WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA5WS_ntuple", folder+"dataA5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5WS_tf", folder+"dataA5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5WS_DNN", folder+"dataA5WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA5WS", folder+"dataA5WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA_ntuple", folder+"dataA.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA_tf", folder+"dataA_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA_DNN", folder+"dataA_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA", folder+"dataA_tauDNN_mva.root", "tree"); 

			// data from B era
			filemanager.AddItem("dataB1_ntuple", folder+"dataB1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1_tf", folder+"dataB1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1_DNN", folder+"dataB1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1", folder+"dataB1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB2_ntuple", folder+"dataB2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_tf", folder+"dataB2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_DNN", folder+"dataB2_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2", folder+"dataB2_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("dataB2_SB", folder+"dataB2_tauDNN_mva_SB.root", "tree"); 

			filemanager.AddItem("dataB3_ntuple", folder+"dataB3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3_tf", folder+"dataB3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3_DNN", folder+"dataB3_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3", folder+"dataB3_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB4_ntuple", folder+"dataB4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4_tf", folder+"dataB4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4_DNN", folder+"dataB4_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4", folder+"dataB4_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB5_ntuple", folder+"dataB5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5_tf", folder+"dataB5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5_DNN", folder+"dataB5_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5", folder+"dataB5_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB_ntuple", folder+"dataB.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB_tf", folder+"dataB_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB_DNN", folder+"dataB_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB", folder+"dataB_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB1WS_ntuple", folder+"dataB1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1WS_tf", folder+"dataB1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1WS_DNN", folder+"dataB1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB1WS", folder+"dataB1WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB2WS_ntuple", folder+"dataB2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2WS_tf", folder+"dataB2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2WS_DNN", folder+"dataB2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2WS", folder+"dataB2WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB3WS_ntuple", folder+"dataB3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3WS_tf", folder+"dataB3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3WS_DNN", folder+"dataB3WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB3WS", folder+"dataB3WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB4WS_ntuple", folder+"dataB4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4WS_tf", folder+"dataB4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4WS_DNN", folder+"dataB4WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB4WS", folder+"dataB4WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB5WS_ntuple", folder+"dataB5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5WS_tf", folder+"dataB5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5WS_DNN", folder+"dataB5WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB5WS", folder+"dataB5WS_tauDNN_mva.root", "tree"); 

			// data from C era
			filemanager.AddItem("dataC1_ntuple", folder+"dataC1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1_tf", folder+"dataC1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1_DNN", folder+"dataC1_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1", folder+"dataC1_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC2_ntuple", folder+"dataC2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2_tf", folder+"dataC2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2_DNN", folder+"dataC2_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2", folder+"dataC2_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("dataC2_SB", folder+"dataC2_tauDNN_mva_SB.root", "tree"); 

			filemanager.AddItem("dataC3_ntuple", folder+"dataC3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3_tf", folder+"dataC3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3_DNN", folder+"dataC3_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3", folder+"dataC3_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC4_ntuple", folder+"dataC4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4_tf", folder+"dataC4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4_DNN", folder+"dataC4_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4", folder+"dataC4_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC5_ntuple", folder+"dataC5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5_tf", folder+"dataC5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5_DNN", folder+"dataC5_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5", folder+"dataC5_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC1WS_ntuple", folder+"dataC1.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1WS_tf", folder+"dataC1_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1WS_DNN", folder+"dataC1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC1WS", folder+"dataC1WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC2WS_ntuple", folder+"dataC2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2WS_tf", folder+"dataC2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2WS_DNN", folder+"dataC2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC2WS", folder+"dataC2WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC3WS_ntuple", folder+"dataC3.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3WS_tf", folder+"dataC3_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3WS_DNN", folder+"dataC3WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC3WS", folder+"dataC3WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC4WS_ntuple", folder+"dataC4.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4WS_tf", folder+"dataC4_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4WS_DNN", folder+"dataC4WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC4WS", folder+"dataC4WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC5WS_ntuple", folder+"dataC5.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5WS_tf", folder+"dataC5_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5WS_DNN", folder+"dataC5WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC5WS", folder+"dataC5WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataC_ntuple", folder+"dataC.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC_tf", folder+"dataC_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC_DNN", folder+"dataC_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataC", folder+"dataC_tauDNN_mva.root", "tree"); 

			//filemanager.AddItem("data_ntuple", folder+"data.root", "ntuplizer/tree"); 
			//filemanager.AddItem("data_tf", folder+"data_withTFweight.root", "ntuplizer/tree"); 
			//filemanager.AddItem("data_DNN", folder+"data_tauDNN.root", "ntuplizer/tree"); 
			//filemanager.AddItem("data", folder+"data_tauDNN_mva.root", "tree"); 


			filemanager.AddItem("dataD2TauWS_ntuple", folder+"ParkingBPHULA2WSTau.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS_tf", folder+"ParkingBPHULA2WSTau_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS_DNN", folder+"ParkingBPHULA2WSTau_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS", folder+"ParkingBPH2Run2018DTauWS_tauDNN_mva.root", "tree"); //ParkingBPH2Run2018DTauWS
			//filemanager.AddItem("dataD2TauWS", folder+"ParkingBPHULA2WSTau_tauDNN_mva.root", "tree"); //ParkingBPH2Run2018DTauWS

			filemanager.AddItem("Test_ntuple", folder+"Test.root", "ntuplizer/tree"); 
			filemanager.AddItem("Test_tf", folder+"Test_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("Test_DNN", folder+"Test_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("Test", folder+"Test_tauDNN_mva.root", "tree"); 

			const TString specialRunFileName = "Sig"; // Hack to run on given file

			filemanager.AddItem("SpecialRun_ntuple", folder+specialRunFileName+".root", "ntuplizer/tree"); 
			filemanager.AddItem("SpecialRun_tf", folder+specialRunFileName+"_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SpecialRun_DNN", folder+specialRunFileName+"_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SpecialRun", folder+specialRunFileName+"_tauDNN_mva.root", "tree"); 

			// To be deprecated 
			filemanager.AddItem("SigOld_ntuple", folder+"SignalOfficialMC50M.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigOld_tf", folder+"SignalOfficialMC50M_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigOld_DNN", folder+"SignalOfficialMC50M_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigOld", folder+"SignalOfficialMC50M_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigPartOld_ntuple", folder+"SignalOfficialMC50MNoGenMatch.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPartOld_tf", folder+"SignalOfficialMC50MNoGenMatch_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPartOld_DNN", folder+"SignalOfficialMC50MNoGenMatch_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPartOld", folder+"SignalOfficialMC50MNoGenMatch_tauDNN_mva.root", "tree"); 


		}

		else 
		{
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
			filemanager.AddItem("ParkingBPHAllRun2018B", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted.root", "tree"); 


			// Towards stable productions
			filemanager.AddItem("SignalOfficialMC50M", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_MVA", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("SignalOfficialMC50M_test", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50MwithTFweightTest.root", "ntuplizer/tree"); // TODO: drop? 

			filemanager.AddItem("BkgDstarDs", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMMultipleTau_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMMultipleTau_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMultipleTau_MVA", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("Data2018BFirst", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_MVA", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("Data2018BFirst10k_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_withTFweight10k.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst10k_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_tauDNN10k.root", "ntuplizer/tree"); // Fixme 

			// TODO: drop? 
			filemanager.AddItem("Data2018BFirstTest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstAllTau.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirstTest_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstAllTau_withTFweight.root", "ntuplizer/tree"); 

			filemanager.AddItem("BkgBtoDstar3piNonres_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/B0toDstar3piFirst_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstar3piNonres_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/B0toDstar3piFirst_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstar3piNonres_MVA", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/B0toDstar3piFirst_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgBtoDstarDsstar_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsstarInclPrivateProdFirst_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstarDsstar_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsstarInclPrivateProdFirst_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstarDsstar_MVA", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsstarInclPrivateProdFirst_tauDNN_mva.root", "tree"); 
			//filemanager.AddItem("SignalOfficialMC50M_DNN_test", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_tauDNN.root", "ntuplizer/tree"); 

			// For legacy compatibility
			filemanager.AddItem("ParkingBPHAllRun2018B", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_mva", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted_mva.root", "tree"); 
			filemanager.AddItem("ParkingBPHAllRun2018B_mva", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted_mva.root", "tree"); 
			filemanager.AddItem("BkgBtoDstarDs", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsInclPrivateProdFirst.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstar3piNonres", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/B0toDstar3piFirst.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgBtoDstarDsstar", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsstarInclPrivateProdFirst.root", "ntuplizer/tree"); 
		}

		if (!denominator) 
		{
			cut.emplace(std::make_pair("base", "(b_tau_m<1.7)&&(mvaScore>0.7)&&(b_B_m>0.&&b_B_nmu<1&&b_B_nh<1&&b_B_ne<1&&(b_B_npi0==0&&b_B_ngamma<2))")); // (b_tau_m<1.7)&&(b_tau_min_dr_mu>0.5)&&(b_tau_alpha>0)&&(D0_fsig>3.)&&(b_B_m>3.) //&&(b_tau_min_dr_mu>0.5)&&(b_tau_min_dr_e>0.5)//&& (b_B_q2 > 6.) // (b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.) && (b_B_mu_alpha > 1.) &&((b_tau_m_KKpi1>2.)||(b_tau_m_KKpi1<1.9))&&((b_tau_m_KKpi2>2.)||(b_tau_m_KKpi2<1.9)) &&(b_tau_min_dr_mu>0.5)&&(b_tau_min_dr_e>0.5)&&(b_tau_alpha>0) (b_tau_m<1.7)&&(b_tau_alpha>0)&&(D0_fsig>3.)&&(b_B_m>3.)&&(b_tau_vtx4trkProb<0.2)
			cut.emplace(std::make_pair("SR", cut["base"]+TCut(TString::Format("mvaScore>=%f", mvaCutSR)))); 
			cut.emplace(std::make_pair("SB", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutSB, mvaCutSR)))); 
			cut.emplace(std::make_pair("CR", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutCR, mvaCutSB)))); 
		}
		else 
		{
			cut.emplace(std::make_pair("base", "b_B_m>0.&&b_B_nmu<2&&b_B_nh<15&&b_B_ne<1&&b_B_npi0<15&&b_B_fsig>2")); //&&b_B_mm2<1.&&pttau_B_fl>0.2 &&b_B_mm2<1.&&pttau_B_fsig>3
			cut.emplace(std::make_pair("mw", "b_B_m>5.15&&b_B_m<5.4&&b_B_fsig>2.&&b_B_nmu<1&&b_B_ne<1&&b_B_nh<3")); //&&b_B_npi0<3&&pttau_B_ngamma<5
			// superclean: pttau_B_m>0.&&pttau_B_nmu<1&&pttau_B_nh<1&&pttau_B_ne<1&&(pttau_B_npi0+pttau_B_ngamma)<2&&pttau_B_mm2<1.&&pttau_B_fl>0.2 (or with fsig > 7.)
		}

		cutstandalone.emplace(std::make_pair("SR", TCut("base", "(b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.)")+TCut(TString::Format("mvaScore>=%f", mvaCutSR)))); 
		cutstandalone.emplace(std::make_pair("SB", TCut("base", "(b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.)")+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutSB, mvaCutSR)))); 
		cutstandalone.emplace(std::make_pair("CR", TCut("base", "(b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.)")+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutCR, mvaCutSB)))); 

		// https://colorbrewer2.org/?type=diverging&scheme=RdYlBu&n=7 
		//colors = {new TColor(TColor::GetFreeColorIndex(), 165,0,38), new TColor(TColor::GetFreeColorIndex(), 215,48,39), new TColor(TColor::GetFreeColorIndex(), 244,109,67), new TColor(TColor::GetFreeColorIndex(), 253,174,97), new TColor(TColor::GetFreeColorIndex(), 254,224,144), new TColor(TColor::GetFreeColorIndex(), 255,255,191), new TColor(TColor::GetFreeColorIndex(), 224,243,248), new TColor(TColor::GetFreeColorIndex(), 171,217,233), new TColor(TColor::GetFreeColorIndex(), 116,173,209), new TColor(TColor::GetFreeColorIndex(), 69,117,180), new TColor(TColor::GetFreeColorIndex(), 49,54,149)}; 
			// {new TColor(TColor::GetFreeColorIndex(), 215,48,39), new TColor(TColor::GetFreeColorIndex(), 252,141,89), new TColor(TColor::GetFreeColorIndex(), 254,224,144), new TColor(TColor::GetFreeColorIndex(), 255,255,191), new TColor(TColor::GetFreeColorIndex(), 224,243,248), new TColor(TColor::GetFreeColorIndex(), 145,191,219), new TColor(TColor::GetFreeColorIndex(), 69,117,180)}; 

		//mycolors.reserve(colors.size()); 
		//for (auto color : colors) 
		//{
		//	mycolors.push_back(color->GetNumber()); 
		//}

		//color = {{"Sig", mycolors[0]}, {"B0toDstarDs", mycolors[1]}, {"BkgDstarDs", mycolors[1]}, {"B0toDstarDsstar", mycolors[2]}, {"BkgDstarDsstar", mycolors[2]}, {"B0toDstarD", mycolors[3]}, {"B0toDstarD0K", mycolors[5]}, {"ButoDstarDK", mycolors[4]}, {"B0toDstar3pi", mycolors[7]}, {"BkgDstara1", mycolors[6]},{"WS", mycolors[10]}, {"WSTau", mycolors[10]}, {"dataD2WS", mycolors[10]}, {"dataD2TauWS", mycolors[10]}, }; 
			// {{"Sig", mycolors[0]}, {"SigPart", mycolors[1]}, {"B0toDstarDs", mycolors[2]}, {"BkgDstarDs", mycolors[2]}, {"B0toDstarDsstar", mycolors[3]}, {"BkgDstarDsstar", mycolors[3]}, {"BkgDstara1", mycolors[4]},{"WS", mycolors[6]}, {"WSTau", mycolors[5]}, {"dataD2WS", mycolors[6]}, {"dataD2TauWS", mycolors[5]}, {"B0toDstarD0K", mycolors[5]}}; 

		CommonInitialisation(denominator); 

		model = {	
			{"v1", "./data/tautagger/batchsize_10/serialized"}, 
			{"v2", "./data/tautagger/FirstTopUp/serialized"}, 
			{"v3", "./data/tautagger/FirstTopUp/serialized"},  
			{"v3.5", "./data/tautagger/FirstTopUp/serialized"},  
			{"v4", "./data/tautagger/NewSelTopUpNoOverlap/serialized"}, 
			{"v6.7", "./data/tautagger/FlightSigCorrNoCharge/serialized"}, 
			{"v6.8", "./data/tautagger/FlightSigCorrNoCharge/serialized"},
			{"v6.9", "./data/tautagger/FlightSigCorrNoCharge/serialized"},
			{"v6.945", "./data/tautagger/FlightSigCorrNoCharge/serialized"},
			{"v6.95", "./data/tautagger/FlightSigCorrNoCharge/serialized"},
			{"v7", "./data/tautagger/trainingv18/serialized"}
		}; 

		MVA = {	
			{"v1", "./anaMVA/secondtraining/model_optimized/weights.xml"}, 
			{"v2", "./anaMVA/secondtraining/model_optimized/weights.xml"}, 
			{"v3", "./anaMVA/secondtraining/model_optimized/weights.xml"},  
			{"v3.5", "./anaMVA/NewSelection/model_optimized/weights.xml"},  
			{"v4", "./anaMVA/NewSelection/model_optimized/weights.xml"}, 
			{"v6.7", "./anaMVA/NewFixTauFL/model_optimized/weights.xml"}, 
			{"v6.8", "./anaMVA/NewIsoWithCutsvsdata/model_optimized/weights.xml"}, //./anaMVA/NewIsoVariablesAgainstWS/model_optimized/weights.xml" ./anaMVA/NewIsoVariables/model_optimized/weights.xml
			{"v6.9", "./anaMVA/NewFixTauFL/model_optimized/weights.xml"}, //LatestVsData
			{"v6.95", "./anaMVA/TrimmedVariablesWS/model_optimized/weights.xml"}, //LatestVsData
			{"v7", "./anaMVA/trainingv7BvariablesonB/model_optimized/weights.xml"} //trainingv7onD1
		}; 

		//legends = {{"Sig", "signal"}, {"SigPart", "part. signal"}, {"B0toDstarDs", "B^{0}#rightarrowD*D_{s}"}, {"BkgDstarDs", "B^{0}#rightarrowD*D_{s}"}, {"B0toDstarDsstar", "B^{0}#rightarrowD*D*_{s}"}, {"BkgDstarDsstar", "B^{0}#rightarrowD*D*_{s}"}, {"BkgDstara1", "B^{0}#rightarrowD*a_{1}"}, {"B0toDstar3pi", "B^{0}#rightarrowD^{*}3pi"}, {"B0toDstarD", "B^{0}#rightarrowD^{*}D"}, {"ButoDstarDK", "B^{+}#rightarrowD^{*}DK"}, {"B0toDstarD0K", "B^{0}#rightarrowD^{*}D^{0}K"},{"WS", "|q_{B}| = 2  WS"}, {"WSTau", "|q_{#tau}| = 3  WS"}, {"dataD2WS", "|q_{B}|=2 WS"}, {"dataD2TauWS", "|q_{#tau}|=3 WS"}}; 

		//labels = {{"b_tau_rhomass1", "Invariant m_{#rho}"}, {"b_tau_rhomass2", "Invariant m_{#rho}"}, {"b_B_q2", "q2"}, {"b_B_m", "Reconstructed m_{B}"}}; 

	}


	std::vector<TString> LoadRegions(const TString& path) 
	{
		std::ifstream file(path+"/Info.json", std::ifstream::binary);
		Json::Value regions;
		file >> regions;

		std::vector<TString> loaded; 

		for (auto item : regions.getMemberNames()) 
		{
			//std::cout << item << std::endl; 

			for (auto region : regions[item].getMemberNames()) 
			{
				auto& regiondata = regions[item][region]; 
				assert(regiondata.size() == 2); 

				/*for (auto element : regiondata) 
				{
					std::cout << element << std::endl; 
				}*/

				TString name = TString::Format("%s_%s", item.c_str(), region.c_str());
				filemanager.AddItem(name, regiondata[0].asString(), regiondata[1].asString()); 
				loaded.push_back(name);
			}
		}

		/*for (auto item : regions)
		{
			std::cout << item << " " << std::endl; 
		}*/


		return std::move(loaded); 
	}



	std::vector<TString> Load(const TString& regions, const bool denominator = false) 
	{
		CommonInitialisation(denominator); 

		auto loaded = LoadRegions(regions); 

		std::cout << "Size: " << loaded.size() << std::endl; 
		for (auto item : loaded) 
		{
			std::cout << "item: "; 
			std::cout << item << std::endl; 
		}

		return loaded; 
	}


	TChain* GetSample(const TString& name) 
	{
		TString samplename = name; 
		TString suffix = "";
		
		TChain *chain = filemanager.GetItem<TChain*>(name, true); 
		if (!chain) 
		{
			if (name.Contains("_")) 
			{
				// Split and get back after
				auto tokens = name.Tokenize("_"); 
				samplename = static_cast<TObjString*>(tokens->At(0))->GetString(); 
				suffix = TString(name).ReplaceAll(samplename, ""); 
			}
			std::cout << samplename << " " << suffix << std::endl;
		
			chain = new TChain(name, name); 
			for (auto item : samples.at(samplename.Data()).fileRefs) 
			{
				TString itemname = TString(item)+suffix; 
				std::cout << itemname << std::endl;
				chain->AddFile(TString::Format("%s/%s", filemanager.GetFile(itemname).c_str(), filemanager.GetObject(itemname).c_str())); 
			}
		}
		std::cout << "Number of events " << chain->GetEntries() << std::endl;

		return chain; 
	}

}




#endif

