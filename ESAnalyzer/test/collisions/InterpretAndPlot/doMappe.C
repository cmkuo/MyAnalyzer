#include <iomanip>
#include <iostream>


bool Check_list(int i, int j, int k, int m, std::vector<int>& local_k, std::vector<int>& local_m){
  bool pippo = false;
  if(i == 0 && j == 0){
  int sizeList = 49;
  int zList[49]       = {0,   0, 0,   0,  0,  0,  0,  0,  0,  0,  0   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  int pList[49]       = {0,   0, 0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  int kList[49]       = {1,   2, 14, 14, 14, 14, 16, 16, 18, 18, 21, 21, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 35, 38};
  int mList[49]       = {14, 12, 24, 24, 24, 24, 26, 26, 27, 27, 27, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 24, 24, 24, 24, 14, 14};
  int local_kList[49] = {1,   1, 14, 14, 14, 14, 16, 16, 18, 18, 21, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 35, 38};
  int local_mList[49] = {13, 26, 16, 17, 22, 23, 14, 25, 13, 26, 13, 26, 14, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 16, 17, 22, 23, 13, 13};
  
  for(int iC=0; iC<sizeList; ++iC){
    if(i == zList[iC] && j == pList[iC] && k == kList[iC] && m == mList[iC]){
      local_k.push_back(local_kList[iC]);
      local_m.push_back(local_mList[iC]);
      pippo = true;
    }
  }
  }

  if(i == 0 && j == 1){
  int sizeList = 52;
  int zList[52]       = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  int pList[52]       = { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
  int kList[52]       = { 2,  2,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 22, 22, 23, 23, 25, 25, 26, 26, 26, 27};
  int mList[52]       = {10, 10, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,  2, 22, 22, 27, 27,  1, 24, 24, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 24, 24,  2, 22, 22,  2};
  int local_kList[52] = { 1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 22, 22, 23, 23, 25, 25, 26, 26, 26, 26};
  int local_mList[52] = {25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26,  1, 18, 21, 25, 26, 38, 16, 23, 25, 26, 25, 26, 14, 25, 26, 14, 25, 26, 14, 25, 14, 25, 16, 23,  1, 18, 21, 38};
  
  for(int iC=0; iC<sizeList; ++iC){
    if(i == zList[iC] && j == pList[iC] && k == kList[iC] && m == mList[iC]){
      local_k.push_back(local_kList[iC]);
      local_m.push_back(local_mList[iC]);
      pippo = true;
    }
  }
  }

  if(i == 1 && j == 0){
  int sizeList = 58;
  int zList[58]       = { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
  int pList[58]       = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  int kList[58]       = { 1,  2, 14, 14, 14, 14, 16, 16, 18, 18, 21, 21, 23, 23, 23, 24, 25, 25, 25, 25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 38};
  int mList[58]       = {14, 12, 24, 24, 24, 24, 26, 26, 27, 27, 27, 27, 26, 26, 33, 33, 24, 24, 24, 24,  6, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 14};
  int local_kList[58] = { 1,  1, 14, 14, 14, 14, 16, 16, 18, 18, 21, 21, 23, 23, 23, 24, 25, 25, 25, 25, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 38};
  int local_mList[58] = {13, 26, 16, 17, 22, 23, 14, 25, 13, 26, 13, 26, 14, 25, 32, 32, 16, 17, 22, 23,  5,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 13};
  
  for(int iC=0; iC<sizeList; ++iC){
    if(i == zList[iC] && j == pList[iC] && k == kList[iC] && m == mList[iC]){
      local_k.push_back(local_kList[iC]);
      local_m.push_back(local_mList[iC]);
      pippo = true;
    }
  }
  }

  if(i == 1 && j == 1){
  int sizeList = 20;
  int zList[20]       = { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
  int pList[20]       = { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
  int kList[20]       = {13, 13, 13, 14, 14, 14, 16, 16, 17, 17, 22, 22, 23, 23, 25, 25, 26, 26, 26, 27};
  int mList[20]       = { 2, 22, 22,  1, 24, 24, 26, 26, 26, 26, 26, 26, 26, 26, 24, 24,  2, 22, 22,  2};
  int local_kList[20] = {13, 13, 13, 13, 14, 14, 16, 16, 17, 17, 22, 22, 23, 23, 25, 25, 26, 26, 26, 26};
  int local_mList[20] = { 1, 18, 21, 38, 16, 23, 14, 25, 14, 25, 14, 25, 14, 25, 16, 23,  1, 18, 21, 38};
  
  for(int iC=0; iC<sizeList; ++iC){
    if(i == zList[iC] && j == pList[iC] && k == kList[iC] && m == mList[iC]){
      local_k.push_back(local_kList[iC]);
      local_m.push_back(local_mList[iC]);
      pippo = true;
    }
  }
  }

  return (pippo);

}

bool Check_Postlist(int i, int j, int k, int m, std::vector<int>& local_k, std::vector<int>& local_m){
  bool pippo = false;
  int sizeList = 2;
  int zList[2]       = {0, 1};
  int pList[2]       = {0, 0};
  int kList[2]       = {38, 38};
  int mList[2]       = {25, 25};
  int local_kList[2] = {38, 38};
  int local_mList[2] = {26, 26};
  
  for(int iC=0; iC<sizeList; ++iC){
    if(i == zList[iC] && j == pList[iC] && k == kList[iC] && m == mList[iC]){
      local_k.push_back(local_kList[iC]);
      local_m.push_back(local_mList[iC]);
      pippo = true;
    }
  }
  
  return (pippo);

}



//void doMappe()
void doMappe()
{
  gStyle->SetPaintTextFormat("1.1f");
  gStyle->SetMarkerSize(0.7);


  //  TFile *fout = TFile::Open("Ming_timingMap_245192_v2.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_All.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_AllRuns.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_AllRuns.root", "read");
  TFile *fout = TFile::Open("Run2015_B/doMap_collision2015_RunB_251643_251721_251883_All.root", "read");

  TH2F* mapT[2][2];
  TH2F* mapOccupancy[2][2];
  TH2F* mapTrms[2][2];
  TH2F* mapTChi2[2][2];

  int Zval = 1;
  int Pval = 1;

  std::vector<int> local_k;
  std::vector<int> local_m;


  std::ofstream outFile(Form("timingSensor_RunB_LG_Z%dP%d.txt", Zval, Pval), std::ios::out);

    for (int i=Zval; i<Zval+1; ++i){
    for (int j=Pval; j<Pval+1; ++j) {

      mapT[i][j] = (TH2F*)fout->Get(Form("mapT_Z%d_P%d", i, j));
      mapTrms[i][j]= (TH2F*)fout->Get(Form("mapTrms_Z%d_P%d", i, j));
      mapOccupancy[i][j]= (TH2F*)fout->Get(Form("mapOccupancy_Z%d_P%d", i, j));
      mapTChi2[i][j]= (TH2F*)fout->Get(Form("mapTChi2_Z%d_P%d", i, j));

      for (int k=0; k<40; ++k){
 	for (int m=0; m<40; ++m){

	  if(mapT[i][j]->GetBinContent(k+1, m+1)  != 0.) {
	    /*
	    std::cout << i << " \t " << j << " \t " << k << " \t " << m << " \t ";
	    std::cout.precision(3);
	    std::cout << mapT[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	    std::cout.precision(3);
	    std::cout << mapTrms[i][j]->GetBinContent(k+1, m+1) << " \t "; 
	    std::cout.precision(3);
	    std::cout << mapTChi2[i][j]->GetBinContent(k+1, m+1) << " \t "; 
	    std::cout.precision(3);
	    std::cout << mapOccupancy[i][j]->GetBinContent(k+1, m+1) << std::endl;
	    */

	    local_k.clear();
	    local_m.clear();
	    if(Check_list(i, j, k, m, local_k, local_m)){
	      for(unsigned int iSize=0; iSize<local_k.size(); ++iSize){
		outFile << i << " \t " << j << " \t " << local_k.at(iSize) << " \t " << local_m.at(iSize) << " \t "; 
		outFile.precision(3);
		outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
	      }
	      local_k.clear();
	      local_m.clear();
	    }
	    outFile << i << " \t " << j << " \t " << k << " \t " << m << " \t "; 
	    outFile .precision(3);
	    outFile << mapT[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	    outFile.precision(3);
	    outFile << mapTrms[i][j]->GetBinContent(k+1, m+1) << " \t ";   
	    outFile.precision(3);
	    outFile << mapTChi2[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	    outFile.precision(3);
	    outFile << mapOccupancy[i][j]->GetBinContent(k+1, m+1) << std::endl;         
	    if(Check_Postlist(i, j, k, m, local_k, local_m)){
	      for(unsigned int iSize=0; iSize<local_k.size(); ++iSize){
		outFile << i << " \t " << j << " \t " << local_k.at(iSize) << " \t " << local_m.at(iSize) << " \t "; 
		outFile.precision(3);
		outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
	      }
	      local_k.clear();
	      local_m.clear();
	    }
	  }

	  /*
	  if(mapT[i][j]->GetBinContent(k+1, m+1) != 0. && 
	     mapOccupancy[i][j]->GetBinContent(k+1, m+1) > 1.e+03 && 
	     mapOccupancy[i][j]->GetBinContent(k+1, m+1) < 1.e+04){
	    std::cout << i << " \t " << j << " \t " << k << " \t " << m << " \t ";
	    std::cout.precision(3);
	    std::cout << mapT[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	    std::cout.precision(3);
	    std::cout << mapTrms[i][j]->GetBinContent(k+1, m+1) << " \t "; 
	    std::cout.precision(3);
	    std::cout << mapTChi2[i][j]->GetBinContent(k+1, m+1) << " \t "; 
	    std::cout.precision(3);
	    std::cout << mapOccupancy[i][j]->GetBinContent(k+1, m+1) << std::endl;
	    }
	  */

	  if(mapOccupancy[i][j]->GetBinContent(k+1, m+1) == 0) mapT[i][j]->SetBinContent(k+1, m+1, -1000000.);
	  // 	  if(fabs(mapT[i][j]->GetBinContent(k+1, m+1)) > 50.) mapT[i][j]->SetBinContent(k+1, m+1, -100000.);
	  // 	  if(mapOccupancy[i][j]->GetBinContent(k+1, m+1) == 0) mapT[i][j]->SetBinContent(k+1, m+1, 0.0);

	  //	  outFile << std::setprecision(2) << i << " " << std::setprecision(2) << j << " " << std::setprecision(2) << k << " " << std::setprecision(2) << m << std::endl;
	  //	  std::cout << std::setprecision(2) << i << " " << std::setprecision(2) << j << " " << std::setprecision(2) << k << " " << std::setprecision(2) << m << std::endl;

	  
 	}
      }


      return;

      //      outFile.close();


      TCanvas* cc = new TCanvas();
      //      gPad->SetLogz();
      cc->cd();
      gPad->SetGridx();
      gPad->SetGridy();
      mapT[i][j]->GetXaxis()->SetTitle("x");
      mapT[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      mapT[i][j]->GetZaxis()->SetRangeUser(-10., 10.);
      mapT[i][j]->Draw("colz");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timing_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timing_Z%d_P%d.pdf", i, j), "pdf");

      cc->cd();
      gPad->SetGridx();
      gPad->SetGridy();
      mapT[i][j]->GetXaxis()->SetTitle("x");
      mapT[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      mapT[i][j]->GetZaxis()->SetRangeUser(-10., 10.);
      mapT[i][j]->Draw("colz");
      mapT[i][j]->Draw("text, same");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timing_text_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timing_text_Z%d_P%d.pdf", i, j), "pdf");
      

      cc->cd();
      mapOccupancy[i][j]->GetXaxis()->SetTitle("x");
      mapOccupancy[i][j]->GetYaxis()->SetTitle("y");
      mapOccupancy[i][j]->Draw("colz");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_occupancy_Z%d_P%d.pdf", i, j), "pdf");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_occupancy_Z%d_P%d.png", i, j), "png");

      cc->cd();
      mapTrms[i][j]->GetXaxis()->SetTitle("x");
      mapTrms[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      //      mapTrms[i][j]->GetZaxis()->SetRangeUser(-10., 10.);
      mapTrms[i][j]->Draw("colz");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timingRMS_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timingRMS_Z%d_P%d.pdf", i, j), "pdf");

      cc->cd();
      mapTChi2[i][j]->GetXaxis()->SetTitle("x");
      mapTChi2[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      mapTChi2[i][j]->GetZaxis()->SetRangeUser(0., 1500.);
      mapTChi2[i][j]->Draw("colz");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timingChi2_Z%d_P%d.pdf", i, j), "pdf");
      cc->Print(Form("Run2015_B/mappe_Run2015_B/Run_All_timingChi2_Z%d_P%d.png", i, j), "png");
    }
    }
    outFile.close();

    return;

    //  fout->Close();
  
  
  TCanvas* cc1 = new TCanvas();
  cc1->cd();
  mapT[0][0]->GetXaxis()->SetTitle("x");
  mapT[0][0]->GetYaxis()->SetTitle("y");
  //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);                            
  mapT[0][0]->GetZaxis()->SetRangeUser(-10., 10.);
  mapT[0][0]->Draw("colz");
  mapT[0][0]->Draw("text, same");
  

  TCanvas* cc2 = new TCanvas();
  cc2->cd();
  mapT[0][1]->GetXaxis()->SetTitle("x");
  mapT[0][1]->GetYaxis()->SetTitle("y");
  //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);                            
  mapT[0][1]->GetZaxis()->SetRangeUser(-10., 10.);
  mapT[0][1]->Draw("colz");
  mapT[0][1]->Draw("text, same");
  
  TCanvas* cc3 = new TCanvas();
  cc3->cd();
  mapT[1][0]->GetXaxis()->SetTitle("x");
  mapT[1][0]->GetYaxis()->SetTitle("y");
  //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);                            
  mapT[1][0]->GetZaxis()->SetRangeUser(-10., 10.);
  mapT[1][0]->Draw("colz");
  mapT[1][0]->Draw("text, same");
  
  TCanvas* cc4 = new TCanvas();
  cc4->cd();
  mapT[1][1]->GetXaxis()->SetTitle("x");
  mapT[1][1]->GetYaxis()->SetTitle("y");
  //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);                            
  mapT[1][1]->GetZaxis()->SetRangeUser(-10., 10.);
  mapT[1][1]->Draw("colz");
  mapT[1][1]->Draw("text, same");
  
}
