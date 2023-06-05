#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
from argparse import ArgumentParser
from ROOT import TCanvas, TTree, TFile, TH1D, TPad, TLegend, THStack, RDataFrame
from uncertainties import ufloat
from uncertainties.umath import *



if __name__ == "__main__":

	parser = ArgumentParser(description="SignalBackground")
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("--out", dest="out", action="store", type=str, default="BackgroundEstimateUpdate/", help="Directory where the plots shuld go")
	parser.add_argument("--file", dest="file", action="store", type=str, default="test", help="Turn on debug output")
	parser.add_argument("--sample", dest="sample", action="store", type=str, default="", help="Turn on debug output")
	parser.add_argument("--full", dest="full", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument("--tree", dest="tree", action="store", type=str, default="ntuplizer/tree", help="Turn on debug output")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v5", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument("-f", "--forcepath", dest="forcepath", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	parser.add_argument("--stats", dest="stats", action="store_true", default=False, help="Show stats box in ROOT")

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 

	if (options.stats): 
		ROOT.gStyle.SetOptStat(1111111)
	else: 
		# Don't plot stats box
		ROOT.gStyle.SetOptStat(0) 


	ROOT.gInterpreter.Declare("""
	int PrintDecayStringEvtLoop(std::string decay)
	{
		std::cout << decay << std::endl; 
		return 1; 
	}
	""")

	ROOT.gROOT.LoadMacro("FileFlow.h")
	ROOT.gROOT.LoadMacro("PrintDecayStringPerEvent.C")
	from ROOT import Ana

	if (options.sample == ""): 
		file = TFile.Open(options.file, "READ")
		tree = file.Get(options.tree)

		ROOT.PrintDecayString(tree, options.full)

		file.Close()

	#frame = RDataFrame(tree)

	else: 
		Ana.Init(options.version)

		Ana.filemanager.OpenItem(options.sample)

		#filtered = frame.Filter("pttau_tau_m>1.5").Define("numPrinted", ROOT.PrintDecayString, ("genstring"))

		ROOT.PrintDecayString(Ana.filemanager.GetItem(options.sample), options.full) #+"_ntuple"

	print("Done")
	#file.Close()

