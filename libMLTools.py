#!/usr/bin/env python
from __future__ import division, print_function

import os
import copy
from ROOT import RDataFrame, TGraph

from libEfficiencies import getEffFromInfo, getGenmatchingEff, DumpEffs, ReadEffs, FormatLatex, getOfflineEff




def ComputeRoc(sig, bkg, mvavar = "mvaScore", sigtarget=1., bkgtarget=-1.): 
	import numpy as np
	from sklearn.metrics import roc_curve, auc
	columns = [mvavar]
	signal = sig.AsNumpy(columns)
	background = bkg.AsNumpy(columns)

	siglabels = signal[mvavar]

	#print(siglabels)

	sigtruth = sigtarget*np.ones(len(siglabels))

	#print(sigtruth)

	bkglabels = background[mvavar]
	bkgtruth = bkgtarget*np.ones(len(bkglabels))

	#print(bkglabels)
	
	#print(bkgtruth)

	labels = np.concatenate([siglabels, bkglabels])

	#print(labels)

	truths = np.concatenate([sigtruth, bkgtruth])

	bkgeff, sigeff, _ = roc_curve(truths, labels)

	# creating a TGraph from the efficiency points
	graph = TGraph(len(bkgeff), np.asarray(bkgeff, "d"), np.asarray(sigeff, "d"))

	area = auc(bkgeff, sigeff)

	return graph, area #copy.deepcopy(graph)


if __name__ == "__main__": 
	from argparse import ArgumentParser

	parser = ArgumentParser(description="GetEfficiency")
	#parser.add_argument("filename", action="store", type=str, default="", help="Name of file")
	#parser.add_argument("-N", "--version", dest="iteration", action="store", type=int, default=0, help="Which iteration of inference")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v6.8", help="Which version (cycle) of files to run on")
	parser.add_argument("-g", "--cut", dest="cut", action="store", type=str, default="1", help="Custom cut to be included in eff calculation")
	parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="./plots/testroc/", help="Path for saving plots")
	parser.add_argument('-l', "--variable", dest="variable", action="store", default="mvaScore", help="Variable to reprocess")
	parser.add_argument("-n", "--target", dest="target", action="store", type=float, default=10000., help="Target number of events after selection")

	options = parser.parse_args()


	samples = ["Sig", "dataB2", "datD2WS"] #["Sig", "BkgDstarDs", "BkgDstarDsstar", "BkgB0DD", "BkgBuDXc"] , "BkgDstara1Part"

	import ROOT

	ROOT.gROOT.LoadMacro("FileFlow.h")

	from ROOT import Ana
			
	Ana.Init(options.version)

	for file in samples: 
		Ana.filemanager.OpenItem(file)

	os.system("mkdir -p {}".format(options.out))

	

	# Loading signal and background 
	cutsig = (Ana.cut["base"]+Ana.samples.at("Sig").cut).GetTitle()
	cutbkg = (Ana.cut["base"]+Ana.samples.at("dataB2").cut).GetTitle()

	signal = RDataFrame(Ana.filemanager.GetItem("Sig")).Filter(cutsig)
	background = RDataFrame(Ana.filemanager.GetItem("dataB2")).Filter(cutbkg)

	print("Starting to compute ROC curve... ")
	roc, auc = ComputeRoc(signal, background, options.variable)
	print("Computed ROC curve. ")

	canv = ROOT.TCanvas("canv", "canv", 800, 600)
	roc.Draw("AP")
	canv.Draw()
	roc.SetMarkerColor(ROOT.kBlue)
	#roc.SetMarkerSize(1)
	#roc.SetMarkerStyle(8)
	canv.Draw()
	canv.Print(options.out+"ROC.pdf")
	print("Area under curve (A.U.C.): {}".format(auc))

	from libUtils import HoldUntilKeyPress
	HoldUntilKeyPress()


	Ana.filemanager.CloseAll()


