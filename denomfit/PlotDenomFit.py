#!/usr/bin/env python


from __future__ import division, print_function


import ROOT
from ROOT import RooRealVar, RooDataHist, RooArgSet, RooWorkspace, RooExponential, RooCrystalBall, RooAddPdf, RooArgList, TColor
from ROOT.RooFit import Components, LineColor, FillColor, DrawOption, LineWidth, LineStyle, Name


if __name__ == "__main__":
	print("Start plotting")

	fitfile = ROOT.TFile.Open("higgsCombineTest.MultiDimFit.mH120.root", "READ")

	workspace = fitfile.Get("w")

	workspace.loadSnapshot("MultiDimFit")


	model = workspace.pdf("model_s") #model_s

	# Creating the RooFit plot
	var = workspace.var("b_B_m")
	frame = var.frame()

	datahist = workspace.data("data_obs")

	datahist.plotOn(frame)

	totalpdf = model.getPdf("bin0") #model.plotOn(frame)

	totalpdf.plotOn(frame)

	totalpdf.plotOn(frame, Components("shapeSig_sig_bin0"), LineColor(ROOT.kRed), DrawOption("LF"), FillColor(TColor.GetColorTransparent(ROOT.kRed-7,0.3)), LineWidth(1), Name("Signal"))
	totalpdf.plotOn(frame, Components("shapeBkg_comb_bin0"), LineColor(ROOT.kBlue+3), LineWidth(2), LineStyle(ROOT.kDotted), Name("Combinatorial"))

	canvas = ROOT.TCanvas("canvas", "canvas", 800, 600)
	frame.Draw()

	canvas.Draw()

	canvas.Print("./plots/FitDenominator.pdf")
