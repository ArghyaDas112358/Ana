#!/usr/bin/env python

from __future__ import division, print_function


import os
import anaConfig
from ROOT import Ana, RDataFrame, TCanvas, kBlue, kGreen
from libMLTools import GetROC, GetFom
from libUtils import HoldUntilKeyPress
from anaPrepareRegions import PrepareRegions, PrepareRegionsSimple, PrepareSamples



from argparse import ArgumentParser

parser = ArgumentParser(description="GetEfficiency")
#parser.add_argument("filename", action="store", type=str, default="", help="Name of file")
#parser.add_argument("-N", "--version", dest="iteration", action="store", type=int, default=0, help="Which iteration of inference")
parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v7", help="Which version (cycle) of files to run on")
parser.add_argument("-g", "--cut", dest="cut", action="store", type=str, default="1", help="Custom cut to be included in eff calculation")
parser.add_argument("-o", "--out", dest="out", action="store", type=str, default="./plots/testroc/", help="Path for saving plots")
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

print(frames)


variables = ["b_tau_m", "mvaScore", "b_tau_min_dr_mu", "b_tau_alpha", "b_B_fsig", "b_B_m", "b_B_nmu", "b_B_nh", "b_B_ne", "b_B_npi0", "b_B_ngamma", "Dstar_vprob"]

for variable in variables: 
	# Loading signal and background 
	cutsig = (Ana.cut["base"]+Ana.samples.at("Sig").cut).GetTitle()
	cutbkg = (Ana.cut["base"]+Ana.samples.at("data").cut).GetTitle()

	stage = "all"
	signal = frames[sample[anaConfig.Sig]][stage] #RDataFrame(Ana.filemanager.GetItem(anaConfig.Sig)).Filter(cutsig)
	background = frames[sample[anaConfig.data]][stage] #RDataFrame(Ana.filemanager.GetItem(anaConfig.data)).Filter(cutbkg)

	print("Starting to compute ROC curve... ")
	roc, auc = GetROC(signal, background, variable)
	print("Computed ROC curve. ")

	print("Area under curve (A.U.C.): {}".format(auc))

	fom, maxsig, cutvalue = GetFom(signal, background, variable)
	
	print("Maximum significance of {} with cut at value {}".format(maxsig, cutvalue))

HoldUntilKeyPress()


Ana.filemanager.CloseAll()

