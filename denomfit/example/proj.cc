{
    using namespace RooFit;

    TFile *fin = new TFile("higgsCombineTest.MultiDimFit.mH120.root");
    RooWorkspace *w = (RooWorkspace *)fin->Get("w");
    
    TCanvas *canvas = new TCanvas("canvas","");
    RooPlot *frame = w->var("mass")->frame(Title(" "));
    w->data("data_obs")->plotOn(frame,MarkerStyle(1));
    
    w->loadSnapshot("MultiDimFit");
    RooSimultaneousOpt *model_s = (RooSimultaneousOpt*)w->pdf("model_s");
    model_s->getPdf("bin0")->plotOn(frame);
    
    frame->GetXaxis()->SetTitle("M(J/#psi K^{#pm}) [GeV]");
    frame->Draw();
    canvas->SaveAs("proj.pdf");
}
