#include <iomanip>
#include <iostream>


//void doMappe()
void doMappe()
{
  gStyle->SetPaintTextFormat("1.1f");
  gStyle->SetMarkerSize(0.7);


  //  TFile *fout = TFile::Open("Ming_timingMap_245192_v2.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_All.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_AllRuns.root", "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_AllRuns.root", "read");
  TFile *fout = TFile::Open("Ming_timingMap_AllRuns_245137_246092_MinB.root", "read");

  TH2F* mapT[2][2];
  TH2F* mapOccupancy[2][2];
  TH2F* mapTrms[2][2];
  TH2F* mapTChi2[2][2];


  //  std::ofstream outFile("outFile.txt", std::ios::out);

    for (int i=1; i<2; ++i){
    for (int j=1; j<2; ++j) {

      mapT[i][j] = (TH2F*)fout->Get(Form("mapT_Z%d_P%d", i, j));
      mapTrms[i][j]= (TH2F*)fout->Get(Form("mapTrms_Z%d_P%d", i, j));
      mapOccupancy[i][j]= (TH2F*)fout->Get(Form("mapOccupancy_Z%d_P%d", i, j));
      mapTChi2[i][j]= (TH2F*)fout->Get(Form("mapTChi2_Z%d_P%d", i, j));

      for (int k=0; k<40; ++k){
 	for (int m=0; m<40; ++m){

	  if(mapT[i][j]->GetBinContent(k+1, m+1)  != 0.) {
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
      cc->Print(Form("mappe_AllRuns/Run_All_timing_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("mappe_AllRuns/Run_All_timing_Z%d_P%d.pdf", i, j), "pdf");

      cc->cd();
      gPad->SetGridx();
      gPad->SetGridy();
      mapT[i][j]->GetXaxis()->SetTitle("x");
      mapT[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      mapT[i][j]->GetZaxis()->SetRangeUser(-10., 10.);
      mapT[i][j]->Draw("colz");
      mapT[i][j]->Draw("text, same");
      cc->Print(Form("mappe_AllRuns/Run_All_timing_text_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("mappe_AllRuns/Run_All_timing_text_Z%d_P%d.pdf", i, j), "pdf");
      

      cc->cd();
      mapOccupancy[i][j]->GetXaxis()->SetTitle("x");
      mapOccupancy[i][j]->GetYaxis()->SetTitle("y");
      mapOccupancy[i][j]->Draw("colz");
      cc->Print(Form("mappe_AllRuns/Run_All_occupancy_Z%d_P%d.pdf", i, j), "pdf");
      cc->Print(Form("mappe_AllRuns/Run_All_occupancy_Z%d_P%d.png", i, j), "png");

      cc->cd();
      mapTrms[i][j]->GetXaxis()->SetTitle("x");
      mapTrms[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      //      mapTrms[i][j]->GetZaxis()->SetRangeUser(-10., 10.);
      mapTrms[i][j]->Draw("colz");
      cc->Print(Form("mappe_AllRuns/Run_All_timingRMS_Z%d_P%d.png", i, j), "png");
      cc->Print(Form("mappe_AllRuns/Run_All_timingRMS_Z%d_P%d.pdf", i, j), "pdf");

      cc->cd();
      mapTChi2[i][j]->GetXaxis()->SetTitle("x");
      mapTChi2[i][j]->GetYaxis()->SetTitle("y");
      //      mapT[i][j]->GetZaxis()->SetRangeUser(-20., 20.);
      mapTChi2[i][j]->GetZaxis()->SetRangeUser(0., 1500.);
      mapTChi2[i][j]->Draw("colz");
      cc->Print(Form("mappe_AllRuns/Run_All_timingChi2_Z%d_P%d.pdf", i, j), "pdf");
      cc->Print(Form("mappe_AllRuns/Run_All_timingChi2_Z%d_P%d.png", i, j), "png");
    }
  }

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
