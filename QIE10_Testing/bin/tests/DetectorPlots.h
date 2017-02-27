#include "TCanvas.h"
#include "TStyle.h"
#include "TH2D.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TPad.h"

int phiOffset(Int_t depth) {
  if ((depth == 1) || (depth == 2)){
    return 0;
  } else if ((depth == 3) || (depth == 4)) {
    return 1;
  } else {
    return 0;
  }
}

float etaOffset(Int_t depth) {
  if (depth%2 == 0) {
    return 0.5;
  } else if (depth%2 == 1) {
    return 0.;
  } else {
    return 0.;
  }
}

class DetectorHist {
  public:
    Int_t eta_num = 41;
    Int_t phi_num = 72;
    Int_t depth_num = 4;

    Double_t eta_min = 29.;
    Double_t eta_max = 41.;
};

/////////////////////////////////////////////////////
///////////////// SQUARE HIST ///////////////////////
/////////////////////////////////////////////////////

class SquareHist: public DetectorHist {
  public:
    TH2F* hist;
    int z_min;
    int z_max;
    SquareHist(const char* title, float z_min_in, float z_max_in);
    ~SquareHist(void);
    void Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data);
    void Draw(int run_num,const char* folder_name, const char* png_name);
};

SquareHist::SquareHist(const char* title, float z_min_in, float z_max_in) {
  char name[512] = "";
  sprintf(name,"%s_SquareHist",title);
  hist = new TH2F(name, title, phi_num, 1., phi_num+1, (eta_num*4)+1, -eta_max-0.5, eta_max+0.5);
  z_min = z_min_in;
  z_max = z_max_in;
}

SquareHist::~SquareHist() {
  hist->Delete();
}

void SquareHist::Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data){
  if (eta<0) {
    hist->Fill(phi+phiOffset(depth), eta+etaOffset(depth), data );
  }
  if (eta>0) {
    hist->Fill(phi+phiOffset(depth), eta-etaOffset(depth), data );
  }
  
}

void SquareHist::Draw(int run_num,const char* folder_name, const char* png_name) {
  TCanvas* c_temp = new TCanvas("c_temp", "c_temp", 100, 100, 1024, 1024);
  c_temp->cd();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);
  gStyle->SetNumberContours(50);
  hist->GetXaxis()->SetTitle("i#phi");
  hist->GetYaxis()->SetTitle("i#eta");
  hist->Draw("COLZ");
  hist->GetZaxis()->SetRangeUser(z_min,z_max);
  hist->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char hist_line2[512] = "";
  sprintf(hist_line2,"run %i",run_num);
  TPaveText* hist_title = (TPaveText*)gPad->FindObject("title");
  hist_title->SetY1NDC(0.91);
  hist_title->SetY2NDC(0.98);
  hist_title->InsertText(hist_line2);
  char figure_name[512] = "";
  sprintf(figure_name,"../../img/%i/%s/%s.png",run_num,folder_name,png_name);
  c_temp->SaveAs(figure_name);
  c_temp->Clear();
}

//////////////////////////////////////////////////////////////
///////////////// SQUARE HIST INVERTED ///////////////////////
//////////////////////////////////////////////////////////////

class SquareHistInv: public DetectorHist {
  public:
    TH2F* hist;
    int z_min;
    int z_max;
    SquareHistInv(const char* title, float z_min_in, float z_max_in);
    ~SquareHistInv(void);
    void Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data);
    void Draw(int run_num,const char* folder_name, const char* png_name);
};

SquareHistInv::SquareHistInv(const char* title, float z_min_in, float z_max_in) {
  char name[512] = "";
  sprintf(name,"%s_SquareHistInv",title);
  hist = new TH2F(name, title, phi_num, 1., phi_num+1, 4*(eta_max-eta_min)+5, eta_min-eta_max-1.25, eta_max-eta_min+1.25);
  z_min = z_min_in;
  z_max = z_max_in;
}

SquareHistInv::~SquareHistInv() {
  hist->Delete();
}

void SquareHistInv::Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data){
  if (eta<0) { 
    hist->Fill(phi+phiOffset(depth), -0.5+-1*float(eta_max-abs(eta))-etaOffset(depth), data );
  }
  if (eta>0) {
    hist->Fill(phi+phiOffset(depth), 0.5+float(eta_max-eta)+etaOffset(depth), data );
  }
  
}

void SquareHistInv::Draw(int run_num,const char* folder_name, const char* png_name) {
  TCanvas* c_temp = new TCanvas("c_temp", "c_temp", 100, 100, 1024, 1024);
  c_temp->cd();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);
  gStyle->SetNumberContours(50);
  hist->GetXaxis()->SetTitle("i#phi");
  hist->GetYaxis()->SetTitle("i#eta_{max}-i#eta");
  hist->Draw("COLZ");
  hist->GetZaxis()->SetRangeUser(z_min,z_max);
  hist->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char hist_line2[512] = "";
  sprintf(hist_line2,"run %i",run_num);
  TPaveText* hist_title = (TPaveText*)gPad->FindObject("title");
  hist_title->SetY1NDC(0.91);
  hist_title->SetY2NDC(0.98);
  hist_title->InsertText(hist_line2);
  char figure_name[512] = "";
  sprintf(figure_name,"../../img/%i/%s/%s.png",run_num,folder_name,png_name);
  c_temp->SaveAs(figure_name);
  c_temp->Clear();
}

////////////////////////////////////////////////////
///////////////// POLAR HIST ///////////////////////
////////////////////////////////////////////////////

class PolarHist: public DetectorHist {
  public:
    char titleM[512];
    char titleP[512];
    TH2F* histM;
    TH2F* histP;
    int z_min;
    int z_max;
    PolarHist(const char* title, float z_min_in, float z_max_in);
    ~PolarHist(void);
    void Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data);
    void Draw(int run_num,const char* folder_name, const char* png_name);
};

PolarHist::PolarHist(const char* title, float z_min_in, float z_max_in) {
  char nameM[512] = "";
  char nameP[512] = "";
  sprintf(nameM,"%s_PolarHist_M",title);
  sprintf(nameP,"%s_PolarHist_P",title);
  histM = new TH2F(nameM, title, phi_num, 0., 2.*TMath::Pi(), (eta_num*2)+2, 0., eta_max+1.25);
  histP = new TH2F(nameP, title, phi_num, 0., 2.*TMath::Pi(), (eta_num*2)+2, 0., eta_max+1.25);
  z_min = z_min_in;
  z_max = z_max_in;
}

PolarHist::~PolarHist() {
  histM->Delete();
  histP->Delete();
}

void PolarHist::Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data){
  if (eta<0) {
    histM->Fill(2*TMath::Pi()*(phi+phiOffset(depth))/(phi_num+0.5), 0.5+abs(float(eta))+etaOffset(depth), data );
  }
  if (eta>0) {
    histP->Fill(2*TMath::Pi()*(phi+phiOffset(depth))/(phi_num+0.5), 0.5+float(eta)+etaOffset(depth), data );
  }
}

void PolarHist::Draw(int run_num,const char* folder_name, const char* png_name) {
  TCanvas* c_temp = new TCanvas("c_temp", "c_temp", 100, 100, 2048, 1024);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);
  gStyle->SetNumberContours(50);
  c_temp->Divide(2,1);
  c_temp->cd(1);
  TH2F* frameM = new TH2F("framing hist minus", "framing hist minus", depth_num*eta_num/2, -eta_max-4, eta_max+4,  depth_num*eta_num/2, -eta_max-4, eta_max+4);
  frameM->SetTitle(histM->GetTitle());
  frameM->Draw("COLZ");
  frameM->GetXaxis()->SetTitle("i#eta");
  frameM->GetYaxis()->SetTitle("i#eta");
  histM->Draw("COLZ POL SAME");
  histM->GetZaxis()->SetRangeUser(z_min,z_max);
  histM->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char histM_line2[512] = "";
  sprintf(histM_line2,"HFM -- run %i",run_num);
  TPaveText* histM_title = (TPaveText*)gPad->FindObject("title");
  histM_title->SetY1NDC(0.91);
  histM_title->SetY2NDC(0.98);
  histM_title->InsertText(histM_line2);
  c_temp->cd(2);
  TH2F* frameP = new TH2F("framing hist plus", "framing hist plus", depth_num*eta_num/2, -eta_max-4, eta_max+4,  depth_num*eta_num/2, -eta_max-4, eta_max+4);
  frameP->SetTitle(histP->GetTitle());
  frameP->Draw("COLZ");
  frameP->GetXaxis()->SetTitle("i#eta");
  frameP->GetYaxis()->SetTitle("i#eta");
  histP->Draw("COLZ POL SAME");
  histP->GetZaxis()->SetRangeUser(z_min,z_max);
  histP->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char histP_line2[512] = "";
  sprintf(histP_line2,"HFP -- run %i",run_num);
  TPaveText* histP_title = (TPaveText*)gPad->FindObject("title");
  histP_title->SetY1NDC(0.91);
  histP_title->SetY2NDC(0.98);
  histP_title->InsertText(histP_line2);
  char figure_name[512] = "";
  sprintf(figure_name,"../../img/%i/%s/%s.png",run_num,folder_name,png_name);
  c_temp->SaveAs(figure_name);
  c_temp->Clear();
  frameM->Delete();
  frameP->Delete();
}

/////////////////////////////////////////////////////////////
///////////////// POLAR HIST INVERTED ///////////////////////
/////////////////////////////////////////////////////////////

class PolarHistInv: public DetectorHist {
  public:
    char titleM[512];
    char titleP[512];
    TH2F* histM;
    TH2F* histP;
    int z_min;
    int z_max;
    PolarHistInv(const char* title, float z_min_in, float z_max_in);
    ~PolarHistInv(void);
    void Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data);
    void Draw(int run_num,const char* folder_name, const char* png_name);
};

PolarHistInv::PolarHistInv(const char* title, float z_min_in, float z_max_in) {
  char nameM[512] = "";
  char nameP[512] = "";
  sprintf(nameM,"%s_PolarHistInv_M",title);
  sprintf(nameP,"%s_PolarHistInv_P",title);
  histM = new TH2F(nameM, title, phi_num, 0., 2.*TMath::Pi(), 2*(eta_max-eta_min)+4, 0., eta_max-eta_min+1.75);
  histP = new TH2F(nameP, title, phi_num, 0., 2.*TMath::Pi(), 2*(eta_max-eta_min)+4, 0., eta_max-eta_min+1.75);
  z_min = z_min_in;
  z_max = z_max_in;
}

PolarHistInv::~PolarHistInv() {
  histM->Delete();
  histP->Delete();
}

void PolarHistInv::Fill(Int_t eta,Int_t phi,Int_t depth, Double_t data){
  if (eta<0) {
    histM->Fill(2*TMath::Pi()*(phi+phiOffset(depth))/(phi_num+0.5), 1.5+eta_max-float(abs(eta))+etaOffset(depth), data );
  }
  if (eta>0) {
    histP->Fill(2*TMath::Pi()*(phi+phiOffset(depth))/(phi_num+0.5), 1.5+eta_max-float(eta)+etaOffset(depth), data );
  }
}

void PolarHistInv::Draw(int run_num,const char* folder_name, const char* png_name) {
  TCanvas* c_temp = new TCanvas("c_temp", "c_temp", 100, 100, 2048, 1024);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);
  gStyle->SetNumberContours(50);
  c_temp->Divide(2,1);
  c_temp->cd(1);
  TH2F* frameM = new TH2F("framing hist minus", "framing hist minus", 2*(eta_max-eta_min), eta_min-eta_max-3, eta_max-eta_min+3, 2*(eta_max-eta_min), eta_min-eta_max-3, eta_max-eta_min+3);
  frameM->SetTitle(histM->GetTitle());
  frameM->Draw("COLZ");
  frameM->GetXaxis()->SetTitle("i#eta_{max}-i#eta");
  frameM->GetYaxis()->SetTitle("i#eta_{max}-i#eta");
  histM->Draw("COLZ POL SAME");
  histM->GetZaxis()->SetRangeUser(z_min,z_max);
  histM->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char histM_line2[512] = "";
  sprintf(histM_line2,"HFM -- run %i",run_num);
  TPaveText* histM_title = (TPaveText*)gPad->FindObject("title");
  histM_title->SetY1NDC(0.91);
  histM_title->SetY2NDC(0.98);
  histM_title->InsertText(histM_line2);
  c_temp->cd(2); 
  TH2F* frameP = new TH2F("framing hist plus", "framing hist plus", 2*(eta_max-eta_min), eta_min-eta_max-3, eta_max-eta_min+3, 2*(eta_max-eta_min), eta_min-eta_max-3, eta_max-eta_min+3);
  frameP->SetTitle(histP->GetTitle());
  frameP->Draw("COLZ");
  frameP->GetXaxis()->SetTitle("i#eta_{max}-i#eta");
  frameP->GetYaxis()->SetTitle("i#eta_{max}-i#eta");
  histP->Draw("COLZ POL SAME");
  histP->GetZaxis()->SetRangeUser(z_min,z_max);
  histP->GetZaxis()->SetLabelSize(0.02);
  c_temp->Update();
  char histP_line2[512] = "";
  sprintf(histP_line2,"HFP -- run %i",run_num);
  TPaveText* histP_title = (TPaveText*)gPad->FindObject("title");
  histP_title->SetY1NDC(0.91);
  histP_title->SetY2NDC(0.98);
  histP_title->InsertText(histP_line2);
  char figure_name[512] = "";
  sprintf(figure_name,"../../img/%i/%s/%s.png",run_num,folder_name,png_name);
  c_temp->SaveAs(figure_name);
  c_temp->Clear();
  frameM->Delete();
  frameP->Delete();
}
