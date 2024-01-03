/*
 This is a "BPH-friendly" ML fit with combine example. It reads the signal MC and data for the
 B+ -> J/psi K+ events from BPH-15-004 and construct a triple-Gaussian signal model, an exponetial
 function for combinatorial events, and an error function for partial reconstruction J/psi+X events.
 
 The required root files are available at https://cernbox.cern.ch/s/zmNnt1Jy2oJwpch
*/
{
    using namespace RooFit;

    RooRealVar mass("mass","",5.0,5.8); // mass observable
    
    TFile *f_mc = new TFile("nt_mc.root"); // load signal MC ntuple
    TNtupleD *nt_mc = (TNtupleD*)f_mc->Get("nt");
    RooDataSet *rds_mc = new RooDataSet("rds_mc","",nt_mc,RooArgSet(mass));
    
    // Build a triple-Gaussian for signal
    RooRealVar mean("mean","",5.279,5.2,5.4);
    RooRealVar sigma1("sigma1","",0.015,0.001,0.050);
    RooRealVar sigma2("sigma2","",0.030,0.002,0.100);
    RooRealVar sigma3("sigma3","",0.200,0.010,0.400);
    RooRealVar frac1("frac1","",0.5,0.,1.);
    RooRealVar frac2("frac2","",0.5,0.,1.);
    
    RooRealVar dm("dm","",0.,-0.05,+0.05); // data mean w.r.t. MC
    RooRealVar ds("ds","",0.,-0.20,+0.20); // data width w.r.t. MC
    RooFormulaVar mean_c("mean_c","","(@0+@1)",RooArgList(mean,dm));
    RooFormulaVar sigma1_c("sigma1_c","","(@0*(1.+@1))",RooArgList(sigma1,ds));
    RooFormulaVar sigma2_c("sigma2_c","","(@0*(1.+@1))",RooArgList(sigma2,ds));
    RooFormulaVar sigma3_c("sigma3_c","","(@0*(1.+@1))",RooArgList(sigma3,ds));
    dm.setConstant(true);
    ds.setConstant(true);
            
    RooGaussian gauss1("gauss1","",mass,mean_c,sigma1_c);
    RooGaussian gauss2("gauss2","",mass,mean_c,sigma2_c);
    RooGaussian gauss3("gauss3","",mass,mean_c,sigma3_c);
    RooAddPdf model_sig("model_sig","",RooArgList(gauss1,gauss2,gauss3),RooArgList(frac1,frac2),true);
    model_sig.fitTo(*rds_mc);
    
    // Expected signal normalization
    RooRealVar lumi("lumi","",48.1); // integrated lumi = 48.1 [+-2.3%]
    RooRealVar xsec("xsec","",9.9); // FONLL xsec = 9.9 +3.3/-2.2 mub
    RooRealVar bf("bf","",6.116E-5); // BF(B+ -> J/psi K+ -> mu+mu-K+) = 6.116E-5 [+-3.1%]
    RooRealVar accxeff("accxeff","",0.039); // acceptance x efficiency = 0.039 [+-12.8%]
    RooFormulaVar model_sig_norm("model_sig_norm","","(@0*@1*@2*@3*2.0*1E6)",RooArgList(lumi,xsec,bf,accxeff));
    
    // Signal MC fit projection
    TCanvas *canvas_mc = new TCanvas("canvas_mc","");
    RooPlot *frame_mc = mass.frame(Title(" "));
    rds_mc->plotOn(frame_mc,MarkerStyle(1));
    model_sig.plotOn(frame_mc);
    frame_mc->GetXaxis()->SetTitle("M(J/#psi K^{#pm}) [GeV]");
    frame_mc->Draw();
    canvas_mc->SaveAs("proj_mc.pdf");
    
    TFile *f_data = new TFile("nt_data.root"); // load data ntuple
    TNtupleD *nt_data = (TNtupleD *)f_data->Get("nt");
    RooDataSet *rds_data = new RooDataSet("rds_data","",nt_data,RooArgSet(mass));
    
    // Exponential PDF for combinatorial background
    RooRealVar alpha("alpha","",-1.5,-5.0,1.0);
    RooExponential model_comb("model_comb","",mass,alpha);
    
    // Erf(x) PDF for partial reconstruction J/psi+X background
    RooRealVar shift_jpsix("shift_jpsix","",5.14,5.12,5.16);
    RooRealVar scale_jpsix("scale_jpsix","",50.,30.,1000.);
    RooGenericPdf model_jpsix("model_jpsix","","TMath::Erfc((@0-@1)*@2)",RooArgList(mass,shift_jpsix,scale_jpsix));
    
    // Normalization & joint background model
    RooRealVar model_comb_norm("model_comb_norm","",1000.,0.,rds_data->numEntries());
    RooRealVar model_jpsix_norm("model_jpsix_norm","",1000.,0.,rds_data->numEntries());
    RooAddPdf model_bkg("model_bkg","",RooArgList(model_comb,model_jpsix),RooArgList(model_comb_norm,model_jpsix_norm));
    
    mass.setRange("sideband_l",5.0,5.18);
    mass.setRange("sideband_r",5.4,5.80);
    mass.setRange("full",5.0,5.8);
    model_bkg.fitTo(*rds_data,Range("sideband_l,sideband_r"));
    
    // Data sideband fit projection
    TCanvas *canvas_data = new TCanvas("canvas_data","");
    RooPlot *frame_data = mass.frame(Title(" "));
    rds_data->plotOn(frame_data,MarkerStyle(1));
    model_bkg.plotOn(frame_data);
    frame_data->GetXaxis()->SetTitle("M(J/#psi K^{#pm}) [GeV]");
    frame_data->Draw();
    canvas_data->SetLogy();
    canvas_data->SaveAs("proj_sideband.pdf");
        
    // Dump PDF & norm & data to workspace
    RooWorkspace *wspace = new RooWorkspace("wspace");
    
    // fix MC signal model parameters, float the mean shift & width scale
    mean.setConstant(true);
    sigma1.setConstant(true);
    sigma2.setConstant(true);
    sigma3.setConstant(true);
    frac1.setConstant(true);
    frac2.setConstant(true);
    dm.setConstant(false);
    ds.setConstant(false);
    
    // import everything needed
    wspace->import(model_sig);
    wspace->import(model_sig_norm);
    wspace->import(model_comb);
    wspace->import(model_comb_norm);
    wspace->import(model_jpsix);
    wspace->import(model_jpsix_norm);
    wspace->import(*rds_data);
    
    wspace->writeToFile("wspace.root");
}
