#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import copy
import json
from argparse import ArgumentParser
from ROOT import RDataFrame
from uncertainties import ufloat
from uncertainties.umath import * 
from collections import OrderedDict, defaultdict


#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/libFunctions.C+")#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/libFunctions.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/ExperimentSpecificLayer.C")
#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/FileManager/CFileManager.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/CMS/tdrstyle.C")
#ROOT.gROOT.LoadMacro("FileFlow.h")
#ROOT.gROOT.LoadMacro("Tau.h")
#ROOT.setTDRStyle()
#import CMS_lumi
import anaConfig
from ROOT import Ana


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
) # Move this somewehre else

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


def SaveRegions(frames, path, objectinfile="tree"): 
	print("Saving files under: {}".format(path))
	os.system("mkdir -p {}".format(path))
	snapshotOptions = ROOT.RDF.RSnapshotOptions()

	info = defaultdict(dict)
	for item, content in frames.items(): 
		for key, value in content.items(): 

			filename = "{}/{}_{}.root".format(path, item, key)
			frame = frames[item][key]
			ROOT.fixStringVariables(frame)
			frame.Snapshot(objectinfile, filename, ROOT.purgeColumns(frame.GetColumnNames()), snapshotOptions)

			info[item][key] = (filename, objectinfile)

			print("Saved {}".format(filename))

	with open(path+"/Info.json", "w") as file: 
		json.dump(info, file, ensure_ascii=False, sort_keys=False) #encoding="utf8", 


def LoadRegions(path, openfile=True): 
	frames = defaultdict(dict)
	with open(path+"/Info.json", "r") as file: 
		info = json.load(file) #, encoding="utf8"
		for item, content in info.items(): 
			for key, value in content.items(): 
				inf = info[item][key]
				assert(len(inf)==2)

				reference = "{}_{}".format(item, key)
				filename = inf[0]
				objectinfile = inf[1]
				Ana.filemanager.AddItem(reference, filename, objectinfile)
				if (openfile): 
					Ana.filemanager.OpenItem(reference)

				frames[item][key] = RDataFrame(Ana.filemanager.GetItem(reference))
	return frames



if __name__ == "__main__":

	parser = ArgumentParser(description="SaveRegions")
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v1", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	
	options = parser.parse_args()


	anaConfig.OpenFiles(options.version)

	if (options.debug): anaConfig.DebugMode()


	from anaPrepareRegions import PrepareRegions, PurgeRegions

	frames, _ = PrepareRegions()

	frames = PurgeRegions(frames)

	if options.debug: print(frames)


	# Add info on objects and folder composition into a json at saving, to complete filemanager 
	# (sample, region name: file, tree, unrolled hist, ...)


	folder = os.path.dirname(Ana.filemanager.GetFile("Sig"))+"/testRegion"
	SaveRegions(frames, folder)

	loadedframes = LoadRegions(folder)

	print(loadedframes)

	tree = Ana.filemanager.GetItem("Sig_SR")
	canvas = ROOT.TCanvas("canvas", "canvas", 800, 600)
	tree.Draw("b_B_m")
	canvas.Print(folder+"/testhisto.pdf")
	

	anaConfig.CloseFiles()



