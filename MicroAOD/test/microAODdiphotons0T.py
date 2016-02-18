import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'PLS170_V7AN1::All'
#process.GlobalTag.globaltag = 'auto:run2_mc'
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 5000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag, '76X_mcRun2_asymptotic_v12')

# Fix because auto:run2_mc points to MCRUN2_74_V9::All
current_gt = process.GlobalTag.globaltag.value()
if current_gt.count("::All"):
    new_gt = current_gt.replace("::All","")
    print 'Removing "::All" from GlobalTag by hand for condDBv2: was %s, now %s' % (current_gt,new_gt)
    process.GlobalTag.globaltag = new_gt


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")
process.RandomNumberGeneratorService.flashggRandomizedPhotons = cms.PSet(
          initialSeed = cms.untracked.uint32(16253245)
        )

### 76X ###

process.source = cms.Source("PoolSource",
                            inputCommands = cms.untracked.vstring(
                                "keep *",
                                "drop *_slimmedMETsNoHF_*_*"
                            ),
                            fileNames=cms.untracked.vstring(
                                #"/store/mc/RunIIFall15MiniAODv1/VBFHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/047F5248-5AAA-E511-8219-02163E017790.root"
                                #"/store/mc/RunIIFall15DR76/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/25nsFlat10to25TSG_76X_mcRun2_asymptotic_v12-v1/00000/0C6C1B51-9198-E511-B305-002590747D94.root"
                                #"/store/mc/RunIISpring15DR74/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/0TPU25nsData2015v1_magnetOffBS0T_74X_mcRun2_0T_v0-v2/00000/0E666F83-8FBD-E511-BCF9-02163E012FC9.root"
                                #"/store/data/Run2015D/DoubleEG/MINIAOD/16Dec2015-v2/00000/000298CD-87A6-E511-9E56-002590593878.root"
                                "/store/mc/RunIIFall15MiniAODv2/GJet_Pt-15to6000_TuneCUETP8M1_Flat_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_magnetOff_76X_mcRun2_asymptotic_v12-v1/00000/7247044E-8BB8-E511-90BC-002590DE6E52.root"
                                #"file:grav.root"
                                #"/store/mc/RunIIFall15MiniAODv2/RSGravToGG_kMpl-001_M-750_TuneCUEP8M1_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_0T_magnetOffBS0T_76X_mcRun2_0T_v1-v1/20000/143182C1-BACA-E511-934D-0CC47A78A3D8.root",
                                #"/store/mc/RunIIFall15MiniAODv1/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/005667A9-CBB8-E511-86C3-90B11C066D31.root"
                                #"/store/data/Run2015D/DoubleEG_0T/MINIAOD/27Jan2016-v2/70000/007648DA-5DC7-E511-A7FF-0025905C2CE4.root"
                            ))

########### 


process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")
from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
process.flashggDiPhotonsTrkCount = flashggDiPhotons.clone()
process.flashggDiPhotonsTrkCount.VertexSelectorName = "FlashggTracksMultiplicityVertexSelector"

# NEEDED FOR ANYTHING PRIOR TO reMiniAOD
#process.weightsCount.pileupInfo = "addPileupInfo"

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand
process.flashggPrunedGenParticles.select.extend(["keep status = 3",
                                                 "keep status = 22",
                                                 "keep status = 23",
                                                 "++keep pdgId = 22",
                                                 "keep++ pdgId = 39",
                                                 "keep++ pdgId = 5000039",
                                                 ])
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )

# All jets are now handled in MicroAODCustomize.py
# Switch from PFCHS to PUPPI with puppi=1 argument (both if puppi=2)

process.p = cms.Path(process.flashggMicroAODSequence+process.flashggDiPhotonsTrkCount)
process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
#process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
#                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning


from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)

if "DY" in customize.datasetName or "SingleElectron" in customize.datasetName or "DoubleEG" in customize.datasetName:
  customize.customizeHLT(process)
