import FWCore.ParameterSet.Config as cms

treeMaker = cms.EDAnalyzer("EdmToNtupleNoMask",
   verbosity = cms.untracked.int32(0),
   detIdVec = cms.vint32( 51001,51002,51011,51012 ),
   detNamesVec = cms.vstring( "det51001","det51002","det51011","det51012" ),
   tdcAddress = cms.string("0x030000FF"),
   hvAddress = cms.string("0x050000FF"),
   dutAngAddress = cms.string("0x040000FF")
)

