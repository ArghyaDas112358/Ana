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
#include "../FileFlow.h"

#define DrawResiduals

using namespace RooFit ;
using namespace RooStats ;

void fit_jpsikpipi();
void FitWithsPlot() { fit_jpsikpipi(); }
void fit_jpsikpipi() {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  Ana::Init("v7.01", 1);

  Int_t ci1 = 1756;
  TColor *color1 = new TColor(ci1, 215./255., 48. /255., 39. /255.);

  RooRealVar b_B_m ("b_B_m", "b_B_m", 4.9, 6.0);
  RooRealVar b_B_eta ("b_B_eta", "b_B_eta", -3, 3);
  RooRealVar b_B_pt ("b_B_pt", "b_B_pt", 0, 100);
  RooRealVar b_B_phi ("b_B_phi", "b_B_phi", -4, 4);

  RooRealVar b_tau_rhomass1 ("b_tau_rhomass1", "b_tau_rhomass1", 0, 5);
  RooRealVar b_tau_phimass1 ("b_tau_phimass1", "b_tau_phimass1", 0, 5);

  TFile *ntuple_data = new TFile("/eos/home-m/mhuwiler/public/RDstarHadronic/data/dataWithCuts.root"); // datanal MC
  TTree* tree_data   = (TTree*) ntuple_data->Get("tree");
  RooArgSet Variablesd(b_B_m, b_B_eta, b_B_pt, b_B_phi, b_tau_rhomass1, b_tau_phimass1);
  RooDataSet *data = new RooDataSet("data", "data", Ana::GetSample("dataD123_DNN"), Variablesd); //, puweight.GetName());

  TFile *ntuple_mc = new TFile("../B0toDstarrho0piWithCuts.root"); 
  TTree* tree_mc   = (TTree*) ntuple_mc->Get("tree");
  RooDataSet *mc = new RooDataSet("mc", "mc", Ana::GetSample("B0toDstar3pi_DNN"), Variablesd);

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

  c->SaveAs("canvas_b_B_m_sPlot.pdf");

  // splot stuff
  RooWorkspace* ws = new RooWorkspace("myWS");
  ws->import(total_signal_data);
  ws->import(*data, Rename("sPlotdata"));

  RooAbsPdf*  sPloteOniaSignal = ws->pdf("total_signal_data");
  RooRealVar* SignalPromptYield  = ws->var("N_data_mSig");
  RooRealVar* BckgdPromptYield = ws->var("Ntotalbackground");

  SignalPromptYield->setConstant();
  BckgdPromptYield->setConstant();

  RooStats::SPlot* sData = new RooStats::SPlot("sData","An SPlot", *data, sPloteOniaSignal, RooArgList(*SignalPromptYield, *BckgdPromptYield));

  RooDataSet * data_weighted = new RooDataSet(data->GetName(), data->GetTitle(), data, *data->get(), 0, "N_data_mSig_sw");

  // plotting
  TF1 *f_1 = new TF1("f_1", "0.5", -1000, 1000); f_1->SetLineColor(kBlue); f_1->SetLineStyle(7); 
  TF1 *f_2 = new TF1("f_2", "1.5", -1000, 1000); f_2->SetLineColor(kBlue); f_2->SetLineStyle(7); 
  TF1 *f_straighline0 = new TF1("f_straighline0", "1", -1000, 1000); f_straighline0->SetLineColor(kRed+2); //f_straighline0->SetLineWidth(0.8);

  TCanvas *c_01 = new TCanvas("c_01", "c_01", 1200, 400); c_01->Divide(3,1);

  c_01->cd(1); 
  RooPlot *frame_b_B_eta_weighted = b_B_eta.frame(Title("B eta"), Bins(10));
  data_weighted->plotOn(frame_b_B_eta_weighted, XErrorSize(0), DataError(RooAbsData::Poisson));
  frame_b_B_eta_weighted->Draw(); //frame_b_B_eta_weighted->GetYaxis()->SetRangeUser(0., 10.); 

  TH1* tmp_eta_06 = data_weighted->createHistogram("b_B_eta",10);
  TH1* tmp_eta_6  = mc->createHistogram("b_B_eta",10); tmp_eta_6->Scale(N_data_mSig.getVal() / mc->sumEntries()); tmp_eta_6->Draw("hesame"); tmp_eta_6->SetLineColor(1756); tmp_eta_6->SetMarkerSize(0);
  TH1F *h_eta_pull = new TH1F("h_eta_pull", "h_eta_pull", 10, -3, 3); h_eta_pull->SetBinErrorOption(TH1::kPoisson); h_eta_pull->Divide(tmp_eta_06, tmp_eta_6); //h_eta_pull->Draw("e1same"); 
  TPad *padlb_B_eta1 = new TPad("padlb_B_eta1", "The pad 80% of the height",0.0,0.15,1.0,1.0,21);
  TPad *pad2b_B_eta1 = new TPad("pad2b_B_eta1", "The pad 20% of the height",0.0,0.0,1.0,0.15,22);
  padlb_B_eta1->SetFillColor(0); pad2b_B_eta1->SetFillColor(0);
  padlb_B_eta1->Draw();
  pad2b_B_eta1->Draw();
  pad2b_B_eta1->cd(); h_eta_pull->Draw("e1"); h_eta_pull->GetYaxis()->SetNdivisions(4); f_straighline0->Draw("same"); f_1->Draw("same"); f_2->Draw("same"); h_eta_pull->Draw("e1same"); h_eta_pull->GetYaxis()->SetRangeUser(0,2); h_eta_pull->GetYaxis()->SetLabelSize(0.2); //h_eta_pull->GetXaxis()->SetRangeUser(10, 70);
  padlb_B_eta1->cd(); tmp_eta_06->Draw("e1");  tmp_eta_6->Draw("hesame"); //tmp_eta_06->GetXaxis()->SetRangeUser(10, 70); tmp_eta_06->GetYaxis()->SetRangeUser(-.4, 10.); 

  c_01->cd(2); int B_pt_bins = 20;
  RooPlot *frame_b_B_pt_weighted = b_B_pt.frame(Title("B pt"), Bins(B_pt_bins));
  data_weighted->plotOn(frame_b_B_pt_weighted, XErrorSize(0), DataError(RooAbsData::Poisson));
  frame_b_B_pt_weighted->Draw(); //frame_b_B_pt_weighted->GetYaxis()->SetRangeUser(0., 10.); 

  TH1* tmp_pt_06 = data_weighted->createHistogram("b_B_pt",B_pt_bins);
  TH1* tmp_pt_6  = mc->createHistogram("b_B_pt",B_pt_bins); tmp_pt_6->Scale(N_data_mSig.getVal() / mc->sumEntries()); tmp_pt_6->Draw("hesame"); tmp_pt_6->SetLineColor(1756); tmp_pt_6->SetMarkerSize(0);
  TH1F *h_pt_pull = new TH1F("h_pt_pull", "h_pt_pull", B_pt_bins, 0, 100); h_pt_pull->SetBinErrorOption(TH1::kPoisson); h_pt_pull->Divide(tmp_pt_06, tmp_pt_6); //h_pt_pull->Draw("e1same"); 
  TPad *padlb_B_pt1 = new TPad("padlb_B_pt1", "The pad 80% of the height",0.0,0.15,1.0,1.0,21);
  TPad *pad2b_B_pt1 = new TPad("pad2b_B_pt1", "The pad 20% of the height",0.0,0.0,1.0,0.15,22);
  padlb_B_pt1->SetFillColor(0); pad2b_B_pt1->SetFillColor(0);
  padlb_B_pt1->Draw();
  pad2b_B_pt1->Draw();
  pad2b_B_pt1->cd(); h_pt_pull->Draw("e1"); h_pt_pull->GetYaxis()->SetNdivisions(4); f_straighline0->Draw("same"); f_1->Draw("same"); f_2->Draw("same"); h_pt_pull->Draw("e1same"); h_pt_pull->GetYaxis()->SetRangeUser(0,2); h_pt_pull->GetYaxis()->SetLabelSize(0.2); //h_pt_pull->GetXaxis()->SetRangeUser(10, 70);
  padlb_B_pt1->cd(); tmp_pt_06->Draw("e1");  tmp_pt_6->Draw("hesame"); //tmp_pt_06->GetXaxis()->SetRangeUser(10, 70); tmp_pt_06->GetYaxis()->SetRangeUser(-.4, 10.); 

  c_01->cd(3); 
  RooPlot *frame_b_B_phi_weighted = b_B_phi.frame(Title("B phi"), Bins(10));
  data_weighted->plotOn(frame_b_B_phi_weighted, XErrorSize(0), DataError(RooAbsData::Poisson));
  frame_b_B_phi_weighted->Draw(); //frame_b_B_phi_weighted->GetYaxis()->SetRangeUser(0., 10.); 

  TH1* tmp_phi_06 = data_weighted->createHistogram("b_B_phi",10);
  TH1* tmp_phi_6  = mc->createHistogram("b_B_phi",10); tmp_phi_6->Scale(N_data_mSig.getVal() / mc->sumEntries()); tmp_phi_6->Draw("hesame"); tmp_phi_6->SetLineColor(1756); tmp_phi_6->SetMarkerSize(0);
  TH1F *h_phi_pull = new TH1F("h_phi_pull", "h_phi_pull", 10, -4, 4); h_phi_pull->SetBinErrorOption(TH1::kPoisson); h_phi_pull->Divide(tmp_phi_06, tmp_phi_6); //h_phi_pull->Draw("e1same"); 
  TPad *padlb_B_phi1 = new TPad("padlb_B_phi1", "The pad 80% of the height",0.0,0.15,1.0,1.0,21);
  TPad *pad2b_B_phi1 = new TPad("pad2b_B_phi1", "The pad 20% of the height",0.0,0.0,1.0,0.15,22);
  padlb_B_phi1->SetFillColor(0); pad2b_B_phi1->SetFillColor(0);
  padlb_B_phi1->Draw();
  pad2b_B_phi1->Draw();
  pad2b_B_phi1->cd(); h_phi_pull->Draw("e1"); h_phi_pull->GetYaxis()->SetNdivisions(4); f_straighline0->Draw("same"); f_1->Draw("same"); f_2->Draw("same"); h_phi_pull->Draw("e1same"); h_phi_pull->GetYaxis()->SetRangeUser(0,2); h_phi_pull->GetYaxis()->SetLabelSize(0.2); //h_phi_pull->GetXaxis()->SetRangeUser(10, 70);
  padlb_B_phi1->cd(); tmp_phi_06->Draw("e1");  tmp_phi_6->Draw("hesame"); //tmp_phi_06->GetXaxis()->SetRangeUser(10, 70); tmp_phi_06->GetYaxis()->SetRangeUser(-.4, 10.); 
  c_01->SaveAs("./plots/sPlotBKinematics.pdf");
  c_01->SaveAs("./plots/sPlotBKinematics.root");


  // new tcanvas
  TCanvas *c_02 = new TCanvas("c_02", "c_02", 800, 400); c_02->Divide(2,1);

  c_02->cd(1); 
  RooPlot *frame_b_tau_phimass1_weighted = b_tau_phimass1.frame(Title("B eta"), Bins(10));
  data_weighted->plotOn(frame_b_tau_phimass1_weighted, XErrorSize(0), DataError(RooAbsData::Poisson));
  frame_b_tau_phimass1_weighted->Draw(); //frame_b_tau_phimass1_weighted->GetYaxis()->SetRangeUser(0., 10.); 

  TH1* tmp_phimasswith_06 = data_weighted->createHistogram("b_tau_phimass1",10);
  TH1* tmp_phimasswith_6  = mc->createHistogram("b_tau_phimass1",10); tmp_phimasswith_6->Scale(N_data_mSig.getVal() / mc->sumEntries()); tmp_phimasswith_6->Draw("hesame"); tmp_phimasswith_6->SetLineColor(1756); tmp_phimasswith_6->SetMarkerSize(0);
  TH1F *h_phimasswith_pull = new TH1F("h_phimasswith_pull", "h_phimasswith_pull", 10, -3, 3); h_phimasswith_pull->SetBinErrorOption(TH1::kPoisson); h_phimasswith_pull->Divide(tmp_phimasswith_06, tmp_phimasswith_6); //h_phimasswith_pull->Draw("e1same"); 
  TPad *padlb_tau_phimass11 = new TPad("padlb_tau_phimass11", "The pad 80% of the height",0.0,0.15,1.0,1.0,21);
  TPad *pad2b_tau_phimass11 = new TPad("pad2b_tau_phimass11", "The pad 20% of the height",0.0,0.0,1.0,0.15,22);
  padlb_tau_phimass11->SetFillColor(0); pad2b_tau_phimass11->SetFillColor(0);
  padlb_tau_phimass11->Draw();
  pad2b_tau_phimass11->Draw();
  pad2b_tau_phimass11->cd(); h_phimasswith_pull->Draw("e1"); h_phimasswith_pull->GetYaxis()->SetNdivisions(4); f_straighline0->Draw("same"); f_1->Draw("same"); f_2->Draw("same"); h_phimasswith_pull->Draw("e1same"); h_phimasswith_pull->GetYaxis()->SetRangeUser(0,2); h_phimasswith_pull->GetYaxis()->SetLabelSize(0.2); //h_phimasswith_pull->GetXaxis()->SetRangeUser(10, 70);
  padlb_tau_phimass11->cd(); tmp_phimasswith_06->Draw("e1");  tmp_phimasswith_6->Draw("hesame"); //tmp_phimasswith_06->GetXaxis()->SetRangeUser(10, 70); tmp_phimasswith_06->GetYaxis()->SetRangeUser(-.4, 10.); 

  c_02->cd(2); int B_rhomass1_bins = 20;
  RooPlot *frame_b_tau_rhomass1_weighted = b_tau_rhomass1.frame(Title("B pt"), Bins(B_rhomass1_bins));
  data_weighted->plotOn(frame_b_tau_rhomass1_weighted, XErrorSize(0), DataError(RooAbsData::Poisson));
  frame_b_tau_rhomass1_weighted->Draw(); //frame_b_tau_rhomass1_weighted->GetYaxis()->SetRangeUser(0., 10.); 

  TH1* tmp_rhomass1_06 = data_weighted->createHistogram("b_tau_rhomass1",B_rhomass1_bins);
  TH1* tmp_rhomass1_6  = mc->createHistogram("b_tau_rhomass1",B_rhomass1_bins); tmp_rhomass1_6->Scale(N_data_mSig.getVal() / mc->sumEntries()); tmp_rhomass1_6->Draw("hesame"); tmp_rhomass1_6->SetLineColor(1756); tmp_rhomass1_6->SetMarkerSize(0);
  TH1F *h_rhomass1_pull = new TH1F("h_rhomass1_pull", "h_rhomass1_pull", B_rhomass1_bins, 0, 100); h_rhomass1_pull->SetBinErrorOption(TH1::kPoisson); h_rhomass1_pull->Divide(tmp_rhomass1_06, tmp_rhomass1_6); //h_rhomass1_pull->Draw("e1same"); 
  TPad *padlb_tau_rhomass11 = new TPad("padlb_tau_rhomass11", "The pad 80% of the height",0.0,0.15,1.0,1.0,21);
  TPad *pad2b_tau_rhomass11 = new TPad("pad2b_tau_rhomass11", "The pad 20% of the height",0.0,0.0,1.0,0.15,22);
  padlb_tau_rhomass11->SetFillColor(0); pad2b_tau_rhomass11->SetFillColor(0);
  padlb_tau_rhomass11->Draw();
  pad2b_tau_rhomass11->Draw();
  pad2b_tau_rhomass11->cd(); h_rhomass1_pull->Draw("e1"); h_rhomass1_pull->GetYaxis()->SetNdivisions(4); f_straighline0->Draw("same"); f_1->Draw("same"); f_2->Draw("same"); h_rhomass1_pull->Draw("e1same"); h_rhomass1_pull->GetYaxis()->SetRangeUser(0,2); h_rhomass1_pull->GetYaxis()->SetLabelSize(0.2); //h_rhomass1_pull->GetXaxis()->SetRangeUser(10, 70);
  padlb_tau_rhomass11->cd(); tmp_rhomass1_06->Draw("e1");  tmp_rhomass1_6->Draw("hesame"); //tmp_rhomass1_06->GetXaxis()->SetRangeUser(10, 70); tmp_rhomass1_06->GetYaxis()->SetRangeUser(-.4, 10.); 
  c_02->SaveAs("./plots/sPlotrhomasses.pdf");
  c_02->SaveAs("./plots/sPlotrhomasses.root");



}
