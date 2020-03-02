#include "flashgg/Taggers/interface/TTH_DNN_Helper.h"

namespace flashgg
{
    TTH_DNN_Helper::TTH_DNN_Helper(bool debug):
        debug_(debug), inputs_set_(false), preprocess_(false)
    {}

    void TTH_DNN_Helper::SetPreprocessingSchemes(std::vector<double> preprocess_scheme_global_features_mean, 
                                                 std::vector<double> preprocess_scheme_global_features_stddev, 
                                                 std::vector<double> preprocess_scheme_object_features_mean, 
                                                 std::vector<double> preprocess_scheme_object_features_stddev) 
    {

        preprocess_scheme_global_features_mean_ = preprocess_scheme_global_features_mean;
        preprocess_scheme_global_features_stddev_ = preprocess_scheme_global_features_stddev;
        preprocess_scheme_object_features_mean_ = preprocess_scheme_object_features_mean;
        preprocess_scheme_object_features_stddev_ = preprocess_scheme_object_features_stddev;

        if (debug_) {
            cout << "Global feature means: " << endl;
            for (unsigned int i = 0; i < preprocess_scheme_global_features_mean_.size(); i++)
                cout << i << ": " << preprocess_scheme_global_features_mean_[i] << endl;

            cout << "Global feature stddevs: " << endl;
            for (unsigned int i = 0; i < preprocess_scheme_global_features_stddev_.size(); i++)
                cout << i << ": " << preprocess_scheme_global_features_stddev_[i] << endl;

            cout << "Object feature means: " << endl;
            for (unsigned int i = 0; i < preprocess_scheme_object_features_mean_.size(); i++)
                cout << i << ": " << preprocess_scheme_object_features_mean_[i] << endl;

            cout << "Object feature stddevs: " << endl;
            for (unsigned int i = 0; i < preprocess_scheme_object_features_stddev_.size(); i++)
                cout << i << ": " << preprocess_scheme_object_features_stddev_[i] << endl;
        }

        preprocess_ = true;
    }

    void TTH_DNN_Helper::Preprocess() {
        if (global_features_.size() != preprocess_scheme_global_features_mean_.size() || global_features_.size() != preprocess_scheme_global_features_stddev_.size())
            throw "[DNN Helper] [ERROR]: Size of global features does not match size of preprocessing scheme";

        for (unsigned int i = 0; i < object_features_.size(); i++) {
            if (object_features_[i].size() != preprocess_scheme_object_features_mean_.size() || object_features_[i].size() != preprocess_scheme_object_features_stddev_.size())
                throw "[DNN Helper] [ERROR]: Size of object features does not match size of preprocessing scheme";
        }

        for (unsigned int i = 0; i < global_features_.size(); i++) {
            global_features_[i] += -(preprocess_scheme_global_features_mean_[i]);
            global_features_[i] *= 1./preprocess_scheme_global_features_stddev_[i];
        }
    
        for (unsigned int i = 0; i < object_features_.size(); i++) {
            for (unsigned int j = 0; j < object_features_[i].size(); j++) {
                if (object_features_[i][j] == pad_value)
                    continue;
                object_features_[i][j] += -(preprocess_scheme_object_features_mean_[j]);
                object_features_[i][j] *= 1./preprocess_scheme_object_features_stddev_[j];
            }
        } 
    }

    void TTH_DNN_Helper::SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence) {
        length_global_ = length_global;
        length_object_ = length_object;
        length_object_sequence_ = length_object_sequence;
    }

    void TTH_DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features) {
        if (global_features.size() != length_global_)
            throw "[DNN Helper] [ERROR]: Global features incorrectly configured!";
  
        // Global features
        global_features_ = global_features;

        // Object features //
        // First, set all values to the pad value
        object_features_.resize(length_object_sequence_);
        for (unsigned int i = 0; i < object_features_.size(); i++)
            object_features_[i].resize(length_object_);
        for (unsigned int i = 0; i < object_features_.size(); i++) {
            for (unsigned int j = 0; j < object_features_[i].size(); j++)
                object_features_[i][j] = pad_value; 
        }

        std::vector<std::vector<double>> object_features_unordered;
        object_features_unordered.resize(jets.size());
        for (unsigned int i = 0; i < object_features_unordered.size(); i++)
            object_features_unordered[i].resize(length_object_);

        for (unsigned int i = 0; i < object_features_unordered.size(); i++) {
            object_features_unordered[i][0] = log(jets[i]->pt());
            object_features_unordered[i][1] = jets[i]->eta();
            object_features_unordered[i][2] = jets[i]->phi();
            object_features_unordered[i][3] = log(jets[i]->energy());
            object_features_unordered[i][4] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probb");
            object_features_unordered[i][5] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probbb");
            object_features_unordered[i][6] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probc");
            object_features_unordered[i][7] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probudsg");
        }

        // Third, order jets by pT
        std::vector<std::pair<unsigned int, double> > pt_ordering;

        for (unsigned int i = 0; i < jets.size(); i++)
            pt_ordering.push_back(std::pair<unsigned int, double>(i, jets[i]->pt()));

        std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
        for (unsigned int i = 0; i < std::min(object_features_.size(), pt_ordering.size()); i++) {
            object_features_[i] = object_features_unordered[pt_ordering[i].first];
        }

        Preprocess();

        inputs_set_ = true;
    }

    void TTH_DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, 
                                   std::vector<edm::Ptr<flashgg::Muon>> muons, 
                                   std::vector<edm::Ptr<flashgg::Electron>> electrons, 
                                   std::vector<double> global_features) {
        if (global_features.size() != length_global_)
            throw "[DNN Helper] [ERROR]: Global features incorrectly configured!";

        // Global features
        global_features_ = global_features;

        // First, set all values to the pad value
        object_features_.resize(length_object_sequence_);
        for (unsigned int i = 0; i < object_features_.size(); i++)
            object_features_[i].resize(length_object_);
        for (unsigned int i = 0; i < object_features_.size(); i++) {
            for (unsigned int j = 0; j < object_features_[i].size(); j++)
                object_features_[i][j] = pad_value;
        }

        std::vector<std::vector<double>> object_features_unordered;
        object_features_unordered.resize(jets.size() + electrons.size() + muons.size());
        for (unsigned int i = 0; i < object_features_unordered.size(); i++)
            object_features_unordered[i].resize(length_object_);

        for (unsigned int i = 0; i < jets.size(); i++) {
            if (debug_)
                cout << "Filling jet " << i << endl;
            object_features_unordered[i][0] = log(jets[i]->pt());
            object_features_unordered[i][1] = jets[i]->eta();
            object_features_unordered[i][2] = jets[i]->phi();
            object_features_unordered[i][3] = log(jets[i]->energy());
            object_features_unordered[i][4] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probb");
            object_features_unordered[i][5] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probbb");
            object_features_unordered[i][6] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probc");
            object_features_unordered[i][7] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probudsg");
            object_features_unordered[i][8] = -2;
        }

        for (unsigned int i = jets.size(); i < jets.size() + electrons.size(); i++) {
            if (debug_)
                cout << "Filling electron " << i << endl;
            unsigned int j = i - jets.size();
            object_features_unordered[i][0] = log(electrons[j]->pt());
            object_features_unordered[i][1] = electrons[j]->eta();
            object_features_unordered[i][2] = electrons[j]->phi();
            object_features_unordered[i][3] = log(electrons[j]->energy());
            object_features_unordered[i][4] = -2; 
            object_features_unordered[i][5] = -2;
            object_features_unordered[i][6] = -2;
            object_features_unordered[i][7] = -2;
            object_features_unordered[i][8] = 1; // 1 for electrons
        }

        for (unsigned int i = jets.size() + electrons.size(); i < jets.size() + electrons.size() + muons.size(); i++) {
            if (debug_)
                cout << "Filling muon " << i << endl;
            unsigned int j = i - (jets.size() + electrons.size());
            object_features_unordered[i][0] = log(muons[j]->pt());
            object_features_unordered[i][1] = muons[j]->eta();
            object_features_unordered[i][2] = muons[j]->phi();
            object_features_unordered[i][3] = log(muons[j]->energy());
            object_features_unordered[i][4] = -2;
            object_features_unordered[i][5] = -2;
            object_features_unordered[i][6] = -2;
            object_features_unordered[i][7] = -2;
            object_features_unordered[i][8] = 0; // 0 for muons
        }

        if (debug_)
            cout << "Just finished filling objects, should have filled " << jets.size() + electrons.size() + muons.size() << " objects." << endl;

        // Third, order jets by pT
        std::vector<std::pair<unsigned int, double> > pt_ordering;

        for (unsigned int i = 0; i < object_features_unordered.size(); i++)
            pt_ordering.push_back(std::pair<unsigned int, double>(i, object_features_unordered[i][0]));

        std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
        for (unsigned int i = 0; i < std::min(object_features_.size(), pt_ordering.size()); i++) {
            object_features_[i] = object_features_unordered[pt_ordering[i].first];
        }

        Preprocess();

        inputs_set_ = true;
    }


    float TTH_DNN_Helper::EvaluateDNN(tensorflow::Session* session) {
        if (!inputs_set_)
            throw "[DNN Helper] [ERROR]: Inputs have not been reset since last evaluation!";

        tensorflow::Tensor global_input(tensorflow::DT_FLOAT, {1, length_global_});
        tensorflow::Tensor object_input(tensorflow::DT_FLOAT, tensorflow::TensorShape({1, length_object_sequence_, length_object_}));

        for (unsigned int i = 0; i < length_global_; i++) {
            global_input.matrix<float>()(0,i) = float(global_features_[i]);
            if (debug_)
                cout << "Global feature " << i << ": " << float(global_features_[i]) << endl;
        }

        for (unsigned int i = 0; i < length_object_sequence_; i++) {
            for (unsigned int j = 0; j < length_object_; j++) {
                object_input.tensor<float,3>()(0,i,j) = float(object_features_[i][j]);
                if (debug_)
	            cout << "Object feature " << i << ", " << j << ": " << float(object_features_[i][j]) << endl;
            } 
        }

        std::vector<tensorflow::Tensor> output;
 
        tensorflow::run(session, {{"input_objects", object_input}, {"input_global", global_input}}, {"output/Sigmoid"}, &output);

        global_features_.clear();
        object_features_.clear(); 
        inputs_set_ = false;

        return output[0].matrix<float>()(0,0);  
    }

    //---Miscellaneous helper functions for DNN
    double convert_tmva_to_prob(double score) {
        // Undo TMVA transformation
        double raw_score = -0.5 * log( (2 / (score + 1)) - 1);

        // Apply logistic (sigmoid) transformation
        double prob = 1 / (1 + exp(-raw_score));
        return prob;
    }

    double helicity(const TLorentzVector particle_1, const TLorentzVector particle_2) {
        TLorentzVector p1 = particle_1;
        TLorentzVector parent = particle_1 + particle_2;

        TVector3 boost_to_parent = -(parent.BoostVector());
        p1.Boost(boost_to_parent);

        TVector3 v1 = p1.Vect();
        TVector3 vParent = parent.Vect();

        double cos_theta_1 = (v1.Dot(vParent)) / (v1.Mag() * vParent.Mag());

        return abs(cos_theta_1);
    }

}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
