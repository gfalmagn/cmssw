

#include "EventFilter/Utilities/interface/MicroStateService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/DebugMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/GlobalMutex.h"


namespace evf{

  MicroStateService::MicroStateService(const edm::ParameterSet& iPS, 
				       edm::ActivityRegistry& reg)
  {
  
    reg.watchPostBeginJob(this,&MicroStateService::postBeginJob);
    reg.watchPostEndJob(this,&MicroStateService::postEndJob);
  
    reg.watchPreProcessEvent(this,&MicroStateService::preEventProcessing);
    reg.watchPostProcessEvent(this,&MicroStateService::postEventProcessing);
    reg.watchPreSource(this,&MicroStateService::preSource);
    reg.watchPostSource(this,&MicroStateService::postSource);
  
    reg.watchPreModule(this,&MicroStateService::preModule);
    reg.watchPostModule(this,&MicroStateService::postModule);
    microstate1_ = "BJ";
    microstate2_ = "INIT";

  }


  MicroStateService::~MicroStateService()
  {
  }

  void MicroStateService::postBeginJob()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "BJD";
  }

  void MicroStateService::postEndJob()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "EJ";
    microstate2_ = "done";
  }

  void MicroStateService::preEventProcessing(const edm::EventID& iID,
					     const edm::Timestamp& iTime)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "PRO";
  }

  void MicroStateService::postEventProcessing(const edm::Event& e, const edm::EventSetup&)
  {
  }
  void MicroStateService::preSource()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "IN";
  }

  void MicroStateService::postSource()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "IND";
  }

  void MicroStateService::preModule(const edm::ModuleDescription& desc)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = desc.moduleLabel_;
  }

  void MicroStateService::postModule(const edm::ModuleDescription& desc)
  {
  }
  
  std::string MicroStateService::getMicroState1()
  { 
	boost::mutex::scoped_lock sl(lock_);
	return microstate1_;
  }

  std::string MicroStateService::getMicroState2()
  { 
	boost::mutex::scoped_lock sl(lock_);
	return microstate2_;
  }

} //end namespace evf

