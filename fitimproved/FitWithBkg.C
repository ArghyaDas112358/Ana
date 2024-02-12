// signal histo ===> "bg_res"
//
// please fit data_obs with parameterized signal histo + whatever function you want

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooAddPdf.h"
#include "RooConstVar.h"
#include "RooAbsReal.h"
#include "RooBinning.h"
#include "RooCBShape.h"
#include "RooCategory.h"
#include "RooChebychev.h"
#include "RooConstVar.h"
#include "RooDLLSignificanceMCSModule.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooDecay.h"
#include "RooErrorVar.h"
#include "RooExponential.h"
#include "RooExtendPdf.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#include "RooFormulaVar.h"
#include "RooGaussModel.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooHist.h"
#include "RooHistPdf.h"
#include "RooMCStudy.h"
#include "RooNLLVar.h"
#include "RooNumConvPdf.h"
#include "RooLandau.h"
#include "RooPlot.h"
#include "RooPolynomial.h"
#include "RooProdPdf.h"
#include "RooProfileLL.h"
#include "RooRealVar.h"
#include "RooSimultaneous.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "TColor.h"
#include "RooStats/MaxLikelihoodEstimateTestStat.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/NumberCountingPdfFactory.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/RatioOfProfiledLikelihoodsTestStat.h"
#include "RooStats/SPlot.h"
#include "RooVoigtian.h"
#include "RooWorkspace.h"
#include "RooArgusBG.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH3.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveLabel.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TTree.h"
#include "RooKeysPdf.h"
#include "RooNDKeysPdf.h"

#define DrawResiduals

using namespace RooFit ;
using namespace RooStats ;

void fit_jpsikpipi();
void FitWithBkg() { fit_jpsikpipi(); }
void fit_jpsikpipi() {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  RooRealVar b_B_m ("b_B_m", "b_B_m", 4.9, 6.0);

  TFile *ntuple_data = new TFile("/eos/home-m/mhuwiler/public/RDstarHadronic/data/dataWithCuts.root"); // datanal MC
  TTree* tree_data   = (TTree*) ntuple_data->Get("tree");
  RooArgSet Variablesd(b_B_m);
  RooDataSet *data = new RooDataSet("data", "data", tree_data, Variablesd); //, puweight.GetName());

  int Bc_bins = 55;

  // signal
  RooRealVar mean_m  ("mean_m","mean of gaussian", 5.28, 5.25, 5.3);
  RooRealVar sigma_m ("sigma_m","Scale Factor 1",  7.95105e-02, 7.95105e-02-30.*2.08484e-03 ,7.95105e-02+30.*2.08484e-03);
  RooGaussian mSig01   ("mSig01","signal p.d.f.", b_B_m, mean_m, sigma_m);

  RooRealVar frac ("frac", "frac", 0.8, 0., 1);

  RooRealVar sigma_cb ("sigma_cb","Scale Factor 1", 4.31021e-02, 4.31021e-02-3.*4.31021e-03, 4.31021e-02+3.*4.31021e-03);
  RooRealVar alpha("alpha", "Alpha", 1.2); //5,0.1,10);
  RooRealVar n("n", "Order", 10);          //6,0.1,10);
  RooCBShape  mSig31   ("mSig31","signa p.d.f.", b_B_m, mean_m, sigma_m, alpha,n);

  RooAddPdf mSig0 ("mSig0", "mSig0", RooArgList(mSig01, mSig31), frac);

  RooRealVar N_data_mSig     ("N_data_mSig",     "N_data_mSig",    80000., 0., 100000000.);
//  RooExtendPdf e_data_mSig   ("e_data_mSig",  "e_data_mSig",  mSig0,  N_data_mSig);  //  << -- -- -- -- in case you want to use CB+Gauss
  RooExtendPdf e_data_mSig   ("e_data_mSig",  "e_data_mSig",  mSig01,  N_data_mSig);  // << -- -- -- -- only Gauss

  // define background
  // exponential
  RooRealVar        c0("c0", "c0", -.5, -4., 0.);
  RooExponential mBkg0("mBkg0", "exponential", b_B_m, c0);

  //Error Function
  RooRealVar meanErf   ("meanErf", "mean of the Erf gaussian",     5.11262, 5.1, 5.2); 
  RooRealVar sigmaErf  ("sigmaErf", "width of the Erf gaussian",   0.0386, 0.010, 0.040); 
  RooRealVar ErfOffset ("ErfOffset", "Offset of Erf exponential", 5.05442e+00, 5.01, 5.1); //6.100, 6.000, 6.200);  //5.07743e+00); //5.100, 5.000, 5.200); 
//  RooRealVar ErfSlope  ("ErfSlope", "Erf Slope",                   0, -3., 3.);
//  RooGenericPdf Erf    ("Erf", "Error Function", "TMath::Exp(TMath::Abs(ErfSlope)*(b_B_m-ErfOffset))*TMath::Erfc((b_B_m-meanErf)/sigmaErf)", RooArgSet(b_B_m, meanErf, sigmaErf,ErfSlope,ErfOffset));
  RooGenericPdf Erf    ("Erf", "Error Function", "TMath::Erfc((b_B_m-meanErf)/sigmaErf)", RooArgSet(b_B_m, meanErf, sigmaErf,ErfOffset));

  RooRealVar frac0 ("frac0", "frac0", 0.5, 0., 1.);
  RooAddPdf totalbackground ("totalbackground", "totalbackground", RooArgSet(mBkg0, Erf), frac0);

  RooRealVar Ntotalbackground ("Ntotalbackground", "Ntotalbackground", 10000, 0, 1000000);
  RooExtendPdf etotalbackground ("etotalbackground", "etotalbackground", totalbackground, Ntotalbackground);
//  RooExtendPdf etotalbackground ("etotalbackground", "etotalbackground", mBkg0, Ntotalbackground);

  RooAddPdf total_signal_data ("total_signal_data", "total_signal_data", RooArgSet(etotalbackground, e_data_mSig));

  RooFitResult *fr_data = total_signal_data.fitTo(*data, NumCPU(4, kTRUE), Save(), Extended());

  // plot
  RooPlot *frame_data_fit = b_B_m.frame(Title("b_B_m 1 fit"), Bins(Bc_bins));
  data->plotOn(frame_data_fit, XErrorSize(0), Name("plotmc"));
  total_signal_data.plotOn(frame_data_fit, LineColor(kPink-1), Name("total_data"));
  total_signal_data.plotOn(frame_data_fit, Components("e_data_mSig"), LineColor(kAzure+2));
  total_signal_data.plotOn(frame_data_fit, Components("etotalbackground"), LineColor(kAzure+3));
  total_signal_data.plotOn(frame_data_fit, Components("Erf"), LineColor(kAzure+5));
  data->plotOn(frame_data_fit, XErrorSize(0));

  TCanvas *c = new TCanvas("c", "c", 1000, 1000); c->cd();
  #ifdef DrawResiduals
    RooPlot* dummy_frame_03 = b_B_m.frame(Title("dummy frame to extract residuals"), Bins(Bc_bins));
  
    data->plotOn(dummy_frame_03,XErrorSize(0));
    total_signal_data.plotOn(dummy_frame_03);
  
    RooHist* h_residuals_mass_03 = dummy_frame_03->pullHist();
    RooPlot* Z_mass_residual_frame_03 = b_B_m.frame(Bins(Bc_bins));
  
    Z_mass_residual_frame_03->GetYaxis()->SetTitleSize(0.34);
    Z_mass_residual_frame_03->GetYaxis()->SetLabelSize(.30);
    Z_mass_residual_frame_03->GetYaxis()->SetTitleOffset(.2);
    Z_mass_residual_frame_03->GetYaxis()->SetNdivisions(5);
    Z_mass_residual_frame_03->addPlotable(h_residuals_mass_03, "P");
  
    TPad *pad31 = new TPad("pad31", "The pad3 80% of the height",0.0,0.05,1.0,1.0,21);
    TPad *pad32 = new TPad("pad32", "The pad3 20% of the height",0.0,0.0,1.0,0.1,22);
    pad31->SetFillColor(0); pad32->SetFillColor(0);
    pad31->Draw();
    pad32->Draw();
    pad32->cd(); Z_mass_residual_frame_03->Draw(); 
    TF1 *f_straighline = new TF1("f_straighline", "0", -1000, 1000); f_straighline->SetLineColor(kRed+2); //f_straighline->SetLineWidth(0.8);
    f_straighline->Draw("same");// f_1->Draw("same"); f_2->Draw("same");
    pad31->cd();
  #endif
  frame_data_fit->GetXaxis()->SetNdivisions(504);
  frame_data_fit->Draw(); //c_data->cd(1)->SetLogy(1);

  c->SaveAs("canvas_b_B_m_2016.pdf");


}
