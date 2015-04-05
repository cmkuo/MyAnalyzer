Double_t fitf(Double_t *x, Double_t *par) {

  Double_t wc = 0.07291;
  Double_t n  = 1.798; // n-1 (in fact)
  Double_t v1 = pow(wc/n*(x[0]-par[1]), n);
  Double_t v2 = TMath::Exp(n-wc*(x[0]-par[1]));
  Double_t v  = par[0]*v1*v2;
  
  if (x[0] < par[1]) v = 0;
  
  return v;
}

void xFitShape(Int_t runId=119878, Int_t z=1, Int_t p=0, Int_t x=23, Int_t y=34) {

  //Reset ROOT and connect tree file
  gROOT->Reset();
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetOptTitle(kTRUE);  
  Char_t fname[200];
  sprintf(fname, "beamsplash_%08d.root", runId);
  //sprintf(fname, "../trees/tree_BeamSplash_1257640341.root");
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
  
  TH1F *hMIP = new TH1F("hMIP", "hMIP", 200, 0, 200);  
  TH1F *hT0  = new TH1F("hT0", "hT0", 100, -50, 50);
  
  TCanvas *c1 = new TCanvas("c1", "c1", 900, 600);
  c1->Divide(8,4);
  TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
  TCanvas *c3 = new TCanvas("c3", "c3", 900, 600);
  c3->Divide(2,1);
  
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += EventTree->GetEntry(i);
    
    cout<<"=== "<<run<<" === "<<event<<endl;
    //if (event != 287) continue;
    Int_t good[32];
    for (int j=0; j<32; ++j) {

      good[j] = 0;

      counts[j][0] = ADC0[z][p][x][y][j];
      counts[j][1] = ADC1[z][p][x][y][j];
      counts[j][2] = ADC2[z][p][x][y][j];
      
      ccounts[j][0] = cADC0[z][p][x][y][j];
      ccounts[j][1] = cADC1[z][p][x][y][j];
      ccounts[j][2] = cADC2[z][p][x][y][j];

      if (counts[j][0] > -10000) good[j] = 1;
    }
    
    Int_t t0 = -5;
    tx[0] = t0;
    tx[1] = t0+25;
    tx[2] = t0+50;       

    Double_t para[10];    
    Double_t mips[32];
    Double_t strip[32];
    TGraph *gr[32];
    Double_t totMu = 0;
    for (int j=0; j<32; ++j) {
      strip[j] = j+1;
      mips[j] = 0;
      if (good[j] == 0) continue;
      
      cout<<" === "<<endl;
      //cout<<j<<" "<<counts[j][0]<<" "<<counts[j][1]<<" "<<counts[j][2]<<endl;
      //cout<<j<<" "<<ccounts[j][0]<<" "<<ccounts[j][1]<<" "<<ccounts[j][2]<<endl;

      gr[j] = new TGraph(3, tx, counts[j]);
      func->SetParameters(50, 10);
      gr[j]->Fit("func", "M");
      func->GetParameters(para);
      
      hMIP->Fill(para[0]);
      cout<<j<<" "<<para[0]/55.<<endl;
      mips[j] = para[0]/55.;
      totMu += para[0]/55.;
      hT0->Fill(para[1]);
    }
    cout<<totMu<<endl;
    for (int j=0; j<32; ++j) {    
      c1->cd(j+1);
      if (good[j] == 0) continue;
      gr[j]->SetTitle("");
      gr[j]->SetMaximum(5000);
      gr[j]->SetMinimum(-20);      
      gr[j]->Draw("a");
      gr[j]->SetMarkerStyle(20);
      gr[j]->SetMarkerSize(1.5);
      gr[j]->Draw("ap");
    }
    c1->Update();

    TGraph *grMIP = new TGraph(32, strip, mips);
    c2->cd();
    grMIP->Draw("a");
    grMIP->SetMarkerStyle(20);
    grMIP->Draw("ap");
    c2->Update();

    c3->cd(1);
    hMIP->Draw();
    c3->cd(2);
    hT0->Draw();
    c3->Update();
    
    cin.get();
  }
  
}
