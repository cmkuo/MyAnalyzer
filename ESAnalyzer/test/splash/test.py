import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Demo")

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# setup any defaults you want
options.files = '/store/caf/user/ccecal/TPG/splashes_239754_5events_April2015_MinimumBias.root'
options.output = 'beamsplash_00239754.root'
options.parseArguments()

process.load("FWCore.MessageLogger.MessageLogger_cfi")

#process.load("DQM.Integration.test.FrontierCondition_GT_cfi")
process.load("DQM.Integration.test.FrontierCondition_GT_Offline_cfi") 

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

#process.source = cms.Source('NewEventStreamFileReader',
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    options.files
        #'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/239/754/00000/30C3CEC6-6EDB-E411-A2E3-02163E0136CE.root',
        #'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/239/754/00000/4010520D-70DB-E411-9C96-02163E01367A.root',
        #'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/239/754/00000/E02FF3C0-6FDB-E411-9B8B-02163E0136CE.root'
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

