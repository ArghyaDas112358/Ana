#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
ROOT.gROOT.LoadMacro("Particle.h+")
ROOT.gROOT.LoadMacro("HHbbtautauAnaElements.C+")
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
Ana.filemanager.AddItem("official", "/eos/home-m/mhuwiler/data/HHtobbtautau/NanoAODv12/Run3Summer22NanoAODv12_1-1.root", "Events")


def loadFile(desc): 
	frame = ROOT.RDataFrame(Ana.filemanager.GetItem(desc, True))
	ROOT.SetOwnership(frame, 0)
	return generalise(frame)


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


def loadFileBase(path, writemode = False, treename = "Events"): 
	flag = "WRITE" if writemode else "READ" 
	file = ROOT.TFile.Open(path, flag)
	tree = copy.deepcopy(file.Get(treename))
	tree.Print()
	frame = generalise(ROOT.RDataFrame(tree))
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


def generalise(df): 
	return ROOT.ROOT.RDF.AsRNode(df)


def PlotSimple(frame, variable, histomodel): 
	name = "{}".format(variable)
	outfolder = "."
	drawlegend = False
	canvas = TCanvas(name, variable, 800, 600)

	legend = TLegend(canvas.GetLeftMargin()+0.35, 
		1.-canvas.GetTopMargin()-.2, 
		canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
		1.-canvas.GetTopMargin() )

	histo = frame.Histo1D(histomodel, variable)
	histo.SetMarkerStyle(8) # Large scalable dot
	histo.SetMarkerSize(0.5)
	histo.SetLineColor(ROOT.kBlack)
	#histo.SetTitle("{}_{}".format(variable, region)) # TODO: Delete once the binning is centralised
	#histo.SetFillColor(ROOT.kBlack)
	#legend.AddEntry(histo.GetPtr(), "histo", "PE")
	histo.Draw("E")

	
	if (drawlegend): legend.Draw()
	legend.SetBorderSize(1)
	legend.SetMargin(0.3)
	legend.SetTextSize(0.04)

	canvas.Print(outfolder+name+".png")
	canvas.Print(outfolder+name+".pdf")



if __name__ == "__main__":

	parser = ArgumentParser(description="Selection") 
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
	parser.add_argument("--prefix", dest="xrdpfx", action="store", type=str, default="root://cms-xrd-global.cern.ch//", help="XRootD prefix to be used to access files")
	parser.add_argument("--file", dest="file", action="store", type=str, default="", help="File name")
	parser.add_argument("--tree", dest="tree", action="store", type=str, default="Events", help="Path of tree within file")
	parser.add_argument("-o", "--out", dest="outputpath", action="store", type=str, default="./temp", help="Local output path")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument("--test", dest="test", action="store_true", default=False, help="Process a reduced number of files for testing purposes")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")

	

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 


	Ana.Init()


	#ROOT.gSystem.Load("HHbbtautauAnaELements.so")
	#ROOT.gSystem.Load("MyDict.so")
	

	sample = loadFile("sigggF")
	#sample = loadFileBase(options.file)

	if (options.test): 
		sample = generalise(sample.Range(0, 5000))

	print(sample)

	nBins = 50
	hist = ("", "#mu p_{T};#mu p_{T};", nBins, 0., 30.)

	PlotSimple(sample, "Electron_pt", hist)

	
	Ana.filemanager.CloseAll()

	print("Hello")


	
	




	

