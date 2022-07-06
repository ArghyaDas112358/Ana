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


def GetFom(sigeffs, bkgeffs, sigInSample=1., bkgInSample=1): 

	assert(len(sigeffs) == len(bkgeffs))
	numPoints = len(sigeffs)

	FOM = ROOT.TH1D("FOM", "", numPoints, -1., 1.)

	for point in range(0, numPoints): 
		sigEff = sigeffs[point]
		bkgEff = bkgeffs[point]
		
		print "Sig eff: {}, bkg eff: {}".format(sigEff, bkgEff)

		B = bkgInSample*bkgEff
		S = sigInSample*sigEff

		Sigma = 0 if (B == 0) else S/math.sqrt(B) #Sigma = 0 if (S+B == 0) else S/math.sqrt(S+B)

		FOM.SetBinContent(numPoints -1 - point, Sigma)
		#FOM.SetBinError(point, error)

	return FOM


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

	#ROOT.gROOT.ForceStyle()


	filemanager.AddItem("signal", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted_mvanew.root", "tree")
	filemanager.AddItem("bkgnoresponse", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPH1-3Run2018B_converted.root", "tree")
	filemanager.AddItem("background", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPHRun2018B_converted_mvanew.root", "tree")
	filemanager.AddItem("oldroc",  "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/newtest/plots.root", "xgboOptimized/scikit-like_ROC")
	#filemanager.AddItem("TMVAROC", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/newtest/plots.root", "xgboOptimized/TMVA-like_ROC")
	#filemanager.AddItem("bkgEff", "/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/MVA/trainingBayesian/newtest/plots.root", "xgboOptimized/bkgEff(sigEff)")

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
	bkgeff, sigeff, _ = roc_curve(truths, labels)
	print "Computed ROC curve. "


	canv = ROOT.TCanvas("canv", "canv", 800, 600)
	graph = ROOT.TGraph(len(bkgeff), np.asarray(bkgeff, "d"), np.asarray(sigeff, "d"))
	oldgraph = filemanager.GetItem("oldroc")
	oldgraph.Draw("AP")
	graph.Draw("SAME P")
	canv.Draw()
	graph.SetMarkerColor(ROOT.kBlue)
	oldgraph.SetMarkerColor(ROOT.kGreen)
	#oldgraph.SetLineColor(ROOT.kGreen)
	oldgraph.SetMarkerSize(1)
	oldgraph.SetMarkerStyle(8)

	legend = ROOT.TLegend(0.6, 0.2, 0.9, 0.3)
	legend.AddEntry(oldgraph, "Initial variables")
	legend.AddEntry(graph, "New variables")
	legend.SetBorderSize(0)
  	legend.SetFillColor(0)
  	legend.SetTextSize(0.04)
	legend.Draw()

	oldgraph.GetXaxis().SetTitleSize(0.04)
	oldgraph.GetYaxis().SetTitleSize(0.04)
	canv.Print(outputfolder+"NewRoc.pdf")


	sigeffcorr = np.sort(sigeff)
	bkgeffcorr = np.sort(bkgeff) #np.ones(len(bkgeff)) - np.sort(bkgeff)

	print sigeffcorr 
	print bkgeffcorr

	fom = GetFom(sigeffcorr, bkgeffcorr)

	fomcanvas = ROOT.TCanvas("fomcanvas", "fomcanvas", 800, 600)
	fom.Draw()
	fom.SetLineColor(ROOT.kGreen+2)
	#fom.SetMarkerColor(ROOT.kGreen)
	#fom.SetMarkerStyle(1)
	#fom.SetMarkerSize(2)
	fomcanvas.Draw()
	fomcanvas.Print(outputfolder+"Fom.pdf")






