#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
from argparse import ArgumentParser
from ROOT import TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from uncertainties import ufloat
from uncertainties.umath import * 


#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/libFunctions.C+")#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/libFunctions.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/ExperimentSpecificLayer.C")
#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/FileManager/CFileManager.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/CMS/tdrstyle.C")
ROOT.gROOT.LoadMacro("FileFlow.h")
ROOT.gROOT.LoadMacro("Tau.h")
#ROOT.setTDRStyle()
#import CMS_lumi
from ROOT import Ana



webpublication =False



if __name__ == "__main__":

	parser = ArgumentParser(description="SignalBackground")
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v1", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	
	options = parser.parse_args()


	# Global initialisations
	Ana.Init(options.version)


	data = "dataD2" #"dataB2"
	filesUsed = ["Sig", data, "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "B0toDstar3pi", "dataD2WS"] #"SigPart", "dataD2WS", "dataD2TauWS", , "B0toDstar5pi" ["Sig", "dataD1", "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "BkgDstara1", "B0toDstar3pi", "dataD2WS"]

	regions = ["SR", "CR", "SB"]

	nBins = 6
	rangeMin = 0.2 #0.37
	rangeMax = 1.5 #1.43
	

	for file in filesUsed: 
		Ana.filemanager.OpenItem(file)


	# Starting script 
	samples = {}
	frames = collections.defaultdict(dict)
	baseline = collections.defaultdict(dict)

	for item in filesUsed:  
		samples[item] = ROOT.RDataFrame(Ana.filemanager.GetItem(item))
		frames[item]["baseline"] =  samples[item].Filter((Ana.cut["base"]+Ana.samples.at(item).cut).GetTitle()) #Ana.cut["base"].GetTitle() "1."
		frames[item]["all"] = samples[item].Filter("1.")
		#baseline[item]
		for region in regions: 
			cut = (Ana.cut[region]+Ana.samples.at(item).cut).GetTitle()
			#if "WS" in item: 
			#	cut = Ana.cutstandalone[region].GetTitle()
			if (options.debug): print("Using following cut string (from TCut): {}".format(cut))
			frames[item][region] = samples[item].Filter(cut)
			ROOT.SetOwnership(frames[item][region], 0)
			# For histogram legacy compatibility

	if options.debug: print(frames)


	tree = ROOT.TString("tree")

	ROOT.gInterpreter.Declare('''
		template<typename T>
		void fixStringVariables(T &dataframe)
		{
			#include "stringbranches.gcf"

			for (auto branch : stringbranches) // Hack to fix string branche 
			{
				dataframe = dataframe.Redefine(branch, [](const ROOT::RVec<std::string> &v) {return std::vector<std::string>(v.begin(), v.end());}, {branch}); 
			}
		}
		'''
	) 

	ROOT.gInterpreter.Declare('''
		std::vector<std::string>& purgeColumns(std::vector<std::string> &&columns)
		{
			const std::vector<std::string> blacklist = {"v_taucandidates", "b_tau"};
   			// a lambda that checks if `s` is in the blacklist
   			auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };

   			// removing elements from std::vectors is not pretty, see https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
   			columns.erase(std::remove_if(columns.begin(), columns.end(), is_blacklisted), columns.end());

   			return columns; 
		}
		'''
	) 


	snapshotOptions = ROOT.RDF.RSnapshotOptions()

	tosave = frames[data]["SB"]

	ROOT.fixStringVariables(tosave)

	tosave.Snapshot("tree", "file.root", ROOT.purgeColumns(tosave.GetColumnNames()), snapshotOptions)
	

	Ana.filemanager.CloseAll()



