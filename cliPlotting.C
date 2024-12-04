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

	obj->Draw(); 

	canv->Draw(options); 

	for (auto format : formats) 
	{
		canv->Print(name+format); 
	}

	canv->SaveAs(name+".root"); 

	return obj; 	
}


void SetHeader( TVirtualPad* pad, bool outOfFrame=true)
{            
	int iPosX = 1; 

	bool writeExtraText = true;
	float extraOverCmsTextSize  = 0.76;	

	TString cmsText = "CMS"; 
	float cmsTextFont   = 61;  // default is helvetic-bold
	TString extraText   = "Preliminary";
	float extraTextFont = 52;  // default is helvetica-italics	

	// text sizes and text offsets with respect to the top frame
	// in unit of the top margin size
	float lumiTextSize     = 0.5; //0.6
	float lumiTextOffset   = 0.2;
	float cmsTextSize      = 0.75;
	float cmsTextOffset    = 0.1;  // only used in outOfFrame version	

	float relPosX    = 0.045;
	float relPosY    = 0.035;
	float relExtraDY = 1.2;	


  	int alignX_=2;
  	int alignY_=3;
  	
  	if(iPosX==0) 
  	{
  		alignX_=1;
  		alignY_=1;
  	}

  	if( iPosX/10==0 ) alignX_=1;
  
  	if( iPosX/10==1 ) alignX_=1;
  	if( iPosX/10==2 ) alignX_=2;
  	if( iPosX/10==3 ) alignX_=3;
  	//if( iPosX == 0  ) relPosX = 0.12;

  	int align_ = 10*alignX_ + alignY_;

 	float H = pad->GetWh();
	float W = pad->GetWw();
	float l = pad->GetLeftMargin();
	float t = pad->GetTopMargin();
	float r = pad->GetRightMargin();
	float b = pad->GetBottomMargin();
  //  float e = 0.025;

  	pad->cd();

  	TString lumiText = "33.6 fb^{-1} (13 TeV)";

  	TLatex latex;
  	latex.SetNDC();
  	latex.SetTextAngle(0);
  	latex.SetTextColor(kBlack);    

  	float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  	latex.SetTextFont(42);
  	latex.SetTextAlign(31); 
  	latex.SetTextSize(lumiTextSize*t);    
  	latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  	if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11); 
      latex.SetTextSize(cmsTextSize*t);    
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }
  
  	pad->cd();

  float posX_=0;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
	  latex.SetTextFont(cmsTextFont);
	  latex.SetTextSize(cmsTextSize*t);
	  latex.SetTextAlign(align_);
	  latex.DrawLatex(posX_, posY_, cmsText);
	  if( writeExtraText ) 
	    {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
    }
  else if( writeExtraText )
    {
      if( iPosX==0) 
	{
	  posX_ =   l +  relPosX*(1-l-r);
	  posY_ =   1-t+lumiTextOffset*t;
	}
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);      
    }
  return;
}


void Style() 
{
	auto plot = gPad; 
	gStyle->SetOptStat(0);
	plot->SetTitle(""); 
	static_cast<TH1*>(plot->GetPrimitive("htemp"))->SetTitle("");
	plot->Draw(); 
	SetHeader(plot); 

}
