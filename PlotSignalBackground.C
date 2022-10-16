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


using namespace ROOT; 


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


void PlotSignalBackground(TString campaignName = "PlotsBackgroundComponentsNorm/") 
{
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
	filemanager.AddItem("MCSignal", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/PrivateProductionGenDstar_converted.root", "tree"); 
	//filemanager.AddItem("SignalOfficialMC50M", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M.root", "ntuplizer/tree"); 
	filemanager.AddItem("ParkingBPHAllRun2018B", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B.root", "ntuplizer/tree"); 
	filemanager.AddItem("SignalOfficialMC50M_mva", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted_mva.root", "tree"); 
	filemanager.AddItem("ParkingBPHAllRun2018B_mva", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted_mva.root", "tree"); 
	filemanager.AddItem("BkgBtoDstarDs", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsInclPrivateProdFirst.root", "ntuplizer/tree"); 
	filemanager.AddItem("BkgBtoDstar3piNonres", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/B0toDstar3piFirst.root", "ntuplizer/tree"); 
	filemanager.AddItem("BkgBtoDstarDsstar", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/BkgDstarDsstarInclPrivateProdFirst.root", "ntuplizer/tree"); 

	// Towards stable productions
filemanager.AddItem("SignalOfficialMC50M", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M.root", "ntuplizer/tree"); 
filemanager.AddItem("SignalOfficialMC50M_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_withTFweight.root", "ntuplizer/tree"); 
filemanager.AddItem("SignalOfficialMC50M_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_tauDNN.root", "ntuplizer/tree"); 
filemanager.AddItem("SignalOfficialMC50M_test", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50MwithTFweightTest.root", "ntuplizer/tree"); 
filemanager.AddItem("Data2018BFirstTest", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstAllTau.root", "ntuplizer/tree"); 
filemanager.AddItem("Data2018BFirstTest_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstAllTau_withTFweight.root", "ntuplizer/tree"); 
filemanager.AddItem("BkgDstarDsMMultipleTau", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/firstDstarDsMultipleTau.root", "ntuplizer/tree"); 
filemanager.AddItem("Data2018BFirst", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst.root", "ntuplizer/tree"); 
//filemanager.AddItem("Data2018BFirst_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_withTFweight.root", "ntuplizer/tree"); 
filemanager.AddItem("Data2018BFirst_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_tauDNN.root", "ntuplizer/tree"); 
filemanager.AddItem("Data2018BFirst10k_tf", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_withTFweight10k.root", "ntuplizer/tree"); 
//filemanager.AddItem("Data2018BFirst10k_DNN", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/prod2018BFirst_tauDNN10k.root", "ntuplizer/tree"); 




	filemanager.OpenAllItems(); 

	gStyle->SetOptStat(0); 


	auto dataframe = RDataFrame(*filemanager.GetItem<TTree*>("DstarDsMCfirst")); // tree100k

	auto sampleMC = RDataFrame(*filemanager.GetItem<TTree*>("Data2018BFirst")); 
	

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


	auto frame2 = dataframe.Define("P_D0", P_v, {"BsDstarTauNu_D0_pt", "BsDstarTauNu_D0_eta", "BsDstarTauNu_D0_phi", "BsDstarTauNu_D0_mass"}) // auto frame2 = dataframe.Define("LV_D0", "TLorentzVector LV_D0; LV_D0.SetPtEtaPhiM(BsDstarTauNu_D0_pt, BsDstarTauNu_D0_eta, BsDstarTauNu_D0_phi, BsDstarTauNu_D0_mass); return LV_D0"); 
					.Define("P_Ds", P_v, {"BsDstarTauNu_Ds_pt", "BsDstarTauNu_Ds_eta", "BsDstarTauNu_Ds_phi", "BsDstarTauNu_Ds_mass"})
					.Define("P_tau", P_v, {"BsDstarTauNu_tau_pt", "BsDstarTauNu_tau_eta", "BsDstarTauNu_tau_phi", "BsDstarTauNu_tau_mass"})
					.Define("B_mass", invMass_v, {"P_Ds", "P_tau"}); 

	

	auto histo1 = frame2.Histo1D("B_mass"); 

	auto histo2 = frame2.Histo2D({"Bmass_vs_Dmass", "Correlation plot between B and D masses", 100, 0., 7000., 100, 0., 5000.}, "BsDstarTauNu_B_mass", "BsDstarTauNu_D0_unfit_mass"); 


	//std::map<TString, TString> quantitiesToPlot2D = {{"BsDstarTauNu_B_mass", "BsDstarTauNu_D0_unfit_mass"}, {"BsDstarTauNu_B_mass", "BsDstarTauNu_Ds_unfit_mass"}, {"BsDstarTauNu_B_mass", "BsDstarTauNu_tau_mass"}, {"BsDstarTauNu_Ds_unfit_mass", "BsDstarTauNu_tau_mass"}}; 
	std::vector<std::pair<TString, TString> > quantitiesToPlot2D = {{"BsDstarTauNu_B_mass", "BsDstarTauNu_D0_unfit_mass"}, {"BsDstarTauNu_B_mass", "BsDstarTauNu_Ds_unfit_mass"}, {"BsDstarTauNu_B_mass", "BsDstarTauNu_tau_mass"}, {"BsDstarTauNu_tau_mass", "BsDstarTauNu_Ds_unfit_mass"}}; 


	std::vector<TString> quantitiesToPlot = {"BsDstarTauNu_D0_pt[1]", "BsDstarTauNu_D0_eta[1]", "BsDstarTauNu_D0_phi", "BsDstarTauNu_D0_unfit_mass", "BsDstarTauNu_D0_mass", 
											"BsDstarTauNu_Ds_pt", "BsDstarTauNu_Ds_eta", "BsDstarTauNu_Ds_phi", "BsDstarTauNu_Ds_unfit_mass", "BsDstarTauNu_Ds_mass", 
											"BsDstarTauNu_tau_pt", "BsDstarTauNu_tau_eta", "BsDstarTauNu_tau_phi", "BsDstarTauNu_tau_mass", //"BsDstarTauNu_tau_unfit_mass",
											"BsDstarTauNu_B_pt", "BsDstarTauNu_B_eta", "BsDstarTauNu_B_phi", "BsDstarTauNu_B_mass"}; //"BsDstarTauNu_B_unfit_mass",  "BsDstarTauNu_Ds_unfit_mass-BsDstarTauNu_D0_unfit_mass"

	quantitiesToPlot = {}; 

	std::vector<TString> quantitiesToPlotFromTree = {"BsDstarTauNu_D0_pt>>h(50, 0., 30.)", "BsDstarTauNu_D0_eta>>h(50, -3., 3.)", "BsDstarTauNu_D0_phi>>h(50, -3.5, 3.5)", "BsDstarTauNu_D0_unfit_mass>>h(100, 1.7, 2.)", "BsDstarTauNu_D0_mass>>h(50, 1.5, 2.5.)", 
											"BsDstarTauNu_Ds_pt>>h(50, 0., 50.)", "BsDstarTauNu_Ds_eta>>h(50, -3., 3.)", "BsDstarTauNu_Ds_phi>>h(50, -3.5, 3.5)", "BsDstarTauNu_Ds_unfit_mass>>h(50, 1.9, 2.1)", "BsDstarTauNu_Ds_mass>>h(50, 1.5, 2.5)", 
											"BsDstarTauNu_tau_pt>>h(100, 0., 50.)", "BsDstarTauNu_tau_eta>>h(100, -3., 3.)", "BsDstarTauNu_tau_phi>>h(100, -3.5, 3.5)", "BsDstarTauNu_tau_mass>>h(100, 0., 30.)", //"BsDstarTauNu_tau_unfit_mass",
											//"tau_doca>>h(100, -7., 2.)", "tau_docaerror>>h(100, 0., 1.)", "tau_docasigma>>h(100, -40., 20.)", "tau_tracks_dR_Dstar", 
											"BsDstarTauNu_B_q2>>h(100, 0., 5.)", "BsDstarTauNu_B_mm2>>h(100, -0.01, 0.005)", 
											"BsDstarTauNu_B_pt>>h(100, 0., 80.)", "BsDstarTauNu_B_eta>>h(100, -3., 3.)", "BsDstarTauNu_B_phi>>h(100, -3.5, 3.5)", "BsDstarTauNu_B_mass>>h(100, 0., 30.)", "BsDstarTauNu_Ds_unfit_mass-BsDstarTauNu_D0_unfit_mass>>h(200, 0.1, 0.2)", 
											"BsDstarTauNu_D0_fl3d>>h(200, -2., 5.)", "BsDstarTauNu_D0_fls3d>>h(200, 0., 10.)", "BsDstarTauNu_D0_pvip>>h(200, -1., 1.)", "BsDstarTauNu_D0_pvips>>h(200, -1., 10.)", "BsDstarTauNu_D0_lip>>h(200, -1., 1.)", "BsDstarTauNu_D0_lips>>h(200, -1., 10.)", 
											"BsDstarTauNu_Ds_fl3d>>h(200, -2., 5.)", "BsDstarTauNu_Ds_fls3d>>h(100, -0.01, 10.)", "BsDstarTauNu_Ds_pvip>>h(200, -1., 1.)", "BsDstarTauNu_Ds_pvips>>h(200, -1., 10.)", "BsDstarTauNu_Ds_lip>>h(200, -1., 1.)", "BsDstarTauNu_Ds_lips>>h(200, -0.5, 0.5)", 
											"BsDstarTauNu_tau_fl3d>>h(200, -0.5, 0.5)", "BsDstarTauNu_tau_fls3d>>h(200, -0.5, 10.)", "BsDstarTauNu_tau_pvip>>h(200, -0.5, 0.5)", "BsDstarTauNu_tau_pvips>>h(200, -1., 20.)", "BsDstarTauNu_tau_lip>>h(200, -1., 1.)", "BsDstarTauNu_tau_lips>>h(200, -1., 10.)", 
											"track_genmatched_doca>>h(100, -1.5, 0.5)", "track_genmatched_docaerror>>h(100, -1., 2.)", "track_genmatched_docasigma>>h(100, -5., 10.)", "track_genmatched_dR_Dstar>>h(200, -1., 10.)", "track_genmatched_doca2D>>h(200, -2., 2.)", "track_genmatched_doca2Derror>>h(200, -1., 2.)", "track_genmatched_doca2Dsigma>>h(200, -100., 100.)", "track_genmatched_pvAssociationQuality>>h(200, -100., 100.)", 
											"track_genmatched_pt>>h(100, -1., 30.)", "track_genmatched_eta>>h(100, -3., 3.)", "track_genmatched_phi>>h(100, -3.5, 3.5)", "track_genmatched_isAssociatedToPV>>h(200, -1., 2.)", "track_genmatched_dzToPV>>h(100, -1., 1.)",
											"BsDstarTauNu_tau_sumofdnn>>h(200, -0.5, 3.5)", "max(max(BsDstarTauNu_tau_pfidx1, BsDstarTauNu_tau_pfidx2), BsDstarTauNu_tau_pfidx3)>>h(200, -0.5, 15.)", "min(min(BsDstarTauNu_tau_pfidx1, BsDstarTauNu_tau_pfidx2), BsDstarTauNu_tau_pfidx3)>>h(200, -0.5, 15.)", "BsDstarTauNu_tau_max_dr_3prong>>h(100, 0., 1.)"
											}; //"BsDstarTauNu_B_unfit_mass",  "BsDstarTauNu_Ds_unfit_mass-BsDstarTauNu_Ds_unfit_mass" 

	std::vector<TString> quantitiesNew = { "BsDstarTauNu_tau_rhomass1>>h(100, 0., 3.)", "BsDstarTauNu_tau_rhomass2>>h(100, 0., 3.)", "BsDstarTauNu_B_mass>>h(100, 2., 6.)", "BsDstarTauNu_tau_mass>>h(100, 0., 2.)" }; 

	/*quantitiesToPlot2D*/ quantitiesToPlotFromTree = quantitiesNew; 


	//quantitiesToPlotFromTree = {}; 
	quantitiesToPlot2D = {}; 


	TTree *tree = filemanager.GetItem<TTree*>("DstarDsMCfirst"); 

	std::vector<TString> formatsToPlot = {".pdf", ".png"}; 

	//TString campaignName = "PlotsDstarDsVsSignal/"; 

	bool normalise = true; 

	TString outfolder = "plots/"+campaignName; //plots/"+campaignName; //"plots/PlotsGenmactchedFinal/"

	bool webpublication = false; 

	if (gSystem->AccessPathName(outfolder)) gSystem->Exec("mkdir -p "+outfolder); 

	TString webfolder = "/eos/home-m/mhuwiler/www/Analysis/"+campaignName; // "/eos/home-m/mhuwiler/www/Analysis/DataMCplotsGenmatchedFinal/"
	if (webpublication) 
	{ 
		if (gSystem->AccessPathName(webfolder)) gSystem->Exec("mkdir -p "+webfolder); 
		TString webenginesource = "/eos/home-m/mhuwiler/software/php-plots/"; 
		gSystem->Exec("cp -r "+webenginesource+"res "+webfolder); 
		gSystem->Exec("cp "+webenginesource+"index.php "+webfolder); 
		// Get and edit the permission file
		ifstream accessFileSource(webenginesource+"example/htaccess"); 
		ofstream accessFileTarget(webfolder+".htaccess"); 
		std::string accessFileLine; 
		while (getline(accessFileSource, accessFileLine)) 
		{
			TString targetFileLine(accessFileLine); 
			targetFileLine.ReplaceAll("/<me>/<my-project>/", webfolder); 
			accessFileTarget << targetFileLine; 
		}
		accessFileSource.close(); 
		accessFileTarget.close(); 

		// Hack to have the file synchronised over the cernbox client(without hidden fyle sync), needs to copy it back from htaccess to .htaccess in the target directory. 
		gSystem->Exec("cp "+webfolder+".htaccess "+webfolder+"htaccess"); 

		// TODO: cleanup this hacky editing of variable! 
		webfolder = webfolder+"plots/"; 
		if (gSystem->AccessPathName(webfolder)) gSystem->Exec("mkdir -p "+webfolder); 
	}


	for (auto quantity : quantitiesToPlot) 
	{
		TCanvas *canvas = new TCanvas(quantity, quantity, 800, 600); 

		Double_t minVal = 0, maxVal = 100.; 

		if (quantity.Contains("_pt")) 
		{
			minVal = 0.; 
			maxVal = 100.; 
		}
		else if (quantity.Contains("_eta")) 
		{
			minVal = -3.; 
			maxVal = 3.; 
		}
		else if (quantity.Contains("_phi")) 
		{
			minVal = -3.5; 
			maxVal = 3.5; 
		}
		else if (quantity.Contains("_mass")) 
		{
			minVal = 0.; 
			maxVal = 20.; 
		}
		if ((quantity.Contains("D0") or quantity.Contains("Ds")) and quantity.Contains("_mass")) 
		{
			minVal = 1.; 
			maxVal = 2.; 
		}

		TH1D modelhisto(quantity, quantity, 200, minVal, maxVal); 

		//auto histo = dataframe.Histo1D(quantity.Data()); 

		//auto histMC = sampleMC.Histo1D(quantity.Data()); 

		//auto histoData = histo.GetPtr(); 

		//auto histoMC = histMC.GetPtr(); 

		//TODO: edit histogram 


		
		TH1D *histoData = new TH1D("histoData", "histoData", 200, minVal, maxVal); 
		filemanager.GetItem<TTree*>("prodlatest")->Draw(TString::Format("%s>>histoData", quantity.Data())); 
		histoData->GetYaxis()->SetTitleOffset(0.9); 
		TH1D *histoMC = new TH1D("histoMC", "histoMC", 200, minVal, maxVal); 
		filemanager.GetItem<TTree*>("MC")->Draw(TString::Format("%s>>histoMC", quantity.Data())); 

		histoMC->Scale(histoData->Integral()/histoMC->Integral()); 

		histoData->SetLineColor(kBlue); 
		histoMC->SetLineColor(kRed); 

		histoData->SetLineWidth(2); 
		histoMC->SetLineWidth(2); 


		TLegend *legend= new TLegend( canvas->GetLeftMargin(), 
                                    1-canvas->GetTopMargin()-.15, 
                                    //canvas->GetLeftMargin()+.4, 
                                    canvas->GetLeftMargin()+(1.-(canvas->GetLeftMargin()+canvas->GetRightMargin())),
                                    1-canvas->GetTopMargin() );
      	legend->SetFillStyle(1);
      	legend->AddEntry(histoData,"data","F");
      	legend->AddEntry(histoMC,"signal MC","F");
      	legend->SetBorderSize(1);
      	legend->SetMargin( 0.3 );
      	legend->SetTextSize(0.04);

      	histoData->SetLineColor(kBlue); 
      	histoMC->SetLineColor(kRed); 

      	Float_t sc = 1.3;
      	histoData->SetMaximum( TMath::Max(histoData->GetMaximum(), histoMC->GetMaximum())*sc );


		//histo->GetXaxis()->SetRangeUser(0., 100.); 

		canvas->Draw(); 

		for (auto format : formatsToPlot) 
		{
			canvas->Print(outfolder+quantity+format); 
		}

		delete canvas; 
	}

	for (auto quantity : quantitiesToPlot2D) 
	{
		TString name = quantity.first+"_vs_"+quantity.second; 

		std::cout << "Plotting: " << name << std::endl; 

		TCanvas *canvas = new TCanvas(name, name, 800, 600); 

		//Double_t minHvalue = *(dataframe.Min<Double_t>(quantity.first.Data())); 

		//Double_t maxHvalue = *(dataframe.Max<Double_t>(quantity.first.Data())); 

		TH2D modelhisto(name, name, 100, 0., 7., 100, 1.5, 2.2); 

		auto histo = dataframe.Histo2D(modelhisto, quantity.first.Data(), quantity.second.Data()); 

		TGraph2D *graph = new TGraph2D(histo.GetPtr()); 

		//TODO: edit histogram 

		//histo->SetBinContent(0, 0, -1.); 
		Int_t j = 0; 
		for (unsigned int i=0; i<histo->GetNbinsX(); i++) 
		{
			for (unsigned int j=0; j<histo->GetNbinsY(); j++) 
			{
				auto value = histo->GetBinContent(i, j); 
				//std::cout << "Value: " << value << std::endl; 
				if (value <= 0.) 
				{
					histo->SetBinContent(i, j, 0.01); 
				}
			}
		}

		histo->Draw("COLZ"); 

		canvas->Draw(); 

		for (auto format : formatsToPlot) 
		{
			canvas->Print(outfolder+name+format); 
		}

		delete canvas; 
	}

	for (auto quantity : quantitiesToPlotFromTree) 
	{
		auto strings = quantity.Tokenize(">>"); 

		//strings->Print(); 

		TString name = static_cast<TObjString*>(strings->At(0))->GetString(); 

		std::cout << "To be drawn: " << quantity << std::endl; 
		TCanvas *canvas = new TCanvas(name, name, 800, 600); 

		
		//tree->Draw(quantity); 

		if (not quantity.Contains(">>")) quantity+=">>h"; 

		TString quantityData = quantity; 
		quantityData.ReplaceAll("_genmatched", ""); 

		std::cout << "Quantity: " << quantity << std::endl; 
		std::cout << "Quantity data: " << quantityData << std::endl; 

		//if (quantity.Contains("track_genmatched_dR_Dstar")) quantityData.ReplaceAll("track_", "track_tracks_"); 
		//if (quantity.Contains("track_genmatched_pvAssociationQuality")) quantityData.ReplaceAll("track_", "track_track_"); 
		//if (quantity.Contains("track_dR_Dstar")) quantityData.ReplaceAll("track_", "track_tracks_"); 
		//if (quantity.Contains("track_pvAssociationQuality")) quantityData.ReplaceAll("track_", "track_track_"); 

		std::cout << "Quantity data: " << quantityData << std::endl; 

		//histo->GetXaxis()->SetRangeUser(0., 100.); 

		TString cut = "(BsDstarTauNu_D0_vprob>0.1) && (BsDstarTauNu_Ds_vprob>0.1)"; 

		filemanager.GetItem<TTree*>("Data2018BFirst")->Draw(quantityData.ReplaceAll(">>h", ">>h1"), cut); 
		TH1 *histoData = static_cast<TH1*>(canvas->GetPrimitive("h1")); 
		histoData->GetYaxis()->SetTitleOffset(0.9); 
		histoData->SetTitle(""); 
		filemanager.GetItem<TTree*>("SignalOfficialMC50M")->Draw(TString(quantity).ReplaceAll(">>h", ">>h2"), cut); 
		TH1 *histoMC = static_cast<TH1*>(canvas->GetPrimitive("h2")); 
		histoMC->SetTitle(""); 
		filemanager.GetItem<TTree*>("BkgDstarDsMMultipleTau")->Draw(TString(quantity).ReplaceAll(">>h", ">>h3"), cut); 
		TH1 *histoBkgDs = static_cast<TH1*>(canvas->GetPrimitive("h3")); 
		histoBkgDs->SetTitle(""); 
		filemanager.GetItem<TTree*>("BkgBtoDstarDsstar")->Draw(TString(quantity).ReplaceAll(">>h", ">>h4"), cut); 
		TH1 *histoBkgDsstar = static_cast<TH1*>(canvas->GetPrimitive("h4")); 
		histoBkgDsstar->SetTitle(""); 
		filemanager.GetItem<TTree*>("BkgBtoDstar3piNonres")->Draw(TString(quantity).ReplaceAll(">>h", ">>h5"), cut); 
		TH1 *histoBkg3Pi = static_cast<TH1*>(canvas->GetPrimitive("h5")); 
		histoBkg3Pi->SetTitle(""); 

		// Plot roc curve here 
		//if (name == "track_genmatched_doca") 
		//{
		//	TString outname = outfolder+name+"ROC.root"; 
		//	std::cout << "making ROC curve for: " << name << " in file: " << outname << std::endl; 
		//	TCanvas *efficiencyCanvas = new TCanvas("efficiencyCanvas", "efficiencyCanvas", 800, 600); 
		//	filemanager.GetItem<TTree*>("MCgenmatched")->Draw(name+">>h(1000, -1.5, 0.5)"); 

//		//	TH1 * effHisto = static_cast<TH1*>(efficiencyCanvas->GetPrimitive("h")); 
		//	Double_t denominator = effHisto->Integral(); 

//		//	for (unsigned int i=0; i<1000; i++) {
		//		
		//	}
		//}


		std::cout << "Number of MC events: " << histoMC->Integral() << std::endl; 

		//histoMC->Scale(histoData->Integral()/histoMC->Integral()); 
		if (normalise) 
		{
			//histoData->Scale(1./histoData->Integral()); 
			histoMC->Scale(49.7/histoMC->Integral()); 
			histoBkgDs->Scale(249./histoBkgDs->Integral());
			histoBkgDsstar->Scale(1./histoBkgDsstar->Integral()); 
			histoBkg3Pi->Scale(1./histoBkg3Pi->Integral()); 
		}

		histoData->SetLineColor(kBlue); 
		histoMC->SetLineColor(kRed); 
		histoBkgDs->SetLineColor(kGreen); 
		histoBkgDsstar->SetLineColor(kGreen+3); 
		histoBkg3Pi->SetLineColor(kOrange+2); 

		histoData->SetLineWidth(2); 
		histoMC->SetLineWidth(2); 
		histoBkgDs->SetLineWidth(2); 
		histoBkgDsstar->SetLineWidth(2); 
		histoBkg3Pi->SetLineWidth(2); 

		std::vector<double> maxes = { histoData->GetMaximum(), histoMC->GetMaximum(), histoBkgDs->GetMaximum() }; 



		TLegend *legend= new TLegend( canvas->GetLeftMargin()+0.35, 
                                    1-canvas->GetTopMargin()-.15, 
                                    //canvas->GetLeftMargin()+.4, 
                                    canvas->GetLeftMargin()+(1.-(canvas->GetLeftMargin()+canvas->GetRightMargin())),
                                    1-canvas->GetTopMargin() );
      	legend->SetFillStyle(1);
      	legend->AddEntry(histoData,"data","F");
      	legend->AddEntry(histoMC,"signal MC (genmatched)","F");
      	legend->AddEntry(histoBkgDs, "B^{0}#rightarrow D^{#ast}D_{s} Inclusive"); 
      	legend->AddEntry(histoBkgDsstar, "B^{0}#rightarrow D^{*}D_{s}^{*} Inclusive"); 
      	legend->AddEntry(histoBkg3Pi, "B^{0}#rightarrow D^{*}3#pi Non resonant"); 
      	legend->SetBorderSize(1);
      	legend->SetMargin( 0.3 );
      	legend->SetTextSize(0.04);

      	histoData->SetLineColor(kBlue); 
      	histoMC->SetLineColor(kRed); 

      	//histoData->SetTitle(name); 

      	Float_t sc = 1.3;
      	//if not (histo->GetMaximum())
      	auto it = max_element(std::begin(maxes), std::end(maxes));

      	histoData->SetMaximum( *it*static_cast<float>(sc) ); // TMath::Max(histoData->GetMaximum(), histoMC->GetMaximum())*sc
      	std::cout << histoData->GetMaximum() << " " << histoMC->GetMaximum() << std::endl; 

      	histoData->Draw("HIST"); 
      	histoMC->Draw("HISTSAME"); 
      	histoBkgDs->Draw("HISTSAME"); 
      	histoBkgDsstar->Draw("HISTSAME"); 
      	histoBkg3Pi->Draw("HISTSAME"); 

      	legend->Draw(); 


		//histo->GetXaxis()->SetRangeUser(0., 100.); 

		canvas->Draw(); 

		for (auto format : formatsToPlot) 
		{
			canvas->Print(outfolder+name+format); 
			if (webpublication) canvas->Print(webfolder+name+format); 
		}

		delete canvas; 
	}

	//histo1->Draw(); 

	//histo2->Draw(); 

	//canvas->Draw(); 

	//canvas->Print("Example.pdf"); // Works 

	//Pause(5); 

	//PauseUntilEnter(); //system("pause"); 

	filemanager.CloseAll(); 


}

