#ifndef FLASHgg_TMVAReaderContainer_h
#define FLASHgg_TMVAReaderContainer_h

#include <string>
#include <vector>
#include <unordered_map>

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "TMVA/Reader.h"

using namespace std;

namespace flashgg {

    class TMVAReaderContainer
    {

    public:
        //--ctors
        TMVAReaderContainer() {};
        TMVAReaderContainer(std::vector<edm::ParameterSet> tmva_configs);

        //---dtor
        ~TMVAReaderContainer();        

        //---methods
        void SetVariable(string mva, string variable, float value=0);
        double EvaluateMVA(string mva);

    private:
        std::unordered_map<std::string, TMVA::Reader*> readers_;
        std::unordered_map<std::string, std::vector<float> >  variables_;        
        std::unordered_map<std::string, std::unordered_map<std::string, unsigned int > >  var_indices_;
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
