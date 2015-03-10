#include <memory>
#include <string>  
#include <iostream> 

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
//#include "Validation/RecoEgamma/interface/EgammaObjects.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h" 
#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "TTree.h"
#include "TRandom.h"

#include <fstream>
#include <vector>

using namespace edm;
using namespace reco;   
using namespace std;  

typedef std::map<DetId, EcalRecHit> RecHitsMap;
class ecalRecHitLess : public std::binary_function<EcalRecHit, EcalRecHit, bool> {
  
public:

  bool operator()(EcalRecHit x, EcalRecHit y) { 
    return (x.energy() > y.energy()); 
  }
};

class CollisionAnalyzer : public edm::EDAnalyzer {
  
public:
  
  explicit CollisionAnalyzer(const edm::ParameterSet&);
  ~CollisionAnalyzer();
  
private:
  
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  InputTag vtxlabel_;
  InputTag esSimHits_;
  InputTag gtdigilabel_;
  InputTag digilabel_;
  InputTag rechitlabel_;
  InputTag tracklabel_;
  InputTag eerechitlabel_;
  InputTag herechitlabel_;
  InputTag endcapRawSuperClusterCollection_;
  InputTag CaloTowerCollection_;
  bool IsCosmic_;
  Service<TFileService> fs;  
 
  TTree *tree_;
  
  vector<EEDetId> detIdEERecHits; 
  vector<EcalRecHit> EERecHits; 
  vector<EcalRecHit> seedsEndCap;
  vector<EEDetId> usedXtalsEndCap;

  TrackBase::TrackQuality trkQuality_;

  Int_t run;
  Int_t event;
  Int_t orbit;
  Int_t bx;
  Int_t lumis;
  Bool_t isData;
  Int_t bit0;
  Int_t bit34;
  Int_t bit40;
  Int_t bit41;
  Int_t bitBH;
  Int_t nVtx;
  Float_t vtx[100][3];
  Int_t vtxNTrk[100];
  Int_t vtxNDF[100];
  Float_t vtxD0[100];
  Int_t nSHES;
  Int_t shES_z[2000];
  Int_t shES_p[2000];
  Int_t shES_x[2000];
  Int_t shES_y[2000];
  Int_t shES_s[2000];
  Float_t shES_e[2000];
  Int_t nRH;
  Int_t rh_z[200000];
  Int_t rh_p[200000];
  Int_t rh_x[200000];
  Int_t rh_y[200000];
  Int_t rh_s[200000];
  Float_t rh_t[200000];
  Float_t rh_posx[200000];
  Float_t rh_posy[200000];
  Float_t rh_posz[200000];
  Float_t rh_eta[200000];
  Float_t rh_phi[200000];
  Int_t rh_ADC0[200000];
  Int_t rh_ADC1[200000];
  Int_t rh_ADC2[200000];
  Int_t rh_status[200000];
  Float_t RH[200000];
  Int_t nSC;
  Int_t nBC[500];
  Float_t scEn[500];
  Float_t scEta[500];
  Float_t scPhi[500];
  Float_t scRawEn[500];
  Float_t scESEn[500];
  Float_t scEtaWidth[500];
  Float_t scPhiWidth[500];
  Float_t scE1x5[500];
  Float_t scE2x5[500];
  Float_t scE3x3[500];
  Float_t scE5x5[500];
  Float_t scSigmaIEtaIEta[500];
  Float_t scR9[500];
  Float_t bcEta[500][10];
  Float_t bcPhi[500][10];
  Float_t bcX[500][10];
  Float_t bcY[500][10];
  Float_t bcEn[500][10];
  Int_t nBCRH[500][10];
  Int_t bcRHx[500][10][30];
  Int_t bcRHy[500][10][30];
  Int_t bcRHz[500][10][30];
  Int_t st1z[500][10]; 
  Int_t st1p[500][10]; 
  Int_t st1x[500][10]; 
  Int_t st1y[500][10]; 
  Int_t st1s[500][10]; 
  Int_t st2z[500][10]; 
  Int_t st2p[500][10]; 
  Int_t st2x[500][10]; 
  Int_t st2y[500][10]; 
  Int_t st2s[500][10];
  Int_t nEERH;
  Int_t eerh_z[10000];
  Int_t eerh_x[10000];
  Int_t eerh_y[10000];
  Int_t eerh_flag[10000];
  Float_t eerh_eta[10000];
  Float_t eerh_phi[10000];
  Float_t eerh_e[10000];
  Float_t eerh_t[10000];
  Int_t   eerh_st1z[10000]; 
  Int_t   eerh_st1p[10000]; 
  Int_t   eerh_st1x[10000]; 
  Int_t   eerh_st1y[10000]; 
  Int_t   eerh_st1s[10000]; 
  Int_t   eerh_st2z[10000]; 
  Int_t   eerh_st2p[10000]; 
  Int_t   eerh_st2x[10000]; 
  Int_t   eerh_st2y[10000]; 
  Int_t   eerh_st2s[10000];
  Float_t en_eep;
  Float_t en_eem;
  Float_t en_hep;
  Float_t en_hem;
  Int_t   nTrk;
  Int_t   nGoodTrk;
  Float_t trkPt[2000];
  Float_t trkEta[2000];
  Float_t trkPhi[2000];
  Float_t trkOuterPt[2000];
  Float_t trkOuterX[2000];
  Float_t trkOuterY[2000];
  Float_t trkOuterZ[2000];
  Float_t trkOuterPhi[2000];
  Float_t trkOuterEta[2000];
  Float_t trkInnerPt[2000];
  Float_t trkInnerX[2000];
  Float_t trkInnerY[2000];
  Float_t trkInnerZ[2000];
  Float_t trkInnerPhi[2000];
  Float_t trkInnerEta[2000];
  Float_t trkD0[2000];
  Float_t trkD0Error[2000];
  Float_t trkNormalizedChi2[2000];
  Int_t    trkNumOfValidHits[2000];
  Int_t    trkNumofValidStripHits[2000];
  Int_t    trkNumofValidStripTIBHits[2000];
  Int_t    trkNumofValidStripTIDHits[2000];
  Int_t    trkNumofValidStripTOBHits[2000];
  Int_t    trkNumofValidStripTECHits[2000];
  Int_t    trkNumofStripLayer[2000];       
  Int_t    trkNumofStripTIBLayer[2000];
  Int_t    trkNumofStripTIDLayer[2000];
  Int_t    trkNumofStripTOBLayer[2000];
  Int_t    trkNumofStripTECLayer[2000];
  Int_t    trkESz[2000][2];
  Int_t    trkESp[2000][2];
  Int_t    trkESx[2000][2];
  Int_t    trkESy[2000][2];
  Int_t    trkESs[2000][2];
  Int_t    trkEEz[2000];
  Int_t    trkEEx[2000];
  Int_t    trkEEy[2000];
  Float_t trkEEEta[2000];
  Float_t trkEEPhi[2000];
  Float_t trkCaloTowerEta[2000];
  Float_t trkCaloTowerPhi[2000];
  Int_t    trkHEz[2000];
  Float_t trkHEEta[2000];
  Float_t trkHEPhi[2000];

  Int_t    nMC;
  Int_t    mcPID[500];
  Float_t mcPt[500];
  Float_t mcMass[500];
  Float_t mcEta[500];
  Float_t mcPhi[500];
  Int_t    mcGMomPID[500];
  Int_t    mcMomPID[500];
  Float_t mcMomPt[500];
  Float_t mcMomMass[500];
  Float_t mcMomEta[500];
  Float_t mcMomPhi[500];

  Int_t    nMu;
  Float_t muPt[500];
  Float_t muEta[500];
  Float_t muPhi[500];
  Float_t muIsoTrk[500];
  Float_t muIsoEcal[500];
  Float_t muIsoHcal[500];
  Bool_t   muGlobal[500];
  Bool_t   muTracker[500];
  Bool_t   muStandAlone[500];
  Int_t    muESz[500][2];
  Int_t    muESp[500][2];
  Int_t    muESx[500][2];
  Int_t    muESy[500][2];
  Int_t    muESs[500][2];

  TrackDetectorAssociator trackAssociator_;
  TrackAssociatorParameters parameters_;
};

CollisionAnalyzer::CollisionAnalyzer(const edm::ParameterSet& ps) {

  vtxlabel_         = ps.getParameter<InputTag>("VtxLabel");
  esSimHits_        = ps.getParameter<InputTag>("ESSimHits");
  gtdigilabel_      = ps.getParameter<InputTag>("GTDigiLabel");
  digilabel_        = ps.getParameter<InputTag>("DigiLabel");
  rechitlabel_      = ps.getParameter<InputTag>("RecHitLabel");
  tracklabel_       = ps.getParameter<InputTag>("TrackLabel");
  eerechitlabel_    = ps.getParameter<InputTag>("EERecHitLabel");
  herechitlabel_    = ps.getParameter<InputTag>("HERecHitLabel");
  endcapRawSuperClusterCollection_ = ps.getParameter<edm::InputTag>("endcapRawSuperClusterCollection");
  IsCosmic_         = ps.getUntrackedParameter<bool>("IsCosmic", false);

  //map<string,double> providedParameters;
  //providedParameters.insert(std::make_pair("LogWeighted", true));
  //providedParameters.insert(std::make_pair("X0", 0.89));
  //providedParameters.insert(std::make_pair("T0_barl", 5.7));
  //providedParameters.insert(std::make_pair("T0_endc", 3.1));
  //providedParameters.insert(std::make_pair("T0_endcPresh", 1.2));
  //providedParameters.insert(std::make_pair("W0", 4.2));

  // TrackAssociator parameters
  edm::ParameterSet parameters = ps.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  edm::ConsumesCollector iC = consumesCollector();
  parameters_.loadParameters( parameters, iC );
  
  trackAssociator_.useDefaultPropagator();
  
  tree_ = fs->make<TTree>("tree", "tree");
  tree_->Branch("run", &run, "run/I");
  tree_->Branch("event", &event, "event/I");
  tree_->Branch("orbit", &orbit, "orbit/I");
  tree_->Branch("bx", &bx, "bx/I");
  tree_->Branch("lumis", &lumis, "lumis/I");
  tree_->Branch("isData", &isData, "isData/O");
  tree_->Branch("nVtx", &nVtx, "nVtx/I");
  tree_->Branch("vtx", vtx, "vtx[nVtx][3]/F");
  tree_->Branch("vtxNTrk", vtxNTrk, "vtxNTrk[nVtx]/I");
  tree_->Branch("vtxNDF", vtxNDF, "vtxNDF[nVtx]/I");
  tree_->Branch("vtxD0", vtxD0, "vtxD0[nVtx]/F");
  tree_->Branch("nSHES", &nSHES, "nSHES/I" );
  tree_->Branch("shES_z", shES_z, "shES_z[nSHES]/I");
  tree_->Branch("shES_p", shES_p, "shES_p[nSHES]/I");
  tree_->Branch("shES_x", shES_x, "shES_x[nSHES]/I");
  tree_->Branch("shES_y", shES_y, "shES_y[nSHES]/I");
  tree_->Branch("shES_s", shES_s, "shES_s[nSHES]/I");
  tree_->Branch("shES_e", shES_e, "shES_e[nSHES]/F");
  tree_->Branch("nRH", &nRH, "nRH/I");
  tree_->Branch("rh_z", rh_z, "rh_z[nRH]/I");
  tree_->Branch("rh_p", rh_p, "rh_p[nRH]/I");
  tree_->Branch("rh_x", rh_x, "rh_x[nRH]/I");
  tree_->Branch("rh_y", rh_y, "rh_y[nRH]/I");
  tree_->Branch("rh_s", rh_s, "rh_s[nRH]/I");
  tree_->Branch("rh_t", rh_t, "rh_t[nRH]/F");
  tree_->Branch("rh_posx", rh_posx, "rh_posx[nRH]/F");
  tree_->Branch("rh_posy", rh_posy, "rh_posy[nRH]/F");
  tree_->Branch("rh_posz", rh_posz, "rh_posz[nRH]/F");
  tree_->Branch("rh_eta", rh_eta, "rh_eta[nRH]/F");
  tree_->Branch("rh_phi", rh_phi, "rh_phi[nRH]/F");
  tree_->Branch("rh_ADC0", rh_ADC0, "rh_ADC0[nRH]/I");
  tree_->Branch("rh_ADC1", rh_ADC1, "rh_ADC1[nRH]/I");
  tree_->Branch("rh_ADC2", rh_ADC2, "rh_ADC2[nRH]/I");
  tree_->Branch("rh_status", rh_status, "rh_status[nRH]/I");
  tree_->Branch("RH", RH, "RH[nRH]/F");
  tree_->Branch("nSC", &nSC, "nSC/I");
  tree_->Branch("nBC", nBC, "nBC[nSC]/I");
  tree_->Branch("scEn", scEn, "scEn[nSC]/F");
  tree_->Branch("scEta", scEta, "scEta[nSC]/F");
  tree_->Branch("scPhi", scPhi, "scPhi[nSC]/F");
  tree_->Branch("scRawEn", scRawEn, "scRawEn[nSC]/F");
  tree_->Branch("scESEn", scESEn, "scESEn[nSC]/F");
  tree_->Branch("scEtaWidth", scEtaWidth, "scEtaWidth[nSC]/F");
  tree_->Branch("scPhiWidth", scPhiWidth, "scPhiWidth[nSC]/F");
  tree_->Branch("scE1x5", scE1x5, "scE1x5[nSC]/F");
  tree_->Branch("scE2x5", scE2x5, "scE2x5[nSC]/F");
  tree_->Branch("scE3x3", scE3x3, "scE3x3[nSC]/F");
  tree_->Branch("scE5x5", scE5x5, "scE5x5[nSC]/F");
  tree_->Branch("scSigmaIEtaIEta", scSigmaIEtaIEta, "scSigmaIEtaIEta[nSC]/F");
  tree_->Branch("scR9", scR9, "scR9[nSC]/F");
  tree_->Branch("bcEta", bcEta, "bcEta[nSC][10]/F");
  tree_->Branch("bcPhi", bcPhi, "bcPhi[nSC][10]/F");
  tree_->Branch("bcX", bcX, "bcX[nSC][10]/F");
  tree_->Branch("bcY", bcY, "bcY[nSC][10]/F");
  tree_->Branch("bcEn", bcEn, "bcEn[nSC][10]/F");
  tree_->Branch("st1z", st1z, "st1z[nSC][10]/I");
  tree_->Branch("st1p", st1p, "st1p[nSC][10]/I");
  tree_->Branch("st1x", st1x, "st1x[nSC][10]/I");
  tree_->Branch("st1y", st1y, "st1y[nSC][10]/I");
  tree_->Branch("st1s", st1s, "st1s[nSC][10]/I");
  tree_->Branch("st2z", st2z, "st2z[nSC][10]/I");
  tree_->Branch("st2p", st2p, "st2p[nSC][10]/I");
  tree_->Branch("st2x", st2x, "st2x[nSC][10]/I");
  tree_->Branch("st2y", st2y, "st2y[nSC][10]/I");
  tree_->Branch("st2s", st2s, "st2s[nSC][10]/I");
  tree_->Branch("nBCRH", nBCRH, "nBCRH[nSC][10]/I");
  tree_->Branch("bcRHz", bcRHz, "bcRHz[nSC][10][30]/I");
  tree_->Branch("bcRHx", bcRHx, "bcRHx[nSC][10][30]/I");
  tree_->Branch("bcRHy", bcRHy, "bcRHy[nSC][10][30]/I");
  /*
  tree_->Branch("nEERH", &nEERH, "nEERH/I");
  tree_->Branch("eerh_z", eerh_z, "eerh_z[nEERH]/I");
  tree_->Branch("eerh_x", eerh_x, "eerh_x[nEERH]/I");
  tree_->Branch("eerh_y", eerh_y, "eerh_y[nEERH]/I");
  tree_->Branch("eerh_flag", eerh_flag, "eerh_flag[nEERH]/I");
  tree_->Branch("eerh_eta", eerh_eta, "eerh_eta[nEERH]/F");
  tree_->Branch("eerh_phi", eerh_phi, "eerh_phi[nEERH]/F");
  tree_->Branch("eerh_e", eerh_e, "eerh_e[nEERH]/F");
  tree_->Branch("eerh_t", eerh_t, "eerh_t[nEERH]/F");
  tree_->Branch("eerh_st1z", eerh_st1z, "eerh_st1z[nEERH]/I");
  tree_->Branch("eerh_st1p", eerh_st1p, "eerh_st1p[nEERH]/I");
  tree_->Branch("eerh_st1x", eerh_st1x, "eerh_st1x[nEERH]/I");
  tree_->Branch("eerh_st1y", eerh_st1y, "eerh_st1y[nEERH]/I");
  tree_->Branch("eerh_st1s", eerh_st1s, "eerh_st1s[nEERH]/I");
  tree_->Branch("eerh_st2z", eerh_st2z, "eerh_st2z[nEERH]/I");
  tree_->Branch("eerh_st2p", eerh_st2p, "eerh_st2p[nEERH]/I");
  tree_->Branch("eerh_st2x", eerh_st2x, "eerh_st2x[nEERH]/I");
  tree_->Branch("eerh_st2y", eerh_st2y, "eerh_st2y[nEERH]/I");
  tree_->Branch("eerh_st2s", eerh_st2s, "eerh_st2s[nEERH]/I");
  */
  tree_->Branch("nTrk", &nTrk, "nTrk/I");
  tree_->Branch("nGoodTrk", &nGoodTrk, "nGoodTrk/I");
  tree_->Branch("trkPt", trkPt, "trkPt[nTrk]/F");
  tree_->Branch("trkEta", trkEta, "trkEta[nTrk]/F");
  tree_->Branch("trkPhi", trkPhi, "trkPhi[nTrk]/F");
  /*
  tree_->Branch("trkOuterPt", trkOuterPt, "trkOuterPt[nTrk]/F");
  tree_->Branch("trkOuterX", trkOuterX, "trkOuterX[nTrk]/F");
  tree_->Branch("trkOuterY", trkOuterY, "trkOuterY[nTrk]/F");
  tree_->Branch("trkOuterZ", trkOuterZ, "trkOuterZ[nTrk]/F");
  tree_->Branch("trkOuterPhi", trkOuterPhi, "trkOuterPhi[nTrk]/F");
  tree_->Branch("trkOuterEta", trkOuterEta, "trkOuterEta[nTrk]/F");
  tree_->Branch("trkInnerPt", trkInnerPt, "trkInnerPt[nTrk]/F");
  tree_->Branch("trkInnerX", trkInnerX, "trkInnerX[nTrk]/F");
  tree_->Branch("trkInnerY", trkInnerY, "trkInnerY[nTrk]/F");
  tree_->Branch("trkInnerZ", trkInnerZ, "trkInnerZ[nTrk]/F");
  tree_->Branch("trkInnerPhi", trkInnerPhi, "trkInnerPhi[nTrk]/F");
  tree_->Branch("trkInnerEta", trkInnerEta, "trkInnerEta[nTrk]/F");
  tree_->Branch("trkD0", trkD0, "trkD0[nTrk]/F");
  tree_->Branch("trkD0Error", trkD0Error, "trkD0Error[nTrk]/F");
  tree_->Branch("trkNormalizedChi2", trkNormalizedChi2, "trkNormalizedChi2[nTrk]/F");
  tree_->Branch("trkNumOfValidHits", trkNumOfValidHits, "trkNumOfValidHits[nTrk]/I");
  tree_->Branch("trkNumofValidStripHits", trkNumofValidStripHits, "trkNumofValidStripHits[nTrk]/I");
  tree_->Branch("trkNumofValidStripTIBHits", trkNumofValidStripTIBHits, "trkNumofValidStripTIBHits[nTrk]/I");
  tree_->Branch("trkNumofValidStripTIDHits", trkNumofValidStripTIDHits, "trkNumofValidStripTIDHits[nTrk]/I");
  tree_->Branch("trkNumofValidStripTOBHits", trkNumofValidStripTOBHits, "trkNumofValidStripTOBHits[nTrk]/I");
  tree_->Branch("trkNumofValidStripTECHits", trkNumofValidStripTECHits, "trkNumofValidStripTECHits[nTrk]/I");
  tree_->Branch("trkNumofStripLayer", trkNumofStripLayer, "trkNumofStripLayer[nTrk]/I");
  tree_->Branch("trkNumofStripTIBLayer", trkNumofStripTIBLayer, "trkNumofStripTIBLayer[nTrk]/I");
  tree_->Branch("trkNumofStripTIDLayer", trkNumofStripTIDLayer, "trkNumofStripTIDLayer[nTrk]/I");
  tree_->Branch("trkNumofStripTOBLayer", trkNumofStripTOBLayer, "trkNumofStripTOBLayer[nTrk]/I");
  tree_->Branch("trkNumofStripTECLayer", trkNumofStripTECLayer, "trkNumofStripTECLayer[nTrk]/I");
  */
  tree_->Branch("trkESz", trkESz, "trkESz[nTrk][2]/I");
  tree_->Branch("trkESp", trkESp, "trkESp[nTrk][2]/I");
  tree_->Branch("trkESx", trkESx, "trkESx[nTrk][2]/I");
  tree_->Branch("trkESy", trkESy, "trkESy[nTrk][2]/I");
  tree_->Branch("trkESs", trkESs, "trkESs[nTrk][2]/I");
  tree_->Branch("trkEEz", trkEEz, "trkEEz[nTrk]/I");
  //tree_->Branch("trkEEx", trkEEx, "trkEEx[nTrk]/I");
  //tree_->Branch("trkEEy", trkEEy, "trkEEy[nTrk]/I");
  //tree_->Branch("trkEEEta", trkEEEta, "trkEEEta[nTrk]/F");
  //tree_->Branch("trkEEPhi", trkEEPhi, "trkEEPhi[nTrk]/F");
  //tree_->Branch("trkCaloTowerEta", trkCaloTowerEta, "trkCaloTowerEta[nTrk]/F");
  //tree_->Branch("trkCaloTowerPhi", trkCaloTowerPhi, "trkCaloTowerPhi[nTrk]/F");
  //tree_->Branch("trkHEz", trkHEz, "trkHEz[nTrk]/I");
  //tree_->Branch("trkHEEta", trkHEEta, "trkHEEta[nTrk]/F");
  //tree_->Branch("trkHEPhi", trkHEPhi, "trkHEPhi[nTrk]/F");
  tree_->Branch("nMC", &nMC, "nMC/I");
  tree_->Branch("mcPID", mcPID, "mcPID[nMC]/I");
  tree_->Branch("mcPt", mcPt, "mcPt[nMC]/F");
  tree_->Branch("mcMass", mcMass, "mcMass[nMC]/F");
  tree_->Branch("mcEta", mcEta, "mcEta[nMC]/F");
  tree_->Branch("mcPhi", mcPhi, "mcPhi[nMC]/F");
  tree_->Branch("mcGMomPID", mcGMomPID, "mcGMomPID[nMC]/I");
  tree_->Branch("mcMomPID", mcMomPID, "mcMomPID[nMC]/I");
  tree_->Branch("mcMomPt", mcMomPt, "mcMomPt[nMC]/F");
  tree_->Branch("mcMomMass", mcMomMass, "mcMomMass[nMC]/F");
  tree_->Branch("mcMomEta", mcMomEta, "mcMomEta[nMC]/F");
  tree_->Branch("mcMomPhi", mcMomPhi, "mcMomPhi[nMC]/F");
  tree_->Branch("nMu", &nMu, "nMu/I");
  tree_->Branch("muPt", muPt, "muPt[nMu]/F");
  tree_->Branch("muEta", muEta, "muEta[nMu]/F");
  tree_->Branch("muPhi", muPhi, "muPhi[nMu]/F");
  tree_->Branch("muIsoTrk", muIsoTrk, "muIsoTrk[nMu]/F");
  tree_->Branch("muIsoEcal", muIsoEcal, "muIsoEcal[nMu]/F");
  tree_->Branch("muIsoHcal", muIsoHcal, "muIsoHcal[nMu]/F");
  tree_->Branch("muGlobal", muGlobal, "muGlobal[nMu]/O");
  tree_->Branch("muTracker", muTracker, "muTracker[nMu]/O");
  tree_->Branch("muStandAlone", muStandAlone, "muStandAlone[nMu]/O");
  tree_->Branch("muESz", muESz, "muESz[nMu][2]/I");
  tree_->Branch("muESp", muESp, "muESp[nMu][2]/I");
  tree_->Branch("muESx", muESx, "muESx[nMu][2]/I");
  tree_->Branch("muESy", muESy, "muESy[nMu][2]/I");
  tree_->Branch("muESs", muESs, "muESs[nMu][2]/I");

}

CollisionAnalyzer::~CollisionAnalyzer() {
}

void CollisionAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // Get the tracker and ECAL geometry and topology
  //ESHandle<TrackerGeometry> trackerHandle;
  //iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  //const TrackerGeometry* theTracker = trackerHandle.product();

  ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);

  const CaloSubdetectorGeometry *geometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  ESHandle<CaloTopology> theCaloTopology;
  iSetup.get<CaloTopologyRecord>().get(theCaloTopology);
  //const CaloTopology *topology = theCaloTopology.product();

  //const CaloSubdetectorTopology *topology_ee;
  //topology_ee = theCaloTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);

  run   = iEvent.id().run();
  event = iEvent.id().event();
  orbit = iEvent.orbitNumber();
  bx    = iEvent.bunchCrossing();
  lumis = iEvent.luminosityBlock();
  isData = iEvent.isRealData();

  // vertex
  nVtx = 0;
  Handle<VertexCollection> recVtxs;
  if (iEvent.getByLabel(vtxlabel_, recVtxs)) {
    
    for (unsigned int i=0; i<recVtxs->size(); ++i) 
      if (!((*recVtxs)[i].isFake())) {
	vtx[nVtx][0] = (*recVtxs)[i].x();
	vtx[nVtx][1] = (*recVtxs)[i].y();
	vtx[nVtx][2] = (*recVtxs)[i].z();
	vtxNTrk[nVtx] = (*recVtxs)[i].tracksSize();
	vtxNDF[nVtx] = (*recVtxs)[i].ndof();
	vtxD0[nVtx] = (*recVtxs)[i].position().rho(); 

	nVtx++;
      }
  }

  // ES digis and RecHits
  nRH = 0;
  Handle<ESDigiCollection> digis;
  Handle<ESRecHitCollection> ESRecHit;
  if ( iEvent.getByLabel(rechitlabel_, ESRecHit) ) {
    
    for (ESRecHitCollection::const_iterator hitItr = ESRecHit->begin(); hitItr != ESRecHit->end(); ++hitItr) {
      
      ESDetId rhid = ESDetId(hitItr->id());

      const CaloCellGeometry *esCell = geometry_p->getGeometry(rhid);
      GlobalPoint espoint = esCell->getPosition();
     
      rh_ADC0[nRH] = -1;
      rh_ADC1[nRH] = -1;
      rh_ADC2[nRH] = -1;
 
      if ( iEvent.getByLabel(digilabel_, digis) ) {
	
	for (ESDigiCollection::const_iterator digiItr = digis->begin(); digiItr != digis->end(); ++digiItr) {
	  
	  ESDataFrame dataframe = (*digiItr);
	  ESDetId id = dataframe.id();
	  
	  if (id == rhid) {
	    rh_ADC0[nRH] = dataframe.sample(0).adc();
	    rh_ADC1[nRH] = dataframe.sample(1).adc();
	    rh_ADC2[nRH] = dataframe.sample(2).adc();
	  }
	}
      }
      
      rh_posx[nRH] = espoint.x();
      rh_posy[nRH] = espoint.y();
      rh_posz[nRH] = espoint.z();
      rh_eta[nRH]  = espoint.eta();
      rh_phi[nRH]  = espoint.phi();
      
      rh_z[nRH] = rhid.zside();
      rh_p[nRH] = rhid.plane();
      rh_x[nRH] = rhid.six();
      rh_y[nRH] = rhid.siy();
      rh_s[nRH] = rhid.strip();
      
      rh_t[nRH] = hitItr->time();
      RH[nRH]   = hitItr->energy();
      rh_status[nRH] = hitItr->recoFlag();

      nRH++;
    }
  } 
  
  // Get MC particle's information
  nMC = 0;
  const Candidate *mom = 0;

  if (!isData) {
     Handle<GenParticleCollection> genParticles;
     iEvent.getByLabel("genParticles", genParticles);
     const GenParticleCollection *mcParticle = genParticles.product();
     
     for (GenParticleCollection::const_iterator aMC = mcParticle->begin(); aMC != mcParticle->end(); aMC++) {
       
       if (aMC->status() == 3) continue;
       if ((fabs(aMC->pdgId())==11 && aMC->status()==1) || (aMC->pdgId()==22 && aMC->status()==1) || (aMC->pdgId()==111 && aMC->status()==2) || (fabs(aMC->pdgId())==211 && aMC->status()==1)) {
	 
	 mcPID[nMC]  = aMC->pdgId();
	 mcPt[nMC]   = aMC->pt();
	 mcMass[nMC] = aMC->mass();
	 mcEta[nMC]  = aMC->eta();
	 mcPhi[nMC]  = aMC->phi();
	 mcGMomPID[nMC] = -999;
	 mcMomPID[nMC]  = -999;
	 mcMomPt[nMC]   = -999;
	 mcMomMass[nMC] = -999;
	 mcMomEta[nMC]  = -999;
	 mcMomPhi[nMC]  = -999;
	 
	 for (size_t j=0; j<aMC->numberOfMothers(); ++j) {
	   mom = aMC->mother(j);
	   mcMomPID[nMC] = mom->pdgId();
	   mcMomPt[nMC] = mom->pt();
	   mcMomMass[nMC] = mom->mass();
	   mcMomEta[nMC] = mom->eta();
	   mcMomPhi[nMC] = mom->phi();
	   if (mom->mother()) mcGMomPID[nMC] = mom->mother()->pdgId();
	 }
	 
	 nMC++;
       }
     }

     // read in sim tracks and vertex
     //Handle<SimTrackContainer> simTracks;
     //iEvent.getByType<SimTrackContainer>(simTracks);
     //Handle<SimVertexContainer> simVertices;
     //iEvent.getByType<SimVertexContainer>(simVertices);

     // store ES sim hits as well
     map<ESDetId, double> map_essimhit; 
     map<ESDetId, double>::const_iterator i_map_essimhit;
     Handle<PCaloHitContainer> EcalHitsES;
     iEvent.getByLabel(esSimHits_, EcalHitsES);

     vector<PCaloHit> theESCaloHits;
     if ( EcalHitsES.isValid() ) {
       theESCaloHits.insert(theESCaloHits.end(), EcalHitsES->begin(), EcalHitsES->end());
 
       for (vector<PCaloHit>::iterator isim = theESCaloHits.begin(); isim != theESCaloHits.end(); ++isim) {

	 ESDetId esshid (isim->id()) ;
	 map_essimhit[esshid] += isim->energy();

	 //Float_t tmpSHE = map_essimhit[esshid];
	 
	 //if (tmpSHE*1000000./81.08*55 > 2800) { 
	   
	   // SimTrackContainer::const_iterator simTrk = simTracks->begin();
	   //for( ; simTrk != simTracks->end(); ++simTrk) {
	   //if ((unsigned int) isim->geantTrackId() == (unsigned int) simTrk->trackId()) {
	   //if (fabs(simTrk->type()) == 2112) {
		 
	   //int vertexIndex = simTrk->vertIndex();
	   //SimVertex vertex(math::XYZVectorD(0.,0.,0.),0);
	   //if (vertexIndex >= 0) vertex = (*simVertices)[vertexIndex];
	   
	   //cout<<"sat : "<<esshid<<endl;
	   //cout<<"par : "<<isim->energy()<<" "<<isim->time()<<" "<<simTrk->type()<<" "<<simTrk->momentum().Pt()<<" "<<simTrk->momentum().P()<<" "<<vertex.position().x()<<" "<<vertex.position().y()<<" "<<vertex.position().z()<<endl;
	   //}
	   //}
	   //} 
	 //}
       }
     }  
  
     nSHES = 0;  
     for (i_map_essimhit = map_essimhit.begin(); i_map_essimhit != map_essimhit.end(); ++i_map_essimhit) {
       //cout<<i_map_essimhit->first<<" "<<i_map_essimhit->second<<endl;       
       shES_z[nSHES] = i_map_essimhit->first.zside();
       shES_p[nSHES] = i_map_essimhit->first.plane();
       shES_x[nSHES] = i_map_essimhit->first.six();
       shES_y[nSHES] = i_map_essimhit->first.siy();
       shES_s[nSHES] = i_map_essimhit->first.strip();
       shES_e[nSHES] = i_map_essimhit->second;
       nSHES++;
     }

  }

  tree_->Fill();
}

void CollisionAnalyzer::beginJob() {
}

void CollisionAnalyzer::endJob() {
}

DEFINE_FWK_MODULE(CollisionAnalyzer);
