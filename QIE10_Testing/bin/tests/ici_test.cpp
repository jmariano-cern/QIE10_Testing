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

void ici_test(Int_t run_num) {

  gErrorIgnoreLevel = kWarning;

  float ici_slope_low = 1.0;
  float ici_slope_high = 1.1;
  float ici_offset_low = -0.0;
  float ici_offset_high = 250.0;
  float ici_rms_high = 1.0;
  float ici_ratio_low = 0.98;

  float ici_slope;
  float ici_offset;

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char hist3_name[512];
  char hist4_name[512];
  char hist5_name[512];
  char hist6_name[512];
  char file0_name[512];
  char outputfile0_name[512];
  char dir_name[512];
  char figure0_name[512];
  char figure1_name[512];
  char figure2_name[512];
  char figure3_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/ici_test",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/ici_test/rootFiles",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TH1F *h3_temp = new TH1F();
  TH1F *h4_temp = new TH1F();
  TH1F *h5_temp = new TH1F();
  TH1F *h6_temp = new TH1F();
  TFile *_file0 =  new TFile();
  TFile *output_file =  new TFile();

  sprintf(file0_name,"../../dat/QIE10testing_%i_6.root",run_num);
  _file0 = TFile::Open(file0_name);
  
  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);

  gStyle->SetOptStat(0);
  
  sprintf(hist4_name,"%s/%s","ici_scan_EV","ici_scan_EV");
  sprintf(figure1_name,"../../img/%i/ici_test/Scan.png",run_num);
  h4_temp = (TH1F*)_file0->Get(hist4_name);
  h4_temp->Draw("color");
  c1->SaveAs(figure1_name);
  c1->Clear();
  h4_temp->Delete();

  sprintf(hist5_name,"%s/%s","qratio_EV","qratio_EV");
  sprintf(figure2_name,"../../img/%i/ici_test/Ratio.png",run_num);
  h5_temp = (TH1F*)_file0->Get(hist5_name);
  h5_temp->Draw("box");
  c1->SaveAs(figure2_name);
  c1->Clear();
  h5_temp->Delete();

  sprintf(hist6_name,"%s/%s","T_abs_EV","T_abs_EV");
  sprintf(figure3_name,"../../img/%i/ici_test/Timing.png",run_num);
  h6_temp = (TH1F*)_file0->Get(hist6_name);
  h6_temp->Draw();
  c1->SaveAs(figure3_name);
  c1->Clear();
  h6_temp->Delete();

  TF1 *fit_ici = new TF1();

  bool*** lv2_err_map_slope = create_error_map();
  bool*** lv2_err_map_offset = create_error_map();
  bool*** lv2_err_map_rms = create_error_map();
  bool*** lv2_err_map_ratio = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  bool*** lv2_err_map_ref = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  sprintf(outputfile0_name,"../../img/%i/ici_test/rootFiles/ICITest_HF%i_Slot%i.root",run_num,h+1,s+1);
	  output_file = new TFile(outputfile0_name,"RECREATE");
	  
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->mkdir(dir_name);
	      sprintf(dir_name,"QIE%i/pulses",q+1);
	      output_file->mkdir(dir_name);

	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","ici_scan_CH","ici_scan_CH",h+1,s+1,q+1);
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","T_abs_CH","T_abs_CH",h+1,s+1,q+1);
	      sprintf(hist2_name,"%s/%s_HF%i_Slot%i_QIE%i","qratio_ICI7_CH","qratio_ICI7_CH",h+1,s+1,q+1);
	      sprintf(hist3_name,"%s/%s_HF%i_Slot%i_QIE%i","pulse_ICI7_CH","pulse_ICI7_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      h2_temp = (TH1F*)_file0->Get(hist2_name);
	      h3_temp = (TH1F*)_file0->Get(hist3_name);
	      fit_ici = new TF1("fit_ici",linear_fit,0,10000,2);
	      fit_ici->SetParNames("slope","offset");
	      h0_temp->Fit("fit_ici","Q");
	      ici_slope = fit_ici->GetParameter(0);
	      ici_offset = fit_ici->GetParameter(1);
	      if ((ici_slope < ici_slope_low) || (ici_slope > ici_slope_high) || (h0_temp->GetEntries() < 10)) {
		h0_temp->Draw("color");
		sprintf(figure0_name,"../../img/%i/ici_test/ici_scan_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
		c1->SaveAs(figure0_name);
		c1->Clear();
		lv2_err_map_slope[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ((ici_offset < ici_offset_low) || (ici_offset > ici_offset_high) || (h0_temp->GetEntries() < 10)) {
		if (lv2_err_map_gen[h][s][q] == 1) {
		  h0_temp->Draw("color");
		  sprintf(figure0_name,"../../img/%i/ici_test/ici_scan_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
		  c1->SaveAs(figure0_name);
		  c1->Clear();
		}
		lv2_err_map_offset[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      //cout << "Timing mean: " << h1_temp->GetMean() << ", rms: " << h1_temp->GetRMS() << ", nentries: " << h1_temp->GetEntries() << endl;
	      if ((h1_temp->GetRMS() > ici_rms_high) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_rms[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ((h2_temp->GetMean() < ici_ratio_low) || (h2_temp->GetEntries() < 10)) {
		h3_temp->Draw("box");
		sprintf(figure0_name,"../../img/%i/ici_test/pulse_ICI7_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
		c1->SaveAs(figure0_name);
		//cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << endl;
		//cout << "Slope: " << ici_slope << ", Offset: " << ici_offset << ", RMS: " << h1_temp->GetRMS() << ", Ratio: " << h2_temp->GetMean() << endl;
		lv2_err_map_ratio[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      h3_temp->GetXaxis()->SetRangeUser(6.5,7.5);
	      cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " -- " << h3_temp->GetMean(2) ;
	      if (h3_temp->GetMean(2) > 10) {
		cout << " <<< AFTERPULSE";
		lv2_err_map_ref[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }
	      cout << endl;
	      output_file->cd();
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->cd(dir_name);
	      h0_temp->Write();
	      h1_temp->Write();
	      h2_temp->Write();
	      h0_temp->Delete();
	      h1_temp->Delete();
	      h2_temp->Delete();
	      h3_temp->Delete();
	      fit_ici->Delete();
	      c1->Clear();
	      sprintf(dir_name,"QIE%i/pulses",q+1);
	      output_file->cd(dir_name);
	      for (int i_pulse = 0; i_pulse < 8; i_pulse++){
		sprintf(hist3_name,"pulse_ICI%i_CH/pulse_ICI%i_CH_HF%i_Slot%i_QIE%i",i_pulse,i_pulse,h+1,s+1,q+1);
		h3_temp = (TH1F*)_file0->Get(hist3_name);
		h3_temp->Write();
		h3_temp->Delete();
	      }
	    }
	  }
	  output_file->Write();
	  output_file->Close();
	}
      }
    }
  }
  draw_map(lv2_err_map_slope, run_num, "ici_test", "ICIscanSlope");
  draw_map(lv2_err_map_offset, run_num, "ici_test", "ICIscanOffset");
  draw_map(lv2_err_map_rms, run_num, "ici_test", "ICItimingRMS");
  draw_map(lv2_err_map_ratio, run_num, "ici_test", "ICIratio");
  draw_map(lv2_err_map_gen, run_num, "ici_test", "ICIall");
  draw_map(lv2_err_map_ref, run_num, "ici_test", "ICIref");

} // close function
