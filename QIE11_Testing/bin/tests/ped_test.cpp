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

#include "../../src/mask.h"
#include "../../src/draw_map.h"

Double_t ped_scan_fit(Double_t *x,Double_t *par) {
  Double_t out_val;
  if (x[0] < par[0]) {
    out_val = 0.;
  }
  else {
    out_val = (x[0]-par[0]) * par[1];
  }
  return out_val;
}

using namespace std;

void ped_test(Int_t run_num) {

  float ped_mean_low = 1.5;
  float ped_mean_high = 6.5;
  float ped_rms_low = 0.1;
  float ped_rms_high = 2.0;
  float ped_top_low = -3.0;
  float ped_top_high = 3.0;
  float ped_slope_low = 1.8;
  float ped_slope_high = 2.6;

  float ped_top;
  float ped_slope;

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char file0_name[512];
  char dir_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/ped_test",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TFile *_file0 =  new TFile();
  
  sprintf(file0_name,"../../dat/QIE11testing_%i_4.root",run_num);
  _file0 = TFile::Open(file0_name);

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  sprintf(hist2_name,"%s/%s","PedScanQav_EV","PedScanQav_EV");
  sprintf(figure0_name,"../../img/%i/ped_test/Scan.png",run_num);
  h2_temp = (TH1F*)_file0->Get(hist2_name);
  h2_temp->Draw("box");
  c1->SaveAs(figure0_name);
  c1->Clear();
  h2_temp->Delete();
  
  TF1 *fit_scan = new TF1();

  int occupancy;

  bool*** lv2_err_map_mean = create_error_map();
  bool*** lv2_err_map_rms = create_error_map();
  bool*** lv2_err_map_top = create_error_map();
  bool*** lv2_err_map_slope = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","DefaultPed_CH","DefaultPed_CH",h+1,s+1,q+1);
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","PedScanQav_CH","PedScanQav_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      if ((h0_temp->GetMean() < ped_mean_low) || (h0_temp->GetMean() > ped_mean_high) || (h0_temp->GetEntries() < 10)) {
		lv2_err_map_mean[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      occupancy = 0;
	      for (int binx=43 ; binx<64 ; binx++) {
		for (int biny=4 ; biny<14 ; biny++) {
		  //cout << "binx: " << binx << ", biny: " << biny << ", occupancy: " << h1_temp->GetBinContent(binx,biny) << endl;
		  occupancy += h1_temp->GetBinContent(binx,biny);
		}
	      }
	      if (occupancy > 0){
		cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " --OCCUPANCY: " << occupancy << endl;
	      }
	      if ( (h0_temp->GetRMS() < ped_rms_low) || (h0_temp->GetRMS() > ped_rms_high)) {
		lv2_err_map_rms[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      fit_scan = new TF1("fit_scan",ped_scan_fit,-0.5,63.5,2);
	      fit_scan->SetParNames("turn-on-point","slope");
	      h1_temp->Fit("fit_scan","Q");
	      ped_top = fit_scan->GetParameter(0);
	      ped_slope = fit_scan->GetParameter(1);	      
	      cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " -- TurnOnPoint: " << ped_top << ", Slope: " << ped_slope << endl;
	      if ((ped_top > ped_top_high) || (ped_top < ped_top_low) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_top[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ((ped_slope > ped_slope_high) || (ped_slope < ped_slope_low) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_slope[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      h0_temp->Delete();
	      h1_temp->Delete();
	      fit_scan->Delete();
	    }
	  }
	}
      }
    }
  }
  draw_map(lv2_err_map_mean, run_num, "ped_test", "DefaultPedestalMean");
  draw_map(lv2_err_map_rms, run_num, "ped_test", "DefaultPedestalRMS");
  draw_map(lv2_err_map_top, run_num, "ped_test", "PedestalScanElbow");
  draw_map(lv2_err_map_slope, run_num, "ped_test", "PedestalScanSlope");
  draw_map(lv2_err_map_gen, run_num, "ped_test", "PedestalAll");

} // close function
