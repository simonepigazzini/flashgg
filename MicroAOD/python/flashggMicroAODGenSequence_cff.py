import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggPrunedGenParticles_cfi import flashggPrunedGenParticles
from flashgg.MicroAOD.flashggGenElectrons_cfi import flashggGenElectrons
from flashgg.MicroAOD.flashggGenPhotons_cfi import flashggGenPhotons
from flashgg.MicroAOD.flashggGenPhotonsExtra_cfi import flashggGenPhotonsExtra


flashggMicroAODGenSequence = cms.Sequence(flashggPrunedGenParticles+flashggGenElectrons+flashggGenPhotons*flashggGenPhotonsExtra
                                        )
