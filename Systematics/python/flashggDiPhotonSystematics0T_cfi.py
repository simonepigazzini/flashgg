import FWCore.ParameterSet.Config as cms

scaleBinsRereco = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
            cms.PSet( lowBounds = cms.vdouble(0.000) , upBounds = cms.vdouble(1.000) , values = cms.vdouble(1./1.0148   - 1.) ,  uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000) , upBounds = cms.vdouble(1.500) , values = cms.vdouble(1./1.0103   - 1.) ,  uncertainties = cms.vdouble( 0.00060 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500) , upBounds = cms.vdouble(2.000) , values = cms.vdouble(1./0.994006 - 1.) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000) , upBounds = cms.vdouble(6.000) , values = cms.vdouble(1./0.990905 - 1.) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
                    ))

smearBinsRereco = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
            cms.PSet( lowBounds = cms.vdouble(0.000 ) , upBounds = cms.vdouble(1.000) , values = cms.vdouble( 0.00788265 ) ,  uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 ) , upBounds = cms.vdouble(1.500) , values = cms.vdouble( 0.014427   ) ,  uncertainties = cms.vdouble( 0.00060 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 ) , upBounds = cms.vdouble(2.000) , values = cms.vdouble( 0.01977    ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 ) , upBounds = cms.vdouble(6.000) , values = cms.vdouble( 0.0235932  ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
                    ))


flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggDiPhotons"),
                SystMethods2D = cms.VPSet(),
                # the number of syst methods matches the number of nuisance parameters
                # assumed for a given systematic uncertainty and is NOT required
                # to match 1-to-1 the number of bins above.
                SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleEB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("abs(superCluster.eta)<1.5"),
                                                  BinList = scaleBinsRereco,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleEE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("abs(superCluster.eta)>=1.5"),
                                                  BinList = scaleBinsRereco,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearEB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("abs(superCluster.eta)<1.5"),
                                                  BinList = smearBinsRereco,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearEE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("abs(superCluster.eta)>=1.5"),
                                                  BinList = smearBinsRereco,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        )
                                            )


