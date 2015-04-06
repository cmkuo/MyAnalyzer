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

// Draw ES borders (Ming's copyright, the idea is borrowed from Giuseppe ;-))
void drawBorders( int plane, float sx, float sy ) {
  
  const int ixES[346] = {
    1, 13,  5,  5,  5,  7,  7,  7,  7,  9,  9,  9, 11, 11, 13, 13, 13, 15, 15, 15, 
    15, 15, 15, 19, 19, 19, 21, 21, 21, 23, 23, 23, 25, 25, 25, 27, 27, 27, 27, 29, 
    29, 29, 29, 31, 31, 31, 31, 31, 31, 33, 33, 33, 35, 35, 39, 27, 35, 35, 35, 33, 
    33, 33, 33, 31, 31, 31, 29, 29, 27, 27, 27, 25, 25, 25, 25, 25, 25, 21, 21, 21, 
    19, 19, 19, 17, 17, 17, 15, 15, 15, 13, 13, 13, 13, 11, 11, 11, 11,  9,  9,  9,
    9,  9,  9,  7,  7,  7,  5,  5, 
    
    1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6, 
    6,  6,  6,  7,  7,  7,  7,  9,  9,  9,  9, 10, 10, 10, 10, 13, 13, 13, 13, 15, 
    15, 15, 15, 25, 25, 25, 25, 27, 27, 27, 27, 30, 30, 30, 30, 31, 31, 31, 31, 33, 
    33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 35, 36, 36, 36, 36, 37, 37, 37, 37, 38, 
    38, 38, 38, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 36, 35, 
    35, 35, 35, 34, 34, 34, 34, 33, 33, 33, 33, 31, 31, 31, 31, 30, 30, 30, 30, 27, 
    27, 27, 27, 25, 25, 25, 25, 15, 15, 15, 15, 13, 13, 13, 13, 10, 10, 10, 10,  9, 
    9,  9,  9,  7,  7,  7,  7,  6,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  3, 
    3,  3,  3,  2,  2,  2,  2,  1,  1,  1,
    
    13, 13, 13, 14, 14, 14, 14, 15, 15, 16, 16, 16, 16, 18, 18, 18, 18, 22, 22, 22, 
    22, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 26, 26, 
    26, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 16, 16, 16, 15, 
    15, 15, 15, 14, 14, 14, 14, 13 
  };
  
  const int  iyES[346] = {
    20, 20, 20, 30, 30, 30, 30, 36, 32, 32, 32, 20, 20, 38, 39, 26, 26, 26, 26, 24, 
    40, 30, 30, 30, 40, 27, 40, 35, 35, 35, 35, 26, 24, 40, 26, 26, 26, 33, 33, 33, 
    38, 24, 24, 24, 20, 24, 24, 28, 28, 28, 28, 36, 20, 33, 20, 20, 20, 10, 10, 10, 
    10,  4,  8,  8,  8, 20, 20,  2,  1, 14, 14, 14, 14, 16,  0, 10, 10, 10, 13,  0, 
    0,  5,  5,  5,  5, 14, 16,  0, 14, 14, 14,  7,  7,  7,  2, 16, 16, 16, 20, 16, 
    16, 12, 12, 12, 12,  4,  7, 20, 
    
    20, 26, 26, 26, 26, 28, 28, 28, 28, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 
    33, 34, 34, 34, 34, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 
    39, 40, 40, 40, 40, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 
    36, 34, 34, 34, 34, 33, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 31, 28, 28, 28, 
    28, 26, 26, 26, 26, 14, 14, 14, 14, 12, 12, 12, 12,  9,  9,  9,  9,  8,  8,  8, 
    8,  7,  7,  7,  7,  6,  6,  6,  6,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2, 
    2,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3, 
    3,  4,  4,  4,  4,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  9,  9,  9, 
    9, 12, 12, 12, 12, 14, 14, 14, 14, 20,  
    
    18, 22, 22, 22, 22, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 
    26, 26, 26, 25, 25, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 
    16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 
    15, 16, 16, 16, 16, 18, 18, 18
  };
  
  const int  lsES[54] = { // line style
    1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 
    1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 
    2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2
  };
  
  const int  lwES[54] = { // line width
    2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 
    2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 
    1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1
  };
  
  TLine l;
  
  switch (plane){
    
  case 1:   //ES+F
    for ( int i=0; i<346; i=i+2) {
      if (i<54*2) {
	l.SetLineStyle(lsES[i/2]);
	l.SetLineWidth(lwES[i/2]);
      } else {
	l.SetLineStyle(3);
	l.SetLineWidth(2);
      } 
      //cout<<ixES[i]+sx<<" "<<iyES[i]+sy<<" "<<ixES[i+1]+sx<<" "<<iyES[i+1]+sy<<endl;
      l.DrawLine(ixES[i]+sx, iyES[i]+sy, ixES[i+1]+sx, iyES[i+1]+sy);
    }
    break;
    
  case 2:   //ES-F
    for ( int i=0; i<346; i=i+2) {
      if (i<54*2) {
	l.SetLineStyle(lsES[i/2]);
	l.SetLineWidth(lwES[i/2]);
      } else {
	l.SetLineStyle(3);
	l.SetLineWidth(2);
      } 
      l.DrawLine(40-ixES[i]+sx, iyES[i]+sy, 40-ixES[i+1]+sx, iyES[i+1]+sy);
    }
    break;
    
  case 3:    //ES+R
    for ( int i=0; i<346; i=i+2) {
      if (i<54*2) {
	l.SetLineStyle(lsES[i/2]);
	l.SetLineWidth(lwES[i/2]);
      } else {
	l.SetLineStyle(3);
	l.SetLineWidth(2);
      } 
      l.DrawLine(40-iyES[i]+sx, ixES[i]+sy, 40-iyES[i+1]+sx, ixES[i+1]+sy);
    }
    break;
    
  case 4:    //ES-R
    for ( int i=0; i<346; i=i+2) {
      if (i<54*2) {
	l.SetLineStyle(lsES[i/2]);
	l.SetLineWidth(lwES[i/2]);
      } else {
	l.SetLineStyle(3);
	l.SetLineWidth(2);
      } 
      l.DrawLine(iyES[i]+sx, ixES[i]+sy, iyES[i+1]+sx, ixES[i+1]+sy);
    }
    break;
    
  default:
    break;
    
  }
}

void xFitShapeAll(Int_t runId=239821) {
  
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
  sprintf(fname, "beamsplash_%08d.root", runId);
  TFile *f = new TFile(fname);
  TTree *EventTree = (TTree*) f->Get("EventTree");
  
  //Declaration of leaves types
  Int_t           run;
  Int_t           lumis;
  Int_t           event;
  Int_t           ADC0[2][2][40][40][32];
  Int_t           ADC1[2][2][40][40][32];
  Int_t           ADC2[2][2][40][40][32];
  Int_t           cADC0[2][2][40][40][32];
  Int_t           cADC1[2][2][40][40][32];
  Int_t           cADC2[2][2][40][40][32];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("lumis",&lumis);
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

  TH1F *hT0espf = new TH1F("hT0espf", "T0 ES+ F", 100, -50, 50);
  TH1F *hT0espr = new TH1F("hT0espr", "T0 ES+ R", 100, -50, 50);
  TH1F *hT0esmf = new TH1F("hT0esmf", "T0 ES- F", 100, -50, 50);
  TH1F *hT0esmr = new TH1F("hT0esmr", "T0 ES- R", 100, -50, 50);

  TH1F *hADC1espf = new TH1F("hADC1espf", "ADC1 ES+ F", 4095, 0, 4095);
  TH1F *hADC1espr = new TH1F("hADC1espr", "ADC1 ES+ R", 4095, 0, 4095);
  TH1F *hADC1esmf = new TH1F("hADC1esmf", "ADC1 ES- F", 4095, 0, 4095);
  TH1F *hADC1esmr = new TH1F("hADC1esmr", "ADC1 ES- R", 4095, 0, 4095);
  
  TH2F *hespf = new TH2F("hespf", "ES+ F", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hespr = new TH2F("hespr", "ES+ R", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hesmf = new TH2F("hesmf", "ES- F", 40, 0.5, 40.5, 40, 0.5, 40.5);
  TH2F *hesmr = new TH2F("hesmr", "ES- R", 40, 0.5, 40.5, 40, 0.5, 40.5);

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  c1->Divide(2,2);
  TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
  TCanvas *c3 = new TCanvas("c3", "c3", 900, 600);
  c3->Divide(2,2);
  TCanvas *c4 = new TCanvas("c4", "c4", 900, 600);
  c4->Divide(2,2);

  Int_t evId = -1;
  Int_t nAnalyzedEvents = 0;

  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);
    
    hMIP->Reset();

    hT0espf->Reset();
    hT0espr->Reset();
    hT0esmf->Reset();
    hT0esmr->Reset();

    hADC1espf->Reset();
    hADC1espr->Reset();
    hADC1esmf->Reset();
    hADC1esmr->Reset();

    hespf->Reset();
    hespr->Reset();
    hesmf->Reset();
    hesmr->Reset();

    nAnalyzedEvents++;
    if (nAnalyzedEvents < 73) continue;
    //if (run==239821 && event!=28658) continue;
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

	      if (z==0 && p==0) hT0espf->Fill(para[1]);
	      if (z==0 && p==1) hT0espr->Fill(para[1]);
	      if (z==1 && p==0) hT0esmf->Fill(para[1]);
	      if (z==1 && p==1) hT0esmr->Fill(para[1]);

	      if (z==0 && p==0) hADC1espf->Fill(counts[j][1]);
	      if (z==0 && p==1) hADC1espr->Fill(counts[j][1]);
	      if (z==1 && p==0) hADC1esmf->Fill(counts[j][1]);
	      if (z==1 && p==1) hADC1esmr->Fill(counts[j][1]);

	      navt++;
	      avt += para[1];
	      /*
	      c2->cd();
	      gr->SetTitle("");
	      gr->Draw("a");
	      gr->SetMarkerStyle(20);
	      gr->Draw("ap");
	      c2->Update();
	      cout<<z<<" "<<p<<" "<<x<<" "<<y<<" "<<j<<" "<<para[0]<<" "<<para[1]<<endl;
	      cout<<counts[j][0]<<" "<<counts[j][1]<<" "<<counts[j][2]<<endl;
	      cin.get();
	      */
	      
	    }
	    //cout<<x+1<<" "<<y+1<<" "<<avt<<" "<<navt<<endl;
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

    cout<<"=== "<<run<<" === "<<lumis<<" === "<<event<<endl;
    cout<<"Done"<<endl;

    Char_t tname[200];
    sprintf(tname, "Run %d LS %d Event %d", runId, lumis, evId);  
    c2->cd();
    hMIP->SetTitle(tname);
    hMIP->GetXaxis()->SetTitle("Deposited energy per strip (KeV)");
    hMIP->SetLineWidth(3);
    hMIP->SetLineColor(4);
    hMIP->Draw();
    c2->Update();

    c3->cd(1);
    sprintf(tname, "Run %d LS %d Ev %d ES+ F", runId, lumis, evId);  
    hT0espf->SetTitle(tname);
    hT0espf->GetXaxis()->SetTitle("T0 (ns)");
    hT0espf->SetLineWidth(3);
    hT0espf->SetLineColor(4);
    hT0espf->Draw();
    c3->cd(2);
    sprintf(tname, "Run %d LS %d Ev %d ES- F", runId, lumis, evId);  
    hT0esmf->SetTitle(tname);
    hT0esmf->GetXaxis()->SetTitle("T0 (ns)");
    hT0esmf->SetLineWidth(3);
    hT0esmf->SetLineColor(4);
    hT0esmf->Draw();
    c3->cd(3);
    sprintf(tname, "Run %d LS %d Ev %d ES+ R", runId, lumis, evId);  
    hT0espr->SetTitle(tname);
    hT0espr->GetXaxis()->SetTitle("T0 (ns)");
    hT0espr->SetLineWidth(3);
    hT0espr->SetLineColor(4);
    hT0espr->Draw();
    c3->cd(4);
    sprintf(tname, "Run %d LS %d Ev %d ES- R", runId, lumis, evId);  
    hT0esmr->SetTitle(tname);
    hT0esmr->GetXaxis()->SetTitle("T0 (ns)");
    hT0esmr->SetLineWidth(3);
    hT0esmr->SetLineColor(4);
    hT0esmr->Draw();
    c3->Update();
    
    c4->cd(1);
    sprintf(tname, "Run %d LS %d Ev %d ES+ F", runId, lumis, evId);  
    hADC1espf->SetTitle(tname);
    hADC1espf->GetXaxis()->SetTitle("ADC 1");
    hADC1espf->SetLineWidth(3);
    hADC1espf->SetLineColor(4);
    hADC1espf->Draw();
    c4->cd(2);
    sprintf(tname, "Run %d LS %d Ev %d ES- F", runId, lumis, evId);  
    hADC1esmf->SetTitle(tname);
    hADC1esmf->GetXaxis()->SetTitle("ADC 1");
    hADC1esmf->SetLineWidth(3);
    hADC1esmf->SetLineColor(4);
    hADC1esmf->Draw();
    c4->cd(3);
    sprintf(tname, "Run %d LS %d Ev %d ES+ R", runId, lumis, evId);  
    hADC1espr->SetTitle(tname);
    hADC1espr->GetXaxis()->SetTitle("ADC 1");
    hADC1espr->SetLineWidth(3);
    hADC1espr->SetLineColor(4);
    hADC1espr->Draw();
    c4->cd(4);
    sprintf(tname, "Run %d LS %d Ev %d ES- R", runId, lumis, evId);  
    hADC1esmr->SetTitle(tname);
    hADC1esmr->GetXaxis()->SetTitle("ADC 1");
    hADC1esmr->SetLineWidth(3);
    hADC1esmr->SetLineColor(4);
    hADC1esmr->Draw();
    c4->Update();    

    Float_t tMax = 20;
    Float_t tMin = -40;
    c1->cd(1);
    sprintf(tname, "Run %d LS %d Ev %d ES+ F", runId, lumis, evId);  
    hespf->SetTitle(tname);
    hespf->SetMaximum(tMax);
    hespf->SetMinimum(tMin);
    hespf->GetXaxis()->SetTitle("Si X");
    hespf->GetYaxis()->SetTitle("Si Y");
    hespf->GetZaxis()->SetTitle("TO (ns)");
    hespf->Draw("colz");
    drawBorders( 1, 0.5, 0.5 );
    c1->cd(2);
    sprintf(tname, "Run %d LS %d Ev %d ES- F", runId, lumis, evId);  
    hesmf->SetTitle(tname);
    hesmf->SetMaximum(tMax);
    hesmf->SetMinimum(tMin);
    hesmf->GetXaxis()->SetTitle("Si X");
    hesmf->GetYaxis()->SetTitle("Si Y");
    hesmf->GetZaxis()->SetTitle("TO (ns)");
    hesmf->Draw("colz");
    drawBorders( 2, 0.5, 0.5 );
    c1->cd(3);
    sprintf(tname, "Run %d LS %d Ev %d ES+ R", runId, lumis, evId);  
    hespr->SetTitle(tname);
    hespr->SetMaximum(tMax);
    hespr->SetMinimum(tMin);
    hespr->GetXaxis()->SetTitle("Si X");
    hespr->GetYaxis()->SetTitle("Si Y");
    hespr->GetZaxis()->SetTitle("TO (ns)");
    hespr->Draw("colz");
    drawBorders( 3, 0.5, 0.5 );
    c1->cd(4);
    sprintf(tname, "Run %d LS %d Ev %d ES- R", runId, lumis, evId);  
    hesmr->SetTitle(tname);
    hesmr->SetMaximum(tMax);
    hesmr->SetMinimum(tMin);
    hesmr->GetXaxis()->SetTitle("Si X");
    hesmr->GetYaxis()->SetTitle("Si Y");
    hesmr->GetZaxis()->SetTitle("TO (ns)");
    hesmr->Draw("colz");
    drawBorders( 4, 0.5, 0.5 );
    c1->Update();    

    sprintf(tname, "es_timing_%d_%d_%d.png", runId, lumis, evId);  
    c1->Print(tname);
    sprintf(tname, "energy_spectrum_%d_%d_%d.png", runId, lumis, evId);  
    c2->Print(tname);
    sprintf(tname, "t0_%d_%d_%d.png", runId, lumis, evId);  
    c3->Print(tname);
    sprintf(tname, "ADC1_%d_%d_%d.png", runId, lumis, evId);  
    c4->Print(tname);
    
    //cin.get();
  }

}

