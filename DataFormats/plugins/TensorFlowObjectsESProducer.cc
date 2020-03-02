#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/typelookup.h"

#include "flashgg/DataFormats/interface/TensorFlowWrapperObj.h"
#include "flashgg/DataFormats/interface/TensorFlowWrapperRcd.h"

namespace flashgg {
    class TensorFlowObjectESProducer : public edm::ESProducer
    {
    public:
        TensorFlowObjectESProducer( const edm::ParameterSet & );
        ~TensorFlowObjectESProducer() override {};
        
        std::shared_ptr<TensorFlowWrapperObj> produce(const TensorFlowWrapperRcd & iRcd);

    private:
        TensorFlowWrapperObj tfobj_;

    };

    TensorFlowObjectESProducer::TensorFlowObjectESProducer( const edm::ParameterSet & pSet ):
        tfobj_(pSet.getParameter<vector<edm::ParameterSet> >("nn_configs"))
    {
        setWhatProduced(this);
    }

    std::shared_ptr<TensorFlowWrapperObj> TensorFlowObjectESProducer::produce( const TensorFlowWrapperRcd & iRcd)
    {
        return std::shared_ptr<TensorFlowWrapperObj>(&tfobj_);
    }
}

typedef flashgg::TensorFlowObjectESProducer FlashggTensorFlowObjectESProducer;
DEFINE_FWK_EVENTSETUP_MODULE(FlashggTensorFlowObjectESProducer);

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
