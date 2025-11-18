#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
import anaConfig
from ROOT import Ana, TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from argparse import ArgumentParser


filedict = {"sigggF": "/eos/home-m/mhuwiler/data/HHtobbtautau/NanoAODv15/signalggF.root", }
Ana.filemanager.AddItem("sigggF", "/eos/home-m/mhuwiler/data/HHtobbtautau/NanoAODv15/signalggF.root", "Events")


def loadFile(desc): 
	frame = ROOT.RDataFrame(Ana.filemanager.GetItem(desc, True))
	ROOT.SetOwnership(frame, 0)
	return frame


def loadFileFull(desc, writemode = False, treename = "Events"): 
	path = filedict[desc]
	flag = "WRITE" if writemode else "READ" 
	file = ROOT.TFile.Open(path, flag)
	tree = copy.deepcopy(file.Get(treename))
	tree.Print()
	frame = ROOT.RDataFrame(tree)
	print(frame)
	ROOT.SetOwnership(frame, 0)
	return frame


def dropBranchNames(frame, filename, exclusionlist = []):
	with open(filename, "w") as file: 
		for name in frame.GetColumnNames(): 
			name = str(name)
			#print("{} {}".format(name, [(excluded in name) for excluded in exclusionlist]))
			if not (any([excluded in name for excluded in exclusionlist])): 
				file.write("{}\n".format(name))



if __name__ == "__main__":

	parser = ArgumentParser(description="PlotBeforeMVA") 
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
	parser.add_argument("--prefix", dest="xrdpfx", action="store", type=str, default="root://cms-xrd-global.cern.ch//", help="XRootD prefix to be used to access files")
	parser.add_argument("-o", "--out", dest="outputpath", action="store", type=str, default="./temp", help="Local output path")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument("--test", dest="test", action="store_true", default=False, help="Process a reduced number of files for testing purposes")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")

	

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 


	Ana.Init()


	ROOT.gROOT.LoadMacro("HHbbtautauAnaElements.C+")
	

	sig = loadFile("sigggF")

	print(sig)

	dropBranchNames(sig, "branchnames.txt", ["L1", "HLT", "DST"])

	sig = Ana.GetP4(sig, "Muon")

	if (options.test): 
		sig.Range(0, 100).Snapshot("Events", "./Test.root")
	else: 
		sig.Snapshot("Events", "./Test.root")

	print("Hello")


	
	




	

