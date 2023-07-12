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
	std::unordered_map<std::string, SampleData> samples = {
		// B0 decays
		{"Sig", {
			0, 
			"signal", 
			"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", 
			{"Sig"}}
		}, 
		{"B0toDstarDs", SampleData(
			1, 
			"B^{0}#rightarrowD*D_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^+$", 
			{"B0toDstarDs"})
		},
		{"B0toDstarDsstar", SampleData(
			1, 
			"B^{0}#rightarrowD*D*_{s}", 
			"$B^0\\rightarrow D^{*-}D_{s}^{*+}$", 
			{"B0toDstarDsstar"})
		},
		{"B0toDstarDs1", SampleData(
			1, 
			"B^{0}#rightarrowD*D_{s1}", 
			"$B^{0}\\rightarrow D^{*}D_{s1}$", 
			{"B0toDstarDs1"})
		},
		{"B0toDstarD", SampleData(
			1, 
			"B^{0}#rightarrowD^{*}D", 
			"", 
			{"B0toDstarD"})
		},
		{"B0toDstarD0K", SampleData(
			1, 
			"B^{0}#rightarrowD^{*}D^{0}K", 
			"$B^0\\rightarrow D^{*}D^{0}K$", 
			{"B0toDstarD0K"})
		},
		{"BkgDstara1", SampleData(
			1, 
			"B^{0}#rightarrowD*a_{1}", 
			"$B^0\\rightarrow D^{*-}a_{1}^+$", 
			{"BkgDstara1"})
		},
		{"B0toDstar3pi", SampleData(
			1, 
			"B^{0}#rightarrowD^{*}3pi", 
			"", 
			{"B0toDstar3pi"})
		},
		// Bu decays 
		{"ButoDstarDK", SampleData(
			1, 
			"B^{+}#rightarrowD^{*}DK", 
			"", 
			{"ButoDstarDK"})
		}, 
		// To be completed
		{"BkgBuDXc", SampleData(
			1, 
			"B^{+}#rightarrowD*D", 
			"$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
			{"BkgBuDXc"})
		},
		{"BkgB0DD", SampleData(
			1, 
			"B^{0}#rightarrowDD", 
			"$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", 
			{"BkgB0DD"})
		},
		// data driven components
		{"WS", SampleData(
			1, 
			"|q_{B}| = 2  WS", 
			"$WS~|q_{B}|}~=~2$", 
			{"WS"})
		},
		{"WSTau", SampleData(
			1, 
			"|q_{#tau}| = 3  WS", 
			"$WS~|q_{#tau}|~=~3$", 
			{"WSTau"})
		},
		{"dataD2WS", SampleData(
			1, 
			"|q_{B}| = 2  WS", 
			"$WS~|q_{B}|}~=~2$", 
			{"dataD2WS"})
		},
		{"dataD2TauWS", SampleData(
			1, 
			"|q_{#tau}| = 3  WS", 
			"$WS~|q_{#tau}|~=~3$", 
			{"dataD2TauWS"})
		},
	};

	return samples; 
}

