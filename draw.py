import os, copy
from ROOT import gStyle, TCanvas, TLegend, TH1F, gROOT, TFile, TColor
#from common.officialStyle import officialStyle
#from common.DisplayManager_postfit import DisplayManager
#from common.DataMCPlot import *


gROOT.SetBatch(True)
gStyle.SetOptStat(False)
#gROOT.SetBatch(False)
#officialStyle(gStyle)
gStyle.SetOptTitle(0)

def ensureDir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)


#shape_file = '/work/ytakahas/work/analysis/CMSSW_10_2_10/src/rJpsi/anal/dev/datacard_MUSF_blind/tau_rhomass_unrolled_coarse_new.root'
#shape_file = '/work/ytakahas/work/analysis/CMSSW_10_2_10/src/rJpsi/anal/dev/datacard_MUSF_blind/tau_rhomass_unrolled_new.root'
#file_shape = TFile(shape_file)
#data_sb = file_shape.Get('sb/data_obs')
#nbin = data_sb.GetXaxis().GetNbins()

# Color blind friendly palette
# https://colorbrewer2.org/?type=diverging&scheme=RdYlBu&n=7 
mycolors = [TColor(215,48,39), TColor(252,141,89), TColor(254,224,144), TColor(255,255,191), TColor(224,243,248), TColor(145,191,219), TColor(69,117,180)]
#[red, oragnge, dark yellow, yellow, pale blue, blue, darker blue] # TODO: make an enum for them
colors = []
for color in mycolors: 
    colors.append(color.GetNumber())



filename = 'fitDiagnosticsTest.root'

file = TFile(filename)

print file

process = {
    'data':{'name':'data'},
    'total_signal':{'name':'SignalOfficialMC50M_MVA'},
    'total_background':{'name':'total_background'}, 
    'comb_bkg':{'name':'bkg'},
    'part_bkg':{'name':'BkgDstarDsMultipleTau_MVA'},
}

#for ibin in range(1, nbin+1):
#    process['bg_bin' + str(ibin)] = {'name':'bg_bin' + str(ibin)}


ensureDir('plots/combine/')

#nbin = 20

for ftype in ['fit_s', 'fit_b']: #'prefit'
#for ftype in ['prefit', 'fit_b']:


    for cr in ['CR', 'SB']:
#    for cr in ['sr', 'sb']:
#    for cr in ['rJpsi_sr_1_2018']:

        hists = {}
    
        ymax = -1

        
        for ii, var in process.iteritems():
    
            hist = file.Get('shapes_' + ftype + '/' + cr + '/' + var['name'])
            
            print hist, 'shapes_' + ftype + '/' + cr + '/' + var['name']

            hist.SetFillStyle(0)
#            print file
            
            nbin = hist.GetXaxis().GetNbins()
#            print nbin
            
#            if var['name'].find('bin')!=-1:
#                hist.SetLineColor(2)
#                hist.SetLineStyle(2)

            if ii.find('signal')!=-1:
                hist.SetLineColor(2)
                #hist.SetLineStyle(2)
            elif ii.find('data')!=-1:
                hist.SetMarkerStyle(20)
                hist.SetMarkerSize(1)

            hist.SetLineWidth(3)
        
            hists[ii] = copy.deepcopy(hist)

            print hist.GetMaximum()

            if ymax < hist.GetMaximum()*1.1: ymax = hist.GetMaximum()*1.1

            
        canvas = TCanvas('canvas_' + ftype + '_' + cr)
#        canvas.SetLogy()

        frame = TH1F('frame_' + ftype + '_' + cr, 'fname_' + ftype + '_' + cr, nbin, 15., 100.)
        frame.GetXaxis().SetTitle('Tau rhomasses unrolled bin ID')
        frame.GetYaxis().SetTitle('Events')
  
#        if ftype.find('prefit')!=-1:
        frame.SetMaximum(ymax*2.)
        frame.SetMinimum(0.)
#        else:
#            frame.SetMaximum(ymax*8)
#            frame.SetMinimum(0.8)

        frame.Draw()

        hs = copy.deepcopy(hists['total_background'])
        hs.Add(copy.deepcopy(hists['total_signal']))
        hs.SetFillStyle(1)
        hs.SetFillColor(colors[0])
        hs.SetLineColor(colors[0])
        hs.Draw('hsame')

        hists['total_background'].SetFillStyle(1)
        hists['total_background'].SetFillColor(colors[2])
        hists['total_background'].SetLineColor(colors[2])
        hists['total_background'].Draw('hsame')
        
        hists['part_bkg'].SetFillStyle(1)
        hists['part_bkg'].SetFillColor(colors[6])
        hists['part_bkg'].SetLineColor(colors[6])
        hists['part_bkg'].Draw('hsame')
        
        #hists['total_signal'].SetLineColor(colors[0])
        hists['total_signal'].Draw('hsame')
        hists['data'].Draw('epzsame')

        canvas.RedrawAxis()                
        canvas.SaveAs('plots/combine/' + ftype + '_' + cr + '.gif')


    # Checking consistency of shapes 
    newcanvas = TCanvas("newcanvas", "newcanvas", 800, 600)
    legend = TLegend(  canvas.GetLeftMargin()+0.55, 
                                1-canvas.GetTopMargin()-.15, 
                                canvas.GetLeftMargin()+(1.-(canvas.GetLeftMargin()+canvas.GetRightMargin())),
                                1-canvas.GetTopMargin())
    histSB = file.Get("shapes_fit_s/SB/bkg").DrawCopy("HIST E")
    histCR = file.Get("shapes_fit_s/CR/bkg").DrawCopy("HIST E SAME")
    histCR.SetLineColor(3)
    histSB.SetLineColor(4)
    histCR.Scale(1./histCR.Integral())
    histSB.Scale(1./histSB.Integral())
    histCR.SetFillColor(3)
    histSB.SetFillColor(4)
    histCR.SetFillStyle(3003) #3003
    histSB.SetFillStyle(3356)
    legend.AddEntry(histCR, "CR", "L")
    legend.AddEntry(histSB, "SB", "L")
    legend.SetTextSize(0.04)
    legend.Draw()
    newcanvas.Draw()
    newcanvas.Print("plots/combine/ShapeDstarDs.pdf")
