#include <vector>
#include <string>
#include <unordered_map>
#include "TCut.h"
#include "TColor.h"



class SampleData 
{
public: 

	SampleData(int initColor, std::string initLegend, TCut initCut = "", std::string initLatex = "", std::initializer_list<std::string> initRefList = {})
		: color(initColor), legend(initLegend), cut(initCut), latex(initLatex), fileRefs(initRefList.begin(), initRefList.end()) 
	{
		if (initLatex == "") 
		{
			latex = TString::Format("$%s$", TString(initLegend).ReplaceAll("#", "\\").ReplaceAll("rightarrow", "rightarrow ").Data()).Data(); // If no latex string provided, use the legend and make it proper latex
		}
	}; 

	int color = -999.; 
	std::string legend = ""; 
	TCut cut = ""; 
	std::string latex = ""; 
	std::vector<std::string> fileRefs; 

}; 



namespace Ana {
	
	std::unordered_map<std::string, SampleData> InitSamples() 
	{
		// https://colorbrewer2.org/?type=diverging&scheme=RdYlBu&n=7#type=diverging&scheme=RdYlBu&n=11
		std::vector<std::vector<int> > colors = 
		{	

			{164, 176, 37}, //{165,0,38},
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

		TCut genMatchCut = "(Dstar_match)&&(b_tau_match)";

		std::unordered_map<std::string, SampleData> samples = {
			// B0 decays
			{"Sig", {
				mycolors[0], 
				"signal", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", 
				{"Sig"}}
			}, 
			{"SigTest", {
				mycolors[0], 
				"signal", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", 
				{"Sig"}}
			}, 
			{"B0toDstarDs", SampleData(
				mycolors[1], 
				"B^{0}#rightarrowD*D_{s}", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}D_{s}^+$", 
				{"B0toDstarDs"})
			},
			{"B0toDstarDsstar", SampleData(
				mycolors[2], 
				"B^{0}#rightarrowD*D*_{s}", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}D_{s}^{*+}$", 
				{"B0toDstarDsstar"})
			},
			{"B0toDstarDs1", SampleData(
				mycolors[2], 
				"B^{0}#rightarrowD*D_{s1}", 
				genMatchCut, 
				"$B^{0}\\rightarrow D^{*}D_{s1}$", 
				{"B0toDstarDs1"})
			},
			{"B0toDstarDs0star", SampleData(
				mycolors[2], 
				"B^{0}#rightarrowD*D_{s0}*", 
				genMatchCut, 
				"$B^{0}\\rightarrow D^{*}D_{s0}^{*}$", 
				{"B0toDstarDs0star"})
			},
			{"B0toDstarD", SampleData(
				mycolors[3], 
				"B^{0}#rightarrowD*D", 
				genMatchCut, 
				"", 
				{"B0toDstarD"})
			},
			{"B0toDstarD0K", SampleData(
				mycolors[5], 
				"B^{0}#rightarrowD*D^{0}K", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*}D^{0}K$", 
				{"B0toDstarD0K"})
			},
			{"B0toDstarD0Kstar", SampleData(
				mycolors[6], 
				"B^{0}#rightarrowD*D^{0}K*", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*}D^{0}K^{*}$", 
				{"B0toDstarD0K"})
			},
			{"BkgDstara1", SampleData(
				mycolors[6], 
				"B^{0}#rightarrowD*a_{1}", 
				"1", 
				"$B^0\\rightarrow D^{*-}a_{1}^+$", 
				{"BkgDstara1"})
			},
			{"B0toDstara1", SampleData(
				mycolors[7], 
				"B^{0}#rightarrowD*a_{1}", 
				"1", 
				"$B^0\\rightarrow D^{*-}a_{1}^+$", 
				{"BkgDstara1"})
			},
			{"B0toDstar3pi", SampleData(
				mycolors[8], 
				"B^{0}#rightarrowD*3pi", 
				genMatchCut, 
				"", 
				{"B0toDstar3pi"})
			},
			{"B0toDstar3pipi0", SampleData(
				mycolors[9], 
				"B^{0}#rightarrowD*3#pi#pi^0", 
				genMatchCut, 
				"", 
				{"B0toDstar3pipi0"})
			},
			{"B0toDstar5pi", SampleData(
				mycolors[7], 
				"B^{0}#rightarrowD*35#pi", 
				genMatchCut, 
				"", 
				{"B0toDstar5pi"})
			},
			// Bu decays 
			{"ButoDstarDK", SampleData(
				mycolors[4], 
				"B^{+}#rightarrowD*DK", 
				genMatchCut, 
				"", 
				{"ButoDstarDK"})
			}, 
			{"B0toDstarDsX", SampleData(
				mycolors[4], 
				"B^{+}#rightarrowD*D_{s}X", 
				genMatchCut, 
				"", 
				{"B0toDstarDsX"})
			}, 
			{"ButoDstarDK", SampleData(
				mycolors[4], 
				"B^{+}#rightarrowD*DK", 
				genMatchCut, 
				"", 
				{"ButoDstarDK"})
			}, 
			{"ButoDstarXc", SampleData(
				mycolors[4], 
				"B^{+}#rightarrowD*X_{c}", 
				genMatchCut, 
				"", 
				{"ButoDstarXc"})
			}, 
			// To be completed
			{"BkgBuDXc", SampleData(
				mycolors[8], 
				"B^{+}#rightarrowD*D", 
				"1",
				"$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
				{"BkgBuDXc"})
			},
			{"BkgB0DD", SampleData(
				mycolors[8], 
				"B^{0}#rightarrowDD", 
				genMatchCut, 
				"$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
				{"BkgB0DD"})
			},
			{"BstoDD", SampleData(
				mycolors[8], 
				"B^{s}#rightarrowDD", 
				genMatchCut, 
				"$B_{s}^{0}\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
				{"BstoDD"})
			},
			{"data", SampleData(
				 mycolors[10], 
				"data", 
				"1", 
				"data", 
				{"dataD1", "dataD2", "dataD3", "dataD4", "dataD5"})
			},
			{"dataD1", SampleData(
				 mycolors[10], 
				"data", 
				"1", 
				"data", 
				{"dataD1"})
			},
			{"dataD2", SampleData(
				 mycolors[10], 
				"data", 
				"1", 
				"data", 
				{"dataD2"})
			},
			{"dataB2", SampleData(
				 mycolors[10], 
				"data", 
				"1", 
				"data", 
				{"dataB2"})
			},
			// data driven components
			{"WS", SampleData(
				 mycolors[10], 
				"|q_{B}| = 2  WS", 
				"1", 
				"$WS~|q_{B}|}~=~2$", 
				{"WS"})
			},
			{"WSTau", SampleData(
				 mycolors[10], 
				"|q_{#tau}| = 3  WS", 
				"1", 
				"$WS~|q_{#tau}|~=~3$", 
				{"WSTau"})
			},
			{"dataD2WS", SampleData(
				 mycolors[10], 
				"|q_{B}| = 2  WS", 
				"1", 
				"$WS~|q_{B}|}~=~2$", 
				{"dataD2WS"})
			},
			{"dataD1WS", SampleData(
				 mycolors[10], 
				"|q_{B}| = 2  WS", 
				"1", 
				"$WS~|q_{B}|}~=~2$", 
				{"dataD1WS"})
			},
			{"dataD2TauWS", SampleData(
				 mycolors[10], 
				"|q_{#tau}| = 3  WS", 
				"1", 
				"$WS~|q_{#tau}|~=~3$", 
				{"dataD2TauWS"})
			},
			// For backwards compatibility purposes # TODO: make a maping for sample names 
			{"BkgDstarDs", SampleData(
				mycolors[1], 
				"B^{0}#rightarrowD*D_{s}", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}D_{s}^+$", 
				{"BkgDstarDs"})
			},
			{"BkgDstarDsstar", SampleData(
				mycolors[2], 
				"B^{0}#rightarrowD*D*_{s}", 
				genMatchCut, 
				"$B^0\\rightarrow D^{*-}D_{s}^{*+}$", 
				{"BkgDstarDsstar"})
			},
		};

		return samples; 
	}
} // namespace Ana

