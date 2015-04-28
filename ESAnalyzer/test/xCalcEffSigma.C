//******************************************//
// author: Chris Seez
//
//  This function evaluates the effective
//  sigma of a histogram.
//
//
//  TODO list:
//
//******************************************//

Double_t effSigma(TH1 *hist)
{

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;

}

void xCalcEffSigma() {

  gROOT->Reset();

  TFile *f = new TFile("eReso_E150.root");

  TH1F *hNoTh = (TH1F*) f->Get("SH_eRESO_E50_NG");
  TH1F *hLGLG = (TH1F*) f->Get("SH_eRESO_E50_LGLG");
  TH1F *hHGLG = (TH1F*) f->Get("SH_eRESO_E50_HGLG");
  TH1F *hHGHG = (TH1F*) f->Get("SH_eRESO_E50_HGHG");
  TH1F *hEE   = (TH1F*) f->Get("SH_eRESO_E50_EE");

  Double_t sNoTh = effSigma(hNoTh);
  Double_t sLGLG = effSigma(hLGLG);
  Double_t sHGLG = effSigma(hHGLG);
  Double_t sHGHG = effSigma(hHGHG);
  Double_t sEE   = effSigma(hEE);

  cout<<"effective sigma : "<<endl;
  cout<<"No threshold "<<sNoTh<<endl;   
  cout<<"LG-LG        "<<sLGLG<<endl;   
  cout<<"HG-LG        "<<sHGLG<<endl;   
  cout<<"HG-HG        "<<sHGHG<<endl;   
  cout<<"EE only      "<<sEE<<endl; 

}


