import ROOT
import os
import math
from sklearn.metrics import roc_curve
import numpy as np


#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/libFunctions.C+")#ROOT.gROOT.LoadMacro("/eos/home-m/mhuwiler/plugins/libFunctions.C")
#ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/ExperimentSpecificLayer.C")
ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/FileManager/CFileManager.C")
ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/CMS/tdrstyle.C")
ROOT.gROOT.LoadMacro("/Users/mhuwiler/coding/plugins/Drawing/RatioCanvas.h")
ROOT.setTDRStyle()
#import CMS_lumi





plotstats = False

webpublication =False


outputfolder = "./plots/MVAFigures/"

os.system("mkdir -p "+outputfolder)

logscale = [""]

presentationfolder = "../Presentations/PresentationVFS_15_6_22/reveal.js-master/Figures.root"


#ROOT.ExperimentSpecificLayer.SetStyle("CMS")

#layer = ROOT.ExperimentSpecificLayer("CMS", 21, 11, "Preliminary"); 

#factor = 1.0
#CMS_lumi.writeExtraText = True
#CMS_lumi.extraText = "Simulation Preliminary"
#CMS_lumi.lumiTextSize = 0.45*factor
#CMS_lumi.lumiTextOffset = 0.2*factor
#CMS_lumi.cmsTextSize = 0.75*factor
#CMS_lumi.lumi_sqrtS = "14 TeV"
#CMS_lumi.lumi_13TeV = "2021"

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
			rep = raw_input( "(type 'yes' or 'no'): " ).lower()
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


# Web publication
if (webpublication): 
	webfolder = "/eos/home-m/mhuwiler/www/Analysis/MVAinputVariables/"
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


if __name__ == "__main__": 

	filemanager = ROOT.FileManager()


	filemanager.AddItem("signal", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted_mvanew.root", "tree")
	filemanager.AddItem("bkgnoresponse", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPH1-3Run2018B_converted.root", "tree")
	filemanager.AddItem("background", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted_mvanew.root", "tree")

	filemanager.OpenAllItems()

	#from anaMVA.loadSamples import loadSamples
	#samples = loadSamples(featuresToLoad, numLoad)


	#signal = [samples["signal"]]
	#background = [samples["background"]]

	signal = ROOT.RDataFrame(filemanager.GetItem("signal")).Filter("mvaScore>-1").AsNumpy()
	background = ROOT.RDataFrame(filemanager.GetItem("background")).Filter("mvaScore>-1").AsNumpy()

	print signal.keys()

	siglabels = signal["mvaScore"]

	print siglabels

	sigtruth = np.ones(len(siglabels))

	print sigtruth

	bkglabels = background["mvaScore"]
	bkgtruth = -np.ones(len(bkglabels))

	print bkglabels
	
	print bkgtruth

	labels = np.concatenate([siglabels, bkglabels])

	print labels

	truths = np.concatenate([sigtruth, bkgtruth])

	print "Starting to compute ROC curve... "
	sigeff, bkgeff, _ = roc_curve(truths, labels)
	print "Computed ROC curve. "


	canv = ROOT.TCanvas("canv", "canv", 800, 600)
	graph = ROOT.TGraph(len(bkgeff), np.asarray(sigeff, "d"), np.asarray(bkgeff, "d"))
	graph.Draw()
	canv.Draw()
	canv.Print(outputfolder+"NewRoc.pdf")






