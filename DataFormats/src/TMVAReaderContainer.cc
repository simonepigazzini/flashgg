#include "flashgg/DataFormats/interface/TMVAReaderContainer.h"

namespace flashgg
{
    //---constructor
    TMVAReaderContainer::TMVAReaderContainer(std::vector<edm::ParameterSet> tmva_configs)
    {
        for(auto& config : tmva_configs)
        {
            auto mva_name = config.getParameter<std::string>("mva_name");
            auto var_names = config.getParameter<std::vector<std::string> >("mva_variables");
            variables_[mva_name] = std::vector<float>(var_names.size(), 0.);

            readers_[mva_name] = new TMVA::Reader( "!Color:!Silent" );
            unsigned int ivar = 0;
            for(auto& vname : var_names)
            {
                readers_[mva_name]->AddVariable(vname, &variables_[mva_name][ivar]);
                var_indices_[mva_name][vname] = ivar;
                ++ivar;
            }

            readers_[mva_name]->BookMVA("BDT", config.getParameter<edm::FileInPath>("weights_file").fullPath().c_str());  
        }
    }

    //---destructor
    TMVAReaderContainer::~TMVAReaderContainer()
    {}

    //---methods
    void TMVAReaderContainer::SetVariable(string mva, string variable, float value)
    {
        if(var_indices_.count(mva) == 1 && var_indices_.at(mva).count(variable) == 1)
        {
            auto& idx = var_indices_.at(mva).at(variable);
            variables_.at(mva).at(idx) = value;
        }
        else
            throw cms::Exception( "TMVAReaderContainer" ) << "Varible [" << variable << "] not found in TMVA::Reader " << mva << std::endl;
                                                           
    }

    double TMVAReaderContainer::EvaluateMVA(string mva)
    {
        if(readers_.count(mva) == 1)
            return readers_[mva]->EvaluateMVA("BDT");
        else
            throw cms::Exception( "TMVAReaderContainer" ) << "TMVA::Reader " << mva << "not found!" << std::endl;
    }
    
}
