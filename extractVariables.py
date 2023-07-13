from __future__ import division, print_function
import ROOT



def extractVariables(variables): 
	vec = ROOT.vector("TString")()
	for item in variables : 
		vec.push_back(ROOT.TString(item[0]+"/"+item[1]))
		print(item[0])
	return vec

def getVariables(): 
	from anaMVA.BDTvariables import features_save
	extractVariables(features_save)


if __name__ == "__main__": 

	from anaMVA.BDTvariables import features_save

	extractVariables(features_save)

	getVariables()


