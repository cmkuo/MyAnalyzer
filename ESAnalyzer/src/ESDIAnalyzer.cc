#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRawData/interface/ESDCCHeaderBlock.h"
#include "DataFormats/EcalRawData/interface/ESKCHIPBlock.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"

#include <fstream>

using namespace edm;
using namespace std;

class ESDIAnalyzer : public edm::EDAnalyzer {

   public:

  explicit ESDIAnalyzer(const edm::ParameterSet&);
  ~ESDIAnalyzer();

   private:

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  InputTag digilabel_;

  int nLines_, runNum_, eCount_, runtype_, seqtype_, dac_, gain_, precision_;
  int firstDAC_, nDAC_, isPed_, vDAC_[5]; 

  Int_t run;
  Int_t event;
  Int_t orbit;
  Int_t bx;
  Int_t lumis;

  int ESFed_[56];
  int DCCErr_[56][6];
  int OptoRX_[56][3];
  int OptoBC_[56][3];
  int FiberStatus_[56][36][16];
  int kFlag1_[1550][16];
  int kFlag2_[1550][16];
  int kBC_[1550];
  int kEC_[1550];

  Service<TFileService> fs;  
  TTree *tree_;

};

ESDIAnalyzer::ESDIAnalyzer(const edm::ParameterSet& ps) {
  
  digilabel_    = ps.getParameter<InputTag>("DigiLabel");

  for (int i=0; i<56; ++i) {
    ESFed_[i] = 0;  
    for (int j=0; j<36; ++j) 
      for (int k=0; k<16; ++k)
	FiberStatus_[i][j][k] = 0;

    for (int j=0; j<6; ++j) 
      DCCErr_[i][j] = 0;
    
    for (int j=0; j<3; ++j) {
      OptoRX_[i][j] = 0;
      OptoBC_[i][j] = 0;
    }
  }

  for (int i=0; i<1550; ++i) {
    kBC_[i] = 0;
    kEC_[i] = 0;
    for (int j=0; j<16; ++j) kFlag1_[i][j] = 0;
    for (int j=0; j<16; ++j) kFlag2_[i][j] = 0;
  }

  eCount_ = 0;
  firstDAC_ = -1;
  nDAC_ = 0;
  isPed_ = 1;

  tree_ = fs->make<TTree>("tree", "tree");
  tree_->Branch("run", &run, "run/I");
  tree_->Branch("event", &event, "event/I");
  tree_->Branch("orbit", &orbit, "orbit/I");
  tree_->Branch("bx", &bx, "bx/I");
  tree_->Branch("lumis", &lumis, "lumis/I");
  tree_->Branch("ESFed", ESFed_, "ESFed[56]/I");
  tree_->Branch("DCCErr", DCCErr_, "DCCErr[56][6]/I");
  tree_->Branch("OptoRX", OptoRX_, "OptoRX[56][3]/I");
  tree_->Branch("OptoBC", OptoBC_, "OptoBC[56][3]/I");
  tree_->Branch("FiberStatus", FiberStatus_, "FiberStatus[56][36][16]/I");
  tree_->Branch("kFlag1", kFlag1_, "kFlag1[1550][16]/I");
  tree_->Branch("kFlag2", kFlag2_, "kFlag2[1550][16]/I");
  tree_->Branch("kBC", kBC_, "kBC[1550]/I");
  tree_->Branch("kEC", kEC_, "kEC[1550]/I");

}

ESDIAnalyzer::~ESDIAnalyzer() {
}

void ESDIAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& iSetup) {

  run   = e.id().run();
  event = e.id().event();
  orbit = e.orbitNumber();
  bx    = e.bunchCrossing();
  lumis = e.luminosityBlock();

  runNum_ = e.id().run();
  eCount_++;

  Handle<ESRawDataCollection> dccs;
  try {
    e.getByLabel(digilabel_, dccs);
  } catch ( cms::Exception &e ) {
    LogDebug("") << "Error! can't get ES raw data collection !" << std::endl;
  }

  Handle<ESLocalRawDataCollection> kchips;
  try {
    e.getByLabel(digilabel_, kchips);
  } catch ( cms::Exception &e ) {
    LogDebug("") << "Error! can't get ES local raw data collection !" << std::endl;
  }

  // DCC 
  vector<int> fiberStatus;
  for (ESRawDataCollection::const_iterator dccItr = dccs->begin(); dccItr != dccs->end(); ++dccItr) {
    ESDCCHeaderBlock dcc = (*dccItr);

    ESFed_[dcc.fedId()-520]++;
    if (dcc.getDCCErrors()<0) DCCErr_[dcc.fedId()-520][0]++;
    else DCCErr_[dcc.fedId()-520][dcc.getDCCErrors()]++;

    if (dcc.getOptoRX0() == 128) OptoRX_[dcc.fedId()-520][0]++;
    if (dcc.getOptoRX1() == 128) OptoRX_[dcc.fedId()-520][1]++;
    if (dcc.getOptoRX2() == 128) OptoRX_[dcc.fedId()-520][2]++;

    if ((dcc.getOptoBC0()+9)%4096 != dcc.getBX()) OptoBC_[dcc.fedId()-520][0]++;
    if ((dcc.getOptoBC1()+9)%4096 != dcc.getBX()) OptoBC_[dcc.fedId()-520][1]++;
    if ((dcc.getOptoBC2()+9)%4096 != dcc.getBX()) OptoBC_[dcc.fedId()-520][2]++;

    fiberStatus = dcc.getFEChannelStatus();

    for (unsigned int i=0; i<fiberStatus.size(); ++i) FiberStatus_[dcc.fedId()-520][i][fiberStatus[i]]++;

    runtype_   = dcc.getRunType();
    seqtype_   = dcc.getSeqType();
    dac_       = dcc.getDAC();
    gain_      = dcc.getGain();
    precision_ = dcc.getPrecision();
  }

  // KCHIP's
  for ( ESLocalRawDataCollection::const_iterator kItr = kchips->begin(); kItr != kchips->end(); ++kItr ) {

    ESKCHIPBlock kchip = (*kItr);

    kFlag1_[kchip.id()][kchip.getFlag1()]++;
    kFlag2_[kchip.id()][kchip.getFlag2()]++;
    if (kchip.getBC() != kchip.getOptoBC()) kBC_[kchip.id()]++;
    if (kchip.getEC() != kchip.getOptoEC()) kEC_[kchip.id()]++;
  }

  tree_->Fill();

}

void ESDIAnalyzer::beginJob(const edm::EventSetup&) {
}

void ESDIAnalyzer::endJob() {

  int nFED = 0;
  int nFiber = 0;
  int nKCHIP = 0;
  int nOptoRX = 0;
  int DCCStatus = 0;
  int KF1Status = 0;
  int KF2Status = 0;
  int FiberStatus = 0;
  int OptoRXStatus = 0;
  int OptoBCStatus = 0;
  int BCStatus = 0;
  int ECStatus = 0;

  // temp. solution for OptoRX
  int fed_rx[56] = {2, 0, 2, 3, 3, 
		    3, 0, 0, 2, 2,
                    3, 3, 3, 0, 3,
                    3, 0, 2, 0, 2,
                    3, 3, 3, 0, 0,
                    2, 2, 3, 3, 2,
                    0, 2, 0, 3, 3,
                    3, 2, 2, 0, 0,
                    3, 3, 0, 3, 3,
                    3, 2, 0, 2, 0,
                    3, 3, 3, 2, 2,
                    0};
  
  string runtype[7] = {"PHYSICS_RUN", "PEDESTAL_RUN", "TESTPULSE_RUN", "TESTPULSE_RUN", "", "GLOBAL_PHYSICS_RUN", "LOCAL_PHYSICS_RUN"};
  //string runtype[7] = {"PEDESTAL_RUN", "TESTPULSE_RUN", "COSMIC_RUN", "BREAM_RUN", "PHYSICS_RUN", "TEST_RUN", "GLOBAL_COSMIC_RUN"};
  string gain[2] = {"LG", "HG"};
  string precision[2] = {"LP", "HP"};

  // Make HTML
  Char_t run_s[50];
  sprintf(run_s, "/esdata/DQM/%08d", runNum_);
  string htmlDir = run_s;
  system(("/bin/mkdir -m 777 -p " + htmlDir).c_str());
  sprintf(run_s, "/esdata/DQM/%08d/gif", runNum_);
  string gifDir = run_s;
  system(("/bin/mkdir -m 777 -p " + gifDir).c_str());
  ofstream htmlFile;
  ofstream htmlFileDI;
  htmlFile.open((htmlDir+"/index.html").c_str());
  htmlFileDI.open((htmlDir+"/DI.html").c_str());

  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "<meta content=\"text/html; charset=UTF-8\" http-equiv=\"content-type\"> " << endl;
  htmlFile << "<title>Preshower TIF DQM</title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<a name=""top""></a>" << endl;
  htmlFile << "<h4>Run Number / Num of Analyzed events :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  if (runtype_==1) htmlFile << " style=\"color: rgb(0, 0, 153);\">" << runNum_ << "&nbsp;&nbsp;&nbsp;/&nbsp;&nbsp;&nbsp;"<< eCount_ <<" ( "<< runtype[runtype_] << " / " << gain[gain_] << " ) "<< "</span></h4>" << endl;
  else if (runtype_==2) htmlFile << " style=\"color: rgb(0, 0, 153);\">" << runNum_ << "&nbsp;&nbsp;&nbsp;/&nbsp;&nbsp;&nbsp;"<< eCount_ <<" ( "<< runtype[runtype_] << " / " << gain[gain_] << " / "<< precision[precision_] <<" ) "<< "</span></h4>" << endl;
  htmlFile << "<hr>" << endl; 
  
  htmlFileDI << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFileDI << "<html>  " << endl;
  htmlFileDI << "<head>  " << endl;
  htmlFileDI << "<meta content=\"text/html; charset=UTF-8\" http-equiv=\"content-type\"> " << endl;
  htmlFileDI << "<title>Preshower TIF DQM</title> " << endl;
  htmlFileDI << "</head>  " << endl;
  htmlFileDI << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFileDI << "<body>  " << endl;
  htmlFileDI << "<a name=""top""></a>" << endl;
  htmlFileDI << "<h2>Run Number / Num of Analyzed events :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFileDI << " style=\"color: rgb(0, 0, 153);\">" << runNum_ << "&nbsp;&nbsp;&nbsp;/&nbsp;&nbsp;&nbsp;"<< eCount_ <<"</span></h2>" << endl;
  htmlFileDI << "<hr>" << endl; 
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"2\">Events received from each FED </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>FED id : </td>" << endl;
  htmlFileDI << "<td>Num of events : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<56; ++i) {
    if (ESFed_[i] != 0) {
      nFED++;      
      htmlFileDI << "<tr align=\"center\">" << endl;
      htmlFileDI << "<td>" << 520+i << "</td>" << endl;
      htmlFileDI << "<td>" << ESFed_[i] << "</td>" << endl;
      htmlFileDI << "</tr>" << endl;
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"4\">DCC errors</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>FED id : </td>" << endl;
  htmlFileDI << "<td>Error code : </td>" << endl;
  htmlFileDI << "<td>Num of errors : </td>" << endl;
  htmlFileDI << "<td>Reason : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<56; ++i) {
    if (ESFed_[i]==0) continue;
    for (int j=0; j<6; ++j) {
      if (j==0) {
	htmlFileDI << "<tr align=\"center\">" << endl;
	htmlFileDI << "<td> " << 520+i <<" </td>" << endl;
	htmlFileDI << "<td> 0 </td>" << endl;
	htmlFileDI << "<td> " << DCCErr_[i][j] << " </td>" << endl;
	htmlFileDI << "<td> no problem </td>" << endl;
	htmlFileDI << "</tr>" << endl;
      } else {
	if (DCCErr_[i][j] != 0) {
	  DCCStatus = 1;
	  htmlFileDI << "<tr align=\"center\">" << endl;
	  htmlFileDI << "<td> " << 520+i <<" </td>" << endl;
	  htmlFileDI << "<td>" << j << "</td>" << endl;
	  htmlFileDI << "<td>" << DCCErr_[i][j] << "</td>" << endl;
	  if (j==1) htmlFileDI << "<td>wrong FED id</td>" << endl;
	  else if (j==2) htmlFileDI << "<td>wrong event length</td>" << endl;
	  else if (j==3) htmlFileDI << "<td>wrong DCC length</td>" << endl;
	  else if (j==4 || j==5) htmlFileDI << "<td>DCC header identifier is not correct</td>" << endl;
	  htmlFileDI << "</tr>" << endl;
	}
      }
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;

  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"4\">OptoRX</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>FED id : </td>" << endl;
  htmlFileDI << "<td>RX1</td>" << endl;
  htmlFileDI << "<td>RX2</td>" << endl;
  htmlFileDI << "<td>RX3</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<56; ++i) {
    if (ESFed_[i]==0) continue;
    htmlFileDI << "<tr align=\"center\">" << endl;
    htmlFileDI << "<td>" << 520+i << "</td>" << endl;
    for (int j=0; j<3; ++j) {
      if (OptoRX_[i][j]>0) nOptoRX++;
      htmlFileDI << "<td>" << OptoRX_[i][j] << "</td>" << endl;
      if (j >= fed_rx[i]) continue; 
      if (OptoRX_[i][j] != eCount_) OptoRXStatus = 1;
    }
    htmlFileDI << "</tr>" << endl;
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "Fiber error code : <br>" << endl;
  htmlFileDI << " 7 : fiber disabled by configuration <br>" << endl;
  htmlFileDI << " 8 : packet absence (because of timeout or crc error) <br>" << endl;
  htmlFileDI << " 9 : null event & ec/bc match <br>" << endl;
  htmlFileDI << "10 : null event & ec/bc mismatch <br>" << endl;
  htmlFileDI << "11 : zero hits & ec/bc mismatch <br>" << endl;
  htmlFileDI << "12 : non-zero hits & ec/bc mismatch <br>" << endl;
  htmlFileDI << "13 : zero hits & ec/bc match <br>" << endl;
  htmlFileDI << "14 : non-zero hits & ec/bc match <br>" << endl;
  htmlFileDI << "15 : invalid code <br>" << endl;
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"18\">Fiber Status</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>FED id : </td>" << endl;
  htmlFileDI << "<td>Fiber id : </td>" << endl;
  htmlFileDI << "<td>Error code 7 :  </td>" << endl;
  for (int i=8; i<16; ++i) htmlFileDI << "<td> " << i << " :  </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<56; ++i) {
    if (ESFed_[i] != 0) {
      for (int j=0; j<36; ++j) {
	htmlFileDI << "<tr align=\"center\">" << endl;
	htmlFileDI << "<td>" << 520+i << "</td>" << endl;
	htmlFileDI << "<td>" << j+1 << "</td>" << endl;
	for (int k=7; k<16; ++k) {
	  htmlFileDI << "<td>" << FiberStatus_[i][j][k] << "</td>" << endl;	
	  if (FiberStatus_[i][j][k]>0 && k!=7 && k!=14)  FiberStatus = 1;
	}
	if (FiberStatus_[i][j][7]==0) nFiber++;
	htmlFileDI << "</tr>" << endl;
      }
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;

  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"3\">KChip flag1 errors </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>KID : </td>" << endl;
  htmlFileDI << "<td>Error code : </td>" << endl;
  htmlFileDI << "<td>Num of errors : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<1550; ++i) {
    int kcount = 0;
    for (int j=0; j<16; ++j) {
      if (kFlag1_[i][j] != 0) {
	kcount++;
	if (j!=0) KF1Status = 1;
	htmlFileDI << "<tr align=\"center\">" << endl;
	htmlFileDI << "<td>" << i << "</td>" << endl;
	htmlFileDI << "<td>" << j << "</td>" << endl;
	htmlFileDI << "<td>" << kFlag1_[i][j] << "</td>" << endl;
	htmlFileDI << "</tr>" << endl;
      }
    }
    if (kcount>0) nKCHIP++;
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"3\">KChip flag2 errors </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>KID : </td>" << endl;
  htmlFileDI << "<td>Error code : </td>" << endl;
  htmlFileDI << "<td>Num of errors : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<1550; ++i) {
    for (int j=0; j<16; ++j) {
      if (kFlag2_[i][j] != 0) {
	if (j!=0) KF2Status = 1;
	htmlFileDI << "<tr align=\"center\">" << endl;
	htmlFileDI << "<td>" << i << "</td>" << endl;
	htmlFileDI << "<td>" << j << "</td>" << endl;
	htmlFileDI << "<td>" << kFlag2_[i][j] << "</td>" << endl;
	htmlFileDI << "</tr>" << endl;
      }
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"4\">OptoRX BC Errors</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>FED id : </td>" << endl;
  htmlFileDI << "<td>RX1</td>" << endl;
  htmlFileDI << "<td>RX2</td>" << endl;
  htmlFileDI << "<td>RX3</td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<56; ++i) {
    if (ESFed_[i]==0) continue;
    htmlFileDI << "<tr align=\"center\">" << endl;
    htmlFileDI << "<td>" << 520+i << "</td>" << endl;
    for (int j=0; j<3; ++j) {
      htmlFileDI << "<td>" << OptoBC_[i][j] << "</td>" << endl;
      if (j >= fed_rx[i]) continue; 
      if (OptoBC_[i][j]>0) OptoBCStatus = 1;
    }
    htmlFileDI << "</tr>" << endl;
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"2\">BC errors </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>KID : </td>" << endl;
  htmlFileDI << "<td>Num of errors : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<1550; ++i) {
    if (kBC_[i] != 0) {
      BCStatus = 1;
      htmlFileDI << "<tr align=\"center\">" << endl;
      htmlFileDI << "<td>" << i << "</td>" << endl;
      htmlFileDI << "<td>" << kBC_[i] << "</td>" << endl;
      htmlFileDI << "</tr>" << endl;
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFileDI << "cellpadding=\"10\" > " << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td colspan=\"2\">EC errors </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  htmlFileDI << "<tr align=\"center\">" << endl;
  htmlFileDI << "<td>KID : </td>" << endl;
  htmlFileDI << "<td>Num of errors : </td>" << endl;
  htmlFileDI << "</tr>" << endl;
  for (int i=0; i<1550; ++i) {
    if (kEC_[i] != 0) {
      ECStatus = 1;
      htmlFileDI << "<tr align=\"center\">" << endl;
      htmlFileDI << "<td>" << i << "</td>" << endl;
      htmlFileDI << "<td>" << kEC_[i] << "</td>" << endl;
      htmlFileDI << "</tr>" << endl;
    }
  }
  htmlFileDI << "</table>" << endl;
  htmlFileDI << "<br>" <<endl;
  
  htmlFileDI << "<hr>" <<endl;

  // Show Run info and data integrity
  htmlFile << "<table border=\"1\" cellspacing=\"0\" " << endl;
  htmlFile << "cellpadding=\"10\" > " << endl;
  htmlFile << "<tr align=\"center\">" << endl;
  htmlFile << "<td colspan=\"12\"><a href=\"DI.html\">Data integrity</a></td>" << endl;
  htmlFile << "</tr>" << endl;
  htmlFile << "<tr align=\"center\">" << endl;
  htmlFile << "<td># of FED</td>" << endl;
  htmlFile << "<td># of OptoRX</td>" << endl;
  htmlFile << "<td># of Fiber</td>" << endl;
  htmlFile << "<td># of KChip</td>" << endl;
  htmlFile << "<td>DCC</td>" << endl;
  htmlFile << "<td>OptoRX</td>" << endl; 
  htmlFile << "<td>Fiber</td>" << endl;
  htmlFile << "<td>KFlag1</td>" << endl;
  htmlFile << "<td>KFlag2</td>" << endl;
  htmlFile << "<td>OptoBC</td>" << endl;
  htmlFile << "<td>BC</td>" << endl;
  htmlFile << "<td>EC</td>" << endl;
  
  htmlFile << "</tr>" << endl;
  htmlFile << "<tr align=\"center\">" << endl;
  htmlFile << "<td>" << nFED << "</td>" << endl;
  htmlFile << "<td>" << nOptoRX << "</td>" << endl;
  htmlFile << "<td>" << nFiber << "</td>" << endl;
  htmlFile << "<td>" << nKCHIP << "</td>" << endl;
  if (DCCStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (OptoRXStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (FiberStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (KF1Status==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (KF2Status==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (OptoBCStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (BCStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  if (ECStatus==0) htmlFile << "<td> <img src=\"../good.png\"></img> </td>" << endl;
  else htmlFile << "<td> <img src=\"../bad.png\"></img> </td>" << endl;
  htmlFile << "</tr>" << endl;
  htmlFile << "</table>" << endl;
  
  htmlFile.close();
  
  htmlFileDI << "</body> " << endl;
  htmlFileDI << "</html> " << endl;
  
  htmlFileDI.close();

  // Make summmary file
  char smryName[150];
  sprintf(smryName,"/nfshome0/esdev/DQM/CMSSW_5_0_1_patch2/src/MyAnalyzer/ESAnalyzer/test/p5/smry/%08d.smry",runNum_);

  string smryDir = smryName; 
  ofstream summaryFile;
  summaryFile.open((smryDir).c_str());

  summaryFile<<eCount_<<" "<<runtype_<<" "<<runtype[runtype_]<<" "<<gain[gain_]<<" "<<precision[precision_]<<" "<<nFED<<" "<<nOptoRX<<" "<<nFiber<<" "<<nKCHIP<<" "<<DCCStatus<<" "<<OptoRXStatus<<" "<<FiberStatus<<" "<<KF1Status<<" "<<KF2Status<<" "<<OptoBCStatus<<" "<<BCStatus<<" "<<ECStatus<<endl;

  summaryFile<<"     "<<endl;
  summaryFile<<" Run Type  = "<<runtype[runtype_]<<endl;
  summaryFile<<" Gain Mode = "<<gain[gain_]<<endl;

  summaryFile.close(); 

}

DEFINE_FWK_MODULE(ESDIAnalyzer);
