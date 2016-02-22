#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"


using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() {}

DiPhotonCandidate::~DiPhotonCandidate() {}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex )
{
    //    std::cout << " DiPhotonCandidate::DiPhotonCandidate " << std::endl;
    vertex_ = vertex;
    viewPho1_ = flashgg::SinglePhotonView( photon1, vertex );
    viewPho2_ = flashgg::SinglePhotonView( photon2, vertex );
    computeP4AndOrder();
}

bool DiPhotonCandidate::operator <( const DiPhotonCandidate &b ) const
{
    return ( sumPt() < b.sumPt() );
}

bool DiPhotonCandidate::operator >( const DiPhotonCandidate &b ) const
{
    return ( sumPt() > b.sumPt() );
}

void DiPhotonCandidate::computeP4AndOrder()
{
    //    std::cout << " START of DiPhotonCandidate::computeP4AndOrder PT M PT1 PT2 " << this->pt() << " " << this->mass() << " " << leadingPhoton()->pt() << " " <<
    //              subLeadingPhoton()->pt() << std::endl;
    if( leadingPhoton()->pt() < subLeadingPhoton()->pt() ) {
        std::swap( viewPho1_, viewPho2_ );
    }
    this->setP4( leadingPhoton()->p4() + subLeadingPhoton()->p4() );
    //    std::cout << " END of DiPhotonCandidate::computeP4AndOrder PT M PT1 PT2 " << this->pt() << " " << this->mass() << " " << leadingPhoton()->pt() << " " <<
    //              subLeadingPhoton()->pt() << std::endl;
}

/*
math::XYZTLorentzVector DiPhotonCandidate::PhoP4Corr( edm::Ptr<flashgg::Photon> photon1 ) const
{
    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vertex_->x();
    float vtx_Y = vertex_->y();
    float vtx_Z = vertex_->z();

    float sc_X = photon1->superCluster()->x();
    float sc_Y = photon1->superCluster()->y();
    float sc_Z = photon1->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( photon1->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), photon1->energy() );
    return corrected_p4;
}
*/


DiPhotonCandidate::LorentzVector DiPhotonCandidate::genP4() const
{
    DiPhotonCandidate::LorentzVector ret(0,0,0,0);
    if( leadingPhoton()->hasMatchedGenPhoton() && subLeadingPhoton()->hasMatchedGenPhoton() ) {
        ret = leadingPhoton()->matchedGenPhoton()->p4() + subLeadingPhoton()->matchedGenPhoton()->p4();
    }
    return ret;
}

void DiPhotonCandidate::computeVtxsExtras(const flashgg::VertexCandidateMap vtxcandmap, float coneSize)
{
    vtxsSumEt2Map_.clear();
    vtxsConesMap_.clear();
    
    for(auto& vtx : vVtxPtr_)
    {
        float sumEt2=0;
        vector<int> cones(3, 0);
        
        //---loop on the charged particles
        auto mapRange = std::equal_range( vtxcandmap.begin(), vtxcandmap.end(), vtx, flashgg::compare_with_vtx() );
        for( auto pair_iter = mapRange.first ; pair_iter != mapRange.second ; pair_iter++ )
        {
            edm::Ptr<pat::PackedCandidate> pfcand = pair_iter->second;

            //---check if charged hadron overlaps with one of the photons
            edm::RefVector<pat::PackedCandidateCollection> associatedLead =
                leadingPhoton()->associatedPackedPFCandidates();
            edm::RefVector<pat::PackedCandidateCollection> associatedSublead =
                subLeadingPhoton()->associatedPackedPFCandidates();
            //---keep associated electrons for Zee
            edm::Ptr<pat::PackedCandidate> leadEle;
            int nass = 0;
            for( auto& associated : associatedLead ) {
                edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated );
                if( associatedPtr == pfcand )
                {
                    if( abs(associatedPtr->pdgId()) == 11)
                        leadEle = pfcand;
                    nass++;
                }
            }
            edm::Ptr<pat::PackedCandidate> subleadEle=leadEle;
            for( auto& associated : associatedSublead ) {
                edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated );
                if( associatedPtr == pfcand )
                {
                    if( abs(associatedPtr->pdgId()) == 11)
                        subleadEle = pfcand;                        
                    nass++;
                }
            }
            if( nass > 0 )
                continue;
            
            //---BDT variables
            sumEt2 += pfcand->et2();
            
            //---fill cones
            float dR_dipho = deltaR( pfcand->momentum().Eta(), pfcand->momentum().Phi(),
                                     eta(), phi() );
            float dR_back = deltaR( pfcand->momentum().Eta(), pfcand->momentum().Phi(),
                                     -eta(), phi()+ROOT::Math::Pi() );
            if( dR_dipho > coneSize && dR_back > coneSize )
                ++cones[1];
            else if( dR_dipho < coneSize )
                ++cones[0];
            else
                ++cones[2];

        }

        //---fill the map        
        vtxsSumEt2Map_[vtx]=sumEt2;
        vtxsConesMap_[vtx]=cones;
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

