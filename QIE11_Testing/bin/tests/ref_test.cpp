#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <fstream>
#include <vector>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TF1.h"
#include "TGraph.h"
#include "TError.h"

#include "../../src/mask.h"
#include "../../src/draw_map.h"

Double_t voltage_scan_fit(Double_t *x,Double_t *par) {
  Double_t out_val;
  if (x[0] < par[0]) {
    out_val = par[2];
  }
  else {
    out_val = (par[1] * (x[0] - par[0]) * (x[0] - par[0])) + par[2];
  }
  return out_val;
}

using namespace std;

void ref_test(Int_t run0_num,Int_t run1_num,Int_t run2_num) {

  //gErrorIgnoreLevel = kWarning;

  char hist0_name[512]; 
  char hist1_name[512]; 
  char hist2_name[512]; 
  char file0_name[512];
  char dir_name[512];
  char title0_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run0_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/ref_test",run0_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F(); 
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TFile *_file0 =  new TFile();
  
  static const int nV = 3;
  static const int nRin = 3;
  float Qsum[nRin][nV];
  float RefRatio[nRin][nV];
  int Rins[3] = {0,7,15};
  int runs[nV];
  runs[0] = run0_num;
  runs[1] = run1_num;
  runs[2] = run2_num;

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  vector<TGraph*> gr;
  TH2F *framer = new TH2F("framer","framer",1,0,10000,1,0.0,0.15);
  TCanvas *c2 = new TCanvas("c1","c1",100,100,1024,768);
  vector<TGraph*> gr2;
  TH2F *framer2 = new TH2F("framer","framer",1,0,10000,1,0.0,40.0);

  sprintf(title0_name,"RefStudy.png");
  sprintf(figure0_name,"../../img/%i/ref_test/%s.png",run0_num,title0_name);
  framer->Draw();
  framer->SetTitle(title0_name);
  framer->GetXaxis()->SetTitle("Injected Charge");
  framer->GetYaxis()->SetTitle("Reflection Ratio");
  framer->GetYaxis()->SetTitleOffset(1.5);	      

  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {		
	      //sprintf(title0_name,"RefStudy_HF%i_SL%i_QI%i.png",h+1,s+1,q+1);
	      //sprintf(figure0_name,"../../img/%i/ref_test/%s.png",run0_num,title0_name);
	      //framer->Draw();
	      //framer->SetTitle(title0_name);
	      //framer->GetXaxis()->SetTitle("Injected Charge");
	      //framer->GetYaxis()->SetTitle("Reflection Ratio");
	      //framer->GetYaxis()->SetTitleOffset(1.5);
	      for (int rin = 0 ; rin < nRin ; rin++) {  
		for (Int_t r = 0; r < nV ; r++) {

		  cur_rin = Rins[rin];
				  
		  sprintf(file0_name,"../../dat/QIE11testing_%i_8.root",runs[r]);
		  _file0 = TFile::Open(file0_name);

		  sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","refabs_vs_rin_CH","refabs_vs_rin_CH",h+1,s+1,q+1);
		  h0_temp = (TH1F*)_file0->Get(hist0_name);
		  h0_temp->GetXaxis()->SetRangeUser(cur_rin-0.5,cur_rin+0.5);
		  RefRatio[rin][r] = h0_temp->GetMean(2);

		  sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","refabs_vs_rin_CH","refabs_vs_rin_CH",h+1,s+1,q+1);
		  h0_temp = (TH1F*)_file0->Get(hist0_name);
		  h0_temp->GetXaxis()->SetRangeUser(cur_rin-0.5,cur_rin+0.5);
		  RefRatio[rin][r] = h0_temp->GetMean(2);

		  sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","Max_CH","Max_CH",h+1,s+1,q+1);
		  h1_temp = (TH1F*)_file0->Get(hist1_name);
		  h1_temp->GetXaxis()->SetRangeUser(cur_rin-0.5,cur_rin+0.5);
		  Qsum[rin][r] = h1_temp->GetMean(2);
		  //Qsum[rin][r] = r;

		  h0_temp->Delete();
		  h1_temp->Delete();
		  h2_temp->Delete();
		  _file0->Close();
		}

		gr.push_back(new TGraph(nV,Qsum[rin],RefRatio[rin]));
		gr.back()->SetLineColor(rin+1);
		gr.back()->Draw("SAME");
	      }

	      //c1->SaveAs(figure0_name);
	      //c1->Clear();

	      //for (int rin = 0 ; rin < nRin ; rin++) {
	      //gr.back()->Delete();
	      //gr.pop_back();
	      //}
	    }
	  }
	}
      }  
    }
  }

  c1->SaveAs(figure0_name);
  c1->Clear();


} // close function
