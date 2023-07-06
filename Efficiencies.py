#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
import json
from argparse import ArgumentParser
from ROOT import TCanvas, TH1D, TPad, TLegend, THStack, RDataFrame
from uncertainties import ufloat
from uncertainties.umath import * 


#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/libFunctions.C+")#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/libFunctions.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/ExperimentSpecificLayer.C")
#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/FileManager/CFileManager.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/CMS/tdrstyle.C")
ROOT.gROOT.LoadMacro("FileFlow.h")
ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/RatioCanvas.h")
#ROOT.setTDRStyle()
#import CMS_lumi
from ROOT import Ana

from libEfficiencies import getEffFromInfo, getGenmatchingEff, DumpEffs, ReadEffs



webpublication =False




# Efficiency calculations 


def InitialEffs(lumi): 
	# Computing the initial efficiencies 
	#effs = collections.defaultdict(dict)
	# Efficiencies relative to the 
	effs = { "Sig": {"br":ufloat(1.84e-2, 2.2e-3), "geneff":ufloat(3.72e-4, 0.), "eff":ufloat(1.4e-3, 0.)}, # TODO: group these with the others into another file 
		"BkgDstarDs": {"br":ufloat(8e-3, 1.1e-3), "geneff":ufloat(1.458e-3, 0.), "eff":ufloat(1.4e-3, 0.)},
		"BkgDstarDsstar": {"br":ufloat(1.77e-2, 1.4e-3), "geneff":ufloat(5.38e-4, 0.), "eff":ufloat(1.4e-3, 0.)}, 
		"BkgDstar3pi": {"br":ufloat(7.21e-3, 2.9e-4), "geneff":ufloat(2.e-5, 0.), "eff":ufloat(1.4e-3, 0.)}, # TODO: obtain ana eff from other script
		"SigPart": {"br":ufloat(1.84e-2, 2.2e-3), "geneff":ufloat(3.72e-4, 0.), "eff":ufloat(1.4e-3, 0.)},
		"BkgDstara1": {"br":ufloat(1.3e-2, 2.7e-3), "geneff":ufloat(3.800e-04, 0.), "eff":ufloat(1.4e-3, 0.)},
		"BkgDstara1Part": {"br":ufloat(1.3e-2, 2.7e-3), "geneff":ufloat(3.800e-04, 0.), "eff":ufloat(1.4e-3, 0.)},
	}
	bbxsec = ufloat(4.72e8, 0.)
	fB0 = fB = ufloat(0.404, 0.006)
	Br_Dstar_D0pi = ufloat(6.77e-1, 0.)
	Br_D0_KPI = ufloat(3.88e-2, 0.)

	expected = {}
	for key, eff in effs.iteritems(): 
		key.replace("Part", "")
	 	expected[key]= lumi*bbxsec*fB0*2.*Br_Dstar_D0pi*Br_D0_KPI*1000.*eff["br"]*eff["geneff"] #*eff["eff"]

	return expected


def CompleteEffsFromFile(effs, version, filemanager): 
	anaeffs = {"Sig":ufloat(1.4e-3, 0.), "BkgDstarDs":ufloat(1.44e-3, 0.), "BkgDstarDsstar":ufloat(2.26e-3, 0.), "BkgDstar3pi":ufloat(5.3e-4, 0.), "SigPart":ufloat(1.27e-3, 0.), "dataD2WS":ufloat(-0.392, 0.), "dataD21TauWS": ufloat(-0.53, 0.)}
	for key, eff in effs.iteritems(): 
		print(key)
		
		efficiency = 1.
		try: 
			file = TFile.Open(filemanager.GetFile(key+"_ntuple"), "READ")
			efftree = file.Get("ntuplizer/EffCalc")
			print(efftree)
			efficiency = getEffFromInfo(efftree)
			file.Close()
		except: 
			efficiency = anaeffs[key]
		effs[key] = eff*efficiency
		#if anaeffs[key].nominal_value < 0.: 
			#effs[key] = anaeffs[key]
	return effs

def ReadEffsFromFile(item, version, filemanager): 
	anaeffs = {"Sig":ufloat(1.4e-3, 0.), "BkgDstarDs":ufloat(1.44e-3, 0.), "BkgDstarDsstar":ufloat(2.26e-3, 0.), "BkgDstar3pi":ufloat(5.3e-4, 0.), "SigPart":ufloat(1.27e-3, 0.), "dataD2WS":ufloat(-0.392, 0.), "dataD21TauWS": ufloat(-0.53, 0.)}
	
	print(item)
		
	efficiency = 1.
	try: 
		file = TFile.Open(filemanager.GetFile(item+"_ntuple"), "READ")
		efftree = file.Get("ntuplizer/EffCalc")
		print(efftree)
		efficiency = getEffFromInfo(efftree)
		file.Close()
	except: 
		efficiency = anaeffs[item]
		#if anaeffs[key].nominal_value < 0.: 
			#effs[key] = anaeffs[key]
	return efficiency


def MultiplyFinalEffs(effs, regioneffs):
	for item, content in regioneffs.iteritems(): 
		print("{}:".format(item))
		for key, value in content.iteritems(): 
			try:
				regioneffs[item][key] = effs[item]*regioneffs[item][key]
				if (effs[item].nominal_value < 0.): 
					regioneffs[item][key]=effs[item]
			except:
				regioneffs[item][key] = -1.
	return regioneffs


def FormatLatex(number, precision = 2): 
	numstring = "{:.{precision}e}".format(number, precision=precision)
	collection = numstring.split("e")
	assert(len(collection)==2)
	num = float(collection[0])
	err = int(collection[1])
	#err = err.replace("+", "")
	latexstring = "{:.{precision}} \\times 10^{{{}}}".format(num, err, precision=precision)
	return latexstring

	


# Web publication
if (webpublication): 
	webfolder = "/eos/home-m/mhuwiler/www/Analysis/BackgroundModellingUpdate/"
	os.system("mkdir -p "+webfolder)
	webenginesource = "/eos/home-m/mhuwiler/software/php-plots/"
	os.system("cp -r "+webenginesource+"res "+webfolder)
	os.system("cp "+webenginesource+"index.php "+webfolder)
	with open(webenginesource+"example/htaccess", "r") as permissionfile: 
		content = permissionfile.read()
		content = content.replace("/<me>/<my-project>/", webfolder)
		file = open(webfolder+".htaccess", "w")
		file.write(content)
		file.close()
	os.system("cp "+webfolder+".htaccess "+webfolder+"htaccess")
	webfolder = webfolder+"plots/"
	os.system("mkdir -p "+webfolder)

textsize = 0.04

numEvents = -1 

def AtomicDraw(histo, name, options = ""): 
	canv = ROOT.TCanvas("canv", "canv", 800, 600)
	histo.DrawCopy(options)
	canv.Draw()
	canv.Print(name)



if __name__ == "__main__": 
	from argparse import ArgumentParser

	parser = ArgumentParser(description="GetEfficiency")
	#parser.add_argument("filename", action="store", type=str, default="", help="Name of file")
	#parser.add_argument("-N", "--version", dest="iteration", action="store", type=int, default=0, help="Which iteration of inference")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v6.7", help="Which version (cycle) of files to run on")
	parser.add_argument("-l", "--lumi", dest="lumi", action="store", type=float, default=41.5, help="Luminostiy processed")
	parser.add_argument("-o", "--object", dest="object", action="store", type=str, default="ntuplizer/EffCalc", help="Efficiency info object within file")

	options = parser.parse_args()


	initial = InitialEffs(36.7)

	print(initial)

	DumpEffs(initial, "./testinitialeffs.json")

	read = ReadEffs("./testinitialeffs.json")

	anaeffs = ReadEffsFromFile("Sig", "v6", Ana.filemanager)

	filtereffs = ReadEffs("./data/etc/FilterEfficiencies.json")

	constants = ReadEffs("./data/etc/Constants.json")

	forcedbr = ReadEffs("./data/etc/ForcedBranchingFractions.json")

	print(constants)

	print(filtereffs)

	N = constants["sigmabb"]*constants["fB0"]*forcedbr["Sig"]

	print("N expected: {}".format(N))

	expected = {}

	lumi = 26.8

	samples = ["Sig", "BkgDstarDs", "BkgDstarDsstar", "B0toDstarD0K", "B0toDstarDs1"] #["Sig", "BkgDstarDs", "BkgDstarDsstar", "BkgB0DD", "BkgBuDXc"] , "BkgDstara1Part"
			
	Ana.Init(options.version)

	template = "{} & ${}$ & ${}$ & ${}$ & ${}$ & ${:fL}$ & ${}$ \\\\\n" #{:.1e} "{} & ${:.{precision}eL}$ & ${:.{precision}eL}$ & ${:.{precision}eL}$ & ${:fL}$ & ${}$ \\\\\n"

	numberafterselection = 10000.

	namedict = {"Sig":"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", "BkgDstarDs": "$B^0\\rightarrow D^{*-}D_{s}^+$", "BkgDstarDsstar": "$B^0\\rightarrow D^{*-}D_{s}^{*+}$", "BkgBuDXc": "$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", "BkgB0DD": "$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", "B0toDstarD0K": "$B^0\\rightarrow D^{*}D^{0}K$", "B0toDstarDs1": "$B^{0}\\rightarrow D^{*}D_{s1}$"} #{"Sig":"$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}\tau^\\pm\\myoversetnu{\\brabar}{\\nu}_\\tau}$", "BkgDstarDs": "$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}D_{s}^\\pm}$", "BkgDstarDsstar": "$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}D_{s}^{*\\pm}}$"} # "BkgBuDXc": "$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}\\pi^+X$", "BkgB0DD": "$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}X$"

	Nexpected = {}


	with open("/Users/mhuwiler/cernbox/DoctoralThesis/Analysis/Presentations/Presentation_23_7_18/efftable.tex", "w") as outfile: 
		outfile.write("\\begin{tabular}{lcccccr}\n")
		outfile.write("sample & $\\epsilon_{filter}$ & $\\epsilon_{ana}$ & $\\epsilon_{match}$ & Br & $N_{exp.}$ & N requested \\\\\n\\hline\n")

		print("Expected yields")
		for sample in samples: 
			# Getting selection efficiency from file 
			item = sample+"_ntuple"
			Ana.filemanager.OpenItem(item)
			file = ROOT.TFile.Open(Ana.filemanager.GetFile(item), "READ")
			info = file.Get("ntuplizer/EffCalc") #options.object

			if not info: 
				raise ValueError("ERROR: No efficiency info found in file. Are you sure this file should contain efficiency information at {} ?".format(options.object))

			eff = getEffFromInfo(info)

			cut = "Dstar_match&&pttau_tau_match"
			genmatcheff = getGenmatchingEff(Ana.filemanager.GetItem(item), cut)

			N = options.lumi*constants["sigmabb"]*constants["fB0"]*2*forcedbr[sample]*constants["BrDstar2D0pi"]*constants["BrD02Kpi"]*1000*filtereffs[sample]*eff
			Nexpected[sample] = N
			#print(10000./eff)
			print("\tN expected for {}: {} (filter eff: {}, ana eff: {}, genmatch eff: {}, number requested: {})".format(sample, N, filtereffs[sample], eff, genmatcheff, numberafterselection/eff))
			numrequested = numberafterselection/(eff*genmatcheff)
			n = round(numrequested.n, -3)
			outfile.write(template.format(namedict[sample], FormatLatex(filtereffs[sample].n), FormatLatex(eff.n), FormatLatex(genmatcheff.n), FormatLatex(forcedbr[sample].n), N, FormatLatex(numrequested.n), precision=2).replace("\\times", "\\cdot"))

		outfile.write("\\end{tabular}\n")

	DumpEffs(Nexpected, "./data/etc/Expectedyields.json")

	Ana.filemanager.CloseAll()

	print(read)


