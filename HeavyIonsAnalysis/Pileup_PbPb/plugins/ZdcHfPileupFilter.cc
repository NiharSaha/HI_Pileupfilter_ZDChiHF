/*
 * ZdcHfPileupFilter
 *
 * Description:
 * An EDFilter to reject non-hadronic and pileup events in heavy-ion collisions.
 * It uses the correlation between the energy in the Zero Degree Calorimeter (ZDC),
 * which is sensitive to spectator neutrons, and the Transeverse energy in the Hadronic Forward
 * (HF) calorimeter, which is sensitive to overall event activity.
 *
 * Logic:
 * The filter calculates a metric: (ZDC_Sum / zdcDivisor) + (HF_Sum_PF / hihfDivisor).
 * Events where this metric is greater than 1.0 are considered pileup or
 * non-hadronic (e.g., electromagnetic peripheral collisions) and are rejected.
 *
 * Authors: Nihar Saha (nihar.ranjan.saha@cern.ch)
 * Date:    October 1, 2025
 *
 */


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "HeavyIonsAnalysis/Pileup_PbPb/interface/ZdcHfPileupFilter.h"

ZdcHfPileupFilter::ZdcHfPileupFilter(const edm::ParameterSet& iConfig)
    : centralityToken_(consumes<reco::Centrality>(iConfig.getParameter<edm::InputTag>("centralitySrc"))),
      pfCandidateToken_(consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfSrc"))),
      zdcDivisor_(iConfig.getParameter<double>("zdcDivisor")),
      hihfDivisor_(iConfig.getParameter<double>("hihfDivisor")),
      debug_(iConfig.getParameter<bool>("debug")) {}

bool ZdcHfPileupFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  edm::Handle<reco::Centrality> centrality;
  iEvent.getByToken(centralityToken_, centrality);

  edm::Handle<pat::PackedCandidateCollection> pfCandidates;
  iEvent.getByToken(pfCandidateToken_, pfCandidates);

  //Calculate ZDC and HF sums ---
  const double zdcSumTotal = centrality->zdcSum();

  double hfSum_pf = 0.0;
  for (const auto& pfcand : *pfCandidates) {
    if (pfcand.pdgId() == 1 || pfcand.pdgId() == 2) { // Hadrons and EM particles
      const bool inHF = (pfcand.eta() > 3.0 && pfcand.eta() < 6.0) || (pfcand.eta() < -3.0 && pfcand.eta() > -6.0);
      if (inHF) {
        hfSum_pf += pfcand.et();
      }
    }
  }

  //Apply the pileup Cut 
  double pileupMetric = (zdcSumTotal / zdcDivisor_) + (hfSum_pf / hihfDivisor_);
  bool isPileup = pileupMetric > 1.0;

  if (debug_) {
    edm::LogInfo("ZdcHfPileupFilter") << "ZDC Sum (Total) = " << zdcSumTotal << ", "
                                     << "hiHF_pf = " << hfSum_pf << ", "
                                     << "Metric = " << pileupMetric
                                     << " | Reject? " << isPileup;
  }

  return !isPileup; 
}

void ZdcHfPileupFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("centralitySrc", edm::InputTag("hiCentrality"));
  desc.add<edm::InputTag>("pfSrc", edm::InputTag("packedPFCandidates"));
  desc.add<double>("zdcDivisor", 700000.0);
  desc.add<double>("hihfDivisor", 9000.0);
  desc.add<bool>("debug", false);
  descriptions.add("zdcHfPileupFilter", desc);
}

DEFINE_FWK_MODULE(ZdcHfPileupFilter);
