#ifndef flashgg_DoubleHTag
#define flashgg_DoubleHTag

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

    class DoubleHTag: public DiPhotonTagBase, public reco::LeafCandidate
    {
    public:
        DoubleHTag();
        ~DoubleHTag();

        DoubleHTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );
        virtual DoubleHTag *clone() const override;
        /// DiPhotonTagBase::tag_t tagEnum() const override { return DiPhotonTagBase::kDoubleH; }

        void setMVA(double x) { mva_ = x; }
        double MVA() const { return mva_; }
      //  void setMVAprob(std::vector<float> &x) const { mva_prob_ = x; }
      //  std::vector<float> MVAprob() const { return mva_prob_; }
        void setMX(double x) { MX_ = x; }
        double MX() const { return MX_; }
        void setGenMhh(double x) { genMhh_ = x; }
        double genMhh() const { return genMhh_; }
        void setGenCosThetaStar_CS(double x) { genCosThetaStar_CS_ = x; }
        double genCosThetaStar_CS() const { return genCosThetaStar_CS_; }
        float ttHScore() const { return ttHScore_; }
        double diphotonPtOverM() const {return diPhoton()->pt()/mass(); }
        double dijetPtOverM() const {return dijet().pt()/mass(); }

        const flashgg::Jet & leadJet() const { return *leadJet_; } 
        const flashgg::Jet & subleadJet() const { return *subleadJet_; } 
        
        const LorentzVector & dijet() const { return dijet_; }
        const flashgg::Jet & VBFleadJet() const {return *VBFleadJet_;}
        const flashgg::Jet & VBFsubleadJet() const { return *VBFsubleadJet_;}
        const LorentzVector & diVBFjet() const {return diVBFjet_; }

        float getCosThetaStar_CS() const;
        float getCosThetaStar_CS_old(float ebeam) const;
        std::vector<float> CosThetaAngles() const;
        float HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const;
        float getPhoJetMinDr() const;
        float getPhoJetOtherDr() const;
        float getSigmaMDecorr() const;
        float getSigmaMOverMJets() const;
        void  setSigmaMDecorrTransf( DecorrTransform* transfEBEB, DecorrTransform* transfNotEBEB){ transfEBEB_= transfEBEB; transfNotEBEB_=transfNotEBEB;}
        LorentzVector getdiHiggsP4() const {return p4();}
        void setBenchmarkReweight(std::vector<float> x) { benchmark_reweights_ = x; }
        float getBenchmarkReweight(int targetNode) const { return benchmark_reweights_[targetNode]; }
        void setEventNumber(double x) { eventNumber_ = x; }
        double eventNumber() const { return eventNumber_; }

         
        //b quark information:   
        void setb1_pt( double x) {b1_pt_ = x;}
        double b1_pt() const {return b1_pt_;}
        void setb2_pt( double x) {b2_pt_ = x;}
        double b2_pt() const {return b2_pt_;}
        void setb1_eta( double x) {b1_eta_ = x;}
        double b1_eta() const {return b1_eta_;}
        void setb2_eta( double x) {b2_eta_ = x;}
        double b2_eta() const {return b2_eta_;}
        void setb1_phi( double x) {b1_phi_ = x;}
        double b1_phi() const {return b1_phi_;}
        void setb2_phi( double x) {b2_phi_ = x;}
        double b2_phi() const {return b2_phi_;}
        void setb1_pz( double x) {b1_pz_ = x;}
        double b1_pz() const {return b1_pz_;}
        void setb2_pz( double x) {b2_pz_ = x;}
        double b2_pz() const {return b2_pz_;}

        void setMbb_gen( double x) {Mbb_gen_ = x;}
        double Mbb_gen() const {return Mbb_gen_;}

        // Forward-backward information:
        void setq1_pt( double x) {q1_pt_ = x;}
        double q1_pt() const {return q1_pt_;}
        void setq2_pt( double x) {q2_pt_ = x;}
        double q2_pt() const {return q2_pt_;}
        void setq1_eta( double x) {q1_eta_ = x;}
        double q1_eta() const {return q1_eta_;}
        void setq2_eta( double x) {q2_eta_ = x;}
        double q2_eta() const {return q2_eta_;}
        void setq1_phi( double x) {q1_phi_ = x;}
        double q1_phi() const {return q1_phi_;}
        void setq2_phi( double x) {q2_phi_ = x;}
        double q2_phi() const {return q2_phi_;}
        void setq1_pz( double x) {q1_pz_ = x;}
        double q1_pz() const {return q1_pz_;}
        void setq2_pz( double x) {q2_pz_ = x;}
        double q2_pz() const {return q2_pz_;}
        void setq1_ID( int x) {q1_ID_ = x;}
        int q1_ID() const {return q1_ID_;}
        void setq2_ID( int x) {q2_ID_ = x;}
        int q2_ID() const {return q2_ID_;}
        void setMqq_gen( double x) {Mqq_gen_ = x;}
        double Mqq_gen() const {return Mqq_gen_;}

        float ttHScore_;
        float sumET_, MET_, phiMET_, dPhi1_, dPhi2_, PhoJetMinDr_,PhoJetOtherDr_, njets_, Xtt0_, Xtt1_, pte1_, pte2_, ptmu1_, ptmu2_, ptdipho_, etae1_, etae2_, etamu1_, etamu2_, etadipho_, phie1_, phie2_, phimu1_, phimu2_, phidipho_, fabs_CosThetaStar_CS_, fabs_CosTheta_bb_, mjj_, ptjet1_, ptjet2_, etajet1_, etajet2_, phijet1_, phijet2_; 
        float sumET() const {return sumET_;}
        float MET() const {return MET_;}
        float phiMET() const {return phiMET_;}
        float dPhi1() const {return dPhi1_;}
        float dPhi2() const {return dPhi2_;}
        float PhoJetMinDr() const {return PhoJetMinDr_;}
        float PhoJetOtherDr() const {return PhoJetOtherDr_;}
        float njets() const {return njets_;}
        float mjj() const {return mjj_;}
        float Xtt0() const {return Xtt0_;}
        float Xtt1() const {return Xtt1_;}
        float pte1() const {return pte1_;}
        float pte2() const {return pte2_;}
        float ptmu1() const {return ptmu1_;}
        float ptmu2() const {return ptmu2_;}
        float ptdipho() const {return ptdipho_;}
        float ptjet1() const {return ptjet1_;}
        float ptjet2() const {return ptjet2_;}
        float etajet1() const {return etajet1_;}
        float etajet2() const {return etajet2_;}
        float phijet1() const {return phijet1_;}
        float phijet2() const {return phijet2_;}
        float etae1() const {return etae1_;}
        float etae2() const {return etae2_;}
        float etamu1() const {return etamu1_;}
        float etamu2() const {return etamu2_;}
        float etadipho() const {return etadipho_;}
        float phie1() const {return phie1_;}
        float phie2() const {return phie2_;}
        float phimu1() const {return phimu1_;}
        float phimu2() const {return phimu2_;}
        float phidipho() const {return phidipho_;}
        float fabs_CosThetaStar_CS() const {return fabs_CosThetaStar_CS_;}
        float fabs_CosTheta_bb() const {return fabs_CosTheta_bb_;}
        //VBF study functions:
//        float VBFJet_mjj_;
        float getVBFJet_mjj() const ; 
        float getMinDeltaR_VBF_gamma() const;
        float getMinDeltaR_VBF_b() const;
        

//        void setisVBF(bool x) { isVBF_ = x; }
//        bool isVBF() const { return isVBF_; }

//        void setMVA(double x) { mva_ = x; }
//        double MVA() const { return mva_; }

    private:
        double mva_, MX_, genMhh_,genCosThetaStar_CS_, b1_pt_, b2_pt_, b1_eta_, b2_eta_, b1_phi_, b2_phi_, b1_pz_, b2_pz_, Mbb_gen_, q1_pt_, q2_pt_, q1_eta_, q2_eta_, q1_phi_, q2_phi_, q1_pz_, q2_pz_, Mqq_gen_;
        int q1_ID_, q2_ID_;
        vector<float> benchmark_reweights_;
 //       std::vector<float> mva_prob_;
         long eventNumber_;
        edm::Ptr<flashgg::Jet> leadJet_, subleadJet_;
        edm::Ptr<flashgg::Jet>  VBFleadJet_, VBFsubleadJet_ ;
        LorentzVector  diVBFjet_ ;
        LorentzVector dijet_;
        DecorrTransform* transfEBEB_;
        DecorrTransform* transfNotEBEB_;
        
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

