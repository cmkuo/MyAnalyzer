import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process('RECOES')

# import of standard configurations
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.GeometryExtended_cff")
process.load("Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cerr'),
    categories = cms.untracked.vstring('EcalDQM', 'fileAction'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string("WARNING"),
        noLineBreaks = cms.untracked.bool(True),
        noTimeStamps = cms.untracked.bool(True),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        fileAction = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
        )
    )
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('COSM72_DEC_V1::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        'file:/data4/cmkuo/testfiles/test.root'
    )                            
                            )

process.load("EventFilter.ESRawToDigi.esRawToDigi_cfi")
process.esRawToDigi.sourceTag = 'source'
process.esRawToDigi.debugMode = False

process.load('RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi')
process.ecalPreshowerRecHit.ESdigiCollection = cms.InputTag("esRawToDigi")

# track associator
process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")
from TrackingTools.TrackAssociator.default_cfi import *

process.coll = cms.EDAnalyzer("CollisionAnalyzer",
                              TrackAssociatorParameterBlock,
                              IsCosmic = cms.untracked.bool(True),
                              VtxLabel = cms.InputTag("offlinePrimaryVertices"),
                              GTDigiLabel = cms.InputTag("gtDigis"),
                              ESSimHits = cms.InputTag("g4SimHits:EcalHitsES"),
                              DigiLabel = cms.InputTag("esRawToDigi"),
                              RecHitLabel = cms.InputTag("ecalPreshowerRecHit:EcalRecHitsES"),
                              TrackLabel = cms.InputTag("generalTracks"),
                              EERecHitLabel = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
                              HERecHitLabel = cms.InputTag("hbhereco"),
                              endcapRawSuperClusterCollection = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
                              OutputFile = cms.untracked.string("collision.root"),
                              LookupTable = cms.untracked.FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat")
                              )

process.TFileService = cms.Service("TFileService", fileName = cms.string("collision.root"))

# Output definition
process.FEVT = cms.OutputModule("PoolOutputModule",
                                splitLevel = cms.untracked.int32(0),
                                #outputCommands = process.RECOEventContent.outputCommands,
                                fileName = cms.untracked.string("fevt.root")
                                )

process.esRawToDigi.sourceTag = cms.InputTag("rawDataCollector")

# Trigger requirements
import copy
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.leptonHLTFilter = copy.deepcopy(process.hltHighLevel)
process.leptonHLTFilter.throw = cms.bool(False)
process.leptonHLTFilter.HLTPaths = ['HLT_L1SingleMuOpen_v*']

process.p = cms.Path(
    process.esRawToDigi*
    #process.ecalPreshowerRecHit*
    process.leptonHLTFilter*
    process.coll)


