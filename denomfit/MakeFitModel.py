#!/usr/bin/env python
from __future__ import division, print_function
import copy as cp



if __name__ == "__main__":

	import ROOT
	from ROOT import RDataFrame, RooRealVar, RooDataHist, RooArgSet, RooWorkspace, RooExponential, RooCrystalBall, RooAddPdf, RooArgList

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

	# Signal model

	#DCB parameters
	par = {}
	par[0]=35000;
	par[1]=5.27;
	par[2]=0.08;
	par[3]=1.;
	par[4]=1.;
	par[5]=1.;
	par[6]=1.;
	mu = RooRealVar("mu","mu",par[1],4.7,5.7);
	width = RooRealVar("width","width",par[2],0.,1.);
	a1 = RooRealVar("a1","a1",par[3],0.,100.);
	p1 = RooRealVar("p1","p1",par[4],0.,100.);
	a2 = RooRealVar("a2","a2",par[5],0.,100.);
	p2 = RooRealVar("p2","p2",par[6],0.,100.);

	signalmodel = RooCrystalBall("dcbPdf","DoubleSidedCB",var,mu,width,a1,p1,a2,p2);

	getattr(workspace, "import")(signalmodel)

	"""
	RooFormulaVar mean("mean","@0+@1", RooArgList(meanvalue, mushift));
	RooFormulaVar sigmap("sigmap","@0*@1",RooArgList(sigwidthscale,sigmapmc));
	RooRealVar cbnp("cbnp","",myconfigs->fcbnp);  
	RooRealVar cbap("cbap","",myconfigs->fcbap);//,-10.,0.);
	RooCBShape CBp("CBp","", mvar, mean, sigmap, cbap, cbnp);

	RooFormulaVar sigmam("sigmam","@0*@1",RooArgList(sigwidthscale,sigmammc));
	RooRealVar cbnm("cbnm","",myconfigs->fcbnm);  
	RooRealVar cbam("cbam","",myconfigs->fcbam);//,0.,10.);
	RooCBShape CBm("CBm","", mvar, mean, sigmam, cbam, cbnm); 

	RooRealVar fracm("fracm","",myconfigs->ffrac);

	RooAddPdf  CB("CB","",RooArgList(CBm,CBp),RooArgList(fracm));

	RooRealVar meanB("meanB","",myconfigs->fmean - 87.19, 5200, 5300); 
	RooCBShape CBpB("CBpB","", mvar, meanB, sigmap, cbap, cbnp); 
	RooCBShape CBmB("CBmB","", mvar, meanB, sigmam, cbam, cbnm); 
	RooAddPdf  CBB("CBB","",RooArgList(CBmB,CBpB),RooArgList(fracm));
	"""

	# Background model
	alpha = RooRealVar("alpha","alpha",-1.5,-5.0,1.0)
	bkgmodel = RooExponential("combinatorial", "combinatorial", var, alpha)
	getattr(workspace, "import")(bkgmodel)

	nsig = RooRealVar("nsig", "nsig", 100, 0., 1000.)
	nbkg = RooRealVar("nbkg", "nbkg", 100, 0., 4000.)
	totalmodel = RooAddPdf("pdf", "pdf", RooArgList(signalmodel, bkgmodel), RooArgList(nsig, nbkg))
	getattr(workspace, "import")(totalmodel)

	# #datahist.Write()
	#sig.Write()
	data.Write()
	workspace.Write()

	file.Close()

	#sigfile.Close()
	datafile.Close()



