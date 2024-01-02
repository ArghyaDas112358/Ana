#!/usr/bin/env python


from __future__ import division, print_function


import ROOT
from ROOT import RooRealVar, RooDataHist, RooArgSet, RooWorkspace, RooExponential, RooCrystalBall, RooAddPdf, RooArgList


if __name__ == "__main__":
	print("Start plotting")

	fitfile = ROOT.TFile.Open("workspace.root", "READ")

	workspace = fitfile.Get("w")

	workspace.loadSnapshot("MultiDimFit")


	model = workspace.pdf("pdf") #model_s

	# Creating the RooFit plot
	var = workspace.var("pttau_B_m")
	frame = var.frame()

	model.plotOn(frame) #model.getPdf("bin0").plotOn(frame)

	datahist = workspace.data("data_obs")

	datahist.plotOn(frame)

	canvas = ROOT.TCanvas("canvas", "canvas", 800, 600)
	frame.Draw()

	canvas.Draw()

	canvas.Print("./plots/FitDenominator.pdf")
