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


if __name__ == "__main__":

	parser = ArgumentParser(description="SaveRegions")
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v1", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	
	options = parser.parse_args()


	anaConfig.OpenFiles(options.version)

	if (options.debug): anaConfig.DebugMode()


	from anaPrepareRegions import PrepareRegions, PurgeRegions, SaveRegions, LoadRegions

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



