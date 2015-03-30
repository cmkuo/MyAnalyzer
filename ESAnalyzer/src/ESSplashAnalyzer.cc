#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRawData/interface/ESDCCHeaderBlock.h"
#include "DataFormats/EcalRawData/interface/ESKCHIPBlock.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"

#include <fstream>

using namespace edm;
using namespace std;

class ESSplashAnalyzer : public edm::EDAnalyzer {

   public:

  explicit ESSplashAnalyzer(const edm::ParameterSet&);
  ~ESSplashAnalyzer();

   private:

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void DoCommonMode(double det_data[], double *cm);
  
  string outputFile_;
  string hotFile_;
  InputTag digilabel_;
  InputTag rechitlabel_;
  InputTag eerechitlabel_;
  InputTag herechitlabel_;
  bool dumpTree_;
  FileInPath lookup_;

  TFile *fout_;
  TTree *tree_;
  TH1F *hcm_[2][2][40][40];

  int nLines_, runNum_, evNum_, eCount_, runtype_, seqtype_, dac_, gain_, precision_;
  int firstDAC_, nDAC_, isPed_, vDAC_[5], side_, layer_, bx_, orbit_;
 
  int senZ_[4288], senP_[4288], senX_[4288], senY_[4288], fed_[4288];
  int ADC0_[2][2][40][40][32]; 
  int ADC1_[2][2][40][40][32]; 
  int ADC2_[2][2][40][40][32]; 
  int cADC0_[2][2][40][40][32]; 
  int cADC1_[2][2][40][40][32]; 
  int cADC2_[2][2][40][40][32]; 
  double RH_[2][2][40][40][32];
  double en_eep;
  double en_eem;
  double en_hep;
  double en_hem;

  int hotChannels_, hotZ[5000], hotP[5000], hotX[5000], hotY[5000], hotS[5000];
};

ESSplashAnalyzer::ESSplashAnalyzer(const edm::ParameterSet& ps) {
  
  outputFile_     = ps.getUntrackedParameter<string>("OutputFile");
  hotFile_        = ps.getUntrackedParameter<string>("HotChannelFile");
  digilabel_      = ps.getParameter<InputTag>("DigiLabel");
  rechitlabel_    = ps.getParameter<InputTag>("RecHitLabel");
  eerechitlabel_  = ps.getParameter<InputTag>("EERecHitLabel");
  herechitlabel_  = ps.getParameter<InputTag>("HERecHitLabel");
  lookup_         = ps.getUntrackedParameter<FileInPath>("LookupTable");
  dumpTree_       = ps.getUntrackedParameter<bool>("DumpTree", false);

  // read in hot channel map
  ifstream hotFile;
  hotFile.open(hotFile_.c_str());

  if( hotFile.is_open() ) {
    hotFile >> hotChannels_ ;
    for (int i=0; i<hotChannels_; ++i) {
      hotFile>> hotZ[i] >> hotP[i] >> hotX[i] >> hotY[i] >> hotS[i];
      //cout<<"hot : "<<hotZ[i]<<" "<<hotP[i]<<" "<<hotX[i]<<" "<<hotY[i]<<" "<<hotS[i]<<endl;
    }
  } else {
    hotChannels_ = 0;
  }

  // read in look-up table
  int iz, ip, ix, iy, fed, kchip, pace, bundle, fiber, optorx;
  ifstream file;

  file.open(lookup_.fullPath().c_str());
  if( file.is_open() ) {

    file >> nLines_;
    for (int i=0; i<nLines_; ++i) {

      file>> iz >> ip >> ix >> iy >> fed >> kchip >> pace >> bundle >> fiber >> optorx;
      senZ_[i] = iz;
      senP_[i] = ip;
      senX_[i] = ix;
      senY_[i] = iy;
      fed_[i]  = fed;
    }

  } else {
    cout<<"ESUnpackerV4::ESUnpackerV4 : Look up table file can not be found in "<<lookup_.fullPath().c_str()<<endl;
  }

  Char_t hname[300];
  // define histograms
  fout_ = TFile::Open(outputFile_.c_str(), "recreate");

  for (int i=0; i<nLines_; ++i) {

    iz = (senZ_[i]==1) ? 0:1;

    sprintf(hname, "CM correction factor Z %d P %d X %d Y %d", senZ_[i], senP_[i], senX_[i], senY_[i]);
    hcm_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1] = new TH1F(hname, hname, 160, -40, 40);

  }

  // by default set layer_ to 1, just in case it crashes when there is no DIGIs !
  layer_ = 1;

  if (dumpTree_) {
    tree_ = new TTree("EventTree", "Event data");
    tree_->Branch("run", &runNum_, "run/I");
    tree_->Branch("event", &evNum_, "event/I");
    tree_->Branch("orbit", &orbit_, "orbit/I");
    tree_->Branch("bx", &bx_, "bx/I");
    tree_->Branch("ADC0", ADC0_, "ADC0[2][2][40][40][32]/I");
    tree_->Branch("ADC1", ADC1_, "ADC1[2][2][40][40][32]/I");
    tree_->Branch("ADC2", ADC2_, "ADC2[2][2][40][40][32]/I");
    tree_->Branch("cADC0", cADC0_, "cADC0[2][2][40][40][32]/I");
    tree_->Branch("cADC1", cADC1_, "cADC1[2][2][40][40][32]/I");
    tree_->Branch("cADC2", cADC2_, "cADC2[2][2][40][40][32]/I");
    tree_->Branch("RH", RH_, "RH[2][2][40][40][32]/D");
    tree_->Branch("eep", &en_eep, "eep/D");
    tree_->Branch("eem", &en_eem, "eem/D");
    tree_->Branch("hep", &en_hep, "hep/D");
    tree_->Branch("hem", &en_hem, "hem/D");
  }

}

ESSplashAnalyzer::~ESSplashAnalyzer() {

  //Char_t dname[300];
  int iz = 0;
  fout_->cd();
  for (int i=0; i<nLines_; ++i) {

    iz = (senZ_[i]==1) ? 0:1;
    
    hcm_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1]->Write();

    if (dumpTree_) tree_->Write();
  }

  fout_->Close();

}

void ESSplashAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& iSetup) {

  runNum_ = e.id().run();
  evNum_ = e.id().event();
  orbit_ = e.orbitNumber();
  bx_ = e.bunchCrossing();
  eCount_++;
  /*
  Handle<ESRawDataCollection> dccs;
  try {
    e.getByLabel(digilabel_, dccs);
  } catch ( cms::Exception &e ) {
    LogDebug("") << "Error! can't get ES raw data collection !" << std::endl;
  }
  */
  Handle<ESDigiCollection> digis;
  try {
    e.getByLabel(digilabel_, digis);
  } catch ( cms::Exception &e ) {
    LogDebug("") << "Error! can't get digi collection !" << std::endl;
  }

  //for (ESRawDataCollection::const_iterator dccItr = dccs->begin(); dccItr != dccs->end(); ++dccItr) {
  //ESDCCHeaderBlock dcc = (*dccItr);
  //bx_    = dcc.getBX();
  //orbit_ = dcc.getOrbitNumber();
  //}
  
  // Digis
  //Need for storing original data
  int data_S0[2][2][40][40][32];
  int data_S1[2][2][40][40][32];
  int data_S2[2][2][40][40][32];
  //int dataCM_S0[2][2][40][40][32];
  //int dataCM_S1[2][2][40][40][32];
  //int dataCM_S2[2][2][40][40][32];
  for (int j=0; j<2; ++j)
    for (int k=0; k<2; ++k)
      for (int m=0; m<40; ++m) 
        for (int n=0; n<40; ++n)
          for (int o=0; o<32; ++o){
            data_S0[j][k][m][n][o] = 0;
            data_S1[j][k][m][n][o] = 0;
            data_S2[j][k][m][n][o] = 0;
            //dataCM_S0[j][k][m][n][o] = 0;
            //dataCM_S1[j][k][m][n][o] = 0;
            //dataCM_S2[j][k][m][n][o] = 0;
	    ADC0_[j][k][m][n][o] = -999999;
	    ADC1_[j][k][m][n][o] = -999999;
	    ADC2_[j][k][m][n][o] = -999999;
	    cADC0_[j][k][m][n][o] = -999999;
	    cADC1_[j][k][m][n][o] = -999999;
	    cADC2_[j][k][m][n][o] = -999999;
	    RH_[j][k][m][n][o] = -999999;
          }

  en_eep = 0;
  en_eem = 0;
  en_hep = 0;
  en_hem = 0;

  int zside, plane, ix, iy, strip, iz;
  for (ESDigiCollection::const_iterator digiItr = digis->begin(); digiItr != digis->end(); ++digiItr) {
    
    ESDataFrame dataframe = (*digiItr);
    ESDetId id = dataframe.id();

    zside = id.zside();
    plane = id.plane();
    ix    = id.six();
    iy    = id.siy();
    strip = id.strip();
    iz = (zside==1) ? 0:1;

    side_  = zside;
    layer_ = plane;

    ADC0_[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(0).adc();
    ADC1_[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(1).adc();
    ADC2_[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(2).adc();

    data_S0[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(0).adc();    //storing S0 data
    data_S1[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(1).adc();    //storing S1 data
    data_S2[iz][plane-1][ix-1][iy-1][strip-1] = dataframe.sample(2).adc();    //storing S2 data

    //cout<<iz<<" "<<plane<<" "<<ix<<" "<<iy<<" "<<strip<<" "<<dataframe.sample(0).adc()<<" "<<dataframe.sample(1).adc()<<" "<<dataframe.sample(2).adc()<<endl;

  }

  double sensor_data_CM_S0[32];
  double sensor_data_CM_S1[32];
  double sensor_data_CM_S2[32];
  double sensor_data_S0[32];
  double sensor_data_S1[32];
  double sensor_data_S2[32];
  double cm;

  for (int i=0; i<nLines_; ++i) {
    
    iz = (senZ_[i]==1) ? 0:1;
    
    for (int is=0; is<32; ++is) {
      sensor_data_CM_S0[is] = data_S0[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
      sensor_data_CM_S1[is] = data_S1[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
      sensor_data_CM_S2[is] = data_S2[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
      sensor_data_S0[is] = data_S0[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
      sensor_data_S1[is] = data_S1[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
      sensor_data_S2[is] = data_S2[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is];  //Read sensor data
    }
    
    for (int z=0; z<hotChannels_; ++z) {
      int hz = (hotZ[z]==1) ? 0:1;
      if (iz==hz && senP_[i]==hotP[z] && senX_[i]==hotX[z] && senY_[i]==hotY[z]) {
	sensor_data_CM_S0[ hotS[z]-1 ] = 4095;
	sensor_data_CM_S1[ hotS[z]-1 ] = 4095;
	sensor_data_CM_S2[ hotS[z]-1 ] = 4095;
      }
    } 

    //int cm1, cm2, cm3;    
    DoCommonMode(sensor_data_CM_S0, &cm);
    hcm_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1]->Fill(cm);  //Fill CM histos per sensor
    //cm1 = cm;
    for (int is=0; is<32; ++is) {
      sensor_data_S0[is] -= cm;
      cADC0_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is] = sensor_data_S0[is];
    }
    
    DoCommonMode(sensor_data_CM_S1, &cm);
    hcm_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1]->Fill(cm);  //Fill CM histos per sensor
    //cm2 = cm;
    for (int is=0; is<32; ++is) {
      sensor_data_S1[is] -= cm;
      cADC1_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is] = sensor_data_S1[is];
    }
    
    DoCommonMode(sensor_data_CM_S2, &cm);
    hcm_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1]->Fill(cm);  //Fill CM histos per sensor
    //cm3 = cm;
    for (int is=0; is<32; ++is) {
      sensor_data_S2[is] -= cm;
      cADC2_[iz][senP_[i]-1][senX_[i]-1][senY_[i]-1][is] = sensor_data_S2[is];
    }
    
  }

  // ES RecHits
  Handle<ESRecHitCollection> ESRecHit;
  if ( e.getByLabel(rechitlabel_, ESRecHit) ) {

    for (ESRecHitCollection::const_iterator hitItr = ESRecHit->begin(); hitItr != ESRecHit->end(); ++hitItr) {

      ESDetId rhid = ESDetId(hitItr->id());
       
      zside = rhid.zside();
      plane = rhid.plane();
      ix    = rhid.six();
      iy    = rhid.siy();
      strip = rhid.strip();
       
      iz = (zside==1) ? 0:1;
      RH_[iz][plane-1][ix-1][iy-1][strip-1] = hitItr->energy();
       
    }
  } 

  // EE RecHits
  Handle<EERecHitCollection> EERecHit;
  if ( e.getByLabel(eerechitlabel_, EERecHit) ) {

    for (EERecHitCollection::const_iterator eehitItr = EERecHit->begin(); eehitItr != EERecHit->end(); ++eehitItr) {

      EEDetId eeid = EEDetId(eehitItr->id());
       
      if (eeid.zside() > 0)
	en_eep += eehitItr->energy();
      else if (eeid.zside() < 0)
	en_eem += eehitItr->energy();
       
    }
  } 

  // HE RecHits
  Handle<HBHERecHitCollection> HBHERecHit;
  if ( e.getByLabel(herechitlabel_, HBHERecHit) ) {
    HBHERecHitCollection::const_iterator irec;
    for (irec = HBHERecHit->begin(); irec != HBHERecHit->end(); ++irec) {
      
      if (irec->id().ieta() > 0) 
	en_hep += irec->energy();
      else if (irec->id().ieta() < 0) 
	en_hem += irec->energy();

    }
  }
  
  if (dumpTree_) tree_->Fill();
}

void ESSplashAnalyzer::DoCommonMode(double det_data[], double *cm) {
  
  int current_min1 = 4095;
  int current_min2 = 4095;
  int current_strip_val1 = 0;
  int index = -1;
  int current_sum1 = 0;
  int threshold1=0;
  int n1=0;
  //int check_bit1_10;
  int corrected_sum1;

  int treshold_const = (int) (4.3*6.5); // move from 7.9 to 6.5 for HG

  for(int i=0; i<32; ++i) {
    current_strip_val1 = (int) det_data[i];
    if(current_min1 > current_strip_val1 ) {
      current_min1 = current_strip_val1;
      index = i;
    }
  }

  for(int i=0; i<32; ++i) {
    if (index==i) continue;
    current_strip_val1 = (int) det_data[i];
    if(current_min2 > current_strip_val1 ) {
      current_min2 = current_strip_val1;
    }
  }
  threshold1 = current_min2 + treshold_const;

  for(int i=0; i<32; ++i) {
    if (index==i) continue;
    current_strip_val1 = (int) det_data[i];
    if(current_strip_val1 <= threshold1) {
      n1++;
      current_sum1 += current_strip_val1;
    }
  }

  float atmp = current_sum1/(float)n1;
  if (atmp>0)
    atmp += 0.5;
  else if (atmp<0)
    atmp -= 0.5;
  corrected_sum1 = atmp;

  *cm = (float)corrected_sum1;

}

void ESSplashAnalyzer::beginJob(const edm::EventSetup&) {
}

void ESSplashAnalyzer::endJob() {
}

DEFINE_FWK_MODULE(ESSplashAnalyzer);
