#include <iomanip>
#include <iostream>
//to be run after doMapTiming.cpp
// root
// .L doMappe.C
//doMappe(nRUN, Z, P)

void doMappe(int nRUN, int Zval, int Pval)
{
  gStyle->SetPaintTextFormat("1.1f");
  gStyle->SetMarkerSize(0.7);

  std::cout << " nRUN = " << nRUN << std::endl;
  std::cout << " Zval = " << Zval << " Pval = " << Pval << " options 11 10 01 00" << std::endl;


  TFile *fout = TFile::Open(Form("Ming_timingMap_AllRuns_%d.root", nRUN), "read");
  //  TFile *fout = TFile::Open("Ming_timingMap_beamsplash_timing_00268006.root", "read");


  TH2F* mapT[2][2];
  TH2F* mapOccupancy[2][2];
  TH2F* mapTrms[2][2];
  TH2F* mapTChi2[2][2];

  //choose among:  11  10  01  00

  //read ES sensor map
  std::vector<int> ES_Z;
  std::vector<int> ES_P;
  std::vector<int> ES_X;
  std::vector<int> ES_Y;
  ES_Z.clear();
  ES_P.clear();
  ES_X.clear();
  ES_Y.clear();

  std::ifstream inFileES;
  inFileES.open(Form("SensorLadder_map/outMap_Z%dP%d.txt", Zval, Pval), std::ios::in);
  while(!inFileES.eof()){
    int iZ, iP, iX, iY, iL; 
    inFileES >> iZ >> iP >> iX >> iY >> iL;
    ES_Z.push_back(iZ);
    ES_P.push_back(iP);
    ES_X.push_back(iX);
    ES_Y.push_back(iY);
  }


  std::ofstream outFile(Form("dataMap/timingSensor_%d_Z%dP%d.txt", nRUN, Zval, Pval), std::ios::out);
  int cLine = 0;
  for(int i=Zval; i<Zval+1; ++i){
    for(int j=Pval; j<Pval+1; ++j) {
      
      mapT[i][j] = (TH2F*)fout->Get(Form("mapT_Z%d_P%d", i, j));
      mapTrms[i][j]= (TH2F*)fout->Get(Form("mapTrms_Z%d_P%d", i, j));
      mapOccupancy[i][j]= (TH2F*)fout->Get(Form("mapOccupancy_Z%d_P%d", i, j));
      mapTChi2[i][j]= (TH2F*)fout->Get(Form("mapTChi2_Z%d_P%d", i, j));

      for(int k=0; k<40; ++k){
 	for(int m=0; m<40; ++m){

	  if(mapT[i][j]->GetBinContent(k+1, m+1) != 0) {

	    while(i != ES_Z.at(cLine) || j != ES_P.at(cLine) || k != ES_X.at(cLine) || m != ES_Y.at(cLine)){
	      // std::cout << "ES      = " << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t " << std::endl;
	      // std::cout << "missing = " << i << " \t " << j << " \t " << k << " \t " << m << " \t " << std::endl;
	      outFile << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t ";
	      outFile.precision(3);
	      outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
	      ++cLine;
	    }
	    if(i == ES_Z.at(cLine) && j == ES_P.at(cLine) && k == ES_X.at(cLine) && m == ES_Y.at(cLine) && 
	       mapOccupancy[i][j]->GetBinContent(k+1, m+1) < 4){
	      outFile << i << " \t " << j << " \t " << k << " \t " << m << " \t "; 
	      outFile .precision(3);
	      outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
	      ++cLine;
	    }
	    if(i == ES_Z.at(cLine) && j == ES_P.at(cLine) && k == ES_X.at(cLine) && m == ES_Y.at(cLine)){	     
	      outFile << i << " \t " << j << " \t " << k << " \t " << m << " \t "; 
	      outFile .precision(3);
	      outFile << mapT[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	      outFile.precision(3);
	      outFile << mapTrms[i][j]->GetBinContent(k+1, m+1) << " \t ";   
	      outFile.precision(3);
	      outFile << mapTChi2[i][j]->GetBinContent(k+1, m+1) << " \t ";  
	      outFile.precision(3);
	      outFile << mapOccupancy[i][j]->GetBinContent(k+1, m+1) << std::endl;         
	      ++cLine;
	    }
	  }
	  /*
	  if(mapOccupancy[i][j]->GetBinContent(k+1, m+1) == 0.){ //&& cLine < ES_Z.size()-1){
	    std::cout << " >>> cLine = " << cLine << " ES_P.at(cLine) = " << ES_P.size() << std::endl;
	    // std::cout << "ES      = " << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t " << std::endl;
	    // std::cout << "empty   = " << i << " \t " << j << " \t " << k << " \t " << m << " \t " << std::endl;
	    // outFile << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t ";
	    // outFile.precision(3);
	    // outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
	    ++cLine;
	  }
	  */
	  //else mapT[i][j]->SetBinContent(k+1, m+1, -1000000.);
	  //	  if(mapOccupancy[i][j]->GetBinContent(k+1, m+1) == 0) mapT[i][j]->SetBinContent(k+1, m+1, -1000000.);
	  if(mapOccupancy[i][j]->GetBinContent(k+1, m+1) < 4) mapT[i][j]->SetBinContent(k+1, m+1, -1000000.);
	}
      }
    }
  }

  for(iC = cLine; iC<(ES_Z.size()-1); ++iC){
    //    std::cout << " cLine = " << cLine << " ES_Z.size() = " << ES_Z.size() << " iC = " << iC << std::endl;  
    //    std::cout << "ES      = " << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t " << std::endl;
    outFile << ES_Z.at(cLine) << " \t " << ES_P.at(cLine) << " \t " << ES_X.at(cLine) << " \t " << ES_Y.at(cLine) << " \t ";
    outFile.precision(3);
    outFile << -999 << " \t " << -999 << " \t " << -999 << " \t " << -999 << std::endl;
    ++cLine;
  }


}
