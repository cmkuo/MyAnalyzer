//g++ -Wall -o xTimingSensor_Add_temp `root-config --cflags --glibs` xTimingSensor_Add_temp.cpp

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TText.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>



Double_t fitf(Double_t *x, Double_t *par) {

  Double_t wc = 0.07291;
  Double_t n  = 1.798; // n-1 (in fact)
  Double_t v1 = pow(wc/n*(x[0]-par[1]), n);
  Double_t v2 = TMath::Exp(n-wc*(x[0]-par[1]));
  Double_t v  = par[0]*v1*v2;
  
  if (x[0] < par[1]) v = 0;
  
  return v;
}


int main(int argc, char* argv[]) {

  int START = atoi(argv[1]);
  std::cout << " >>> START = " << START << std::endl;
  int STOP = atoi(argv[2]);
  std::cout << " >>> STOP = " << STOP << std::endl;

  std::string inFileList(argv[3]);
  std::cout << " >>> inFileList = " << inFileList << std::endl;



  //Reset ROOT and connect tree file
  gROOT->Reset();
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  

  TChain* EventTree = new TChain("coll/tree");
  int nFiles = 0;
  // load files
  std::ifstream inFileES;
  inFileES.open(inFileList.c_str(), std::ios::in);
  while(!inFileES.eof()){
    std::string fileName;
      inFileES >> fileName;
      if((START+nFiles) < STOP){
	std::cout << "Adding... " << fileName << std::endl;
	std::cout << "start+nFiles = " << START+nFiles << " stop = " << STOP << std::endl;
	EventTree->Add(fileName.c_str());
      }
      else{
	std::cout << "Skipping... " << fileName << std::endl;
      }
      ++nFiles;
  }
  
  //Declaration of leaves types
  Int_t           run;
  Int_t           event;
  Int_t           nRH;
  Int_t           rh_z[200000];
  Int_t           rh_p[200000];
  Int_t           rh_x[200000];
  Int_t           rh_y[200000];
  Int_t           rh_s[200000];
  Int_t           rh_status[200000];
  Int_t           rh_ADC0[200000];
  Int_t           rh_ADC1[200000];
  Int_t           rh_ADC2[200000];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("event",&event);
  EventTree->SetBranchAddress("nRH", &nRH);
  EventTree->SetBranchAddress("rh_z",rh_z);
  EventTree->SetBranchAddress("rh_p",rh_p);
  EventTree->SetBranchAddress("rh_x",rh_x);
  EventTree->SetBranchAddress("rh_y",rh_y);
  EventTree->SetBranchAddress("rh_z",rh_z);
  EventTree->SetBranchAddress("rh_s",rh_s);
  EventTree->SetBranchAddress("rh_status",rh_status);
  EventTree->SetBranchAddress("rh_ADC0",rh_ADC0);
  EventTree->SetBranchAddress("rh_ADC1",rh_ADC1);
  EventTree->SetBranchAddress("rh_ADC2",rh_ADC2);
  
  Int_t tx[3], counts[3];
  Long64_t nentries = EventTree->GetEntries();
  std::cout << " >>> nentries = " << nentries << std::endl;


  TF1 *func = new TF1("func", fitf, -200, 200, 2);
  func->SetLineWidth(2);

  Char_t hname[200];
  TH1F *hT[2][2][40][40];
  TH1F *hTiming[2][2];
  for (int i=0; i<2; ++i) 
    for (int j=0; j<2; ++j) {

      sprintf(hname, "Z%d_P%d", i, j);
      hTiming[i][j] = new TH1F(hname, hname, 101, -50.5, 50.5);

      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m) {
	  sprintf(hname, "Z%d_P%d_X%d_Y%d", i, j, k, m);
	  hT[i][j][k][m] = new TH1F(hname, hname, 101, -50.5, 50.5);
	}
    }


  Long64_t nbytes = 0;
  std::cout << " nentries = " << nentries << std::endl;
  for(Long64_t z=0; z<nentries; ++z){

    if(z%10000 == 0) std::cout << "evt " << z << " === " << run << " === " << event << std::endl;
    nbytes += EventTree->GetEntry(z);

    tx[0] = -5;
    tx[1] = 20;
    tx[2] = 45;

    Double_t para[10];    
    //Double_t mips[32];

    for (Int_t i=0; i<nRH; ++i) {

      if (rh_ADC1[i] < 200) continue;
      if (fabs(rh_ADC0[i]) > 20) continue;
      if (rh_ADC1[i] < 0 || rh_ADC2[i] < 0) continue;

      counts[0] = rh_ADC0[i];
      counts[1] = rh_ADC1[i];
      counts[2] = rh_ADC2[i];
      
      TGraph *gr = new TGraph(3, tx, counts);
      func->SetParameters(50, 0);
      gr->Fit("func", "MQ");
      func->GetParameters(para);
      
      if (func->GetChisquare() > 100) continue;	      

      Int_t iz = (rh_z[i] < 0) ? 0 : 1;
      Int_t ip = rh_p[i] - 1;
      Int_t ix = rh_x[i] - 1;
      Int_t iy = rh_y[i] - 1;

      hT[iz][ip][ix][iy]->Fill(para[1]);
      hTiming[iz][ip]->Fill(para[1]);
      //cout<<iz<<" "<<ip<<" "<<ix<<" "<<para[1]<<endl;
      delete gr;
    }
    
  }


  std::cout << " >>> stampa file <<< " << std::endl; 
  
  TFile* fout = new TFile(Form("coll_timing_%d_%d.root",START,STOP), "recreate");
  fout->cd();
  for (int i=0; i<2; ++i)
    for (int j=0; j<2; ++j) {
      hTiming[i][j]->Write();
      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m)
	  hT[i][j][k][m]->Write();
    }
  fout->Close();
 
  std::cout << " fine " << std::endl;
  return 10;
 
}
