# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing


#----------------------------------------------------------------------------

# Setup Settings for ONIA SKIM:

isMC           = True      # if input is MONTECARLO: True or if it's DATA: False
muonSelection  = "Trk"  # Single muon selection: Glb(isGlobal), GlbTrk(isGlobal&&isTracker), Trk(isTracker) are availale

#----------------------------------------------------------------------------


# Print Onia Skim settings:
print( " " ) 
print( "[INFO] Settings used for ONIA SKIM: " )  
print( "[INFO] isMC          = " + ("True" if isMC else "False") )  
print( "[INFO] muonSelection = " + muonSelection )  
print( " " ) 

# set up process
process = cms.Process("Onia2MuMuPAT")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.inputFiles = '/store/user/anstahll/TriggerStudy2016/MC/JpsiMuMuPt040_Pythia6_RECO_20160730/JpsiPythia6Gun/JpsiMuMuPt040_Pythia6_RECO_20160730/160731_013405/0000/step4_JpsiMuMuPt040_Pythia6Gun_RAW2DIGI_L1Reco_RECO_1.root'
options.outputFile = 'onia2MuMuPAT_DATA_80X.root'

options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.categories.extend(["HiOnia2MuMuPAT_muonLessSizeORpvTrkSize"])
process.MessageLogger.cerr.HiOnia2MuMuPAT_muonLessSizeORpvTrkSize = cms.untracked.PSet( limit = cms.untracked.int32(5) )

# load the Geometry and Magnetic Field for the TransientTrackBuilder
process.load('Configuration.StandardSequences.Services_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.Reconstruction_cff')

# Global Tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_GRun', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

# HLT Dimuon Triggers
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
# HLT pPb TEST MENU:  /users/anstahll/PA2016/HIDileptonPA2016/V16
process.hltOniaHI.HLTPaths =  [
  "HLT_PAL1DoubleMu0_MassGT1_v1",
  "HLT_PAL1DoubleMuOpen_MassGT1_v1",
  "HLT_PAL1DoubleMuOpen_OS_v1",
  "HLT_PAL1DoubleMuOpen_SS_v1",
  "HLT_PAL1DoubleMuOpen_v1",
  "HLT_PAL1DoubleMu0_v1",
  "HLT_PAL1DoubleMu0_QGTE8_v1",
  "HLT_PAL1DoubleMu0_QGTE9_v1",
  "HLT_PAL1DoubleMu0_QGTE10_v1",
  "HLT_PAL1DoubleMu0_QGTE11_v1",
  "HLT_PAL1DoubleMu0_QGTE12_v1",
  "HLT_PAL1DoubleMu0_QGTE13_v1",
  "HLT_PAL1DoubleMu0_QGTE14_v1",
  "HLT_PAL1DoubleMu0_QGTE15_v1",
  "HLT_PAL1DoubleMu10_v1",
  "HLT_PAL1DoubleMu10_Mass60to150_v1",
  "HLT_PAL2DoubleMuOpen_v1",
  "HLT_PAL2DoubleMu10_v1",
  "HLT_PAL2DoubleMu10_Mass60to150_v1",
  "HLT_PAL3DoubleMuOpen_v1",
  "HLT_PAL3DoubleMuOpen_HIon_v1",
  "HLT_PAL3DoubleMu10_v1",
  "HLT_PAL3DoubleMu10_Mass60to150_v1",
  "HLT_PAL3DoubleMu10_HIon_v1",
  "HLT_PAL3DoubleMu10_Mass60to150_HIon_v1",
  "HLT_PA2013L2DoubleMu3_v1",
  "HLT_PAL1MuOpen_v1",
  "HLT_PAL1Mu0_NoBptxAND_v1",
  "HLT_PAL1Mu0_v1",
  "HLT_PAL1Mu12_NoBptxAND_v1",
  "HLT_PAL1Mu12_v1",
  "HLT_PAL1Mu15_v1",
  "HLT_PAL2Mu0_v1",
  "HLT_PAL2Mu12_v1",
  "HLT_PAL2Mu15_v1",
  "HLT_PAL3Mu0_HIon_v1",
  "HLT_PAL3Mu0_v1",
  "HLT_PAL3Mu3_v1",
  "HLT_PAL3Mu5_v1",
  "HLT_PAL3Mu7_v1",
  "HLT_PAL3Mu12_v1",
  "HLT_PAL3Mu15_v1",
  "HLT_PA2013Mu3_v1", 
  "HLT_PA2013Mu7_v1",
  "HLT_PA2013Mu12_v1"
  ]

process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","",HLTProName)

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=isMC, HLT=HLTProName, Filter=False, useL1Stage2=True)

### Temporal fix for the PAT Trigger prescale warnings.
process.patTriggerFull.l1GtReadoutRecordInputTag = cms.InputTag("gtDigis","","RECO")
process.patTriggerFull.l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis","","RECO")
process.patTriggerFull.l1tExtBlkInputTag = cms.InputTag("gtStage2Digis","","RECO")
###

##### Onia2MuMuPAT input collections/options
process.onia2MuMuPatGlbGlb.dimuonSelection          = cms.string("mass > 0")
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity   = True
process.onia2MuMuPatGlbGlb.srcTracks                = cms.InputTag("generalTracks")
process.onia2MuMuPatGlbGlb.primaryVertexTag         = cms.InputTag("offlinePrimaryVertices")
process.patMuonsWithoutTrigger.pvSrc                = cms.InputTag("offlinePrimaryVertices")
# Adding muonLessPV gives you lifetime values wrt. muonLessPV only
process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = True
if isMC:
  process.genMuons.src = "genParticles"
  process.onia2MuMuPatGlbGlb.genParticles = "genParticles"

##### Dimuon pair selection
commonP1 = "|| (innerTrack.isNonnull && genParticleRef(0).isNonnull)"
commonP2 = " && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"
if muonSelection == "Glb":
  highP = "isGlobalMuon"; # At least one muon must pass this selection
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "isGlobalMuon"; # BOTH muons must pass this selection
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
elif muonSelection == "GlbTrk":
  highP = "(isGlobalMuon && isTrackerMuon)";
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "(isGlobalMuon && isTrackerMuon)";
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
elif muonSelection == "Trk":
  highP = "isTrackerMuon";
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "isTrackerMuon";
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
else:
  print "ERROR: Incorrect muon selection " + muonSelection + " . Valid options are: Glb, Trk, GlbTrk"

##### Remove few paths for MC
if isMC:
  process.patMuonSequence.remove(process.hltOniaHI)


process.source.fileNames      = cms.untracked.vstring(options.inputFiles)        
process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.outOnia2MuMu.fileName = cms.untracked.string( options.outputFile )
process.e                     = cms.EndPath(process.outOnia2MuMu)
process.schedule              = cms.Schedule(process.Onia2MuMuPAT,process.e)

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
MassReplaceInputTag(process)
