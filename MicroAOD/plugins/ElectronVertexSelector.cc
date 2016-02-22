#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

namespace flashgg {

    class ElectronVertexSelector : public VertexSelectorBase
    {
    public:
        ElectronVertexSelector( const edm::ParameterSet &conf ) :
            VertexSelectorBase( conf )
            {};

        edm::Ptr<reco::Vertex> select( const edm::Ptr<flashgg::Photon> &, const edm::Ptr<flashgg::Photon> &,
                                       const std::vector<edm::Ptr<reco::Vertex> > &,
                                       const VertexCandidateMap &,
                                       const std::vector<edm::Ptr<reco::Conversion> > &,
                                       const std::vector<edm::Ptr<reco::Conversion> > &,
                                       const math::XYZPoint &,
                                       bool
                                       //  const Parameters_Selector_Type&,
                                       //  const float&
                                     ) override;

        edm::Ptr<reco::Vertex> select( const edm::Ptr<flashgg::Photon> &, const edm::Ptr<pat::Jet> &,
                                      const std::vector<edm::Ptr<reco::Vertex> > &,
                                      const VertexCandidateMap &,
                                      const std::vector<edm::Ptr<reco::Conversion> > &,
                                      const std::vector<edm::Ptr<reco::Conversion> > &,
                                      const math::XYZPoint &,
                                       bool
                                      //  const Parameters_Selector_Type&,                                                                                                                         
                                      //  const float&                                                                                                                                             
                                      ) override;

        void writeInfoFromLastSelectionTo( flashgg::DiPhotonCandidate & ) override;
        void writeInfoFromLastSelectionTo( flashgg::PhotonJetCandidate & ) override;

    };

    edm::Ptr<reco::Vertex> ElectronVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
                                                           const edm::Ptr<flashgg::Photon> &g2,
                                                           const std::vector<edm::Ptr<reco::Vertex> > &vtxs,
                                                           const VertexCandidateMap &vertexCandidateMap,
                                                           const std::vector<edm::Ptr<reco::Conversion> > &convs,
                                                           const std::vector<edm::Ptr<reco::Conversion> > &convsSingleLeg,
                                                           const math::XYZPoint &beamSpot,
                                                           bool
            //						      const Parameters_Selector_Type& param,
            //                                                      const float& beamsig
        )
    {
        auto chosenVtx = vtxs[0];
        float eleVtxZ = -1000;
        if(g1->matchedElectron() && g2->matchedElectron() &&
           fabs(g1->getMatchedEleVtx().z()-g2->getMatchedEleVtx().z())<0.1)
            eleVtxZ = (g1->getMatchedEleVtx().z()+g2->getMatchedEleVtx().z())/2;

        if(eleVtxZ != -1000)
        {            
            float minDz=1000;
            for( auto& vtx : vtxs ) {
                float thisVtxDz = fabs(eleVtxZ - vtx->z());
                if( thisVtxDz < minDz ) {
                    minDz = thisVtxDz;
                    chosenVtx = vtx;
                }
            }
        }

        return chosenVtx;
    }

    edm::Ptr<reco::Vertex> ElectronVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
                                                           const edm::Ptr<pat::Jet> &g2,
                                                           const std::vector<edm::Ptr<reco::Vertex> > &vtxs,
                                                           const VertexCandidateMap &vertexCandidateMap,
                                                           const std::vector<edm::Ptr<reco::Conversion> > &convs,
            const std::vector<edm::Ptr<reco::Conversion> > &convsSingleLeg,
            const math::XYZPoint &beamSpot,
            bool
            //						      const Parameters_Selector_Type& param,
            //                                                      const float& beamsig
                                                       )
    {
        return vtxs[0];
    }

    void ElectronVertexSelector::writeInfoFromLastSelectionTo( flashgg::DiPhotonCandidate &dipho )
    {
    }

    void ElectronVertexSelector::writeInfoFromLastSelectionTo( flashgg::PhotonJetCandidate &phojet )
    {
        // No need to store anything if we're just taking the zeroth vertex
    }
}

DEFINE_EDM_PLUGIN( FlashggVertexSelectorFactory,
                   flashgg::ElectronVertexSelector,
                   "FlashggElectronVertexSelector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

