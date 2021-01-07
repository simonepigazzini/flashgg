#include "flashgg/MicroAOD/interface/HadronicActivityProducer.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/GenDiPhoton.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/GenLeptonExtra.h"
#include "flashgg/DataFormats/interface/GenJetExtra.h"
#include "DataFormats/JetReco/interface/GenJet.h"



namespace flashgg { 

	struct DiPhotonVertexId { size_t operator()(flashgg::DiPhotonCandidate const & obj) { return obj.vertexIndex(); } };
	struct DiPhotonTagVertexId { size_t operator()(flashgg::DiPhotonTagBase const & obj) { return obj.diPhoton()->vertexIndex(); } };
	
}


typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonCandidate>, flashgg::Jet, flashgg::DiPhotonVertexId  > FlashggDiPhotonJetHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonCandidate>, flashgg::Electron, flashgg::DiPhotonVertexId  > FlashggDiPhotonElectronHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonCandidate>, flashgg::Muon, flashgg::DiPhotonVertexId  > FlashggDiPhotonMuonHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonCandidate>, flashgg::Met, flashgg::DiPhotonVertexId  > FlashggDiPhotonMetHadronicActivityProducer;
// typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonTagBase>,flashgg::DiPhotonTagVertexId > FlashggDiPhotonTagHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::GenDiPhoton>, flashgg::GenJetExtra> FlashggGenJetExtraHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::GenDiPhoton>, reco::GenJet> FlashggGenJetHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::GenDiPhoton>, flashgg::GenLeptonExtra> FlashggGenLeptonHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::GenDiPhoton>, flashgg::Met> FlashggGenMetHadronicActivityProducer;


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FlashggDiPhotonJetHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggDiPhotonElectronHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggDiPhotonMuonHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggDiPhotonMetHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggGenJetExtraHadronicActivityProducer);\
DEFINE_FWK_MODULE(FlashggGenJetHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggGenLeptonHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggGenMetHadronicActivityProducer);

