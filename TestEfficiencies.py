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



webpublication =False



def LoadFiles(filesUsed, cycle="v1"): # TODO: add into common python include 
	Ana.Init(cycle)
	for item in filesUsed: 
		print("Opening file: {}".format(item))
		Ana.filemanager.OpenItem(item); 
	return Ana.filemanager

#presentationfolder = "../Presentations/Presentation_22_5_10/reveal.js-master/Figures.root"



def formatHistoCommon(hist): 
	hist.SetLineWidth(2)
	hist.SetLineStyle(1)
	hist.SetMarkerStyle(8) # None 0
	hist.SetMarkerSize(0.4)
	#hist.SetLineColor(4)
	hist.GetYaxis().SetRangeUser(0, hist.GetMaximum()*1.4)
	#hist.GetYaxis().SetTitle("Events")
	hist.GetYaxis().SetTitleOffset(1.3)
	if quantity in logscale:
		hist.GetYaxis().SetRangeUser(0.1, hist.GetMaximum()*1000)
		canvas.SetLogy()
	#hist.GetXaxis().SetTitleSize(0.05)
	hist.GetXaxis().SetTitleOffset(1.1)
	canvas.SetTopMargin(0.1)
	#hist.GetXaxis().SetTitle("Probe jet #tau_{21}")
	#hist.GetXaxis().SetTitleSize(0.05)
	#hist.GetXaxis().SetTitleOffset(1.2)
	#hist.GetYaxis().SetTitleSize(0.05)
	#hist.GetYaxis().SetTitleOffset(1.2)
	#hist.GetXaxis().SetLabelSize(0.)
	#hist.GetXaxis().SetLabelOffset(-999.)

	return #histo


def formatAxisCommon(axis): 
	axis.SetTitleSize(0.05)
	axis.SetTitleOffset(1.2)

	return

def PromptYesNo(answerasbool=False): 
		# Inspired from Fabrice Couderc 
		rep = ''
		while not rep in [ 'yes', 'no' ]:
			rep = input( "(type 'yes' or 'no'): " ).lower()
		if (answerasbool): 
			if (rep == 'yes'): 
				return True
			else: 
				return False
		return rep

def getStatsBox(histogram):  #Works 
	dummycanvas = ROOT.TCanvas("dummycanvas", "dummycanvas", 800, 600)
	#histogram.SetStats(True)
	newhistogram = histogram.Clone(histogram.GetName())
	newhistogram.SetStats(True)
	newhistogram.Draw()
	dummycanvas.Update()
	stats = newhistogram.GetListOfFunctions().FindObject("stats").Clone("stats"+histogram.GetName())
	ROOT.SetOwnership(stats, 0)
	return stats

def exportHistFromTree(tree, variable, cut = "1", nbins = 0, minbin = None, maxbin = None): 
	dummycanvas = ROOT.TCanvas("dummycanvas", "dummycanvas", 800, 600)
	binning = ""; 
	if (nbins): 
		if (minbin == None): minbin = tree.CopyTree(cut).GetMinimum(variable) 
		if (maxbin == None): maxbin = tree.CopyTree(cut).GetMaximum(variable)
		binning = "({}, {}, {})".format(nbins, minbin, maxbin)
	tree.Draw(variable+">>h"+binning, cut);
	histo = tree.GetHistogram(); 
	histo.SetDirectory(0); 
	return histo; 

def UnrollHist(histo2D, inverted=True): 
	nx = histo2D.GetNbinsX()
	ny = histo2D.GetNbinsY()
	if inverted: 
		ny = histo2D.GetNbinsX()
		nx = histo2D.GetNbinsY()

	nTotal = nx*ny

	name = "unrolled"+histo2D.GetName()
	unrolled = ROOT.TH1D(name, name, nTotal, 0, 100)

	print("Nunmber of bins: {}, {}".format(nx, ny))

	for i in range(0, nx):
		for j in range(0, ny): # TODO: check overflow is handled properly 
			if inverted: 
				binContent = histo2D.GetBinContent(j, i)
			else: 
				binContent = histo2D.GetBinContent(i, j)
			unrolled.SetBinContent(i+j*nx, binContent)

	return unrolled


# Efficiency calculations 

def GetEfficiencies(frames): 
	keys = [] #TODO: generate a json
	#for item in frames: 
		#localkeys = frames[item].keys()
		#print localkeys
	
	#Credit to: https://stackoverflow.com/questions/35491223/inverting-a-dictionary-with-list-values
	inv_frames = {}
	for k,v in list(frames.items()):
		for x in v:
			inv_frames.setdefault(x,[]).append(k)
	print(inv_frames)

	for key, value in list(inv_frames.items()): 
		print(key)
		for item in value: 
			n = frames[item][key].Count().GetValue()
			N = frames[item]["all"].Count().GetValue()
			eff = n/N
			print(("\t{}: {} ({}/{})".format(item, eff, n, N)))
		print("\n")


def getEff(n, N): 
	eff = float(n)/float(N)
	#print eff
	err = sqrt(eff*(1.-eff)/float(N))
	#print err
	#return eff, err
	return ufloat(eff, err)


def ComputeEfficiencies(frames): 
	efficiencies = collections.defaultdict(dict)
	for item, content in frames.iteritems(): 
		#print("{}:".format(item))
		for key, value in content.iteritems(): 
			n = frames[item][key].Count().GetValue()
			N = frames[item]["all"].Count().GetValue()
			# see: chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://indico.cern.ch/event/66256/contributions/2071577/attachments/1017176/1447814/EfficiencyErrors.pdf
			ne = ufloat(n, sqrt(n))
			NE = ufloat(N, sqrt(N))
			efferr = ne/NE
			#print(efferr)
			#print(("\t{}: {} ({}/{})".format(key, eff, n, N)))
			efficiencies[item][key] = getEff(n, N) #ufloat(eff, err)
	return efficiencies


def PrintEfficiencies(effs): 
	for item, content in effs.iteritems(): 
		print("{}:".format(item))
		for key, value in content.iteritems(): 
			print(("\t{}: {}".format(key, value)))


def DumpEffs(effs, path): 
	effsForWrite = {}
	for item, content in effs.iteritems(): 
			eff = effs[item]
			effsForWrite[item] = (eff.n, eff.s)
	with open(path, "w") as file: 
		json.dump(effsForWrite, file, ensure_ascii=False, encoding="utf8", sort_keys=False)

def ReadEffs(path): 
	effs = {}
	with open(path, "r") as file: 
		effsFromFile = json.load(file, encoding="utf8")
		for item, content in effsFromFile.iteritems(): 
				eff = effsFromFile[item]
				assert(len(eff)==2)
				effs[item] = ufloat(eff[0], eff[1])
	return effs


def DumpEffs2D(effs, path): 
	effsForWrite = collections.defaultdict(dict)
	for item, content in effs.iteritems(): 
		for key, value in content.iteritems(): 
			eff = effs[item][key]
			effsForWrite[item][key] = (eff.n, eff.s)
	with open(path, "w") as file: 
		json.dump(effsForWrite, file, ensure_ascii=False, encoding="utf8", sort_keys=False)


def ReadEffs2D(path): 
	effs = collections.defaultdict(dict)
	with open(path, "r") as file: 
		effsFromFile = json.load(file, encoding="utf8")
		for item, content in effsFromFile.iteritems(): 
			for key, value in content.iteritems(): 
				eff = effsFromFile[item][key]
				assert(len(eff)==2)
				effs[item][key] = ufloat(eff[0], eff[1])
	return effs


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


def getEff(n, N): 
	eff = float(n)/float(N)
	#print eff
	err = sqrt(eff*(1.-eff)/float(N))
	#print err
	#return eff, err
	return ufloat(eff, err)

def getEffFromInfo(tree): 
	frame = RDataFrame(tree)

	n = frame.Sum("numSelected").GetValue()

	N = frame.Sum("numTotal").GetValue()

	eff = getEff(n, N)
	return eff

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

	filtereffs = ReadEffs("../../data/etc/FilterEfficiencies.json")

	constants = ReadEffs("../../data/etc/Constants.json")

	forcedbr = ReadEffs("../../data/etc/ForcedBranchingFractions.json")

	print(constants)

	print(filtereffs)

	N = constants["sigmabb"]*constants["fB0"]*forcedbr["Sig"]

	print("N expected: {}".format(N))

	expected = {}

	lumi = 26.8

	samples = ["Sig", "BkgDstarDs", "BkgDstarDsstar", "BkgB0DD", "BkgBuDXc"] #, "BkgDstara1Part"
			
	Ana.Init(options.version)

	template = "{} & ${}$ & ${:.{precision}eL}$ & ${:.{precision}eL}$ & ${:fL}$ & ${}$ \\\\\n" #{:.1e} "{} & ${:.{precision}eL}$ & ${:.{precision}eL}$ & ${:.{precision}eL}$ & ${:fL}$ & ${}$ \\\\\n"

	numberafterselection = 10000.

	namedict = {"Sig":"$B^0\\rightarrow D^{*-}\\tau^+\\nu_\\tau$", "BkgDstarDs": "$B^0\\rightarrow D^{*-}D_{s}^+$", "BkgDstarDsstar": "$B^0\\rightarrow D^{*-}D_{s}^{*+}$", "BkgBuDXc": "$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$", "BkgB0DD": "$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}$"} #{"Sig":"$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}\tau^\\pm\\myoversetnu{\\brabar}{\\nu}_\\tau}$", "BkgDstarDs": "$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}D_{s}^\\pm}$", "BkgDstarDsstar": "$\\smash{\\myoverset{\\brabar}{B}^0\\rightarrow D^{*\\mp}D_{s}^{*\\pm}}$"} # "BkgBuDXc": "$B^+\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}\\pi^+X$", "BkgB0DD": "$B^0\\rightarrow D_{(s)}^{(*)}D_{(s)}^{(*)}X$"

	Nexpected = {}


	with open("/Users/mhuwiler/cernbox/DoctoralThesis/Analysis/Presentations/PresentationVFS_23_4_26/efftable.tex", "w") as outfile: 
		outfile.write("\\begin{tabular}{lccccr}\n")
		outfile.write("sample & $\\epsilon_{filter}$ & $\\epsilon_{ana}$ & Br & $N_{exp.}$ & N requested \\\\\n\\hline\n")

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

			N = options.lumi*constants["sigmabb"]*constants["fB0"]*2*forcedbr[sample]*constants["BrDstar2D0pi"]*constants["BrD02Kpi"]*1000*filtereffs[sample]*eff
			Nexpected[sample] = N
			#print(10000./eff)
			print("\tN expected for {}: {} (filter eff: {}, ana eff: {}, number requested: {})".format(sample, N, filtereffs[sample], eff, numberafterselection/eff))
			numrequested = numberafterselection/eff
			n = round(numrequested.n, -3)
			outfile.write(template.format(namedict[sample], FormatLatex(filtereffs[sample].n), eff, forcedbr[sample], N, FormatLatex(numrequested.n), precision=2).replace("\\times", "\\cdot"))

		outfile.write("\\end{tabular}\n")

	DumpEffs(Nexpected, "./data/etc/Expectedyields.json")

	Ana.filemanager.CloseAll()

	print(read)


