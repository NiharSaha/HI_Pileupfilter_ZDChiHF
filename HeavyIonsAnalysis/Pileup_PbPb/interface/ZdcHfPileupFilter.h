#ifndef HEAVYIONSANALYSIS_PILEUP_PBBPB_ZDCHFPLEUPFILTER_H
#define HEAVYIONSANALYSIS_PILEUP_PBBPB_ZDCHFPLEUPFILTER_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"


//
// Class Declaration
//

class ZdcHfPileupFilter : public edm::stream::EDFilter<> {
public:
  explicit ZdcHfPileupFilter(const edm::ParameterSet&);
  ~ZdcHfPileupFilter() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  bool filter(edm::Event&, const edm::EventSetup&) override;

  // ---------- Member Data ----------
  const edm::EDGetTokenT<reco::Centrality> centralityToken_;
  const edm::EDGetTokenT<pat::PackedCandidateCollection> pfCandidateToken_;

  const double zdcDivisor_;
  const double hihfDivisor_;
  const bool debug_;
};

#endif
