#!/usr/bin/env python
from __future__ import division, print_function

import CombineHarvester.CombineTools.ch as ch
from ROOT import TFile, TH1F, Double_t
import os
import sys

print(sys.argv)


file = "./workspaceFromExportHists.root" #TFile("workspacereproducer.root") workspacereproducerharvester.root

outdir = "fitmodel"

os.system("mkdir -p {}".format(outdir))

cb = ch.CombineHarvester()
cb.SetVerbosity(3)

sig_procs = ["Sig"] #['Sig']

#bkg_procs = ["B0toDstarDs", "B0toDstarDsstar", "B0toDstarD0K"]

categories = {
    'SR': [(1, 'SR')],
    'CR': [(2, 'CR')],
    'SB': [(3, 'SB')],
    'baseline': [(4, 'baseline')],
    'all': [(5, 'all')],
    }


channels = ['baseline'] # Channels for which to write datacards
prefix = ['fitFirst'] # also called analysis
era = ['2018']


for chn in channels:

    cb.AddObservations(['*'], prefix, era, [chn], categories[chn])

    cb.AddProcesses(sig_procs, prefix, era, [chn], sig_procs, categories[chn], True)

    #cb.AddProcesses(['*'], prefix, era, [chn], bkg_procs, categories[chn], False)

    

cb.cp().process(sig_procs).AddSyst(cb, 'CMS_lumi', 'lnN', ch.SystMap()(1.05))
print("CMS_lumi added.")



print('>> Extracting histograms from input root files...')

# for chn in channels:
#     cb.cp().channel([chn]).ExtractShapes(
#         '%s' % (file),
# #        '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
#         'w:$PROCESS', 'w:$PROCESS_$BIN_$SYSTEMATIC') #, '$BIN/$SYSTEMATIC' 'w:$PROCESS_$BIN_$SYSTEMATIC'

cb.cp().backgrounds().ExtractShapes(
    file, 
    "$BIN/$PROCESS", "");
cb.cp().signals().ExtractShapes(
    file, 
    "$BIN/$PROCESS", "");



print('>> Setting standardised bin names...')
ch.SetStandardBinNames(cb)
cb.PrintAll()

writer = ch.CardWriter('{}/$ANALYSIS_$CHANNEL_$BINID_$ERA_$MASS.txt'.format(outdir),
                       '{}/common/$ANALYSIS_$CHANNEL_$BINID_$ERA_$MASS.input.root'.format(outdir))

writer.SetVerbosity(1)

#outdir = 'output/sm_cards/LIMITS'

for chn in channels:  # plus a subdir per channel
    print('writing', chn, cb.cp().channel([chn]))
    writer.WriteCards(outdir, cb.cp().channel([chn]))


print('>> Done!')


outcard = outdir + '/ggtautau_3prong_1_2015_120.txt'

#os.system(command)

# overwrite extra rateParam
if os.path.isfile(outcard):

    f = open(outcard, 'a')
#    f.write(extraStr)
    f.write('* autoMCStats 0 1\n')
    f.close()

command = 'text2workspace.py ' + outcard + ' -o ' + outdir + '/workspace.root -m 120'
os.system(command)
