#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TText.h"

Double_t fitf(Double_t *x, Double_t *par) {

  Double_t wc = 0.07291;
  Double_t n  = 1.798; // n-1 (in fact)
  Double_t v1 = pow(wc/n*(x[0]-par[1]), n);
  Double_t v2 = TMath::Exp(n-wc*(x[0]-par[1]));
  Double_t v  = par[0]*v1*v2;
  
  if (x[0] < par[1]) v = 0;
  
  return v;
}

//void xFitShapeSensor(Int_t runId=120014) {
void xFitShapeSensor() {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  Char_t fname[200];
  //sprintf(fname, "/data3/ncuhep/splash/ana/splash_%08d.root", runId);
  sprintf(fname, "/data2/cmkuo/preshower/minbias_2015.root");
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
  
  Int_t tx[3], counts[3];
  Long64_t nentries = EventTree->GetEntries();
  
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
  for (Long64_t z=0; z<nentries; ++z) {
  //for (Long64_t z=0; z<100; ++z) {
    nbytes += EventTree->GetEntry(z);
    
    cout<<"=== "<<run<<" === "<<event<<endl;
    //if (event > 124) break;    
    tx[0] = -5;
    tx[1] = 20;
    tx[2] = 45;

    Double_t para[10];    
    //Double_t mips[32];
    TGraph *gr;

    for (int i=0; i<2; ++i)
      for (int j=0; j<2; ++j) 
	for (int k=0; k<40; ++k)
	  for (int m=0; m<40; ++m)
	    for (int n=0; n<32; ++n) {

	      if (ADC0[i][j][k][m][n] < -10000) continue;
	      if (ADC0[i][j][k][m][n] > 20) continue;

	      counts[0] = ADC0[i][j][k][m][n];
	      counts[1] = ADC1[i][j][k][m][n];
	      counts[2] = ADC2[i][j][k][m][n];
	      
	      gr = new TGraph(3, tx, counts);
	      func->SetParameters(50, 10);
	      gr->Fit("func", "MQ");
	      func->GetParameters(para);

	      if (func->GetChisquare() > 100) continue;	      
	      hT[i][j][k][m]->Fill(para[1]);
	      hTiming[i][j]->Fill(para[1]);
	      //cout<<i<<" "<<j<<" "<<k<<" "<<m<<" "<<n<<" "<<para[0]/55.<<" "<<para[1]<<endl;
	    }
    
  }
  
  TFile *fout = new TFile("beamsplash_timing.root", "recreate");
  fout->cd();
  for (int i=0; i<2; ++i)
    for (int j=0; j<2; ++j) {
      hTiming[i][j]->Write();
      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m)
	  hT[i][j][k][m]->Write();
    }
  fout->Close();
  
}
