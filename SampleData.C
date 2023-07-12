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
		{"Sig", {
			0, 
			"signal", 
			"B", 
			{"Sig"}}
		}, 
		{"B0toDstarDs", SampleData(
			1, 
			"B^{0}#rightarrowD*D_{s}", 
			"B", 
			{"B0toDstarDs"})
		}
	};

	return samples; 
}

