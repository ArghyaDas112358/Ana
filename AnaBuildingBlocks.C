#ifndef AnaBuildingBlocks_hxx
#define AnaBuildingBlocks_hxx
#include <string>
#include <vector>
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"


namespace Ana 
{

	std::vector<std::string>& purgeColumns(std::vector<std::string> &&columns, const std::vector<std::string>& blacklist)
	{
   			// a lambda that checks if `s` is in the blacklist
   			auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };

   			// removing elements from std::vectors is not pretty, see https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
   			columns.erase(std::remove_if(columns.begin(), columns.end(), is_blacklisted), columns.end());

   			return columns; 
	}

	// Workaround to save RDataFrames containing string branches
	template<typename T>
	void fixStringVariables(T &dataframe)
	{
		#include "stringbranches.gcf"

		for (auto branch : stringbranches) // Hack to fix string branche 
		{
			dataframe = dataframe.Redefine(branch, [](const ROOT::RVec<std::string> &v) {return std::vector<std::string>(v.begin(), v.end());}, {branch}); 
		}
	}

	void normaliseBinContent(TH1* hist) 
	{
	  	for (unsigned int i=0; i<hist->GetNbinsX(); i++) 
	  	{
	    	hist->SetBinContent(i, hist->GetBinContent(i)/hist->GetBinWidth(i)); 
	  	}
	}



	// Kept for legacy purposes
	namespace old 
	{

		std::vector<std::string>& purgeColumns(std::vector<std::string> &&columns)
		{
			const std::vector<std::string> blacklist = {"v_taucandidates", "b_tau"};
				// a lambda that checks if `s` is in the blacklist
				auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };

				// removing elements from std::vectors is not pretty, see https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
				columns.erase(std::remove_if(columns.begin(), columns.end(), is_blacklisted), columns.end());

				return columns; 
		}

	}

}

#endif


