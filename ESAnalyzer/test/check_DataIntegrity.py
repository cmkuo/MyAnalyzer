import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

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

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        'file:/home/cmkuo/CMSSW/es_workingarea/CMSSW_7_3_2_patch4/src/MyAnalyzer/ESAnalyzer/test/FE382854-96CA-E411-8AEB-02163E011D8B.root'
    )
                            )

process.esRawToDigi = cms.EDProducer("ESRawToDigi",
                                   sourceTag = cms.InputTag("rawDataCollector"),
                                   InstanceES = cms.string(""),
                                   ESdigiCollection = cms.string(""),
                                   debugMode = cms.untracked.bool(False),
                                   LookupTable = cms.FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat")
                                   )

process.di = cms.EDAnalyzer("ESDIAnalyzer",
                            DigiLabel = cms.InputTag("esRawToDigi")
                            )

process.TFileService = cms.Service("TFileService", fileName = cms.string("dataintegrity.root"))

process.p1 = cms.Path(process.esRawToDigi)
process.p2 = cms.Path(process.di)

process.schedule = cms.Schedule(process.p1,process.p2)
