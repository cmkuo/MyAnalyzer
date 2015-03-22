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
#include "TChain.h"

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

      case 1:	//ES+F
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

void xEnergyCor(Int_t runId=120012) {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);
 
  Char_t fname[200];
  //sprintf(fname, "../trees/tree_BeamSplash_1257641879.root");
  //TFile *f = new TFile(fname);
  //TTree *EventTree = (TTree*) f->Get("EventTree");

  TChain *EventTree = new TChain("EventTree");
  sprintf(fname, "../trees/tree_BeamSplash_1257*.root");
  EventTree->Add(fname);
  
  //Declaration of leaves type
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
  Double_t        RH[2][2][40][40][32];
  Double_t        eep;
  Double_t        eem;
  Double_t        hep;
  Double_t        hem;
  
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
  EventTree->SetBranchAddress("RH",RH);
  EventTree->SetBranchAddress("eep",&eep);
  EventTree->SetBranchAddress("eem",&eem);
  EventTree->SetBranchAddress("hep",&hep);
  EventTree->SetBranchAddress("hem",&hem);

  Int_t tx[3], counts[32][3], ccounts[32][3];
  Long64_t nentries = EventTree->GetEntries();
  
  TH2F *hp = new TH2F("hp", "ES+", 1000, 50000, 300000, 1000, 50000, 300000);
  TH2F *hm = new TH2F("hm", "ES-", 1000, 50000, 300000, 1000, 50000, 300000);
  TH2F *h2 = new TH2F("h2", "ES+F vs ES-F", 1000, 50000, 300000, 1000, 50000, 300000);

  Int_t nev = 0;
  Double_t en_eep[5000];
  Double_t en_eem[5000];
  Double_t en_hep[5000];
  Double_t en_hem[5000];
  Double_t en_esp[5000];
  Double_t en_esm[5000];

  Char_t hname[200];
  Double_t enp[2];
  Double_t enm[2];
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);

    if (bx != 100) continue;
    cout<<"=== "<<run<<" === "<<event<<endl;
    for (int p=0; p<2; ++p) {
      enp[p] = 0;
      enm[p] = 0;
    }

    for (int z=0; z<2; ++z) 
      for (int p=0; p<2; ++p)
	for (int x=0; x<40; ++x) 
	  for (int y=0; y<40; ++y) 
	    for (int j=0; j<32; ++j) {
	      
	      counts[j][0] = ADC0[z][p][x][y][j];
	      counts[j][1] = ADC1[z][p][x][y][j];
	      counts[j][2] = ADC2[z][p][x][y][j];

	      if (counts[j][0] > -10000) {
		
		double rh = RH[z][p][x][y][j]*1000000./81.08;

		if (rh > 100) continue;

		if (z==0) enp[p] += rh;
		if (z==1) enm[p] += rh;
		
	      }
	    }
    
    // Fill histograms
    hp->Fill(enp[0], enp[1]);
    hm->Fill(enm[0], enm[1]);
    h2->Fill(enp[0], enm[0]);

    //if ( (enp[0] + enp[1])/1000 < 350 && (hep/1000)>2100) {
      
      en_eep[nev] = eep/1000;
      en_eem[nev] = eem/1000;
      en_hep[nev] = hep/1000;
      en_hem[nev] = hem/1000;
      en_esp[nev] = (enp[0] + enp[1])/1000;
      en_esm[nev] = (enm[0] + enm[1])/1000;
      
      //cout<<en_esp[nev]<<" "<<en_eep[nev]<<" "<<en_hep[nev]<<endl;
      //cout<<en_esm[nev]<<" "<<en_eem[nev]<<" "<<en_hem[nev]<<endl;
      
      nev++;
      
      //}
  }

  TGraph *gr_eehe_p = new TGraph(nev, en_eep, en_hep);
  TGraph *gr_eehe_m = new TGraph(nev, en_eem, en_hem);
  TGraph *gr_eees_p = new TGraph(nev, en_eep, en_esp);
  TGraph *gr_eees_m = new TGraph(nev, en_eem, en_esm);
  TGraph *gr_eshe_p = new TGraph(nev, en_esp, en_hep);
  TGraph *gr_eshe_m = new TGraph(nev, en_esm, en_hem);

  TCanvas *c1 = new TCanvas("c1", "c1", 900, 300);
  c1->Divide(3,1);
  c1->cd(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  hp->GetXaxis()->SetTitle("E_{ES+F} (MIPs)");
  hp->GetYaxis()->SetTitle("E_{ES+R} (MIPs)");
  hp->SetMaximum(5);
  hp->Draw("colz");
  c1->cd(2);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  hm->GetXaxis()->SetTitle("E_{ES-F} (MIPs)");
  hm->GetYaxis()->SetTitle("E_{ES-R} (MIPs)");
  hm->SetMaximum(5);
  hm->Draw("colz");
  c1->cd(3);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  h2->GetXaxis()->SetTitle("E_{ES+F} (MIPs)");
  h2->GetYaxis()->SetTitle("E_{ES-F} (MIPs)");
  h2->SetMaximum(5);
  h2->Draw("colz");

  TCanvas *c2 = new TCanvas("c2", "c2", 700, 500);
  c2->Divide(2,1);
  c2->cd(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eehe_p->Draw("a");
  gr_eehe_p->SetTitle("EE+ VS HE+");
  gr_eehe_p->SetMarkerColor(4);
  gr_eehe_p->SetMarkerStyle(20);
  gr_eehe_p->Draw("ap");
  c2->cd(2);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eehe_m->Draw("a");
  gr_eehe_m->SetTitle("EE- VS HE-");
  gr_eehe_m->SetMarkerColor(4);
  gr_eehe_m->SetMarkerStyle(20);
  gr_eehe_m->Draw("ap");

  TCanvas *c3 = new TCanvas("c3", "c3", 700, 500);
  c3->Divide(2,1);
  c3->cd(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eees_p->Draw("a");
  gr_eees_p->SetTitle("EE+ VS ES+");
  gr_eees_p->SetMarkerColor(4);
  gr_eees_p->SetMarkerStyle(20);
  gr_eees_p->Draw("ap");
  c3->cd(2);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eees_m->Draw("a");
  gr_eees_m->SetTitle("EE- VS ES-");
  gr_eees_m->SetMarkerColor(4);
  gr_eees_m->SetMarkerStyle(20);
  gr_eees_m->Draw("ap");

  TCanvas *c4 = new TCanvas("c4", "c4", 700, 500);
  c4->Divide(2,1);
  c4->cd(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eshe_p->Draw("a");
  gr_eshe_p->SetTitle("ES+ VS HE+");
  gr_eshe_p->SetMarkerColor(4);
  gr_eshe_p->SetMarkerStyle(20);
  gr_eshe_p->Draw("ap");
  c4->cd(2);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr_eshe_m->Draw("a");
  gr_eshe_m->SetTitle("ES- VS HE-");
  gr_eshe_m->SetMarkerColor(4);
  gr_eshe_m->SetMarkerStyle(20);
  gr_eshe_m->Draw("ap");

  c2->Print("eehe.png");
  c3->Print("eees.png");
  c4->Print("eshe.png");
}
