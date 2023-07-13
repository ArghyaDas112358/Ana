from __future__ import division, print_function
import ROOT



def extractVariables(variables): 
	vec = ROOT.vector("TString")()
	for item in variables : 
		vec.push_back(ROOT.TString(item[0]))
		print(item[0])
	return vec


if __name__ == "__main__": 

	from anaMVA.BDTvariables import features_save

	extractVariables(features_save)


