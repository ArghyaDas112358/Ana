#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
from argparse import ArgumentParser
ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana, TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from collections import defaultdict
import anaConfig


#from anaConfig import variables
#ROOT.gInterpreter.AddIncludePath(".")
#ROOT.gROOT.LoadMacro("FileFlow_h.so")
#from ROOT import Ana


def UnrollHist(histo2D, inverted=True): 
	nx = histo2D.GetNbinsX()
	ny = histo2D.GetNbinsY()
	if inverted: 
		ny = histo2D.GetNbinsX()
		nx = histo2D.GetNbinsY()

	nTotal = nx*ny

	name = "unrolled"+histo2D.GetName()
	unrolled = ROOT.TH1D(name, name, nTotal, 0, 100)

	print("Nunmber of bins: {}, {}".format(nx, ny))

	for i in range(0, nx):
		for j in range(0, ny): # TODO: check overflow is handled properly 
			if inverted: 
				binContent = histo2D.GetBinContent(j, i)
			else: 
				binContent = histo2D.GetBinContent(i, j)
			unrolled.SetBinContent(i+j*nx, binContent)

	return unrolled


def PrepareRegions(sampleslist = anaConfig.samples, nBins = 6, rangeMin = 0.2, rangeMax = 1.5, debug=False): 
	samples = {}
	frames = collections.defaultdict(dict)
	histos = collections.defaultdict(dict)
	histosunrolled = collections.defaultdict(dict)

	for item in sampleslist:  
		samples[item] = ROOT.RDataFrame(Ana.filemanager.GetItem(item, True))
		frames[item]["baseline"] =  samples[item].Filter((Ana.cut["base"]+Ana.samples.at(item).cut).GetTitle()) #Ana.cut["base"].GetTitle() "1."
		frames[item]["all"] = samples[item].Filter("1.")
		#baseline[item]
		for region in anaConfig.regions: 
			cut = (Ana.cut[region]+Ana.samples.at(item).cut).GetTitle()
			#if "WS" in item: 
			#	cut = Ana.cutstandalone[region].GetTitle()
			if (debug): print("Using following cut string (from TCut): {}".format(cut))
			frames[item][region] = samples[item].Filter(cut)
			ROOT.SetOwnership(frames[item][region], 0)
			# For histogram legacy compatibility
			histos[item][region] = frames[item][region].Histo2D(("rhomass1", "rhomass2", nBins, rangeMin, rangeMax, nBins, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2")
			histosunrolled[item][region] = UnrollHist(histos[item][region])

	return frames, histosunrolled


def PrepareRegionsSimple(): 
	samples = {}
	frames = collections.defaultdict(dict)

	for item in anaConfig.samples:  
		Ana.filemanager.OpenItem(item)
		samples[item] = ROOT.RDataFrame(Ana.filemanager.GetItem(item))
		frames[item]["baseline"] =  samples[item].Filter((Ana.cut["base"]+Ana.samples.at(item).cut).GetTitle()) #Ana.cut["base"].GetTitle() "1."
		frames[item]["all"] = samples[item].Filter("1.")
		frames[item]["mw"] =  samples[item].Filter((Ana.cut["mw"]+Ana.samples.at(item).cut).GetTitle())
		#baseline[item]
		for region in anaConfig.regions: 
			cut = (Ana.cut[region]+Ana.samples.at(item).cut).GetTitle()
			#if "WS" in item: 
			#	cut = Ana.cutstandalone[region].GetTitle()
			#if (options.debug): print("Using following cut string (from TCut): {}".format(cut))
			frames[item][region] = samples[item].Filter(cut)
			ROOT.SetOwnership(frames[item][region], 0)
			# For histogram legacy compatibility

	return frames


def PrepareSamples(samplelist = anaConfig.samples): 
	samples = {}
	frames = collections.defaultdict(dict)

	for item in samplelist:  
		#Ana.filemanager.OpenItem(item)
		sample = Ana.GetSample(item)
		print(sample.GetEntries())
		samples[item] = ROOT.RDataFrame(sample) #ROOT.RDataFrame(Ana.filemanager.GetItem(item))
		frames[item]["baseline"] =  samples[item].Filter((Ana.cut["base"]+Ana.samples.at(GetBaseName(item)).cut).GetTitle()) #Ana.cut["base"].GetTitle() "1."
		frames[item]["all"] = samples[item].Filter("1.")
		frames[item]["mw"] =  samples[item].Filter((Ana.cut["mw"]+Ana.samples.at(item).cut).GetTitle())

	from libEfficiencies import ComputeEfficiencies
	effs = ComputeEfficiencies(frames)

	return frames, effs


def GetBaseName(samplename): 
	items = samplename.split("_")
	sample = items[0]
	return sample


def SaveDataframe(tosave, filename, objectname = "tree", blacklist = anaConfig.blacklist):
	Ana.fixStringVariables(tosave)
	tosave.Snapshot(objectname, filename, Ana.purgeColumns(tosave.GetColumnNames(), anaConfig.blacklist))
	print("Written frame to {} in {}".format(objectname, filename))


# ROOT.gInterpreter.Declare('''
# 	std::vector<std::string>& purgeColumns(std::vector<std::string> &&columns)
# 	{
# 		const std::vector<std::string> blacklist = {"v_taucandidates", "b_tau"};
# 			// a lambda that checks if `s` is in the blacklist
# 			auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };

# 			// removing elements from std::vectors is not pretty, see https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
# 			columns.erase(std::remove_if(columns.begin(), columns.end(), is_blacklisted), columns.end());

# 			return columns; 
# 	}
# 	'''
# ) 


def SaveRegions(frames, path, update = False, objectinfile="tree"): 
	print("Saving files under: {}".format(path))
	os.system("mkdir -p {}".format(path))
	snapshotOptions = ROOT.RDF.RSnapshotOptions()

	info = defaultdict(dict)
	if (update): 
		with open(path+"/Info.json", "r") as file: 
			info = copy.deepcopy(json.load(file))
	for item, content in frames.items(): 
		for key, value in content.items(): 

			filename = "{}/{}_{}.root".format(path, item, key)
			frame = frames[item][key]
			ROOT.fixStringVariables(frame)
			frame.Snapshot(objectinfile, filename, Ana.old.purgeColumns(frame.GetColumnNames()), snapshotOptions)

			info[item][key] = (filename, objectinfile)

			print("Saved {}".format(filename))

	with open(path+"/Info.json", "w") as file: 
		json.dump(info, file, ensure_ascii=False, sort_keys=False) #encoding="utf8", 


def LoadRegions(path, openfile=True): 
	frames = defaultdict(dict)
	addeditems = []
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
				addeditems.append(reference)
				
	return frames, addeditems


def PurgeRegions(frames, excluded = ["all", "baseline"]): 
	for item, content in frames.items(): 
		for key, value in content.items(): 
			if (key in excluded): 
				frames[item].pop(key)
	return frames


if __name__ == "__main__":

	parser = ArgumentParser(description="SignalBackground") 
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v1", help="Which version (cycle) of files to run on")
	parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="TestPrepareRegions", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 

	outputfolder = "./plots/"+options.out+"/"
	#if (options.forcepath): 
	#	print("WARNING: You have used option '-f' or '--forcepath'. Files will be written to: {}".format(options.out))
	#	outputfolder = options.out+"/"

	os.system("mkdir -p "+outputfolder)


	# Global initialisations
	Ana.Init(options.version)
	

	for file in anaConfig.samples: 
		Ana.filemanager.OpenItem(file)



	frames, histosunrolled = PrepareRegions(6, 0.2, 1.5)

	if options.debug: print(frames)

	# from here on starts teting

	from anaPlotting import PlotComparison

	PlotComparison(frames, anaConfig.data, anaConfig.dataWS, anaConfig.regions, anaConfig.variables, outputfolder, False)


	Ana.filemanager.CloseAll()



