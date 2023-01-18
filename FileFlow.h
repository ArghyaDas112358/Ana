#ifndef FileFlow_h
#define FileFlow_h
#include "plugins/FileManager/CFileManager.C"
#include <string>
#include "TString.h"
#include "TCut.h"


namespace Ana 
{

	FileManager filemanager; 

	std::map<std::string, TCut> cut; //std::map<std::string, std::map<std::sting, TCut> > cuts; 

	std::vector<TColor*> colors; 

	std::vector<Int_t> mycolors; 

	std::unordered_map<std::string, Int_t> color; 

	std::unordered_map<std::string, std::string> model; 


	// Constants being defined centrally 
	Double_t mvaCutSR = 0.9; 
	Double_t mvaCutSB = 0.0; 
	Double_t mvaCutCR = -0.5; 


	void Init(const TString& cycle = "") 
	{
		if (cycle != "") // For legacy purpose 
		{

			TString folder = "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/"+cycle+"/"; 

			filemanager.AddItem("Sig_ntuple", folder+"SignalOfficialMC100MTest.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig_tf", folder+"SignalOfficialMC100MTest_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig_DNN", folder+"SignalOfficialMC100MTest_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("Sig", folder+"SignalOfficialMC100MTest_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigTrain_ntuple", folder+"SignalOfficialMC50and100MTrain.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain_tf", folder+"SignalOfficialMC50and100MTrain_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain_DNN", folder+"SignalOfficialMC50and100MTrain_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigTrain", folder+"SignalOfficialMC50and100MTrain_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("SigPart_ntuple", folder+"SignalOfficialMC100MTestNoGenMatch.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart_tf", folder+"SignalOfficialMC100MTestNoGenMatch_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart_DNN", folder+"SignalOfficialMC100MTestNoGenMatch_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SigPart", folder+"SignalOfficialMC100MTestNoGenMatch_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstarDs_ntuple", folder+"BkgDstarDsIncl.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs_tf", folder+"BkgDstarDsIncl_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs_DNN", folder+"BkgDstarDsIncl_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDs", folder+"BkgDstarDsIncl_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstarDsstar_ntuple", folder+"BkgDstarDsstarIncl.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar_tf", folder+"BkgDstarDsstarIncl_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar_DNN", folder+"BkgDstarDsstarIncl_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsstar", folder+"BkgDstarDsstarIncl_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("BkgDstar3pi_ntuple", folder+"BkgDstar3piNonres.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi_tf", folder+"BkgDstar3piNonres_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi_DNN", folder+"BkgDstar3piNonres_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstar3pi", folder+"BkgDstar3piNonres_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataB2_ntuple", folder+"DataBPH2Run2018B.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_tf", folder+"DataBPH2Run2018B_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2_DNN", folder+"DataBPH2Run2018B_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataB2", folder+"DataBPH2Run2018B_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2_ntuple", folder+"DataBPH2Run2018D.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_tf", folder+"DataBPH2Run2018D_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2_DNN", folder+"DataBPH2Run2018D_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2", folder+"DataBPH2Run2018D_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2WS_ntuple", folder+"ParkingBPH2Run2018DWS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_tf", folder+"ParkingBPH2Run2018DWS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS_DNN", folder+"ParkingBPH2Run2018DWS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2WS", folder+"ParkingBPH2Run2018DWS_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("dataD2TauWS_ntuple", folder+"ParkingBPH2Run2018DTauWS.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS_tf", folder+"ParkingBPH2Run2018DTauWS_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS_DNN", folder+"ParkingBPH2Run2018DTauWS_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("dataD2TauWS", folder+"ParkingBPH2Run2018DTauWS_tauDNN_mva.root", "tree"); 

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

		cut.emplace(std::make_pair("base", "(b_Ds_vprob>0.1) && (b_D0_vprob>0.1) && (mvaScore>-2.)")); 
		cut.emplace(std::make_pair("SR", cut["base"]+TCut(TString::Format("mvaScore>=%f", mvaCutSR)))); 
		cut.emplace(std::make_pair("SB", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutSB, mvaCutSR)))); 
		cut.emplace(std::make_pair("CR", cut["base"]+TCut(TString::Format("(mvaScore >= %f) && (mvaScore < %f)", mvaCutCR, mvaCutSB)))); 

		// https://colorbrewer2.org/?type=diverging&scheme=RdYlBu&n=7 
		colors = {new TColor(TColor::GetFreeColorIndex(), 215,48,39), new TColor(TColor::GetFreeColorIndex(), 252,141,89), new TColor(TColor::GetFreeColorIndex(), 254,224,144), new TColor(TColor::GetFreeColorIndex(), 255,255,191), new TColor(TColor::GetFreeColorIndex(), 224,243,248), new TColor(TColor::GetFreeColorIndex(), 145,191,219), new TColor(TColor::GetFreeColorIndex(), 69,117,180)}; 

		mycolors.reserve(colors.size()); 
		for (auto color : colors) 
		{
			mycolors.push_back(color->GetNumber()); 
		}

		color = {{"Sig", mycolors[0]}, {"SigPart", mycolors[1]}, {"BkgDstarDs", mycolors[2]}, {"BkgDstarDsstar", mycolors[3]}, {"WS", mycolors[6]}, {"WSTau", mycolors[5]}, {"dataD2WS", mycolors[6]}, {"dataD2TauWS", mycolors[5]}}; 

		model = {{"v1", "./data/tautagger/batchsize_10/serialized"}, {"v2", "./data/tautagger/FirstTopUp/serialized"}, {"v3", "./data/tautagger/FirstTopUp/serialized"}}; 

	}

}




#endif

