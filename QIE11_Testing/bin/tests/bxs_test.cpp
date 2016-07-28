#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TF1.h"
#include "TError.h"

#include "../../src/mask.h"
#include "../../src/draw_map.h"

Double_t linear_fit(Double_t *x,Double_t *par) {
  Double_t out_val;
  out_val = (x[0]*par[0]) + par[1];
  return out_val;
}

using namespace std;

void bxs_test(Int_t run_num) {

  gErrorIgnoreLevel = kWarning;

  float bxs_ratio_mean_high = 0.015;
  float bxs_ratio_rms_high = 0.01;

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char hist3_name[512];
  char hist4_name[512];
  char file0_name[512];
  char dir_name[512];
  char figure0_name[512];
  char figure1_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/bxs_test",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TH1F *h3_temp = new TH1F();
  TH1F *h4_temp = new TH1F();
  TFile *_file0 =  new TFile();

  sprintf(file0_name,"../../dat/QIE11testing_%i_9.root",run_num);
  _file0 = TFile::Open(file0_name);
  
  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);

  gStyle->SetOptStat(0);
  
  TF1 *fit_ici = new TF1();

  bool*** lv2_err_map_rms = create_error_map();
  bool*** lv2_err_map_mean = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {

	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","Pulse_BX0_CH","Pulse_BX0_CH",h+1,s+1,q+1);
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","Pulse_BX1_CH","Pulse_BX1_CH",h+1,s+1,q+1);
	      sprintf(hist2_name,"%s/%s_HF%i_Slot%i_QIE%i","Pulse_BX2_CH","Pulse_BX2_CH",h+1,s+1,q+1);
	      sprintf(hist3_name,"%s/%s_HF%i_Slot%i_QIE%i","Pulse_BX3_CH","Pulse_BX3_CH",h+1,s+1,q+1);
	      sprintf(hist4_name,"%s/%s_HF%i_Slot%i_QIE%i","BADratioVsBX_CH","BADratioVsBX_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      h2_temp = (TH1F*)_file0->Get(hist2_name);
	      h3_temp = (TH1F*)_file0->Get(hist3_name);
	      h4_temp = (TH1F*)_file0->Get(hist4_name);

	      sprintf(figure1_name,"../../img/%i/bxs_test/Ratio_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
	      h4_temp->Draw("box");
	      c1->SaveAs(figure1_name);
	      c1->Clear();
	      
	      bool bad_flag;
	      for (int i=0 ; i<4 ; i++) { 
		h4_temp->GetXaxis()->SetRangeUser(i-0.5,i+0.5);
		bad_flag = 0;
		if (h4_temp->GetMean(2) > bxs_ratio_mean_high) {
		  cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1;
		  cout << " <<< AFTERPULSE(mean = " << h4_temp->GetMean(2) << ") on for BXoffset " << i << endl;

		  sprintf(figure0_name,"../../img/%i/bxs_test/Pulse_BX%i_HF%i_Slot%i_QIE%i.png",run_num,i,h+1,s+1,q+1);
		  if (i == 0) {
		    h0_temp->Draw("box");
		  }
		  if (i == 1) {
		    h1_temp->Draw("box");
		  }
		  if (i == 2) {
		    h2_temp->Draw("box");
		  }
		  if (i == 3) {
		    h3_temp->Draw("box");
		  }
		  c1->SaveAs(figure0_name);
		  c1->Clear();
		  bad_flag = 1;

		  lv2_err_map_mean[h][s][q] = 0;
		  lv2_err_map_gen[h][s][q] = 0;
		}
		if (h4_temp->GetRMS(2) > bxs_ratio_rms_high) {
		  cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1;
		  cout << " <<< AFTERPULSE(rms = " << h4_temp->GetRMS(2) << ") on for BXoffset " << i << endl;

		  if (bad_flag == 0) {
		    sprintf(figure0_name,"../../img/%i/bxs_test/Pulse_BX%i_HF%i_Slot%i_QIE%i.png",run_num,i,h+1,s+1,q+1);
		    if (i == 0) {
		      h0_temp->Draw("box");
		    }
		    if (i == 1) {
		      h1_temp->Draw("box");
		    }
		    if (i == 2) {
		      h2_temp->Draw("box");
		    }
		    if (i == 3) {
		      h3_temp->Draw("box");
		    }
		    c1->SaveAs(figure0_name);
		    c1->Clear();
		  }

		  lv2_err_map_rms[h][s][q] = 0;
		  lv2_err_map_gen[h][s][q] = 0;
		}
	      }
	     
	      h0_temp->Delete();
	      h1_temp->Delete();
	      h2_temp->Delete();
	      h3_temp->Delete();
	      h4_temp->Delete();
	      c1->Clear();

	    }
	  }
	}
      }
    }
  }
  draw_map(lv2_err_map_mean, run_num, "bxs_test", "BXSmean");
  draw_map(lv2_err_map_rms, run_num, "bxs_test", "BXSrms");
  draw_map(lv2_err_map_gen, run_num, "bxs_test", "BXSall");

} // close function
