import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Demo")

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.files = 'file:/data4/cmkuo/Comissioning2015/run238546_ls0001_streamA_StorageManager.dat'
options.output = 'test.root'
options.parseArguments()

process.load("FWCore.MessageLogger.MessageLogger_cfi")

#process.load("DQM.Integration.test.FrontierCondition_GT_cfi")
process.load("DQM.Integration.test.FrontierCondition_GT_Offline_cfi") 

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(5)
        )

process.source = cms.Source('NewEventStreamFileReader',
#process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    options.files
    #'file:/data3/ncuhep/splash/digi/BeamSplash_1257738510.root'
    )
                            )

process.load("EventFilter.ESRawToDigi.esRawToDigi_cfi")
process.esRawToDigi.sourceTag = cms.InputTag("rawDataCollector")
process.esRawToDigi.debugMode = False

process.load('RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi')
process.ecalPreshowerRecHit.ESdigiCollection = cms.InputTag("esRawToDigi")

process.splash = cms.EDAnalyzer("ESSplashAnalyzer",
                                DigiLabel = cms.InputTag("esRawToDigi"),
                                RecHitLabel = cms.InputTag("ecalPreshowerRecHit:EcalRecHitsES:Demo"),
                                EERecHitLabel = cms.InputTag('ecalRecHit:EcalRecHitsEE'),
                                HERecHitLabel = cms.InputTag('hbhereco'),
                                OutputFile = cms.untracked.string(options.output),
                                HotChannelFile = cms.untracked.string("hotChannel.dat"),
                                LookupTable = cms.untracked.FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat"),
                                DumpTree = cms.untracked.bool(True)
                                )

#process.patOutputModule = cms.OutputModule("PoolOutputModule",
#                                           fileName = cms.untracked.string('output.root')
#                                           )
#process.out = cms.EndPath(process.patOutputModule)

process.p = cms.Path(process.esRawToDigi*
                     process.ecalPreshowerRecHit*
                     process.splash)

