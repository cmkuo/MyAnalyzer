import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Demo")

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.files = 'file:/lookarea_SM/PrivMinidaq.00119656.0001.A.storageManager.00.0000.dat'
options.output = 'test.root'
options.parseArguments()

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    options.files
    #'file:/data3/ncuhep/splash/digi/BeamSplash_1257738510.root'
    ),
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )

process.ecalPreshowerRecHit = cms.EDProducer("ESRecHitProducer",
                                             ESrechitCollection = cms.string('EcalRecHitsES'),
                                             ESdigiCollection = cms.InputTag("ecalPreshowerDigis"),
                                             algo = cms.string("ESRecHitWorker"),
                                             ESRecoAlgo = cms.int32(1),
                                             ESGain = cms.int32(2),
                                             ESBaseline = cms.int32(0),
                                             ESMIPADC = cms.double(55),
                                             ESMIPkeV = cms.double(81.08)
                                             )

process.splash = cms.EDAnalyzer("ESSplashAnalyzer",
                                #DigiLabel = cms.InputTag("esRawToDigi"),
                                DigiLabel = cms.InputTag("ecalPreshowerDigis"),
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

process.p1 = cms.Path(process.ecalPreshowerRecHit)
process.p2 = cms.Path(process.splash)

process.schedule = cms.Schedule(process.p1,process.p2)



