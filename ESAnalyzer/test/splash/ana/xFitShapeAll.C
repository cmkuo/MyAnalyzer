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

void xFitShapeAll(Int_t runId=120012) {
  
  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  //gStyle->SetOptTitle(kFALSE);  
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadTopMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  Char_t fname[200];
  sprintf(fname, "/data3/ncuhep/splash/ana/splash_%08d.root", runId);
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
  
  TH1F *hMIP = new TH1F("hMIP", "hMIP", 200, 0, 1000);  
  TH1F *hT0  = new TH1F("hT0", "hT0", 100, -50, 50);
  
  TH2F *hespf = new TH2F("hespf", "ES+ F", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hespr = new TH2F("hespr", "ES+ R", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hesmf = new TH2F("hesmf", "ES- F", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hesmr = new TH2F("hesmr", "ES- R", 40, 0.5, 40.5, 40, 0.5, 40.5);

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  c1->Divide(2,2);
  //TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
  //TCanvas *c3 = new TCanvas("c3", "c3", 900, 600);

  Int_t evId = -1;
  
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);
    
    cout<<"=== "<<run<<" === "<<event<<endl;
    if (run==120012 && event!=18) continue;
    evId = event;
    Int_t t0 = -5;
    tx[0] = t0;
    tx[1] = t0+25;
    tx[2] = t0+50;       

    Double_t avt = 0;
    Double_t navt = 0;
    Double_t para[10];    
    
    for (int z=0; z<2; ++z) 
      for (int p=0; p<2; ++p)
	for (int x=0; x<40; ++x)
	  for (int y=0; y<40; ++y) {

	    avt = 0;
	    navt = 0;

	    for (int j=0; j<32; ++j) {
	      	      
	      counts[j][0] = ADC0[z][p][x][y][j];
	      counts[j][1] = ADC1[z][p][x][y][j];
	      counts[j][2] = ADC2[z][p][x][y][j];
	      
	      ccounts[j][0] = cADC0[z][p][x][y][j];
	      ccounts[j][1] = cADC1[z][p][x][y][j];
	      ccounts[j][2] = cADC2[z][p][x][y][j];
	      
	      if (counts[j][0] < -10000) continue;

	      TGraph *gr = new TGraph(3, tx, counts[j]);
	      func->SetParameters(50, 10);
	      gr->Fit("func", "MQ");
	      func->GetParameters(para);

	      hMIP->Fill(para[0]*81.08/55);
	      hT0->Fill(para[1]);	      	      

	      navt++;
	      avt += para[1];

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

	    if (navt != 0) { 
	      if (z==0 && p==0) hespf->Fill(x+1, y+1, avt/navt);
	      if (z==0 && p==1) hespr->Fill(x+1, y+1, avt/navt);
	      if (z==1 && p==0) hesmf->Fill(x+1, y+1, avt/navt);
	      if (z==1 && p==1) hesmr->Fill(x+1, y+1, avt/navt);
	    } else {
	      if (z==0 && p==0) hespf->Fill(x+1, y+1, -10000);
	      if (z==0 && p==1) hespr->Fill(x+1, y+1, -10000);
	      if (z==1 && p==0) hesmf->Fill(x+1, y+1, -10000);
	      if (z==1 && p==1) hesmr->Fill(x+1, y+1, -10000);
	    }

	  }

    cout<<"Done"<<endl;
    //cin.get();
        
  }
  /*
  Char_t tname[200];
  sprintf(tname, "Run %d Event %d", runId, evId);  
  c2->cd();
  hMIP->SetTitle(tname);
  hMIP->GetXaxis()->SetTitle("Deposited energy per strip (KeV)");
  hMIP->SetLineWidth(3);
  hMIP->SetLineColor(4);
  hMIP->Draw();
  c3->cd();
  hT0->SetTitle(tname);
  hT0->GetXaxis()->SetTitle("T0 (ns)");
  hT0->SetLineWidth(3);
  hT0->SetLineColor(4);
  hT0->Draw();
  //c3->Update();
  */
  c1->cd(1);
  hespf->SetMaximum(25);
  hespf->SetMinimum(-10);
  hespf->GetXaxis()->SetTitle("Si X");
  hespf->GetYaxis()->SetTitle("Si Y");
  hespf->GetZaxis()->SetTitle("TO (ns)");
  hespf->Draw("colz");
  c1->cd(2);
  hesmf->SetMaximum(25);
  hesmf->SetMinimum(-10);
  hesmf->GetXaxis()->SetTitle("Si X");
  hesmf->GetYaxis()->SetTitle("Si Y");
  hesmf->GetZaxis()->SetTitle("TO (ns)");
  hesmf->Draw("colz");
  c1->cd(3);
  hespr->SetMaximum(25);
  hespr->SetMinimum(-10);
  hespr->GetXaxis()->SetTitle("Si X");
  hespr->GetYaxis()->SetTitle("Si Y");
  hespr->GetZaxis()->SetTitle("TO (ns)");
  hespr->Draw("colz");
  c1->cd(4);
  hesmr->SetMaximum(25);
  hesmr->SetMinimum(-10);
  hesmr->GetXaxis()->SetTitle("Si X");
  hesmr->GetYaxis()->SetTitle("Si Y");
  hesmr->GetZaxis()->SetTitle("TO (ns)");
  hesmr->Draw("colz");

  c1->Print("es_timing.png");
  /*
  sprintf(tname, "energy_spectrum_%d_%d.png", runId, evId);  
  c2->Print(tname);
  sprintf(tname, "t0_%d_%d.png", runId, evId);  
  c3->Print(tname);
  */
}

