import FWCore.ParameterSet.Config as cms

zdcHfPileupFilter = cms.EDFilter('ZdcHfPileupFilter',
    centralitySrc = cms.InputTag("hiCentrality"),
    pfSrc = cms.InputTag("packedPFCandidates"),
    zdcDivisor = cms.double(700000.0),
    hihfDivisor = cms.double(9000.0),
    debug = cms.bool(False)
)
