#include <vector>
#include <string>
#include <unordered_map>



class SampleData 
{
public: 

	SampleData(int initColor, std::string initLegend, std::string initLatex, std::initializer_list<std::string> initRefList = {})
		: color(initColor), legend(initLegend), latex(initLatex), fileRefs(initRefList.begin(), initRefList.end()) {}; 

	int color = -999.; 
	std::string legend = ""; 
	std::string latex = ""; 
	std::vector<std::string> fileRefs; 

}; 



std::unordered_map<std::string, SampleData> InitSamples() 
{
	std::vector<TColor*> colors = 
	{	
		new TColor(TColor::GetFreeColorIndex(), 165,0,38), 
		new TColor(TColor::GetFreeColorIndex(), 215,48,39), 
		new TColor(TColor::GetFreeColorIndex(), 244,109,67), 
		new TColor(TColor::GetFreeColorIndex(), 253,174,97), 
		new TColor(TColor::GetFreeColorIndex(), 254,224,144), 
		new TColor(TColor::GetFreeColorIndex(), 255,255,191), 
		new TColor(TColor::GetFreeColorIndex(), 224,243,248), 
		new TColor(TColor::GetFreeColorIndex(), 171,217,233), 
		new TColor(TColor::GetFreeColorIndex(), 116,173,209), 
		new TColor(TColor::GetFreeColorIndex(), 69,117,180), 
		new TColor(TColor::GetFreeColorIndex(), 49,54,149)
	}; 

	std::vector<Int_t> mycolors; 
	mycolors.reserve(colors.size()); 
	for (auto color : colors) 
	{
		mycolors.push_back(color->GetNumber()); 
	}

	std::unordered_map<std::string, SampleData> samples = {
		// B0 decays
		{"Sig", {
			mycolors[0], 
			"signal", 
			"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", 
			{"Sig"}}
		}, 
		{"B0toDstarDs", SampleData(
			mycolors[1], 
			"B^{0}#rightarrowD*D_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^+$", 
			{"B0toDstarDs"})
		},
		{"B0toDstarDsstar", SampleData(
			mycolors[2], 
			"B^{0}#rightarrowD*D*_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^{*+}$", 
			{"B0toDstarDsstar"})
		},
		{"B0toDstarDs1", SampleData(
			mycolors[2], 
			"B^{0}#rightarrowD*D_{s1}", 
			"$B^{0}\\rightarrow D^{*}D_{s1}$", 
			{"B0toDstarDs1"})
		},
		{"B0toDstarD", SampleData(
			mycolors[3], 
			"B^{0}#rightarrowD*D", 
			"", 
			{"B0toDstarD"})
		},
		{"B0toDstarD0K", SampleData(
			mycolors[5], 
			"B^{0}#rightarrowD*D^{0}K", 
			"$B^0\\rightarrow D^{*}D^{0}K$", 
			{"B0toDstarD0K"})
		},
		{"BkgDstara1", SampleData(
			mycolors[6], 
			"B^{0}#rightarrowD*a_{1}", 
			"$B^0\\rightarrow D^{*-}a_{1}^+$", 
			{"BkgDstara1"})
		},
		{"B0toDstar3pi", SampleData(
			mycolors[7], 
			"B^{0}#rightarrowD*3pi", 
			"", 
			{"B0toDstar3pi"})
		},
		// Bu decays 
		{"ButoDstarDK", SampleData(
			mycolors[4], 
			"B^{+}#rightarrowD*DK", 
			"", 
			{"ButoDstarDK"})
		}, 
		// To be completed
		{"BkgBuDXc", SampleData(
			mycolors[8], 
			"B^{+}#rightarrowD*D", 
			"$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
			{"BkgBuDXc"})
		},
		{"BkgB0DD", SampleData(
			mycolors[8], 
			"B^{0}#rightarrowDD", 
			"$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
			{"BkgB0DD"})
		},
		// data driven components
		{"WS", SampleData(
			 mycolors[10], 
			"|q_{B}| = 2  WS", 
			"$WS~|q_{B}|}~=~2$", 
			{"WS"})
		},
		{"WSTau", SampleData(
			 mycolors[10], 
			"|q_{#tau}| = 3  WS", 
			"$WS~|q_{#tau}|~=~3$", 
			{"WSTau"})
		},
		{"dataD2WS", SampleData(
			 mycolors[10], 
			"|q_{B}| = 2  WS", 
			"$WS~|q_{B}|}~=~2$", 
			{"dataD2WS"})
		},
		{"dataD2TauWS", SampleData(
			 mycolors[10], 
			"|q_{#tau}| = 3  WS", 
			"$WS~|q_{#tau}|~=~3$", 
			{"dataD2TauWS"})
		},
		// For backwards compatibility purposes # TODO: make a maping for sample names 
		{"BkgDstarDs", SampleData(
			mycolors[1], 
			"B^{0}#rightarrowD*D_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^+$", 
			{"BkgDstarDs"})
		},
		{"BkgDstarDsstar", SampleData(
			mycolors[2], 
			"B^{0}#rightarrowD*D*_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^{*+}$", 
			{"BkgDstarDsstar"})
		},
	};

	return samples; 
}

