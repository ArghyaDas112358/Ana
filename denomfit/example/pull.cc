{
    TFile *fin = new TFile("higgsCombineTest.MultiDimFit.mH120.123456.root");
    TTree *limit = (TTree*)fin->Get("limit");
    float r;
    limit->SetBranchAddress("r", &r);
    TH1D *h_pull = new TH1D("h_pull","",50,-5.,5.);
    
    for(int i=0;i<limit->GetEntries();i+=3) {
        limit->GetEntry(i);
        double r_fit = r;
        limit->GetEntry(i+1);
        double sigma_lo = fabs(r - r_fit);
        limit->GetEntry(i+2);
        double sigma_hi = fabs(r - r_fit);
        
        if (r_fit>1. && sigma_lo>0.)
            h_pull->Fill((r_fit - 1.)/sigma_lo);
        if (r_fit<1. && sigma_hi>0.)
            h_pull->Fill((r_fit - 1.)/sigma_hi);
    }
    TCanvas *canvas = new TCanvas("canvas","");
    gStyle->SetOptFit();
    h_pull->Fit("gaus","L");
    h_pull->GetXaxis()->SetTitle("Pull: (r_{fit}-r_{true})/#sigma_{fit}");
    canvas->SaveAs("pull.pdf");
}
