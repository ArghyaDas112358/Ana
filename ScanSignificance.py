#!/usr/bin/env python

from __future__ import division, print_function


import os
import anaConfig
from ROOT import Ana, RDataFrame, TCanvas, kBlue, kGreen
from libMLTools import GetROC, GetFom, GetROCgeneral
from libUtils import HoldUntilKeyPress
from anaPrepareRegions import PrepareRegions, PrepareRegionsSimple, PrepareSamples



from argparse import ArgumentParser

parser = ArgumentParser(description="GetEfficiency")
#parser.add_argument("filename", action="store", type=str, default="", help="Name of file")
#parser.add_argument("-N", "--version", dest="iteration", action="store", type=int, default=0, help="Which iteration of inference")
parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
parser.add_argument("-g", "--cut", dest="cut", action="store", type=str, default="1", help="Custom cut to be included in eff calculation")
parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="./plots/testroc/", help="Path for saving plots")
parser.add_argument("-s", "--save", dest="save", action="store_true", default=False, help="Save output")
parser.add_argument('-l', "--variable", dest="variable", action="store", default="mvaScore", help="Variable to reprocess")
parser.add_argument("-n", "--target", dest="target", action="store", type=float, default=10000., help="Target number of events after selection")
parser.add_argument('-m', "--denom", dest="denom", action="store_true", default=False, help="Denominator analysis")

options = parser.parse_args()


outputfolder = "./plots/"+options.out+"/"

os.system("mkdir -p "+outputfolder)


postfix = "" #"_DNN_m"
if (options.denom): 
	anaConfig.Denominator()
	postfix = "_DNN"

		
Ana.Init(options.version, options.denom)


samples = anaConfig.samples
sample = {}
for item in samples:
	sample[item] = item+postfix

frames, effs = PrepareSamples(list(sample.values()))

#print(frames)


variables = [("mvaScore", 1), ("b_tau_min_dr_mu", 1), ("b_tau_alpha", 1), ("b_B_fsig", 1), ("b_B_m", 1), ("b_B_nmu", 0), ("b_B_nh", 0), ("b_B_ne", 0), ("b_B_npi0", 0), ("b_B_ngamma", 0), ("Dstar_vprob", 1)] #"b_tau_m", "mvaScore", "b_tau_min_dr_mu", "b_tau_alpha", "b_B_fsig", "b_B_m", "b_B_nmu", "b_B_nh", "b_B_ne", "b_B_npi0", "b_B_ngamma", "Dstar_vprob"

for variable, inverted in variables: 
	#print(variable)
	# Loading signal and background 
	cutsig = (Ana.cut["base"]+Ana.samples.at("Sig").cut).GetTitle()
	cutbkg = (Ana.cut["base"]+Ana.samples.at("data").cut).GetTitle()

	stage = "all"
	signal = frames[sample[anaConfig.Sig]][stage] #RDataFrame(Ana.filemanager.GetItem(anaConfig.Sig)).Filter(cutsig)
	background = frames[sample[anaConfig.data]][stage] #RDataFrame(Ana.filemanager.GetItem(anaConfig.data)).Filter(cutbkg)

	#print("Starting to compute ROC curve... ")
	sigma, auc, cutvalue, roc, fom = GetROCgeneral(signal, background, variable, inverted)
	#print("Computed ROC curve. ")

	#print("Area under curve (A.U.C.): {}".format(auc))

	#fom, maxsig, cutvalue = GetFom(signal, background, variable)
	
	print("\nMaximum significance for variable {} of {} with cut at value {}, auc {}".format(variable, sigma, cutvalue, auc))

	if (options.save): 
		canvas = TCanvas("canvas_{}".format(variable), "canvas_{}".format(variable), 1600, 600)
		canvas.Divide(2, 1)
		canvas.cd(1)
		roc.Draw("AP")
		import ROOT
		roc.SetMarkerColor(ROOT.kBlue)
		roc.SetTitle("ROC")
		roc.GetXaxis().SetTitle("#epsilon_{bkg}")
		roc.GetYaxis().SetTitle("#epsilon_{sig}")
		ROOT.gStyle.SetOptStat(0) 
		canvas.cd(2)
		fom.Draw("E")
		canvas.Draw()
		fom.SetMarkerColor(ROOT.kGreen)
		fom.SetLineColor(ROOT.kGreen)
		fom.SetTitle("F.o.M.")
		fom.GetXaxis().SetTitle(variable)
		name = outputfolder+"/ROCandFOM_{}".format(variable)
		canvas.Print("{}.pdf".format(name))
		canvas.SaveAs("{}.root".format(name))
		if (options.debug): HoldUntilKeyPress()


Ana.filemanager.CloseAll()

