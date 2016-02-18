#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

namespace flashgg {

    class TracksMultiplicityVertexSelector : public VertexSelectorBase
    {
    public:
        TracksMultiplicityVertexSelector( const edm::ParameterSet &conf ) :
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

    private:
        int bestVtx_;
        std::vector<edm::Ptr<reco::Vertex> > vtxs_;
        VertexCandidateMap vertexCandidateMap_;        
    };

    edm::Ptr<reco::Vertex> TracksMultiplicityVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
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
        vertexCandidateMap_ = vertexCandidateMap;
        vtxs_ = vtxs;
        
        int bestVtxSum=0;
        bestVtx_=0;
        for( unsigned int iVtx=0; iVtx<vtxs.size(); ++iVtx ) {
            int thisVtxSum=0;
            auto mapRange = std::equal_range( vertexCandidateMap.begin(), vertexCandidateMap.end(), vtxs[iVtx], flashgg::compare_with_vtx() );
            if( mapRange.first == mapRange.second )
                continue; // no entries for this vertex
            for( auto pair_iter = mapRange.first ; pair_iter != mapRange.second ; pair_iter++ ) {
                edm::Ptr<pat::PackedCandidate> pfcand = pair_iter->second;
                ++thisVtxSum;
            }
            if( thisVtxSum > bestVtxSum ) {
                bestVtxSum = thisVtxSum;
                bestVtx_ = iVtx;
            }
        }

        return vtxs[bestVtx_];
    }

    edm::Ptr<reco::Vertex> TracksMultiplicityVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
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
        vertexCandidateMap_ = vertexCandidateMap;
        vtxs_ = vtxs;
        
        int bestVtxSum=0;
        bestVtx_=0;
        for( unsigned int iVtx=0; iVtx<vtxs.size(); ++iVtx ) {
            int thisVtxSum=0;
            auto mapRange = std::equal_range( vertexCandidateMap.begin(), vertexCandidateMap.end(), vtxs[iVtx], flashgg::compare_with_vtx() );
            if( mapRange.first == mapRange.second )
                continue; // no entries for this vertex
            for( auto pair_iter = mapRange.first ; pair_iter != mapRange.second ; pair_iter++ ) {
                edm::Ptr<pat::PackedCandidate> pfcand = pair_iter->second;
                ++thisVtxSum;
            }
            if( thisVtxSum > bestVtxSum ) {
                bestVtxSum = thisVtxSum;
                bestVtx_ = iVtx;
            }
        }

        return vtxs[bestVtx_];
    }

    void TracksMultiplicityVertexSelector::writeInfoFromLastSelectionTo( flashgg::DiPhotonCandidate &dipho )
    {
        dipho.setVVtxPtr( vtxs_ );
        dipho.computeVtxsExtras( vertexCandidateMap_ );
    }

    void TracksMultiplicityVertexSelector::writeInfoFromLastSelectionTo( flashgg::PhotonJetCandidate &phojet )
    {
        // No need to store anything if we're just taking the zeroth vertex
    }
}

DEFINE_EDM_PLUGIN( FlashggVertexSelectorFactory,
                   flashgg::TracksMultiplicityVertexSelector,
                   "FlashggTracksMultiplicityVertexSelector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

