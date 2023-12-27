#!/usr/bin/env python
from __future__ import division, print_function
import copy as cp



if __name__ == "__main__":

	import ROOT
	from ROOT import RDataFrame, RooRealVar, RooDataHist, RooArgSet, RooWorkspace, RooExponential

	examplehist = ("hist", "hist", 20, 4.5, 6.)
	variable = "pttau_B_m"
	#sigfile = ROOT.TFile.Open("B0toDstarDs_tauDNN.root", "READ")
	#sig = cp.deepcopy(RDataFrame(sigfile.Get("ntuplizer/tree")).Histo1D(("sig", "sig", 40, 0., 20.), variable).GetPtr())

	datafile = ROOT.TFile.Open("../../../data/v7.01/dataD1.root", "READ")
	data = RDataFrame(datafile.Get("ntuplizer/tree")).Histo1D(examplehist, variable)

	var = RooRealVar(variable, variable, 0., 100.)
	fitspace = RooArgSet(var)

	file = ROOT.TFile.Open("workspace.root", "RECREATE")
	workspace = ROOT.RooWorkspace("w")

	# sighist = RooDataHist("sig", "sig", fitspace, sig.GetPtr())
	# getattr(workspace, "import")(sighist)
	# #sighist.Write()

	datahist = RooDataHist("data_obs", "data_obs", fitspace, data.GetPtr())
	getattr(workspace, "import")(datahist)

	signalmodel = 1

	# Background model
	alpha = RooRealVar("alpha","alpha",-1.5,-5.0,1.0)
	bkgmodel = RooExponential("combinatorial", "combinatorial", var, alpha)
	getattr(workspace, "import")(bkgmodel)

	# #datahist.Write()
	#sig.Write()
	data.Write()
	workspace.Write()

	file.Close()

	#sigfile.Close()
	datafile.Close()



