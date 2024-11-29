#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
import anaConfig
#ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana, TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from collections import defaultdict
from argparse import ArgumentParser
from anaPrepareRegions import PrepareRegions, PrepareRegionsSimple, PrepareSamples, UnrollHist, GetEventList


if __name__ == "__main__":

	parser = ArgumentParser(description="PlotBeforeMVA") 
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
	parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="ThesisPlots", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	parser.add_argument('-d', "--denom", dest="denom", action="store_true", default=False, help="Denominator analysis")
	parser.add_argument('-f', "--full", dest="allvars", action="store_true", default=False, help="Plot all variables")
	parser.add_argument("-g", "--cut", dest="cut", action="store", type=str, default="1.", help="Custom cut to be included added")
	parser.add_argument("--veto", dest="vetofile", action="store", type=str, default="", help="Name of JSON file containing ID of selected events")
	parser.add_argument("--bare", dest="bare", action="store_true", default=False, help="Denominator analysis")
	parser.add_argument("--comp", dest="comp", action="store_true", default=False, help="Plot comparisons")
	parser.add_argument("--stack", dest="stack", action="store_true", default=True, help="Plot stacked distributions")

	

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 

	outputfolder = "./plots/"+options.out+"/"
	#if (options.forcepath): 
	#	print("WARNING: You have used option '-f' or '--forcepath'. Files will be written to: {}".format(options.out))
	#	outputfolder = options.out+"/"

	os.system("mkdir -p "+outputfolder)


	postfix = "" #"_DNN_m"
	if (options.denom): 
		anaConfig.Denominator()
		postfix = "_DNN"

	if (options.bare): 
		postfix = "_ntuple"




	# Global initialisations
	Ana.Init(options.version, options.denom)

	variables = anaConfig.variables
	if options.allvars: 
		variables = [item.first for item in Ana.binning]
	print(variables)

	samples = anaConfig.samples #[anaConfig.data, anaConfig.Sig] #anaConfig.samples
	sample = {}
	for item in samples:
		sample[item] = item+postfix
	

	#for file in sample.values(): 
	#	Ana.filemanager.OpenItem(file)


	from anaPlotting import PlotComparison, PlotStack, PlotOverlay, PlotSingle

	signalfile = ROOT.TFile.Open("/Users/mhuwiler/cernbox/DoctoralThesis/Analysis/data/"+options.version+"/Sig_tauDNN.root") 
	signal = signalfile.Get("ntuplizer/tree")
	signal.Print()
	frame = RDataFrame(signal)
	print(frame.GetColumnNames())

	#variable = "Dstar_pt"
	#examplehist = Ana.binning[variable]
	#histo = frame.Histo1D(examplehist, variable)
	#canvas = TCanvas("canvas", "canvas", 800, 600)
	#histo.Draw()
	#canvas.Draw()
	#canvas.Print(outputfolder+"/plot.png")

	variables = ["v_tau_dnn1", "v_tau_dnn2", "v_tau_dnn3"]


	PlotSingle(frame, variables, outputfolder)



