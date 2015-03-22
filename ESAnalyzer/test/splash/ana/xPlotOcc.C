#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TAxis.h"

#include <iostream>
using namespace std;

void xPlotOcc(Int_t runId=119878) {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  Char_t fname[200];
  //sprintf(fname, "splash_%08d.root", runId);
  sprintf(fname, "../trees/tree_BeamSplash_1257641879.root");
  TFile *f = new TFile(fname);
  TTree *EventTree = (TTree*) f->Get("EventTree");
  
  //Declaration of leaves types
  Int_t           run;
  Int_t           event;
  Int_t           orbit;
  Int_t           bx;
  Int_t           ADC0[2][2][40][40][32];
  Int_t           ADC1[2][2][40][40][32];
  Int_t           ADC2[2][2][40][40][32];
  Int_t           cADC0[2][2][40][40][32];
  Int_t           cADC1[2][2][40][40][32];
  Int_t           cADC2[2][2][40][40][32];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("event",&event);
  EventTree->SetBranchAddress("orbit",&orbit);
  EventTree->SetBranchAddress("bx",&bx);
  EventTree->SetBranchAddress("ADC0",ADC0);
  EventTree->SetBranchAddress("ADC1",ADC1);
  EventTree->SetBranchAddress("ADC2",ADC2);
  EventTree->SetBranchAddress("cADC0",cADC0);
  EventTree->SetBranchAddress("cADC1",cADC1);
  EventTree->SetBranchAddress("cADC2",cADC2);
  
  Int_t bxId[10000], orbitId[10000], occ[2][2][10000];
  Long64_t nentries = EventTree->GetEntries();
  
  Int_t nev = 0;
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);
    
    nev++;
    if (nev%200 ==0) cout<<"=== "<<run<<" === "<<event<<endl;
    
    for (int j=0; j<2; ++j)
      for (int k=0; k<2; ++k)
	occ[j][k][nev-1] = 0;
    
    for (int z=0; z<2; ++z) 
      for (int p=0; p<2; ++p)
	for (int x=0; x<40; ++x)
	  for (int y=0; y<40; ++y) 
	    for (int j=0; j<32; ++j) {
	      
	      if (ADC0[z][p][x][y][j] > -10000)
		occ[z][p][nev-1]++;
	      
	    }
    
    //cout<<orbit<<" "<<bx<<" "<<occ[0][0][nev-1]<<endl;
    bxId[nev-1] = bx;
    orbitId[nev-1] = event;
  }
  
  TGraph *gr[2][2];
  TGraph2D *gr2d[2][2];
  for (int k=0; k<2; ++k)
    for (int j=0; j<2; ++j) {
      gr[k][j] = new TGraph(nev, orbitId, occ[k][j]);
      gr2d[k][j] = new TGraph2D(nev, orbitId, bxId, occ[k][j]);
    }

  TCanvas *c1 = new TCanvas("c1", "c1", 900, 600);
  c1->Divide(2,2);
  for (int k=0; k<2; ++k)
    for (int j=0; j<2; ++j) {
      c1->cd(k*2+j+1);
      if (k==0 && j==0) gr[k][j]->SetTitle("ES+ F");
      if (k==0 && j==1) gr[k][j]->SetTitle("ES+ R");
      if (k==1 && j==0) gr[k][j]->SetTitle("ES- F");
      if (k==1 && j==1) gr[k][j]->SetTitle("ES- R");
      gr[k][j]->Draw("a");  
      gr[k][j]->GetXaxis()->SetTitle("Orbit Number");
      gr[k][j]->GetYaxis()->SetTitle("# of hits");
      gr[k][j]->SetLineColor(2);
      gr[k][j]->Draw("al");
    }

  TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
  c2->Divide(2,2);
  for (int k=0; k<2; ++k)
    for (int j=0; j<2; ++j) {
      c2->cd(k*2+j+1);
      if (k==0 && j==0) gr2d[k][j]->SetTitle("ES+ F");
      if (k==0 && j==1) gr2d[k][j]->SetTitle("ES+ R");
      if (k==1 && j==0) gr2d[k][j]->SetTitle("ES- F");
      if (k==1 && j==1) gr2d[k][j]->SetTitle("ES- R");
      gr2d[k][j]->Draw("a");  
      gr2d[k][j]->GetXaxis()->SetTitle("Orbit Number");
      gr2d[k][j]->GetYaxis()->SetTitle("BX Number");
      gr2d[k][j]->GetZaxis()->SetTitle("# of hits");
      gr2d[k][j]->SetMarkerStyle(20);
      gr2d[k][j]->Draw("pcol");
    }

}
