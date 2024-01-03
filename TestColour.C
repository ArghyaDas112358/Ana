#include "TH1.h"
#include "TCanvas.h"
#include "TColor.h"



void TestColour() {
	
		// https://colorbrewer2.org/?type=diverging&scheme=RdYlBu&n=7#type=diverging&scheme=RdYlBu&n=11
		std::cout << TColor::GetFreeColorIndex() << std::endl; 
		std::vector<std::vector<int> > colors = 
		{	

			{165,0,38},
			{215,48,39}, 
			{244,109,67}, 
			{253,174,97}, 
			{254,224,144}, 
			{255,255,191}, 
			{224,243,248}, 
			{171,217,233}, 
			{116,173,209}, 
			{69,117,180}, 
			{49,54,149}
		}; 
		std::cout << TColor::GetFreeColorIndex() << std::endl; 

		std::vector<Int_t> mycolors; 
		std::vector<TColor*> rootcolors; 
		mycolors.reserve(colors.size()); 
		rootcolors.reserve(colors.size()); 
	
		Float_t colorintmax = 256.; 
		for (auto color : colors) 
		{
			//std::cout << color->GetNumber() << std::endl; 
			assert(color.size() == 3); 
			TColor *newcolor = new TColor(TColor::GetFreeColorIndex(), static_cast<Float_t>(color[0])/colorintmax, static_cast<Float_t>(color[1])/colorintmax, static_cast<Float_t>(color[2])/colorintmax);
			rootcolors.push_back(newcolor); 
			mycolors.push_back(newcolor->GetNumber()); 
		}

		TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 

		// Generating random histograms
		std::vector<TH1*> histos; 
		gRandom->SetSeed(12345);
		for (unsigned int i=0; i<colors.size(); i++) 
		{
			TH1* hist = new TH1D(TString::Format("histo%i", i), TString::Format("histo%i", i), 100, -10., 150.); 
			
			gRandom->SetSeed(i); 
			Double_t num = 0; 
			for (unsigned int j=0; j<10000; j++) 
			{
				num = gRandom->Gaus(10.*static_cast<Double_t>(i), 5.); 
				hist->Fill(num); 
			}
			hist->SetLineStyle(1);
			hist->SetLineWidth(2);
			hist->SetFillStyle(1001);
			Float_t r, g, b; 
			rootcolors[i]->GetRGB(r, g, b);
			std::cout << r << " " << g << " " << b << std::endl; 
			hist->SetLineColor(static_cast<Color_t>(mycolors[i])); 
			hist->SetFillColor(mycolors[i]); 
			histos.push_back(hist); 
			if (i == 0) hist->Draw("HIST"); 
			else hist->Draw("HIST SAME"); 
			histos.push_back(hist);
		}

		// Printing histograms
		canvas->Print("TestColor.pdf"); 
		canvas->Print("TestColor.png"); 

} 
