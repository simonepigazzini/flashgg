#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/typelookup.h"

#include "flashgg/DataFormats/interface/TMVAReaderContainer.h"
#include "flashgg/DataFormats/interface/TMVAReaderContainerRcd.h"

namespace flashgg {
    class TMVAReaderContainerESProducer : public edm::ESProducer
    {
    public:
        TMVAReaderContainerESProducer( const edm::ParameterSet & );
        ~TMVAReaderContainerESProducer() override {};
        
        std::shared_ptr<TMVAReaderContainer> produce(const TMVAReaderContainerRcd & iRcd);

    private:
        TMVAReaderContainer readers_;

    };

    TMVAReaderContainerESProducer::TMVAReaderContainerESProducer( const edm::ParameterSet & pSet ):
        readers_(pSet.getParameter<vector<edm::ParameterSet> >("tmva_configs"))
    {
        setWhatProduced(this);
    }

    std::shared_ptr<TMVAReaderContainer> TMVAReaderContainerESProducer::produce( const TMVAReaderContainerRcd & iRcd)
    {
        return std::shared_ptr<TMVAReaderContainer>(&readers_);
    }
}

typedef flashgg::TMVAReaderContainerESProducer FlashggTMVAReaderContainerESProducer;
DEFINE_FWK_EVENTSETUP_MODULE(FlashggTMVAReaderContainerESProducer);

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
