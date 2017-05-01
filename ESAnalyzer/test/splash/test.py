import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Demo")

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
#options.files = '/store/caf/user/ccecal/TPG/splashes_239754_5events_April2015_MinimumBias.root'
#options.files = '/store/caf/user/ccecal/TPG/SplashLikeEvents_2015_run239821.root'
#options.files = 'root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/267/931/00000/362AE1DA-77F2-E511-A403-02163E013917.root'
#options.files = 'root://eoscms//eos/cms/store/caf/user/ccecal/TPG/splash_events_2016_run268006.root'
options.files = 'root://eoscms//store/caf/user/ccecal/TPG/splash2017_run_292901_beam_2_raw.root'
options.output = 'beamsplash_00292901_beam2.root'
options.parseArguments()

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

#process.source = cms.Source('NewEventStreamFileReader',
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    options.files
    )#,
#                            skipEvents=cms.untracked.uint32(10)
                            )

process.load("EventFilter.ESRawToDigi.esRawToDigi_cfi")
process.esRawToDigi.sourceTag = cms.InputTag("rawDataCollector")
process.esRawToDigi.debugMode = False

process.load('RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi')
process.ecalPreshowerRecHit.ESdigiCollection = cms.InputTag("esRawToDigi")

process.splash = cms.EDAnalyzer("ESSplashAnalyzer",
                                DigiLabel = cms.InputTag("esRawToDigi::Demo"),
                                RecHitLabel = cms.InputTag("ecalPreshowerRecHit:EcalRecHitsES:Demo"),
                                EERecHitLabel = cms.InputTag('ecalRecHit:EcalRecHitsEE'),
                                HERecHitLabel = cms.InputTag('hbhereco'),
                                OutputFile = cms.untracked.string(options.output),
                                HotChannelFile = cms.untracked.string("hotChannel.dat"),
                                LookupTable = cms.untracked.FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat"),
                                DumpTree = cms.untracked.bool(True),
                                NumberOfESHitsThreshold = cms.untracked.int32(0)
                                )

process.p = cms.Path(process.esRawToDigi*
                     process.ecalPreshowerRecHit*
                     process.splash)

#process.patOutputModule = cms.OutputModule("PoolOutputModule",
#                                           fileName = cms.untracked.string('output.root')
#                                           )
#process.out = cms.EndPath(process.patOutputModule)
