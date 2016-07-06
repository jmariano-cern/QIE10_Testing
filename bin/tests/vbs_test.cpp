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

Double_t voltage_scan_fit(Double_t *x,Double_t *par) {
  Double_t out_val;
  if (x[0] < par[0]) {
    out_val = 0.;
  }
  else {
    out_val = (par[1] * x[0] * x[0]) + (par[2] * x[0]) + par[3];
  }
  return out_val;
}

using namespace std;

void vbs_test(Int_t run_num) {

  float vbs_top;
  float vbs_a;
  float vbs_b;
  float vbs_c;

  char hist0_name[512];
  char file0_name[512];
  char dir_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/vbs_test",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TFile *_file0 =  new TFile();
  
  sprintf(file0_name,"../../dat/QIE10testing_%i_7.root",run_num);
  _file0 = TFile::Open(file0_name);

  gStyle->SetOptStat(0);


  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  //sprintf(hist2_name,"%s/%s","PedScan_qav_EV","PedScan_qav_EV");
  //sprintf(figure0_name,"../../img/%i/ped_test/Scan.png",run_num);
  //h2_temp = (TH1F*)_file0->Get(hist2_name);
  //h2_temp->Draw("box");
  //c1->SaveAs(figure0_name);
  //c1->Clear();
  //h2_temp->Delete();

  TF1 *vb_scan = new TF1();

  /*
  bool*** lv2_err_map_top = create_error_map();
  bool*** lv2_err_map_a = create_error_map();
  bool*** lv2_err_map_b = create_error_map();
  bool*** lv2_err_map_c = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  */
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","Qsum_vs_Vb_PR","Qsum_vs_Vb_PR",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      vb_scan = new TF1("vb_scan",voltage_scan_fit,-0.5,2.55,4);
	      vb_scan->SetParNames("turn-on-point","ax^2","bx","c");
	      h0_temp->Fit("vb_scan","Q");
	      vbs_top = vb_scan->GetParameter(0);
	      vbs_a = vb_scan->GetParameter(1);	      
	      vbs_b = vb_scan->GetParameter(2);	      
	      vbs_c = vb_scan->GetParameter(3);	      
	      cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " -- TurnOnPoint: " << vbs_top << ", ax^2: " << vbs_a << ", bx: " << vbs_b << ", c: " << vbs_c << endl;
	      sprintf(figure0_name,"../../img/%i/vbs_test/scan_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      c1->SaveAs(figure0_name);
	      c1->Clear();

	      /*
	      if ((ped_top > ped_top_high) || (ped_top < ped_top_low) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_top[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ((ped_slope > ped_slope_high) || (ped_slope < ped_slope_low) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_slope[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      */
	      h0_temp->Delete();
	      vb_scan->Delete();
	    }
	  }
	}
      }
    }
  }
  /*
  draw_map(lv2_err_map_mean, run_num, "ped_test", "DefaultPedestalMean");
  draw_map(lv2_err_map_rms, run_num, "ped_test", "DefaultPedestalRMS");
  draw_map(lv2_err_map_top, run_num, "ped_test", "PedestalScanElbow");
  draw_map(lv2_err_map_slope, run_num, "ped_test", "PedestalScanSlope");
  draw_map(lv2_err_map_gen, run_num, "ped_test", "PedestalAll");
  */
} // close function
