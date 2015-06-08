// g++ -Wall -o doMapTiming `root-config --cflags --glibs` doMapTiming.cpp


#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TPaveStats.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TTree.h"
#include "TChain.h"
#include "TVirtualFitter.h"
#include "TLorentzVector.h"
#include "TLine.h"

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

// #include "xCalcEffSigma.h"
// #include "ScaleEstimators.h"

int main(){
  gROOT->ProcessLine(".L ~/Settings/loader.C+");
  gROOT->ProcessLine(".x ~/public/style.C");

  gStyle->SetPalette( 1, 0 );


  //  TFile* fHistos = TFile::Open("coll_timing_Run_245137_246092_MinB_hadded.root", "read");
  TFile* fHistos = TFile::Open("./collision_Ming_timing_245192_rejectedHits.root", "read");

  TH1F* hT[2][2][40][40];
  TH1F* hTiming[2][2];
  for (int i=0; i<2; ++i) 
    for (int j=0; j<2; ++j) {

      hTiming[i][j] = (TH1F*)fHistos->Get(Form("Z%d_P%d", i, j));

      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m) {
	  hT[i][j][k][m] = (TH1F*)fHistos->Get(Form("Z%d_P%d_X%d_Y%d", i, j, k, m));

	}
    }




 TH2F* mapT[2][2];
 TH2F* mapOccupancy[2][2];
 TH2F* mapTrms[2][2];
 TH2F* mapTChi2[2][2];

 for (int i=0; i<2; ++i){
   for (int j=0; j<2; ++j) {

     mapT[i][j] = new TH2F(Form("mapT_Z%d_P%d", i, j), "", 40, 0., 40., 40, 0., 40.);
     mapT[i][j]->Sumw2();
     mapTrms[i][j] = new TH2F(Form("mapTrms_Z%d_P%d", i, j), "", 40, 0., 40., 40, 0., 40.);
     mapTrms[i][j]->Sumw2();
     mapOccupancy[i][j] = new TH2F(Form("mapOccupancy_Z%d_P%d", i, j), "", 40, 0., 40., 40, 0., 40.);
     mapOccupancy[i][j]->Sumw2();
     mapTChi2[i][j] = new TH2F(Form("mapTChi2_Z%d_P%d", i, j), "", 40, 0., 40., 40, 0., 40.);
     mapTChi2[i][j]->Sumw2();

     for (int k=0; k<40; ++k){
       for (int m=0; m<40; ++m){

	 mapOccupancy[i][j]->Fill(k, m, hT[i][j][k][m]->GetEntries());

	 if(hT[i][j][k][m]->GetEntries() > 0.){ 
	   
	 TF1* fitFunc = new TF1("fitFunc", "gaus", -30., 30.);
	 fitFunc->SetParameters(hT[i][j][k][m]->Integral(), hT[i][j][k][m]->GetMean(), hT[i][j][k][m]->GetRMS());
	 hT[i][j][k][m]->Fit("fitFunc");


	 TCanvas* cc = new TCanvas();
	 cc->cd();
	 hT[i][j][k][m]->Draw();
	 fitFunc->SetLineColor(kRed);
	 fitFunc->Draw("same");
//	 cc->Print(Form("plots_AllRuns/sensor_Z%d_P%d_X%d_Y%d.png", i, j, k, m), ".png");
	 cc->Print(Form("plots_AllRuns_rejectedHits/sensor_Z%d_P%d_X%d_Y%d.png", i, j, k, m), ".png");


	 mapT[i][j]->Fill(k, m, fitFunc->GetParameter(1));
	 mapTrms[i][j]->Fill(k, m, hT[i][j][k][m]->GetRMS());
	 mapTChi2[i][j]->Fill(k, m, fitFunc->GetChisquare()/fitFunc->GetNDF());
	 
	 delete fitFunc; 
	 }
	 // else {

	 // }
       }
     }
   }
 }

 // TFile *fout = new TFile("Ming_timingMap_AllRuns_245137_246092_MinB.root", "recreate");
 TFile *fout = new TFile("Ming_timingMap_AllRuns_rejectedHits.root", "recreate");
 fout->cd();

for (int i=0; i<2; ++i){
   for (int j=0; j<2; ++j) {

     mapT[i][j]->Write();
     mapTrms[i][j]->Write();
     mapOccupancy[i][j]->Write();
     mapTChi2[i][j]->Write();

     /*
     TCanvas* cc = new TCanvas();
     cc->cd();
     mapT[i][j]->GetXaxis()->SetTitle("x");
     mapT[i][j]->GetYaxis()->SetTitle("y");
     mapT[i][j]->Draw("colz");
     cc->Print(Form("mappe_AllRuns/Run_All_timing_Z%d_P%d.png", i, j), "png");

     mapOccupancy[i][j]->GetXaxis()->SetTitle("x");
     mapOccupancy[i][j]->GetYaxis()->SetTitle("y");
     mapOccupancy[i][j]->Draw("colz");
     cc->Print(Form("mappe_AllRuns/Run_All_occupancy_Z%d_P%d.png", i, j), "png");
     */
   }
 }
 fout->Close();



}
