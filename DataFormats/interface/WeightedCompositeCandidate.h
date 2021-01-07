#ifndef FLASHgg_WeightedCompositeCandidate_h
#define FLASHgg_WeightedCompositeCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/GenJetExtra.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "flashgg/DataFormats/interface/GenLeptonExtra.h"
#include "flashgg/DataFormats/interface/Met.h"


namespace flashgg {
    class WeightedCompositeCandidate : public reco::CompositeCandidate, public WeightedObject {

    public:
        WeightedCompositeCandidate(std::string name = "") : reco::CompositeCandidate(), WeightedObject() {}
        WeightedCompositeCandidate(const Candidate& p, const std::string& name = "") :
            CompositeCandidate(p, name),
            WeightedObject()
            {}
        WeightedCompositeCandidate(const Candidate& p, const std::string& name, role_collection const& roles) :
            CompositeCandidate(p, name, roles),
            WeightedObject()
            {}
        virtual ~WeightedCompositeCandidate() override {};
        WeightedCompositeCandidate* clone() const override { return new WeightedCompositeCandidate(*this); };
    
        template<class T>
        void addDaughter(const edm::Ptr<T> cand);  
    };

    template<class T>
    void WeightedCompositeCandidate::addDaughter(const edm::Ptr<T> cand){
        CompositeCandidate::addDaughter(*cand);
        includeWeights(*cand);
    }

    template<> inline
    void WeightedCompositeCandidate::addDaughter(const edm::Ptr<flashgg::GenJetExtra> cand){
        CompositeCandidate::addDaughter(*cand);
    }
  
    template<> inline
    void WeightedCompositeCandidate::addDaughter(const edm::Ptr<reco::GenJet> cand){
        CompositeCandidate::addDaughter(*cand);
    }
  
    template<> inline
    void WeightedCompositeCandidate::addDaughter(const edm::Ptr<flashgg::GenLeptonExtra> cand){
        CompositeCandidate::addDaughter(*cand);
    }
}

#endif
