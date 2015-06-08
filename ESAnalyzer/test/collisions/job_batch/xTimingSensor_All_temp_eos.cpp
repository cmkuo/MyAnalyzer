//g++ -Wall -o xTimingSensor_All_temp_eos `root-config --cflags --glibs` xTimingSensor_All_temp_eos.cpp

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TText.h"

#include <string>


Double_t fitf(Double_t *x, Double_t *par) {

  Double_t wc = 0.07291;
  Double_t n  = 1.798; // n-1 (in fact)
  Double_t v1 = pow(wc/n*(x[0]-par[1]), n);
  Double_t v2 = TMath::Exp(n-wc*(x[0]-par[1]));
  Double_t v  = par[0]*v1*v2;
  
  if (x[0] < par[1]) v = 0;
  
  return v;
}


int main(int argc, char* argv[]) {

  std::string RUN(argv[1]);
  std::cout << " >>> RUN = " << RUN << std::endl;

  std::string STARTstring(argv[2]);
  int START = atoi(argv[2]);
  std::cout << " >>> START = " << START << std::endl;
  std::string STOPstring(argv[3]);
  int STOP = atoi(argv[3]);
  std::cout << " >>> STOP = " << STOP << std::endl;

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  //  sprintf(fname, "/afs/cern.ch/user/a/amartell/eos/cms/store/user/amartell/Commissioning2015/ExpressPhysics/ES_run245162/ES_run245162.root");
  //  sprintf(fname, "/afs/cern.ch/user/a/amartell/eos/cms/store/user/amartell/Commissioning2015/ExpressPhysics/ES_runAll.root");
  //  sprintf(fname, "/tmp/cmkuo/collisions_245137_245204.root");
  //  sprintf(fname, "/tmp/cmkuo/collisions_minbias_245137_245204.root");


  TChain* EventTree = new TChain("coll/tree");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_1.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_10.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_100.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_101.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_102.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_103.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_104.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_105.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_106.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_107.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_108.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_109.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_11.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_110.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_111.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_112.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_113.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_114.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_115.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_116.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_117.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_118.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_119.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_12.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_120.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_121.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_122.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_123.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_124.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_125.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_126.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_127.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MiimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_128.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_129.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_13.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_130.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_131.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_132.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_133.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_134.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_135.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_136.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_137.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_138.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_139.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_14.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_140.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_141.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_142.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_143.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_144.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_145.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_146.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_147.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_148.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_149.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_15.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_150.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_151.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_152.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_153.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_136.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_137.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_138.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_139.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_14.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_140.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_141.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_142.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_143.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_144.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_145.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_146.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_147.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_148.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_149.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_15.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_150.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_151.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_152.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_153.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_154.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_155.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_156.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_157.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_158.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_159.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_16.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_160.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_161.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_162.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_163.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_164.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_165.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_166.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_17.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_18.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_19.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_2.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_20.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_21.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_22.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_23.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_24.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_25.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_26.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_27.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_28.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_29.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_3.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_30.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_31.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_32.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_33.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_34.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_35.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_36.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_37.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_38.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_39.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_4.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_40.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_41.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_42.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_43.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_44.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_45.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_46.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_47.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_48.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_49.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_5.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_50.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_51.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_52.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_53.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_54.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_55.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_56.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_57.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_58.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_59.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_6.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_60.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_61.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_62.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_63.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_64.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_65.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_66.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_67.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_68.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_69.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_7.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_70.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_71.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_72.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_73.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_74.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_75.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_76.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_77.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_78.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_79.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_8.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_80.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_81.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_82.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_83.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_84.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_85.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_86.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_87.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_88.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_89.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_9.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_90.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_91.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_92.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_93.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_94.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_95.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_96.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_97.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_98.root");
  EventTree->Add("root://eoscms//eos/cms/store/user/cmkuo/MinimumBias/crab_job_minbias_commissioning2015_246081_246092/150529_093857/0000/collision_99.root");


  //Declaration of leaves types
  Int_t           run;
  Int_t           event;
  Int_t           nRH;
  Int_t           rh_z[200000];
  Int_t           rh_p[200000];
  Int_t           rh_x[200000];
  Int_t           rh_y[200000];
  Int_t           rh_s[200000];
  Int_t           rh_status[200000];
  Int_t           rh_ADC0[200000];
  Int_t           rh_ADC1[200000];
  Int_t           rh_ADC2[200000];
  
  // Set branch addresses.
  EventTree->SetBranchAddress("run",&run);
  EventTree->SetBranchAddress("event",&event);
  EventTree->SetBranchAddress("nRH", &nRH);
  EventTree->SetBranchAddress("rh_z",rh_z);
  EventTree->SetBranchAddress("rh_p",rh_p);
  EventTree->SetBranchAddress("rh_x",rh_x);
  EventTree->SetBranchAddress("rh_y",rh_y);
  EventTree->SetBranchAddress("rh_z",rh_z);
  EventTree->SetBranchAddress("rh_s",rh_s);
  EventTree->SetBranchAddress("rh_status",rh_status);
  EventTree->SetBranchAddress("rh_ADC0",rh_ADC0);
  EventTree->SetBranchAddress("rh_ADC1",rh_ADC1);
  EventTree->SetBranchAddress("rh_ADC2",rh_ADC2);
  
  Int_t tx[3], counts[3];
  Long64_t nentries = EventTree->GetEntries();
  
  TF1 *func = new TF1("func", fitf, -200, 200, 2);
  func->SetLineWidth(2);

  Char_t hname[200];
  TH1F *hT[2][2][40][40];
  TH1F *hTiming[2][2];
  for (int i=0; i<2; ++i) 
    for (int j=0; j<2; ++j) {

      sprintf(hname, "Z%d_P%d", i, j);
      hTiming[i][j] = new TH1F(hname, hname, 101, -50.5, 50.5);

      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m) {
	  sprintf(hname, "Z%d_P%d_X%d_Y%d", i, j, k, m);
	  hT[i][j][k][m] = new TH1F(hname, hname, 101, -50.5, 50.5);
	}
    }

  std::cout << " >>> nentries = " << nentries << std::endl;

  Long64_t nbytes = 0;
  Long64_t nMax = Long64_t(STOP);
  if(nentries < STOP) nMax = nentries;
  for (Long64_t z=START; z<nMax; ++z) {
    if(z%10000 == 0)    std::cout << " >>> iEvt = " << z << std::endl;
    nbytes += EventTree->GetEntry(z);

    if(z%10000 == 0)   std::cout << "evt " << z << " === " << run << " === " << event << std::endl;

    tx[0] = -5;
    tx[1] = 20;
    tx[2] = 45;

    Double_t para[10];    
    //Double_t mips[32];

    for (Int_t i=0; i<nRH; ++i) {

      if (rh_ADC1[i] < 200) continue;
      if (fabs(rh_ADC0[i]) > 20) continue;
      if (rh_ADC1[i] < 0 || rh_ADC2[i] < 0) continue;

      counts[0] = rh_ADC0[i];
      counts[1] = rh_ADC1[i];
      counts[2] = rh_ADC2[i];
      
      TGraph *gr = new TGraph(3, tx, counts);
      func->SetParameters(50, 0);
      gr->Fit("func", "MQ");
      func->GetParameters(para);
      
      if (func->GetChisquare() > 100) continue;	      

      Int_t iz = (rh_z[i] < 0) ? 0 : 1;
      Int_t ip = rh_p[i] - 1;
      Int_t ix = rh_x[i] - 1;
      Int_t iy = rh_y[i] - 1;

      hT[iz][ip][ix][iy]->Fill(para[1]);
      hTiming[iz][ip]->Fill(para[1]);
      //cout<<iz<<" "<<ip<<" "<<ix<<" "<<para[1]<<endl;
      delete gr;
    }
    
  }


  std::cout << " >>> stampa file <<< " << std::endl; 
  
  TFile* fout = new TFile(Form("coll_timing_Run%s_%s-%s.root",RUN.c_str(),STARTstring.c_str(),STOPstring.c_str()), "recreate");
  fout->cd();
  for (int i=0; i<2; ++i)
    for (int j=0; j<2; ++j) {
      hTiming[i][j]->Write();
      for (int k=0; k<40; ++k)
	for (int m=0; m<40; ++m)
	  hT[i][j][k][m]->Write();
    }
  fout->Close();
 
  std::cout << " fine " << std::endl;
  return 10;
 
}
