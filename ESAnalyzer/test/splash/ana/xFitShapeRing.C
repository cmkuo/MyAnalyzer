#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
using namespace std;

Double_t fitf(Double_t *x, Double_t *par) {

  Double_t wc = 0.07291;
  Double_t n  = 1.798; // n-1 (in fact)
  Double_t v1 = pow(wc/n*(x[0]-par[1]), n);
  Double_t v2 = TMath::Exp(n-wc*(x[0]-par[1]));
  Double_t v  = par[0]*v1*v2;
  
  if (x[0] < par[1]) v = 0;
  
  return v;
}

void xFitShapeRing(Int_t runId=267996, Int_t endcap=1, Int_t ring=1) {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  //gStyle->SetOptStat(kFALSE);
  //gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  Char_t fname[200];
  sprintf(fname, "beamsplash_%08d.root", runId);
  TFile *f = new TFile(fname);
  TTree *EventTree = (TTree*) f->Get("EventTree");
  
  //Declaration of leaves types
  Int_t           run;
  Int_t           event;
  Int_t           ADC0[2][2][40][40][32];
  Int_t           ADC1[2][2][40][40][32];
  Int_t           ADC2[2][2][40][40][32];
  Int_t           cADC0[2][2][40][40][32];
  Int_t           cADC1[2][2][40][40][32];
  Int_t           cADC2[2][2][40][40][32];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("event",&event);
  EventTree->SetBranchAddress("ADC0",ADC0);
  EventTree->SetBranchAddress("ADC1",ADC1);
  EventTree->SetBranchAddress("ADC2",ADC2);
  EventTree->SetBranchAddress("cADC0",cADC0);
  EventTree->SetBranchAddress("cADC1",cADC1);
  EventTree->SetBranchAddress("cADC2",cADC2);
  
  Int_t tx[3], counts[32][3], ccounts[32][3];
  Long64_t nentries = EventTree->GetEntries();
  
  TF1 *func = new TF1("func", fitf, -200, 200, 2);
  func->SetLineWidth(2);
  
  TH1F *hMIP = new TH1F("hMIP", "hMIP", 500, 0, 500);  
  TH1F *hT0  = new TH1F("hT0", "hT0", 100, -50, 50);
  
  //TCanvas *c1 = new TCanvas("c1", "c1", 900, 600);
  //c1->Divide(4,2);
  //c1->Divide(8,4);
  TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
  TCanvas *c3 = new TCanvas("c3", "c3", 900, 600);
  c3->Divide(2,1);
  
  Int_t cr_[2][2][40][40];
  Int_t ringid_[2][2][40][40];

  // read in look-up table
  int iz, ip, ix, iy, fed, kchip, pace, bundle, fiber, optorx, cr, ringid;
  ifstream file;
  for (int z=0; z<2; ++z) 
    for (int p=0; p<2; ++p)
      for (int x=0; x<40; ++x)
	for (int y=0; y<40; ++y) {
	  cr_[z][p][x][y] = -1;  
	  ringid_[z][p][x][y] = -1;
	}

  Int_t nLines_;
  file.open("cr_table.txt");
  if( file.is_open() ) {
    
    file >> nLines_;
    for (int iline=0; iline<nLines_; ++iline) {
      
      file >> iz >> ip >> ix >> iy >> cr >> ringid;

      cr_[(3-iz)/2-1][ip-1][ix-1][iy-1] = cr;
      ringid_[(3-iz)/2-1][ip-1][ix-1][iy-1] = ringid;
    }
  }
  
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);
    
    cout<<"=== "<<run<<" === "<<event<<endl;
    //if (event!=8) continue;
    Int_t t0 = -5;
    tx[0] = t0;
    tx[1] = t0+25;
    tx[2] = t0+50;       

    Double_t para[10];    
    
    for (int z=0; z<2; ++z) 
      for (int p=0; p<2; ++p)
	for (int x=0; x<40; ++x)
	  for (int y=0; y<40; ++y)
	    for (int j=0; j<32; ++j) {
	      	      
	      counts[j][0] = ADC0[z][p][x][y][j];
	      counts[j][1] = ADC1[z][p][x][y][j];
	      counts[j][2] = ADC2[z][p][x][y][j];
	      
	      ccounts[j][0] = cADC0[z][p][x][y][j];
	      ccounts[j][1] = cADC1[z][p][x][y][j];
	      ccounts[j][2] = cADC2[z][p][x][y][j];
	      
	      if (counts[j][0] < -10000) continue;
	      if (cr_[z][p][x][j] != endcap) continue;
	      if (ringid_[z][p][x][y] != ring) continue;

	      TGraph *gr = new TGraph(3, tx, counts[j]);
	      func->SetParameters(50, 10);
	      gr->Fit("func", "MQ");
	      func->GetParameters(para);

	      hMIP->Fill(para[0]);
	      hT0->Fill(para[1]);	      	      
	      /*
	      c2->cd();
	      gr->SetTitle("");
	      gr->Draw("a");
	      gr->SetMarkerStyle(20);
	      gr->Draw("ap");
	      c2->Update();
	      cout<<z<<" "<<p<<" "<<x<<" "<<y<<" "<<j<<endl;
	      cin.get();
	      */
	      /*
	      c3->cd(1);
	      hMIP->Draw();
	      c3->cd(2);
	      hT0->Draw();
	      c3->Update();	     
	      */
	    }

    cout<<"Done"<<endl;
    //cin.get();
        
  }
  
  c3->cd(1);
  hMIP->Draw();
  c3->cd(2);
  hT0->Draw();
  //c3->Update();	     
}

