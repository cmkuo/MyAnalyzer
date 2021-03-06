import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process('RECOES')

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.files = "file:/data4/cmkuo/Comissioning2015/run236254_ls0012_streamA_StorageManager.dat"
options.output = 'collision_00236254.root'
options.parseArguments()

# import of standard configurations
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.GeometryExtended_cff")
process.load("Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('COSM72_DEC_V1::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(250)
    )

# Input source
process.source = cms.Source("NewEventStreamFileReader",
                            fileNames = cms.untracked.vstring(
    options.files
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
                              VtxLabel = cms.InputTag("offlinePrimaryVertices"),
                              GTDigiLabel = cms.InputTag("gtDigis"),
                              ESSimHits = cms.InputTag("g4SimHits:EcalHitsES"),
                              DigiLabel = cms.InputTag("esRawToDigi"),
                              RecHitLabel = cms.InputTag("ecalPreshowerRecHit:EcalRecHitsES:RECOES"),
                              TrackLabel = cms.InputTag("generalTracks"),
                              EERecHitLabel = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
                              HERecHitLabel = cms.InputTag("hbhereco"),
                              endcapRawSuperClusterCollection = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
                              OutputFile = cms.untracked.string(options.output),
                              LookupTable = cms.untracked.FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat")
                              )

process.TFileService = cms.Service("TFileService", fileName = cms.string(options.output))

# Output definition
process.FEVT = cms.OutputModule("PoolOutputModule",
                                splitLevel = cms.untracked.int32(0),
                                #outputCommands = process.RECOEventContent.outputCommands,
                                fileName = cms.untracked.string("fevt.root")
                                )

process.esRawToDigi.sourceTag = cms.InputTag("rawDataCollector")

process.p = cms.Path(process.esRawToDigi*
                     process.ecalPreshowerRecHit*
                     process.coll)


