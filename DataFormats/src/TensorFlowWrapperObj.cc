#include "flashgg/DataFormats/interface/TensorFlowWrapperObj.h"

namespace flashgg
{
    //---constructor
    TensorFlowWrapperObj::TensorFlowWrapperObj(std::vector<edm::ParameterSet> nn_configs)
    {
        for(auto& config : nn_configs)
        {
            auto nn_name = config.getParameter<std::string>("nn_name");
            graphs_[nn_name] = tensorflow::loadGraphDef(config.getParameter<edm::FileInPath>("weights_file").fullPath().c_str());            
            sessions_[nn_name] = tensorflow::createSession(graphs_[nn_name]);  
        }
    }

    //---destructor
    TensorFlowWrapperObj::~TensorFlowWrapperObj()
    {
        for(auto& obj : sessions_)
        {
            tensorflow::closeSession(obj.second);
            delete graphs_[obj.first];
        }
    }

    //---methods
    tensorflow::Session* TensorFlowWrapperObj::GetSession(string nn_name) const
    {
        if(sessions_.count(nn_name) == 1)
            return sessions_.at(nn_name);
        else
            throw cms::Exception( "TensorFlowWrapperObj" ) << "TensorFlow session [" << nn_name << "] not found!" << std::endl 
                                                           << "Please define add the relevant weights files to the FlashggTensorFlowObjectESProducer";
    }
    
}
