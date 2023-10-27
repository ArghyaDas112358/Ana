#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
from argparse import ArgumentParser
ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana, TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
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


def PrepareRegions(nBins = 6, rangeMin = 0.2, rangeMax = 1.5, debug=False): 
	samples = {}
	frames = collections.defaultdict(dict)
	histos = collections.defaultdict(dict)
	histosunrolled = collections.defaultdict(dict)

	for item in anaConfig.samples:  
		samples[item] = ROOT.RDataFrame(Ana.filemanager.GetItem(item))
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



