import FWCore.ParameterSet.Config as cms

flashggGenElectrons = cms.EDFilter("PackedGenParticleSelector",
                                    src = cms.InputTag("packedGenParticles"),
                                   cut = cms.string("abs(pdgId) == 11 && pt > 5.")
                                    )
