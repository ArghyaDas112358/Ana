#!/usr/bin/env python
from __future__ import division, print_function

import os
from ROOT import TCanvas, TLegend, THStack, gROOT


#gROOT.LoadMacro("FileFlow.h")
import ROOT
from ROOT import Ana


def PlotOverlay(frames, dataname, initialcomponents, regions, variables, yields, outfolder, drawlegend=True): 
	# Plotting distributions over each other 
	#outfolder+="overlay/"
	os.system("mkdir -p "+outfolder)
	factor = 1.1 # how much overhead to add to the histos 
	components = copy.deepcopy(initialcomponents)
	components.reverse()
	if (dataname in components): components.remove(dataname)
	for region in regions: 
		for variable in variables: 
			name = "{}_{}".format(variable, region)
			canvas = TCanvas(name, "{} in {}".format(variable, region), 800, 600)

			legend = TLegend(canvas.GetLeftMargin()+0.35, 
	                         	1.-canvas.GetTopMargin()-.2, 
	                            canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
	                           	1.-canvas.GetTopMargin() )

			examplehist = Ana.binning[variable]
			data = frames[dataname][region].Histo1D(examplehist, variable)
			data.SetMarkerStyle(8) # Large scalable dot
			data.SetMarkerSize(0.5)
			data.SetLineColor(ROOT.kBlack)
			data.SetTitle("{}_{}".format(variable, region)) # TODO: Delete once the binning is centralised
			#data.SetFillColor(ROOT.kBlack)
			legend.AddEntry(data.GetPtr(), "data", "PE")
			data.Draw("E")

			maxes = [data.GetMaximum()]

			i = 0
			for component in components: 
				histo = frames[component][region].Histo1D(examplehist, variable)
				ROOT.SetOwnership(histo, 0)
				histo.SetLineStyle(1) # plain
				histo.SetLineWidth(2)
				histo.SetLineColor(Ana.colorold[component.replace("Part", "")]) #colors[i]Ana.color[component.replace("Part", "")]
				#histo.SetFillStyle(3003)
				#histo.SetFillColorAlpha(Ana.color[component], 0.4)
				if (yields[component][region].nominal_value > 0.): 
					histo.Scale(yields[component][region].nominal_value/histo.Integral())
				elif (yields[component][region].nominal_value != -1.):
					histo.Scale(-yields[component][region].nominal_value*histo.Integral())
				#histo.SetMarkerColor(Ana.color[component])
				histo.Draw("HIST SAME")
				maxes.append(histo.GetMaximum())
				legend.AddEntry(histo.GetPtr(), component)
				i+=1

			if (drawlegend): legend.Draw()
			legend.SetBorderSize(1)
			legend.SetMargin(0.3)
			legend.SetTextSize(0.04)

			data.SetMaximum(factor*max(maxes))
			canvas.Draw()

			canvas.Print(outfolder+name+".png")
			canvas.Print(outfolder+name+".pdf")


def PlotStack(frames, dataname, initialcomponents, regions, variables, yields, outfolder, drawlegend=True): 
	# Plotting distributions over each other 
	#outfolder+="stacked/"
	os.system("mkdir -p "+outfolder)
	factor = 1.1 # how much overhead to add to the histos 
	components = copy.deepcopy(initialcomponents)
	if (dataname in components): components.remove(dataname)
	components.reverse()
	notYetDrawn = True
	for region in regions: 
		for variable in variables: 
			name = "{}_{}".format(variable, region)
			canvas = TCanvas(name, "{} in {}".format(variable, region), 800, 600)

			legend = TLegend(canvas.GetLeftMargin()+0.35, 
	                         	1.-canvas.GetTopMargin()-.2, 
	                            canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
	                           	1.-canvas.GetTopMargin() )

			examplehist = Ana.binning[variable]
			data = frames[dataname][region].Histo1D(examplehist, variable)
			data.SetMarkerStyle(8) # Large scalable dot
			data.SetMarkerSize(0.5)
			data.SetLineColor(ROOT.kBlack)
			data.SetTitle("") #data.SetTitle("{}_{}".format(variable, region))
			#data.SetFillColor(ROOT.kBlack)
			legend.AddEntry(data.GetPtr(), "data", "PE")
			data.Draw("E")

			stack = THStack("stack", "Background modelling")
			hists = {}
			for component in components: 
				if "-" in component: 
					comps = component.split("-")
					assert(len(comps)>=2)
					print(component)
					histo = copy.deepcopy(frames[comps[0]][region].Histo1D(examplehist, variable).GetPtr())
					if (yields[comps[0]][region] > 0. ): 
						histo.Scale(yields[comps[0]][region].nominal_value/histo.Integral())
					elif (yields[comps[0]][region].nominal_value != -1.):
						histo.Scale(-yields[comps[0]][region].nominal_value*histo.Integral())
					comps.remove(comps[0])
					for comp in comps: 
						hist = copy.deepcopy(frames[comp][region].Histo1D(examplehist, variable).GetPtr())
						if (yields[comp][region] > 0. ): 
							hist.Scale(yields[comp][region].nominal_value/hist.Integral())
						elif (yields[comp][region].nominal_value != -1.):
							hist.Scale(-yields[comp][region].nominal_value*hist.Integral())
						histo.Add(hist, -1.)
				else: 
					histo = frames[component][region].Histo1D(examplehist, variable).GetPtr()
					if (yields[component][region] > 0. and histo.Integral() > 0): 
						histo.Scale(yields[component][region].nominal_value/histo.Integral())
					elif (yields[component][region].nominal_value != -1.):
						histo.Scale(-yields[component][region].nominal_value*histo.Integral())
				ROOT.SetOwnership(histo, 0)
				histo.SetLineStyle(1) # plain
				histo.SetLineWidth(2)
				color = Ana.samples.at(component.replace("Part", "")).color
				if (not color): 
					color = colors[0]
				histo.SetLineColor(color)
				#histo.SetMarkerColor(Ana.color[component])
				histo.SetFillStyle(1)
				histo.SetFillColor(color)
				hists[component] = histo
				stack.Add(histo)
				#legend.AddEntry(histo.GetPtr(), Ana.legends[component], "F")

			stack.Draw("HIST SAME") #"SAME"
			data.Draw("E SAME") # Plot on top
			if (drawlegend): legend.Draw()
			legend.SetBorderSize(1)
			legend.SetMargin(0.3)
			legend.SetTextSize(0.04)

			#data.GetXaxis().SetTitle(Ana.labels[variable])
			data.GetXaxis().SetTitleSize(0.06)
			data.GetXaxis().SetLabelSize(0.06)
			data.GetYaxis().SetLabelSize(0.06)
			#data.GetYaxis().SetTitle("Counts")
			data.GetYaxis().SetTitleSize(0.06)
			data.GetXaxis().SetTitleOffset(1.2)
			canvas.SetBottomMargin(0.15)
			canvas.SetTopMargin(0.01)
			canvas.SetLeftMargin(0.15)

			maxes = [data.GetMaximum(), stack.GetMaximum()]

			data.SetMaximum(factor*max(maxes))
			canvas.Draw()

			canvas.Print(outfolder+name+".png")
			canvas.Print(outfolder+name+".pdf")

			if ((not drawlegend) and notYetDrawn): 
				#components.reverse()
				canv = TCanvas("legendCanvas", "legenCanvas", 800, 200*len(hists))
				dummy = TCanvas("dummy", "dummy", 800, 600)
				#legend.AddEntry(data.GetPtr(), "data", "PE")
				for component, histo in hists.iteritems(): 
					#histo = frames[component][region].Histo1D(examplehist, variable)
					ROOT.SetOwnership(histo, 0)
					histo.SetLineStyle(1) # plain
					histo.SetLineWidth(2)
					color = Ana.samples.at(component.replace("Part", "")).color
					if "WS" in component: 
						color = Ana.samples.at("WS").color
					histo.SetLineColor(color)
					histo.SetFillStyle(1)
					histo.SetFillColor(color)
					legend.AddEntry(histo, Ana.samples.at(component.replace("Part", "")).legend, "F")
				canv.cd()
				data.SetMarkerSize(4.)
				data.SetLineWidth(4)
				legend.SetX1(0.)
				legend.SetY1(0.)
				legend.SetX2(1.)
				legend.SetY2(1.)
				legend.SetBorderSize(0)
  				legend.SetFillColor(0)
  				legend.SetFillStyle(0)
				legend.SetTextFont(43)
				legend.SetTextSize(canv.GetWh()/(2*stack.GetNhists()))
				legend.Draw()
				canv.Draw()
				canv.Print(outfolder+"legend.png")
				canv.Print(outfolder+"legend.pdf")
				notYetDrawn = False


def PlotComparison(frames, referencename, comparisonname, regions, variables, outfolder, normalise=False): 
	# Plotting distributions over each other 
	#outfolder+="comparisons/"
	os.system("mkdir -p "+outfolder)
	factor = 1.3 # how much overhead to add to the histos 
	for region in regions: 
		for variable in variables: 
			name = "{}_{}".format(variable, region)
			canvas = TCanvas(name, "{} in {}".format(variable, region), 800, 600)

			legend = TLegend(canvas.GetLeftMargin()+0.55, 
	                         	1.-canvas.GetTopMargin()-.11, 
	                            canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
	                           	1.-canvas.GetTopMargin() )

			examplehist = Ana.binning[variable]
			reference = frames[referencename][region].Histo1D(examplehist, variable)
			reference.SetTitle("{}_{}".format(variable, region))
			#reference.SetMarkerStyle(8) # Large scalable dot
			#reference.SetMarkerSize(0.5)
			reference.SetLineWidth(2)
			reference.SetLineColor(ROOT.kBlue)
			reference.SetFillStyle(3003)
			reference.SetFillColor(ROOT.kBlack)
			reference.SetTitle("{}_{}".format(variable, region))
			legend.AddEntry(reference.GetPtr(), referencename, "L")
			reference.Draw("HIST E")

			comparison = frames[comparisonname][region].Histo1D(examplehist, variable)
			comparison.SetLineWidth(2)
			comparison.SetLineColor(ROOT.kRed)
			comparison.SetFillStyle(3356)
			comparison.SetFillColor(ROOT.kRed)
			legend.AddEntry(comparison.GetPtr(), comparisonname, "L")
			if normalise: 
				if "data" in comparisonname: 
					comparison.Scale(dirtynorm[comparisonname][region])
				else: 
					comparison.Scale(dirtynorm[comparisonname][region]/comparison.Integral())
			else: 
				comparison.Scale(reference.Integral()/comparison.Integral())

			comparison.Draw("HIST SAME E") #"SAME"
			legend.Draw()
			legend.SetBorderSize(1)
			legend.SetMargin(0.3)
			legend.SetTextSize(0.04)

			maxes = [reference.GetMaximum(), comparison.GetMaximum()]

			reference.SetMaximum(factor*max(maxes))
			canvas.Draw()

			canvas.Print(outfolder+name+".png")
			canvas.Print(outfolder+name+".pdf")


def PlotFitResult(session, dataname, initialcomponents, variables, outfolder, drawlegend=False): 
	# Plotting distributions over each other 
	#outfolder+="stacked/"
	os.system("mkdir -p "+outfolder)
	name = "FitResult"
	factor = 1.1 # how much overhead to add to the histos 
	components = copy.deepcopy(initialcomponents)
	if (dataname in components): components.remove(dataname)
	components.reverse()
	notYetDrawn = True

	# Setting up plot 
	canvas = TCanvas("fitresult", "Fit result", 800, 600)

	legend = TLegend(canvas.GetLeftMargin()+0.35, 
	                         	1.-canvas.GetTopMargin()-.2, 
	                            canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
	                           	1.-canvas.GetTopMargin() )


	stack = THStack("stack", "Fit result")

	data = session.Get(dataname)

	data.SetMarkerStyle(8) # Large scalable dot
	data.SetMarkerSize(0.5)
	data.SetLineColor(ROOT.kBlack)
	data.SetTitle("") #data.SetTitle("{}_{}".format(variable, region))
	#data.SetFillColor(ROOT.kBlack)
	legend.AddEntry(data, "data", "PE")
	data.Draw("E")

	numcomponents = 0 #1
	for component in components: 
		print(component)
		#component = component.replace("Dist", "")
		histo = session.Get(component)
		#histo.Draw()
		#HoldUntilKeyPress()

		histo.SetLineStyle(1) # plain
		histo.SetLineWidth(2)
		color = Ana.samples.at(component.replace("Part", "")).color
		if (not color): 
			color = colors[0]
		histo.SetLineColor(color)
		#histo.SetMarkerColor(Ana.color[component])
		histo.SetFillStyle(1)
		histo.SetFillColor(color)
		histo.SetTitle("")
		#hists[component] = histo
		legend.AddEntry(histo, Ana.samples.at(component).legend, "F")

		stack.Add(histo)

		#ROOT.SetOwnership(histo, 0)
		numcomponents += 1

	stack.Draw("HIST SAME") #"SAME"
	data.Draw("E SAME") # Plot on top
	if (drawlegend): legend.Draw()
	legend.SetBorderSize(1)
	legend.SetMargin(0.3)
	legend.SetTextSize(0.04)

	#data.GetXaxis().SetTitle(Ana.labels[variable])
	data.GetXaxis().SetTitleSize(0.06)
	data.GetXaxis().SetLabelSize(0.06)
	data.GetYaxis().SetLabelSize(0.06)
	#data.GetYaxis().SetTitle("Counts")
	data.GetYaxis().SetTitleSize(0.06)
	data.GetXaxis().SetTitleOffset(1.2)
	canvas.SetBottomMargin(0.15)
	canvas.SetTopMargin(0.01)
	canvas.SetLeftMargin(0.15)
	canvas.Draw()

	maxes = [data.GetMaximum(), stack.GetMaximum()]

	print(maxes)

	data.SetMaximum(factor*max(maxes))
	canvas.Update()

	canvas.Print(outfolder+name+".png")
	canvas.Print(outfolder+name+".pdf")


	if ((not drawlegend) and notYetDrawn): 
		#components.reverse()
		canv = TCanvas("legendCanvas", "legenCanvas", 800, 200*numcomponents)
		dummy = TCanvas("dummy", "dummy", 800, 600)
		#legend.AddEntry(data, "data", "PE")
		# for component, histo in hists.iteritems(): 
		# 	#histo = frames[component][region].Histo1D(examplehist, variable)
		# 	ROOT.SetOwnership(histo, 0)
		# 	histo.SetLineStyle(1) # plain
		# 	histo.SetLineWidth(2)
		# 	color = Ana.samples.at(component.replace("Part", "")).color
		# 	if "WS" in component: 
		# 		color = Ana.samples.at("WS").color
		# 	histo.SetLineColor(color)
		# 	histo.SetFillStyle(1)
		# 	histo.SetFillColor(color)
		# 	legend.AddEntry(histo, Ana.samples.at(component.replace("Part", "")).legend, "F")
		canv.cd()
		data.SetMarkerSize(4.)
		data.SetLineWidth(4)
		legend.SetX1(0.)
		legend.SetY1(0.)
		legend.SetX2(1.)
		legend.SetY2(1.)
		legend.SetBorderSize(0)
		legend.SetFillColor(0)
		legend.SetFillStyle(0)
		legend.SetTextFont(43)
		legend.SetTextSize(canv.GetWh()/(2*stack.GetNhists()))
		legend.Draw()
		canv.Draw()
		canv.Print(outfolder+"legend.png")
		canv.Print(outfolder+"legend.pdf")
		notYetDrawn = False

