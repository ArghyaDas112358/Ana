#ifndef FileFlow_h
#define FileFlow_h
#include "plugins/FileManager/CFileManager.C"
#include <string>
#include "TString.h"
#include "TCut.h"
#include <unordered_map>
#include "SampleData.C"


namespace Ana 
{

	FileManager filemanager; 

	std::map<std::string, TCut> cut; //std::map<std::string, std::map<std::sting, TCut> > cuts; 

	//std::vector<TColor*> colors; 

	//std::vector<Int_t> mycolors; 

	//std::unordered_map<std::string, Int_t> color; 

	std::unordered_map<std::string, Int_t> colorold; 

	std::unordered_map<std::string, std::string> model; 

	std::unordered_map<std::string, std::string> MVA; 

	//std::unordered_map<std::string, std::string> legends; 

	std::unordered_map<std::string, ROOT::RDF::TH1DModel> binning; 

	//std::unordered_map<std::string, std::string> labels; 

	std::unordered_map<std::string, SampleData> samples; 


	std::map<std::string, TCut> cutstandalone;


	// Constants being defined centrally 
	Double_t mvaCutSR = 0.5; 
	Double_t mvaCutSB = -0.2; 
	Double_t mvaCutCR = -0.5; 


	void Init(const TString& cycle = "") 
	{
		if (cycle != "") // For legacy purpose 
		{

			TString folder = "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/"+cycle+"/"; 

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

			filemanager.AddItem("B0toDstar5pi_ntuple", folder+"B0toDstar5pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi_tf", folder+"B0toDstar5pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi_DNN", folder+"B0toDstar5pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstar5pi", folder+"B0toDstar5pi_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("B0toDstarRho0pi_ntuple", folder+"B0toDstarRho0pi.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi_tf", folder+"B0toDstarRho0pi_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi_DNN", folder+"B0toDstarRho0pi_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("B0toDstarRho0pi", folder+"B0toDstarRho0pi_tauDNN_mva.root", "tree"); 

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

			filemanager.AddItem("dataB2_ntuple", folder+"dataB2.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_tf", folder+"dataB2_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_DNN", folder+"dataB2_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2", folder+"dataB2_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD1_ntuple", folder+"dataD1small.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1_tf", folder+"dataD1small_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1_DNN", folder+"dataD1small_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1", folder+"dataD1small_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2_ntuple", folder+"dataD2Partial.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_tf", folder+"dataD2Partial_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_DNN", folder+"dataD2Partial_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2", folder+"dataD2Partial_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2WS_ntuple", folder+"dataD2WS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_tf", folder+"dataD2WS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_DNN", folder+"dataD2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS", folder+"dataD2WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD1WS_ntuple", folder+"dataD1WS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_tf", folder+"dataD1WS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_DNN", folder+"dataD1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS", folder+"dataD1WS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataA2WS_ntuple", folder+"ParkingBPHULA2WS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS_tf", folder+"ParkingBPHULA2WS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS_DNN", folder+"ParkingBPHULA2WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataA2WS", folder+"ParkingBPH2Run2018DWS_tauDNN_mva.root", "tree"); //ParkingBPH2Run2018DWS
			//filemanager.AddItem("dataD2WS", folder+"ParkingBPHULA2WS_tauDNN_mva.root", "tree"); //ParkingBPH2Run2018DWS

			filemanager.AddItem("dataD1WS_ntuple", folder+"ParkingBPHULD1WS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_tf", folder+"ParkingBPHULD1WS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS_DNN", folder+"ParkingBPHULD1WS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD1WS", folder+"ParkingBPHULD1WS_tauDNN_mva.root", "tree");

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

		cut.emplace(std::make_pair("base", "(b_tau_m<1.7)&&(b_tau_min_dr_mu>0.5)&&(b_tau_min_dr_e>0.5)")); //&&(b_tau_min_dr_mu>0.5)&&(b_tau_min_dr_e>0.5)//&& (b_B_q2 > 6.) // (b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.) && (b_B_mu_alpha > 1.) &&((b_tau_m_KKpi1>2.)||(b_tau_m_KKpi1<1.9))&&((b_tau_m_KKpi2>2.)||(b_tau_m_KKpi2<1.9))
		cut.emplace(std::make_pair("SR", cut["base"]+TCut(TString::Format("mvaScore>=%f", mvaCutSR)))); 
		cut.emplace(std::make_pair("SB", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutSB, mvaCutSR)))); 
		cut.emplace(std::make_pair("CR", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutCR, mvaCutSB)))); 

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

		colorold = {{"Sig", 2}, {"BkgDstarDs", 3}, {"BkgDstarDsstar", 8}, {"BkgDstara1", 4}, {"dataD2WS", 6}, {"dataD2TauWS", 7}, {"other", 9}, {"yetanother", 1}}; // Legacy color scheme 

		model = {	
			{"v1", "./data/tautagger/batchsize_10/serialized"}, 
			{"v2", "./data/tautagger/FirstTopUp/serialized"}, 
			{"v3", "./data/tautagger/FirstTopUp/serialized"},  
			{"v3.5", "./data/tautagger/FirstTopUp/serialized"},  
			{"v4", "./data/tautagger/NewSelTopUpNoOverlap/serialized"}, 
			{"v6.7", "./data/tautagger/FlightSigCorrNoCharge/serialized"}, 
			{"v6.8", "./data/tautagger/FlightSigCorrNoCharge/serialized"}
		}; 

		MVA = {	
			{"v1", "./anaMVA/secondtraining/model_optimized/weights.xml"}, 
			{"v2", "./anaMVA/secondtraining/model_optimized/weights.xml"}, 
			{"v3", "./anaMVA/secondtraining/model_optimized/weights.xml"},  
			{"v3.5", "./anaMVA/NewSelection/model_optimized/weights.xml"},  
			{"v4", "./anaMVA/NewSelection/model_optimized/weights.xml"}, 
			{"v6.7", "./anaMVA/NewFixTauFL/model_optimized/weights.xml"}, 
			{"v6.8", "./anaMVA/NewIsoVariables/model_optimized/weights.xml"}
		}; 

		//legends = {{"Sig", "signal"}, {"SigPart", "part. signal"}, {"B0toDstarDs", "B^{0}#rightarrowD*D_{s}"}, {"BkgDstarDs", "B^{0}#rightarrowD*D_{s}"}, {"B0toDstarDsstar", "B^{0}#rightarrowD*D*_{s}"}, {"BkgDstarDsstar", "B^{0}#rightarrowD*D*_{s}"}, {"BkgDstara1", "B^{0}#rightarrowD*a_{1}"}, {"B0toDstar3pi", "B^{0}#rightarrowD^{*}3pi"}, {"B0toDstarD", "B^{0}#rightarrowD^{*}D"}, {"ButoDstarDK", "B^{+}#rightarrowD^{*}DK"}, {"B0toDstarD0K", "B^{0}#rightarrowD^{*}D^{0}K"},{"WS", "|q_{B}| = 2  WS"}, {"WSTau", "|q_{#tau}| = 3  WS"}, {"dataD2WS", "|q_{B}|=2 WS"}, {"dataD2TauWS", "|q_{#tau}|=3 WS"}}; 

		//labels = {{"b_tau_rhomass1", "Invariant m_{#rho}"}, {"b_tau_rhomass2", "Invariant m_{#rho}"}, {"b_B_q2", "q2"}, {"b_B_m", "Reconstructed m_{B}"}}; 

		binning = {{"b_tau_rhomass1", {"", "#rho_{12} mass;Invariant m_{#rho} [GeV];Counts", 40, 0., 1.5}},
				{"b_tau_rhomass2", {"", "rho_{23} mass;Invariant m_{#rho} [GeV];Counts", 40, 0., 1.5}},
				{"b_B_m", {"", "B mass;Reconstructed m_{B} [GeV];Counts", 40, 0., 6.}},
				{"b_B_q2", {"", "q2;q^{2} [GeV];Counts", 40, 0., 12.}},
				{"B_m", {"", ";B mass [GeV];Counts", 40, 0., 6.}},
				{"B_q2", {"", ";B mass [GeV];Counts", 40, 0., 12.}},
				{"tau_rhomass1", {"", ";#rho_{12} mass [GeV];Counts", 40, 0., 1.5}},
				{"tau_rhomass2", {"", ";#rho_{12} mass [GeV];Counts", 40, 0., 1.5}},
				{"b_tau_m", {"", "#tau mass;Reconstructed m_{#tau} [GeV];Counts", 40, 0., 5.}},
		}; 

		samples = InitSamples(); 

	}

}




#endif

