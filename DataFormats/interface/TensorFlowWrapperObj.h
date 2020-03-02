#ifndef FLASHgg_TensorFlowWrapperObj_h
#define FLASHgg_TensorFlowWrapperObj_h

#include <string>
#include <unordered_map>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

using namespace std;

namespace flashgg {

    class TensorFlowWrapperObj
    {

    public:
        //--ctors
        TensorFlowWrapperObj() {};
        TensorFlowWrapperObj(std::vector<edm::ParameterSet> nn_configs);

        //---dtor
        ~TensorFlowWrapperObj();        

        //---methods
        tensorflow::Session* GetSession(string nn_name) const;

    private:
        std::unordered_map<std::string, tensorflow::GraphDef*> graphs_;
        std::unordered_map<std::string, tensorflow::Session*>  sessions_;        
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
