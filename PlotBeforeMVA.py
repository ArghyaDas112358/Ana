#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
import anaConfig
ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana, TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from collections import defaultdict
from argparse import ArgumentParser
from anaPrepareRegions import PrepareRegions, PrepareRegionsSimple, PrepareSamples, UnrollHist


if __name__ == "__main__":

	parser = ArgumentParser(description="PlotBeforeMVA") 
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
	parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="TauSelectionv7", help="Which version (cycle) of files to run on")
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


	sample = {}
	for item in anaConfig.samples:
		sample[item] = item+"_DNN"


	# Global initialisations
	Ana.Init(options.version)
	

	for file in sample: 
		Ana.filemanager.OpenItem(file)



	frames = PrepareSamples(list(sample.values()))

	if options.debug: print(frames)

	# from here on starts teting

	from anaPlotting import PlotComparison

	PlotComparison(frames, sample[anaConfig.data], sample["Sig"], ["baseline"], anaConfig.variables, outputfolder, False)


	Ana.filemanager.CloseAll()


	from libUtils import GetDate
	datestring = GetDate()

	from webInterface import PublishToWeb
	PublishToWeb(outputfolder, "Variables_{}_beforeBDT_data".format(datestring)) #Variables_23_8_14_beforeBDT_Sigvsdata



