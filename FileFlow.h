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


	// Constants being defined centrally 
	Double_t mvaCutSR = 0.9; 


	void Init(const TString& cycle = "") 
	{
		if (cycle != "") // For legacy purpose 
		{

			TString folder = "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/"+cycle+"/"; 

			filemanager.AddItem("SignalOfficialMC50M", folder+"SignalOfficialMC50M.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_tf", folder+"SignalOfficialMC50M_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_DNN", folder+"SignalOfficialMC50M_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("SignalOfficialMC50M_MVA", folder+"SignalOfficialMC50M_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("SignalOfficialMC50M_test", folder+"SignalOfficialMC50MwithTFweightTest.root", "ntuplizer/tree"); // TODO: drop? 

			filemanager.AddItem("BkgDstarDsMMultipleTau", folder+"firstDstarDsMultipleTau.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMMultipleTau_tf", folder+"firstDstarDsMultipleTau_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMMultipleTau_DNN", folder+"firstDstarDsMultipleTau_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("BkgDstarDsMultipleTau_MVA", folder+"firstDstarDsMultipleTau_tauDNN_mva.root", "tree"); 

			filemanager.AddItem("Data2018BFirst", folder+"prod2018BFirst.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_tf", folder+"prod2018BFirst_withTFweight.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_DNN", folder+"prod2018BFirst_tauDNN.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst_MVA", folder+"prod2018BFirst_tauDNN_mva.root", "tree"); 
			filemanager.AddItem("Data2018BFirst10k_tf", folder+"prod2018BFirst_withTFweight10k.root", "ntuplizer/tree"); 
			filemanager.AddItem("Data2018BFirst10k_DNN", folder+"prod2018BFirst_tauDNN10k.root", "ntuplizer/tree"); // Fixme 

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

			filemanager.AddItem("BkgDstarDsMMultipleTau", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau.root", "ntuplizer/tree"); 
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

		cut.emplace(std::make_pair("base", "(b_Ds_vprob>0.1) && (b_D0_vprob>0.1)")); 
		cut.emplace(std::make_pair("SR", cut["base"]+TCut(TString::Format("mvaScore>=%f", mvaCutSR)))); 

	}

}




#endif

