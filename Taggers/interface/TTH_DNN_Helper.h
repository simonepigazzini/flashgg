#ifndef _TTH_DNN_HELPER_H_
#define _TTH_DNN_HELPER_H_

#include <algorithm>

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/TensorFlowWrapperObj.h"
#include "flashgg/DataFormats/interface/TensorFlowWrapperRcd.h"

namespace flashgg {

    const int pad_value = -9;

    inline
    bool sortByValue(const std::pair<int,double>& pair1, const std::pair<int,double>& pair2 ) {
        return pair1.second > pair2.second;
    }

    class TTH_DNN_Helper
    {
    public:
        TTH_DNN_Helper(bool debug = false);
        ~TTH_DNN_Helper() {};

        void SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence);
        void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features);
        void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, 
                       std::vector<edm::Ptr<flashgg::Muon>> muons, 
                       std::vector<edm::Ptr<flashgg::Electron>> electrons, 
                       std::vector<double> global_features);

        void SetPreprocessingSchemes(std::vector<double> preprocess_scheme_global_features_mean, 
                                     std::vector<double> preprocess_scheme_global_features_stddev, 
                                     std::vector<double> preprocess_scheme_object_features_mean, 
                                     std::vector<double> preprocess_scheme_object_features_stddev);

        float EvaluateDNN(tensorflow::Session* session);

    private:
        void Preprocess();
        
        std::vector<double> 		        global_features_; // e.g. Met, N_jets, max b-tag score, etc.
        std::vector<std::vector<double>> 	object_features_; // pT ordered list of jets (and leptons)

        std::vector<double>                 preprocess_scheme_global_features_mean_;
        std::vector<double>                 preprocess_scheme_global_features_stddev_;
        std::vector<double>                 preprocess_scheme_object_features_mean_;
        std::vector<double>                 preprocess_scheme_object_features_stddev_;

        unsigned int length_global_; // number of global features
        unsigned int length_object_; // number of features per object
        unsigned int length_object_sequence_; // number of objects per sequence

        bool debug_;
        bool inputs_set_;
        bool preprocess_;
    };

    // Miscellaneous helper functions for DNN
    double convert_tmva_to_prob(double score);

    double helicity(const TLorentzVector particle_1, const TLorentzVector particle_2);

}

#endif  // _DNN_HELPER_H_

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
