#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TLegend.h"
#include <iostream>
#include "DrawTMVAHistogram.C"
#include "GetSeparation.C"
#include "FileFlow.h"


using namespace ROOT; 
using namespace Ana; 


std::vector<TString> formats = {".pdf", ".png"};



template <typename T> 
T* PrettyPlot(const TString& name = "plot", const TString& options = "", T *obj = nullptr) 
{
	if (!obj)
	{
		obj = gPad; 
	}

	TCanvas *canv = new TCanvas("canvas", "canvas", 800, 600); 

	obj->DrawCopy(); 

	canv->Draw(options); 

	for (auto format : formats) 
	{
		canv->Print(name+format); 
	}

	canv->SaveAs(name+".root"); 

	return obj; 	
}



TObject* PrettyPlot(const TString& name = "plot", const TString& options = "", TObject *obj = nullptr) 
{
	if (!obj)
	{
		obj = gPad; 
	}

	TCanvas *canv = new TCanvas("canvas", "canvas", 800, 600); 

	obj->DrawCopy(); 

	canv->Draw(options); 

	for (auto format : formats) 
	{
		canv->Print(name+format); 
	}

	canv->SaveAs(name+".root"); 

	return obj; 	
}
