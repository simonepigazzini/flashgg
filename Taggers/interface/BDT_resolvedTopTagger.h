#ifndef _TOPTAGGER_H_
#define _TOPTAGGER_H_

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

#include "DataFormats/Math/interface/LorentzVector.h"
#include <DataFormats/Math/interface/deltaR.h>

#include "flashgg/DataFormats/interface/TMVAReaderContainer.h"
#include "flashgg/DataFormats/interface/TMVAReaderContainerRcd.h"

namespace flashgg {

    typedef math::PtEtaPhiMLorentzVectorD BDT_rTT_ptvec;

    class BDT_rTT_Jet : public BDT_rTT_ptvec{
    public:
        BDT_rTT_Jet() : BDT_rTT_ptvec(0,0,0,0){};
        BDT_rTT_Jet(float pt,float eta, float phi, float mass, float deepcsv, float cvsl, float cvsb, float ptD, float axis1, int mult) // pass axis1 = -log(sqrt(...))
            : BDT_rTT_ptvec(pt,eta,phi,mass), _deepcsv(deepcsv), _cvsl(cvsl), _cvsb(cvsb), _ptD(ptD), _axis1(axis1), _mult(mult){};
        float _deepcsv = 0;
        float _cvsl = 0;
        float _cvsb = 0;
        float _ptD = 0;
        float _axis1 = 0;
        int _mult = 0;
        float deepcsv() const {return _deepcsv;}
        float cvsl() const {return _cvsl;}
        float cvsb() const {return _cvsb;}
        float ptD() const {return _ptD;}
        float axis1() const {return _axis1;}
        float mult() const {return _mult;}
    };

    class BDT_rTT_top {
    public:
        BDT_rTT_top(){};
        BDT_rTT_top(std::shared_ptr<BDT_rTT_Jet> _b, std::shared_ptr<BDT_rTT_Jet> _j2, std::shared_ptr<BDT_rTT_Jet> _j3){
            b = _b;

            if (_j2->pt() > _j3->pt()) { j2 = _j2; j3 = _j3; }
            else { j2 = _j3; j3 = _j2; }
            *p4w = *(dynamic_cast<BDT_rTT_ptvec*>(j2.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(j3.get()));
            *p4 = (b!=NULL) ? *p4w+*(dynamic_cast<BDT_rTT_ptvec*>(b.get())) : *p4w;

        }
        std::shared_ptr<BDT_rTT_ptvec> p4 = std::make_shared<BDT_rTT_ptvec>(0,0,0,0);
        std::shared_ptr<BDT_rTT_ptvec> p4w = std::make_shared<BDT_rTT_ptvec>(0,0,0,0);
        std::shared_ptr<BDT_rTT_Jet> b = nullptr;
        std::shared_ptr<BDT_rTT_Jet> j2 = nullptr;
        std::shared_ptr<BDT_rTT_Jet> j3 = nullptr;
        float score = -99;
        int j1idx = -99;
        int j2idx = -99;
        int j3idx = -99;
    };

    class BDT_resolvedTopTagger {
    public:
        BDT_resolvedTopTagger(std::string reader_name):
            reader_name_(reader_name)
            {};
        ~BDT_resolvedTopTagger(){
            clear();
        };
        void addJet(float pt,float eta, float phi, float mass, float deepcsv, float cvsl, float cvsb, float ptD, float axis1, int mult){
            jets.push_back(std::make_shared<BDT_rTT_Jet>(pt,eta,phi,mass,deepcsv,cvsl,cvsb,ptD,axis1,mult));
        };
        void clear();
        std::vector<float> EvalMVA(TMVAReaderContainer& reader);
        void setDebug(bool val){debug = val;};

    private:

        float EvalScore(TMVAReaderContainer& reader, const std::shared_ptr<BDT_rTT_top>);

        std::vector<std::shared_ptr<BDT_rTT_Jet>> jets;

        std::string reader_name_;

        bool debug = false;

    };

    inline
    void BDT_resolvedTopTagger::clear(){
        jets.clear();
    }

    inline
    std::vector<float> BDT_resolvedTopTagger::EvalMVA(TMVAReaderContainer& reader){

        int njets = jets.size();
        std::sort(jets.begin(),jets.end(),[](const std::shared_ptr<BDT_rTT_Jet> &a, const std::shared_ptr<BDT_rTT_Jet> &b){return a->deepcsv() > b->deepcsv();});

        std::vector<std::shared_ptr<BDT_rTT_top>> allcands;

        for (int i1=0; i1<njets-2; i1++) {
            for (int i2=i1+1; i2<njets-1; i2++){
                for (int i3=i2+1; i3<njets; i3++){
                    auto topcand = std::make_shared<BDT_rTT_top>(jets.at(i1),jets.at(i2),jets.at(i3));
                    if ((fabs(topcand->p4->mass()-175)>80)) continue;

                    topcand->score = EvalScore(reader, topcand);
                    topcand->j1idx = i1;
                    topcand->j2idx = i2;
                    topcand->j3idx = i3;
                    allcands.push_back(topcand);
                }
            }
        }

        if (debug) std::cout << "njets " << njets << std::endl;

        std::vector<float> output(16,-99);
        if (allcands.size()>0) {
            auto top = *std::min_element(allcands.begin(),allcands.end(),[](const std::shared_ptr<BDT_rTT_top> &a, const std::shared_ptr<BDT_rTT_top> &b){return a->score > b->score;});
            output.at(0) = top->score; // mvaValue
            output.at(1) = top->p4->pt(); // HadTop_pt
            output.at(2) = top->p4->eta(); // HadTop_eta
            output.at(3) = top->p4->phi(); // HadTop_phi
            output.at(4) = top->p4->mass(); // HadTop_mass
            output.at(5) = top->p4w->pt(); // W_fromHadTop_pt
            output.at(6) = top->p4w->eta(); // W_fromHadTop_eta
            output.at(7) = top->p4w->phi(); // W_fromHadTop_phi
            output.at(8) = top->p4w->mass(); // W_fromHadTop_mass
            output.at(9) = std::max(top->j2->deepcsv(),top->j3->deepcsv()); // W_fromHadTop_maxCSVjj
            output.at(10) = deltaR(top->j2->eta(),top->j2->phi(),top->j3->eta(),top->j3->phi()); // W_fromHadTop_dRjj
            output.at(11) = deltaR(top->b->eta(),top->b->phi(),top->p4w->eta(),top->p4w->phi()); // W_fromHadTop_dRb
            output.at(12) = top->b->deepcsv(); // b_fromHadTop_CSV
            output.at(13) = top->j1idx;
            output.at(14) = top->j2idx;
            output.at(15) = top->j3idx;
        }
        if (debug) std::cout << "returning " << output.at(0) << std::endl;
        return output;

    };

    inline
    float BDT_resolvedTopTagger::EvalScore(TMVAReaderContainer& reader, const std::shared_ptr<BDT_rTT_top> top){

        reader.SetVariable(reader_name_, "var_b_pt", top->b->pt());
        reader.SetVariable(reader_name_, "var_b_mass", top->b->mass());
        reader.SetVariable(reader_name_, "var_b_ptD", top->b->ptD());
        reader.SetVariable(reader_name_, "var_b_axis1", std::exp(-top->b->axis1())); // training uses definition of axis1 without -log
        reader.SetVariable(reader_name_, "var_b_mult", top->b->mult());
        reader.SetVariable(reader_name_, "var_b_csv", top->b->deepcsv());
        reader.SetVariable(reader_name_, "var_b_cvsb", top->b->cvsb());
        reader.SetVariable(reader_name_, "var_b_cvsl", top->b->cvsl());

        reader.SetVariable(reader_name_, "var_wj1_pt", top->j2->pt());
        reader.SetVariable(reader_name_, "var_wj1_mass", top->j2->mass());
        reader.SetVariable(reader_name_, "var_wj1_ptD", top->j2->ptD());
        reader.SetVariable(reader_name_, "var_wj1_axis1", std::exp(-top->j2->axis1())); // training uses definition of axis1 without -log
        reader.SetVariable(reader_name_, "var_wj1_mult", top->j2->mult());
        reader.SetVariable(reader_name_, "var_wj1_csv", top->j2->deepcsv());
        reader.SetVariable(reader_name_, "var_wj1_cvsb", top->j2->cvsb());
        reader.SetVariable(reader_name_, "var_wj1_cvsl", top->j2->cvsl());

        reader.SetVariable(reader_name_, "var_wj2_pt", top->j3->pt());
        reader.SetVariable(reader_name_, "var_wj2_mass", top->j3->mass());
        reader.SetVariable(reader_name_, "var_wj2_ptD", top->j3->ptD());
        reader.SetVariable(reader_name_, "var_wj2_axis1", std::exp(-top->j3->axis1())); // training uses definition of axis1 without -log
        reader.SetVariable(reader_name_, "var_wj2_mult", top->j3->mult());
        reader.SetVariable(reader_name_, "var_wj2_csv", top->j3->deepcsv());
        reader.SetVariable(reader_name_, "var_wj2_cvsb", top->j3->cvsb());
        reader.SetVariable(reader_name_, "var_wj2_cvsl", top->j3->cvsl());

        reader.SetVariable(reader_name_, "var_b_wj1_deltaR", deltaR(top->b->eta(),top->b->phi(),top->j2->eta(),top->j2->phi()));
        reader.SetVariable(reader_name_, "var_b_wj1_mass", (*(dynamic_cast<BDT_rTT_ptvec*>(top->b.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(top->j2.get()))).mass());
        reader.SetVariable(reader_name_, "var_b_wj2_deltaR", deltaR(top->b->eta(),top->b->phi(),top->j3->eta(),top->j3->phi()));
        reader.SetVariable(reader_name_, "var_b_wj2_mass", (*(dynamic_cast<BDT_rTT_ptvec*>(top->b.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(top->j3.get()))).mass());
        reader.SetVariable(reader_name_, "var_wcand_deltaR", deltaR(top->j2->eta(),top->j2->phi(),top->j3->eta(),top->j3->phi()));
        reader.SetVariable(reader_name_, "var_wcand_mass", top->p4w->mass());
        reader.SetVariable(reader_name_, "var_b_wcand_deltaR", deltaR(top->b->eta(),top->b->phi(),top->p4w->eta(),top->p4w->phi()));
        reader.SetVariable(reader_name_, "var_topcand_mass", top->p4->mass());

        float score = reader.EvaluateMVA(reader_name_);

        if (debug) {
            std::cout << score << std::endl;
        }

        return score;

    };

}

#endif // _TOPTAGGER_H_
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
