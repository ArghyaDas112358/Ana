#!/usr/bin/env python
from __future__ import division, print_function

import ROOT
import os
import math
import collections
import copy
from argparse import ArgumentParser
from ROOT import TCanvas, TH1D, TPad, TLegend


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

def BackgroundShapeUnrolled(reference, estimate, additionalhists, filename): # Works as in PlotDistributions
	#estimate = histos["data"]["CR"]
	#reference = histos["data"]["SR"]
	#additionalhists = [histos["MC"]["SR"], histos["DstarDs"]["SR"]]
	#estimate = UnrollHist(estimate.Histo2D(("rhomass1", "rhomass2", nBins, rangeMin, rangeMax, nBins, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2"))
	#reference = UnrollHist(reference.Histo2D(("rhomass1", "rhomass2", nBins, rangeMin, rangeMax, nBins, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2"))
	#additionalhists = []
	#for item in additionalframes: 
		#additionalhists.append(UnrollHist(SR[key].Histo2D(("rhomass1", "rhomass2", nBins, rangeMin, rangeMax, nBins, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2")))

	colors = [ROOT.kGreen+3, ROOT.kBlue+3] #ROOT.kOrange
	legends = ["Signal MC (signal region)",  "B#rightarrowD*D_{s} MC (signal region)"]
	norms = [1., 1.]

	assert(len(colors) >= len(additionalhists))
	assert(len(legends) >= len(additionalhists))
	assert(len(norms) >= len(additionalhists))

	# Plot the histogram
	estimate.Draw("LE")
	reference.Draw("LE SAME")
	for i, hist in enumerate(additionalhists): 
		additionalhists[i].Draw("LE SAME")

	estimate.GetXaxis().SetRangeUser(18, 100)

	reference.Scale(1./reference.Integral())
	estimate.Scale(1./estimate.Integral())
	estimate.Sumw2()
	reference.Sumw2()
	for i, hist in enumerate(additionalhists): 
		hist.Scale(norms[i]/hist.Integral())
		hist.Sumw2()

	maxes = [estimate.GetMaximum(), reference.GetMaximum()]
	for item in additionalhists: 
		maxes.append(item.GetMaximum())
	themax = max(maxes)

	estimate.SetMaximum(1.1*themax)

	estimate.SetLineColor(ROOT.kRed)
	reference.SetLineColor(ROOT.kBlue)
	for i, hist in enumerate(additionalhists): 
		hist.SetLineColor(colors[i]) 

	estimate.SetTitle("Unrolled 2D #rho mass distribution")

	estimateErr = estimate.DrawCopy("HIST SAME")
	referenceErr = reference.DrawCopy("HIST SAME")
	additionalhistsErr = []
	for hist in additionalhists: 
		additionalhistsErr.append(hist.DrawCopy("HIST SAME"))

	legend = ROOT.TLegend(.65,.60,.90,.85)
	legend.SetBorderSize(0)
	legend.SetFillColor(0)
	legend.SetFillStyle(0)
	legend.SetTextFont(42)
	legend.SetTextSize(0.04)
	legend.AddEntry(referenceErr,"data (signal region)","l")
	legend.AddEntry(estimateErr,"data (sideband)","l")
	for i, hist in enumerate(additionalhists): 
		legend.AddEntry(hist,legends[i],"l")
	legend.Draw()

	histoRatio = estimate.Clone()
	histoRatio.Divide(reference.Clone())

	canvas.Lower()

	histoRatio.Draw("HIST")
	histoRatio.SetLineColor(1)
	histoRatio.DrawCopy("LE SAME")


	canvas.Draw()

	#histoRatio.SetMaximum(0.2)
	#histoRatio.SetMinimum(1.8)
	histoRatio.GetYaxis().SetRangeUser(0.2, 1.8)
	histoRatio.GetYaxis().SetNdivisions(5)
	canvas.RemoveMiddleAxis()

	canvas.Update()

		#FOM = filemanager.GetItem("cutflowGen")

	fomcanvas = ROOT.TCanvas("fomcanvas", "fomcanvas", 800, 600)

	#canv = ROOT.TCanvas("canv", "canv", 800, 600)
	#hist = SR["MC"].Histo2D(("rhomass1", "rhomass2", 15, rangeMin, rangeMax, 15, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2")
	#hist.SetTitle("m(#rho_{1}) vs m(#rho_{2})")
	#hist.DrawCopy("COLZ")
	#canv.Draw()
	#canv.Print(outputfolder+"2Drhomass.pdf")

	

	#formatHistoCommon(histgwf)
#	ROC.SetLineColor(ROOT.kBlue+2)
#	ROC.SetLineWidth(2)
#	ROC.GetXaxis().SetLabelSize(textsize)
#	ROC.GetYaxis().SetLabelSize(textsize)

#	FOM.SetLineColor(ROOT.kGreen)
#	#FOM.SetLineWidth(2)
#	FOM.SetMarkerStyle(7)
#	FOM.SetMarkerColor(ROOT.kGreen)
#	FOM.GetXaxis().SetLabelSize(textsize)
#	FOM.GetYaxis().SetLabelSize(textsize)


	splitfraction = 0.8

	sc = 1.05

	# Making the ratio plots 




	#if plotstats: 
	#	scale = 1./1.2
	#else: 
	#	scale = 1.
	#legend = ROOT.TLegend(.62*scale,.60,.90*scale,.85)
	#legend.SetBorderSize(0)
	#legend.SetFillColor(0)
	#legend.SetFillStyle(0)
	#legend.SetTextFont(42)
	#legend.SetTextSize(0.04)
	#legend.AddEntry(histgwf,"GPU workflow","l")
	#legend.Draw()

	#histlwf.Scale(histgwf.Integral()/histlwf.Integral())



   
	canvas.Update()

	canvas.Print(filename+".pdf")
	if (webpublication): canvas.Print(filename+".png")


	#f = ROOT.TFile.Open(presentationfolder, "Update")
	#fomcanvas.Write()
	#canvas.Write()
	#f.Write()
	#f.Close()


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
			print(("\t{}: {}".format(item, frames[item][key].Count().GetValue())))
		print("\n")


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


def PlotOverlay(frames, dataname, components, regions, variables, outfolder, drawlegend=True): 
	# Plotting distributions over each other 
	outfolder+="/overlay/"
	os.system("mkdir -p "+outfolder)
	factor = 1.1 # how much overhead to add to the histos 
	components.remove(dataname)
	for region in regions: 
		for variable in variables: 
			name = "{}_{}".format(variable, region)
			canvas = TCanvas(name, "{} in {}".format(variable, region), 800, 600)

			legend = TLegend(canvas.GetLeftMargin()+0.35, 
	                         	1.-canvas.GetTopMargin()-.2, 
	                            canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
	                           	1.-canvas.GetTopMargin() )

			data = frames[dataname][region].Histo1D(variable)
			data.SetMarkerStyle(8) # Large scalable dot
			data.SetMarkerSize(0.5)
			data.SetLineColor(ROOT.kBlack)
			#data.SetFillColor(ROOT.kBlack)
			legend.AddEntry(data.GetPtr(), "data", "PE")
			data.Draw("E")

			maxes = [data.GetMaximum()]

			i = 0
			for component in components: 
				histo = frames[component][region].Histo1D(variable)
				ROOT.SetOwnership(histo, 0)
				histo.SetLineStyle(1) # plain
				histo.SetLineWidth(2)
				histo.SetLineColor(colors[i])
				#histo.SetMarkerColor(Ana.color[component])
				histo.Draw("HIST SAME")
				maxes.append(histo.GetMaximum())
				legend.AddEntry(histo.GetPtr(), component)
				i+=1

			legend.Draw()
			legend.SetBorderSize(1);
			legend.SetMargin(0.3);
			legend.SetTextSize(0.04);

			data.SetMaximum(factor*max(maxes))
			canvas.Draw()

			canvas.Print(outfolder+name+".png")
			canvas.Print(outfolder+name+".pdf")




#ROOT.gInterpreter.Declare("""
#	double Rhomass2DUnrolled(Float_t rhomass1, Float_t rhomass2)
#	{
#		return int((std::min(rhomass2, 1.3) - 0.2)/0.22) + 6*int((std::min(tau_rhomass1, 1.3) - 0.2)/0.22); 
#	}
#""")


if __name__ == "__main__":

	parser = ArgumentParser(description="SignalBackground")
	#parser.add_argument("tool", action="store", type=str, help="Which time list you want to analyse")
	parser.add_argument("--out", dest="out", action="store", type=str, default="BackgroundEstimateUpdate/", help="Directory where the plots shuld go")
	parser.add_argument("--name", dest="name", action="store", type=str, default="test", help="Turn on debug output")
	parser.add_argument("-c", "--version", dest="version", action="store", type=str, default="v1", help="Which version (cycle) of files to run on")
	parser.add_argument("--debug", dest="debug", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument("-f", "--forcepath", dest="forcepath", action="store_true", default=False, help="Turn on debug output")
	parser.add_argument('-b', "--batch", dest="batch", action="store_true", default=False, help="Run in batch mode")
	parser.add_argument("--stats", dest="stats", action="store_true", default=False, help="Show stats box in ROOT")

	options = parser.parse_args()

	
	if (options.batch): 
		ROOT.gROOT.SetBatch(1) 

	if (options.stats): 
		ROOT.gStyle.SetOptStat(1111111)
	else: 
		# Don't plot stats box
		ROOT.gStyle.SetOptStat(0) 

	outputfolder = "./plots/"+options.out+"/"
	if (options.forcepath): 
		print("WARNING: You have used option '-f' or '--forcepath'. Files will be written to: {}".format(options.out))
		outputfolder = options.out+"/"

	os.system("mkdir -p "+outputfolder)


	# Global initialisations
	Ana.Init(options.version)


	filesUsed = ["dataD2", "Sig", "BkgDstarDs"]

	regions = ["SR", "CR", "SB"]

	variables = ["b_tau_rhomass1", "b_tau_rhomass2"]


	nBins = 6
	rangeMin = 0.2 #0.37
	rangeMax = 1.5 #1.43
	

	for file in filesUsed: 
		Ana.filemanager.OpenItem(file)


	# Starting script 
	canvas = ROOT.RatioCanvas("romassunrolled", "Unrolled 2D distribution of rho mass", 800, 600)

	samples = {}
	frames = collections.defaultdict(dict)
	histos = collections.defaultdict(dict)
	histosunrolled = collections.defaultdict(dict)

	for item in filesUsed:  
		samples[item] = ROOT.RDataFrame(Ana.filemanager.GetItem(item))
		for region in regions: 
			cut = Ana.cut[region].GetTitle()
			if (options.debug): print("Using following cut string (from TCut): {}".format(cut))
			frames[item][region] = samples[item].Filter(cut)
			ROOT.SetOwnership(frames[item][region], 0)
			# For histogram legacy compatibility
			histos[item][region] = frames[item][region].Histo2D(("rhomass1", "rhomass2", nBins, rangeMin, rangeMax, nBins, rangeMin, rangeMax), "b_tau_rhomass1", "b_tau_rhomass2")
			histosunrolled[item][region] = UnrollHist(histos[item][region])

	if options.debug: print(frames)


	BackgroundShapeUnrolled(histosunrolled["dataD2"]["SR"], histosunrolled["dataD2"]["SB"], [histosunrolled["Sig"]["SR"], histosunrolled["BkgDstarDs"]["SR"]], outputfolder+"UnrolledRhoMass")

	AtomicDraw(frames["Sig"]["SR"].Histo1D("b_tau_rhomass1"), outputfolder+"/SignalFromNew.png")

	colors = [4, 3, 6, 7, 9]

	files = filesUsed
	PlotOverlay(frames, "dataD2", files, regions, variables, outputfolder)

	


	Ana.filemanager.CloseAll()



