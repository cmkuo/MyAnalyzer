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

void xEventDisplay(Int_t runId=120012) {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  gStyle->SetPadRightMargin(0.18);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadTopMargin(0.18);
  gStyle->SetPadBottomMargin(0.18);
 
  Char_t fname[200];
  //sprintf(fname, "/data3/ncuhep/splash/ana/splash_%08d.root", runId);
  sprintf(fname, "../trees/tree_BeamSplash_1257641879.root");
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
  Double_t        RH[2][2][40][40][32];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("event",&event);
  EventTree->SetBranchAddress("ADC0",ADC0);
  EventTree->SetBranchAddress("ADC1",ADC1);
  EventTree->SetBranchAddress("ADC2",ADC2);
  EventTree->SetBranchAddress("cADC0",cADC0);
  EventTree->SetBranchAddress("cADC1",cADC1);
  EventTree->SetBranchAddress("cADC2",cADC2);
  EventTree->SetBranchAddress("RH",RH);
  
  Int_t tx[3], counts[32][3], ccounts[32][3];
  Long64_t nentries = EventTree->GetEntries();
  
  TF1 *func = new TF1("func", fitf, -200, 200, 2);
  func->SetLineWidth(2);

  TCanvas *c1 = new TCanvas("c1", "c1", 900, 900);
  TCanvas *c2 = new TCanvas("c2", "c2", 900, 900);
  TCanvas *c3 = new TCanvas("c3", "c3", 900, 900);
  TCanvas *c4 = new TCanvas("c4", "c4", 900, 900);
  TCanvas *c5 = new TCanvas("c5", "c5", 900, 900);
  TCanvas *c6 = new TCanvas("c6", "c6", 900, 900);
  TCanvas *c7 = new TCanvas("c7", "c7", 900, 900);
  TCanvas *c8 = new TCanvas("c8", "c8", 900, 900);

  Char_t hname[200];
  TH2F *h2[2][2];
  TH2F *hmu[2][2];
  for (int j=0; j<2; ++j)
    for (int k=0; k<2; ++k) {
      sprintf(hname, "h2_z%d_p%d", j, k);
      h2[j][k] = new TH2F(hname, hname, 40, 0.5, 40.5, 40, 0.5, 40.5);
      sprintf(hname, "hmu_z%d_p%d", j, k);
      hmu[j][k] = new TH2F(hname, hname, 40, 0.5, 40.5, 40, 0.5, 40.5);
    }

  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);

    cout<<"=== "<<run<<" === "<<event<<endl;
    //if (event != 453) continue;
    /*
    for (int j=0; j<2; ++j) 
      for (int k=0; k<2; ++k) {
	h2[j][k]->Reset();
	hmu[j][k]->Reset();
      }
    */
    for (int z=0; z<2; ++z) 
      for (int p=0; p<2; ++p)
	for (int x=0; x<40; ++x) 
	  for (int y=0; y<40; ++y) 
	    for (int j=0; j<32; ++j) {
	      
	      counts[j][0] = ADC0[z][p][x][y][j];
	      counts[j][1] = ADC1[z][p][x][y][j];
	      counts[j][2] = ADC2[z][p][x][y][j];

	      Int_t t0 = -5;
	      tx[0] = t0;
	      tx[1] = t0+25;
	      tx[2] = t0+50;       	      	      
	      if (counts[j][0] > -10000) {
		
		Double_t para[10];    
		/*
		TGraph *gr = new TGraph(3, tx, counts[j]); 
		func->SetParameters(50, 10);
		gr->Fit("func", "MQ");
		func->GetParameters(para);
		*/
		para[0] = RH[z][p][x][y][j]*1000000./81.08*55.;
		
		//cout<<z<<" "<<p<<" "<<x<<" "<<y<<" "<<j<<" "<<para[0]<<endl;
		if (para[0] > 0) {
		  h2[z][p]->Fill(x+1, y+1, para[0]);
		  hmu[z][p]->Fill(x+1, y+1, para[0]/55.);
		} else {
		  h2[z][p]->Fill(x+1, y+1, 0);
		  hmu[z][p]->Fill(x+1, y+1, 0);
		}
	      } else {
		h2[z][p]->Fill(x+1, y+1, 0);
		hmu[z][p]->Fill(x+1, y+1, 0);
	      }
	    }
    /*
    c1->cd();
    h2[0][0]->SetTitle("ES+ F");
    h2[0][0]->Draw("colz");        
    drawBorders( 1, 0.5, 0.5 );
    c1->Update();
    c2->cd();
    h2[0][1]->SetTitle("ES+ R");
    h2[0][1]->Draw("colz");        
    drawBorders( 3, 0.5, 0.5 );
    c2->Update();
    c3->cd();
    h2[1][0]->SetTitle("ES- F");
    h2[1][0]->Draw("colz");        
    drawBorders( 2, 0.5, 0.5 );
    c3->Update();
    c4->cd();
    h2[1][1]->SetTitle("ES- R");
    h2[1][1]->Draw("colz");        
    drawBorders( 4, 0.5, 0.5 );
    c4->Update();   
    */
    //hmu[0][0]->SetMaximum(350);
    //hmu[0][1]->SetMaximum(350);
    //hmu[1][0]->SetMaximum(350);
    //hmu[1][1]->SetMaximum(350);
    c5->cd();
    hmu[0][0]->SetTitle("ES+ Front ");
    hmu[0][0]->GetXaxis()->SetTitle("Si X");
    hmu[0][0]->GetYaxis()->SetTitle("Si Y");
    hmu[0][0]->GetZaxis()->SetTitle("Number of MIPs");
    hmu[0][0]->GetZaxis()->SetTitleOffset(1.5);
    hmu[0][0]->Draw("colz");        
    drawBorders( 1, 0.5, 0.5 );
    c5->Update();
    c6->cd();
    hmu[0][1]->SetTitle("ES+ Rear");
    hmu[0][1]->GetXaxis()->SetTitle("Si X");
    hmu[0][1]->GetYaxis()->SetTitle("Si Y");
    hmu[0][1]->GetZaxis()->SetTitle("Number of MIPs");
    hmu[0][1]->GetZaxis()->SetTitleOffset(1.5);
    hmu[0][1]->Draw("colz");        
    drawBorders( 3, 0.5, 0.5 );
    c6->Update();
    c7->cd();
    hmu[1][0]->SetTitle("ES- Front");
    hmu[1][0]->GetXaxis()->SetTitle("Si X");
    hmu[1][0]->GetYaxis()->SetTitle("Si Y");
    hmu[1][0]->GetZaxis()->SetTitle("Number of MIPs");
    hmu[1][0]->GetZaxis()->SetTitleOffset(1.5);
    hmu[1][0]->Draw("colz");        
    drawBorders( 2, 0.5, 0.5 );
    c7->Update();
    c8->cd();
    hmu[1][1]->SetTitle("ES- Rear");
    //TGaxis::SetMaxDigits(2);
    hmu[1][1]->GetXaxis()->SetTitle("Si X");
    hmu[1][1]->GetYaxis()->SetTitle("Si Y");
    hmu[1][1]->GetZaxis()->SetTitle("Number of MIPs");
    hmu[1][1]->GetZaxis()->SetTitleOffset(1.5);
    hmu[1][1]->Draw("colz");        
    drawBorders( 4, 0.5, 0.5 );
    c8->Update();   

    //c1->Print("espf_en.eps");
    //c2->Print("espr_en.eps");
    //c3->Print("esmf_en.eps");
    //c4->Print("esmr_en.eps");
    //c5->Print("espf_nmu.png");
    //c6->Print("espr_nmu.png");
    //c7->Print("esmf_nmu.png");
    //c8->Print("esmr_nmu.png");

    //cin.get();
  }

}
