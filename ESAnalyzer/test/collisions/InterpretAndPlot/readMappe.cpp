//g++ -Wall -o readMappe `root-config --cflags --glibs` readMappe.cpp
// ./readMappe Zside Pside

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
//#include <pair>



//void doMappe()
int main(int argc, char* argv[])
{

  std::string Zside = std::string(argv[1]);
  int Zside_i = atoi(argv[1]);
  std::cout << " >>> Zside = " << Zside << std::endl;

  std::string Pside = std::string(argv[2]);
  int Pside_i = atoi(argv[2]);
  std::cout << " >>> Pside = " << Pside << std::endl;

  gROOT->ProcessLine(".L /Users/Arabella/Public/RootLogon/rootLogon.C");
  gROOT->ProcessLine(".x /Users/Arabella/Public/RootLogon/rootPalette.C");

  //  gStyle->SetPalette( 1, 0 );
  //  gStyle->SetOptStat(0,0,0,0);

  TH2F* map_Z0P0 = new TH2F(Form("map_Z%dP%d", Zside_i, Pside_i), "", 40, 0., 40., 40, 0., 40.);
  TH2F* map_Z0P0_check = new TH2F(Form("map_Z%dP%d_check", Zside_i, Pside_i), "", 40, 0., 40., 40, 0., 40.);

  std::map<std::string, float> lad_time;
  std::map<std::string, int> lad_N;
  std::map<std::pair<int, int>, std::string  > sensor_ladder;

  lad_time.clear();
  lad_N.clear();
  sensor_ladder.clear();

  int iz, ip, ix, iy;
  float time, rms, chi2, occ;
  int iZ, iP, iX, iY;
  std::string iL;


  std::ifstream inFileTime;
  //  inFileTime.open(Form("SensorLadder_map/timingSensor_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);
  inFileTime.open(Form("SensorLadder_map/timingSensor_RunB_LG_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);

  int line = 0;
  std::ifstream inFileLong;
  inFileLong.open(Form("SensorLadder_map/outMap_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);
  while (!inFileLong.eof())
    {
      inFileLong >> iZ >> iP >> iX >> iY >> iL;
      inFileTime >> iz >> ip >> ix >> iy >> time >> rms >> chi2 >> occ;
      //      std::cout << " ix = " << ix << " iy = " << iy << std::endl;

      if(iZ != iz || iP != ip || ix != iX || iy != iY) {
	std::cout << " serius problem " << std::endl;	
	std::cout << " line = " << line << std::endl;
	std::cout << "iZ = " << iZ << " iP = " << iP << " iX = " << iX << " iY = " << iY << std::endl;
	std::cout << "iz = " << iz << " ip = " << ip << " ix = " << ix << " iy = " << iy << std::endl;
	return 100;
      }
      ++line;
      std::pair<int, int> dummyPair(ix, iy);
      sensor_ladder[dummyPair] = iL;
      if(time > -900) {
	lad_time[iL] += time;
	lad_N[iL] += 1;
      }      
    }
  inFileLong.close ();
  inFileTime.close ();

  //  std::ifstream inFileLong;
  //  inFileTime.open(Form("SensorLadder_map/timingSensor_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);
  inFileTime.open(Form("SensorLadder_map/timingSensor_RunB_LG_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);
  inFileLong.open(Form("SensorLadder_map/outMap_Z%dP%d.txt", Zside_i, Pside_i), std::ios::in);
  while (!inFileLong.eof())
    {
      inFileLong >> iZ >> iP >> iX >> iY >> iL;
      inFileTime >> iz >> ip >> ix >> iy >> time >> rms >> chi2 >> occ;
      //std::cout << " ix = " << ix << " iy = " << iy << " t = " << time << " rms = " << rms << " chi2 = " << chi2 << " occ = " << occ << std::endl;


      if(time > -900 && iL != "058") {
	map_Z0P0->Fill(iX, iY, lad_time[iL] / lad_N[iL]);
	map_Z0P0_check->Fill(iX, iY, time);
 	if(ix == 24 || ix == 23) {
 	  std::cout << "iz = " << iz << " ip = " << ip << " ix = " << ix << " iy = " << iy << std::endl;
 	  std::cout << " >>> lad_time[iL] / lad_N[iL] = " << lad_time[iL] / lad_N[iL] << std::endl;
 	  std::cout << " >>> lad_time[iL] = " << lad_time[iL] << std::endl;
 	}
      }
      if(iL == "058") 	{
	map_Z0P0->Fill(iX, iY, -20.);
	map_Z0P0_check->Fill(iX, iY, time);
      }
//       else{
// 	map_Z0P0->Fill(ix, iy, lad_time[il] / lad_N[il]);
//         map_Z0P0_check->Fill(ix, iy, time);
//      }
    }
  inFileLong.close ();


  for (int k=0; k<40; ++k){
    for (int m=0; m<40; ++m){
      if(map_Z0P0_check->GetBinContent(k+1, m+1) == 0) {
	//	std::cout << " k = " << k  << " m = " << m<< std::endl;
	map_Z0P0->SetBinContent(k+1, m+1, -1000000.);
	map_Z0P0_check->SetBinContent(k+1, m+1, -1000000.);
      }
    }
  }



  TCanvas* cc = new TCanvas();
  //      gPad->SetLogz();
  cc->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  map_Z0P0->SetStats(0);
  map_Z0P0->GetXaxis()->SetTitle("x");
  map_Z0P0->GetYaxis()->SetTitle("y");
  //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
  map_Z0P0->GetZaxis()->SetRangeUser(-10., 10.);
  map_Z0P0->Draw("colz");
  cc->Print(Form("Run2015_B/mappe_Run2015_B/Average_timing_Z%dP%d.png", Zside_i, Pside_i), "png");
  cc->Print(Form("Run2015_B/mappe_Run2015_B/Average_timing_Z%dP%d.pdf", Zside_i, Pside_i), "pdf");

  //      gPad->SetLogz();
  cc->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  map_Z0P0_check->SetStats(0);
  map_Z0P0_check->GetXaxis()->SetTitle("x");
  map_Z0P0_check->GetYaxis()->SetTitle("y");
  //mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
  map_Z0P0_check->GetZaxis()->SetRangeUser(-10., 10.);
  map_Z0P0_check->Draw("colz");
  cc->Print(Form("Run2015_B/mappe_Run2015_B/Average_timing_Z%dP%d_check.png", Zside_i, Pside_i), "png");
  cc->Print(Form("Run2015_B/mappe_Run2015_B/Average_timing_Z%dP%d_check.pdf", Zside_i, Pside_i), "pdf");
 
  return 0;
 
}
