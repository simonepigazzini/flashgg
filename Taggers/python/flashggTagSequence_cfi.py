import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFDiPhoDiJetMVA
from flashgg.Taggers.flashggTags_cff import *
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter
from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection, setup_flashggDifferentialPhoIdInputsCorrection

def flashggPrepareTagSequence(process, options):
    setup_flashggDifferentialPhoIdInputsCorrection(process, options)
    flashggPreselectedDiPhotons.src = "flashggDifferentialPhoIdInputsCorrection"

    flashggTagSequence = cms.Sequence(flashggDifferentialPhoIdInputsCorrection
                                      * flashggPreselectedDiPhotons
                                      * flashggDiPhotonMVA
                                      * flashggUnpackedJets
                                      * flashggVBFMVA
                                      * flashggVBFDiPhoDiJetMVA
                                      * ( flashggUntagged
                                      #                                  *( flashggSigmaMoMpToMTag
                                          + flashggVBFTag
                                          + flashggTTHDiLeptonTag
                                          + flashggTTHLeptonicTag
					  + flashggTHQLeptonicTag
#                                     + flashggTTHHadronicTTag                                      
#                                     + flashggTTHHadronicLTag                                      
                                          + flashggTTHHadronicTag
                                      #############old VH tags##############
                                      #                  + flashggVHEtTag
                                      #                  + flashggVHLooseTag
                                      #                  + flashggVHTightTag
                                      ###########updated VH tags############
                                          + flashggVHMetTag
                                          + flashggWHLeptonicTag
                                          + flashggZHLeptonicTag
                                          + flashggVHLeptonicLooseTag
                                          + flashggVHHadronicTag
                                      )
                                      * flashggTagSorter
                                  )


    process.tf_dummy_source = cms.ESSource("EmptyESSource",
                                           recordName = cms.string("flashgg::TensorFlowWrapperRcd"),
                                           firstValid = cms.vuint32(1),
                                           iovIsRunNotTime = cms.bool(True)
                                       )

    process.FlashggTensorFlowContainer = cms.ESProducer("FlashggTensorFlowObjectESProducer",
                                                        nn_configs = cms.VPSet(
                                                            cms.PSet(
                                                                nn_name = cms.string("TTHDiphoDNN"),
                                                                weights_file = cms.FileInPath("flashgg/Taggers/data/Hadronic_ttHHadronic_ttH_vs_dipho_v3.10_8Oct2019_weights.pb")
                                                            ),
                                                            cms.PSet(
                                                                nn_name = cms.string("TTHttGGDNN"),
                                                                weights_file = cms.FileInPath("flashgg/Taggers/data/Hadronic_ttHHadronic_ttH_vs_ttGG_v3.10_8Oct2019_weights.pb")
                                                            ),
cms.PSet(
                                                                nn_name = cms.string("TTHLeptonicDNN"),
                                                                weights_file = cms.FileInPath("flashgg/Taggers/data/Leptonic_ttHLeptonic_ttH_vs_ttGG_v3.10_8Oct2019_weights.pb")
                                                            )
                                                        )
                                                    )

    return flashggTagSequence
