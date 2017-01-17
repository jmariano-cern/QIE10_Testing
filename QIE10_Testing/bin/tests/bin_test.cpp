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

Double_t linear_fit(Double_t *x,Double_t *par) {
  Double_t out_val;
  out_val = (x[0]*par[0]) + par[1];
  return out_val;
}

using namespace std;

void bin_test(Int_t run_num) {

  float ped_mean_low = 1.5;
  float ped_mean_high = 6.5;
  float ped_rms_low = 0.1;
  float ped_rms_high = 2.0;

  float ici_slope_low = 0.95;
  float ici_slope_high = 1.25;
  float ici_offset_low = -30.0;
  float ici_offset_high = 250.0;
  float ici_slope;
  float ici_offset;

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char hist3_name[512];
  char hist4_name[512];
  char hist5_name[512];
  char file0_name[512];
  char outputfile0_name[512];
  char dir_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/bin_test",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/bin_test/rootFiles",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TH1F *h3_temp = new TH1F();
  TH1F *h4_temp = new TH1F();
  TH1F *h5_temp = new TH1F();
  TFile *_file0 =  new TFile();
  TFile *output_file = new TFile();
  
  sprintf(file0_name,"../../dat/QIE10testing_%i_12.root",run_num);
  _file0 = TFile::Open(file0_name);

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  
  TF1 *fit_ici = new TF1();

  bool*** lv2_err_map_mean = create_error_map();
  bool*** lv2_err_map_rms = create_error_map();
  bool*** lv2_err_map_ici_scan = create_error_map();
  bool*** lv2_err_map_ici_pulse7 = create_error_map();
  bool*** lv2_err_map_cid_rotation = create_error_map();
  bool*** lv2_err_map_cid_align = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  sprintf(outputfile0_name,"../../img/%i/bin_test/rootFiles/BurnIn_HF%i_Slot%i.root",run_num,h+1,s+1);
	  output_file = new TFile(outputfile0_name,"RECREATE");
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->mkdir(dir_name);
	      
	      // PEDESTAL
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","BurnIn_Ped_CH","BurnIn_Ped_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      if ((h0_temp->GetMean() < ped_mean_low) || (h0_temp->GetMean() > ped_mean_high) || (h0_temp->GetEntries() < 10)) {
		h0_temp->Draw();
		c1->SetLogy();
		sprintf(hist0_name,"../../img/%i/bin_test/%s_HF%i_Slot%i_QIE%i.png",run_num,"DefaultPed_CH",h+1,s+1,q+1);
		c1->SaveAs(hist0_name);
		c1->SetLogy(false);
		lv2_err_map_mean[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (ped_mean): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
	      }	
	      if ( (h0_temp->GetRMS() < ped_rms_low) || (h0_temp->GetRMS() > ped_rms_high)) {
		h0_temp->Draw();
		c1->SetLogy();
		sprintf(hist0_name,"../../img/%i/bin_test/%s_HF%i_Slot%i_QIE%i.png",run_num,"DefaultPed_CH",h+1,s+1,q+1);
		c1->SaveAs(hist0_name);
		c1->SetLogy(false);		
		lv2_err_map_rms[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (ped_rms): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
	      }	

	      // ICI SCAN
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","BurnIn_ICIScan_CH","BurnIn_ICIScan_CH",h+1,s+1,q+1);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      fit_ici = new TF1("fit_ici",linear_fit,0,10000,2);
              fit_ici->SetParNames("slope","offset");
              h1_temp->Fit("fit_ici","Q");
              ici_slope = fit_ici->GetParameter(0);
              ici_offset = fit_ici->GetParameter(1);
              if ((ici_slope < ici_slope_low) || (ici_slope > ici_slope_high) || (ici_offset < ici_offset_low) || (ici_offset > ici_offset_high) || (h1_temp->GetEntries() < 10)) {
                h1_temp->Draw("color");
                sprintf(hist1_name,"../../img/%i/bin_test/ici_scan_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
                c1->SaveAs(hist1_name);
                lv2_err_map_ici_scan[h][s][q] = 0;
                lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (ici scan): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
              }
              fit_ici->Delete();

	      // ICI PULSE 7
	      /*
	      sprintf(hist2_name,"%s/%s_HF%i_Slot%i_QIE%i","ICI_pulse_7_CH","ICI_pulse_7_CH",h+1,s+1,q+1);
	      h2_temp = (TH1F*)_file0->Get(hist2_name);
	      h2_temp->GetXaxis()->SetRangeUser(6.5,7.5);
              if ((h2_temp->GetMean(2) > 10) || (h2_temp->GetEntries() < 10)) {
		h2_temp->GetXaxis()->SetRangeUser(-0.5,9.5);
                h2_temp->Draw("box");
                sprintf(hist2_name,"../../img/%i/bin_test/ici_pulse7_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
                c1->SaveAs(hist2_name);
                lv2_err_map_ici_pulse7[h][s][q] = 0;
                lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (ici pulse7): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
              }
	      h2_temp->GetXaxis()->SetRangeUser(-0.5,9.5);
	      */

	      // CID ROTATION
	      sprintf(hist3_name,"%s/%s_HF%i_Slot%i_QIE%i","CapIDrot_CH","CapIDrot_CH",h+1,s+1,q+1);
	      h3_temp = (TH1F*)_file0->Get(hist3_name);
	      sprintf(hist4_name,"%s/%s_HF%i_Slot%i_QIE%i","CIDvsTS_CH","CIDvsTS_CH",h+1,s+1,q+1);
	      h4_temp = (TH1F*)_file0->Get(hist4_name);
	      if ( ( h3_temp->GetMean() != 0 ) || ( h3_temp->GetRMS() != 0 ) || (h3_temp->GetEntries() < 10) ) {
                h4_temp->Draw("box");
                sprintf(hist4_name,"../../img/%i/bin_test/CIDvsTS_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
                c1->SaveAs(hist4_name);
                lv2_err_map_cid_rotation[h][s][q] = 0;
                lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (cid rotation): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
              }

	      // CID ALIGNMENT
	      /*
	      sprintf(hist5_name,"%s/%s_HF%i_Slot%i_QIE%i","CapAllign_CH","CapAllign_CH",h+1,s+1,q+1);
	      h5_temp = (TH1F*)_file0->Get(hist5_name);
	      if ( ( h5_temp->GetMean() != 0 ) || ( h5_temp->GetRMS() != 0 ) || (h5_temp->GetEntries() < 10) ) {
		h4_temp->Draw("box");
		sprintf(hist4_name,"../../img/%i/bin_test/CIDvsTS_HF%i_Slot%i_QIE%i.png",run_num,h+1,s+1,q+1);
		c1->SaveAs(hist4_name);
                lv2_err_map_cid_align[h][s][q] = 0;
                lv2_err_map_gen[h][s][q] = 0;
		cout << "Channel failed (cid allignment): HF" <<  h+1 << " SL" << s+1 << " CH" << q+1 << endl;
              }
	      */

	      output_file->cd();
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->cd(dir_name);
	      h0_temp->Write();
	      h1_temp->Write();
	      // h2_temp->Write();
	      h4_temp->Write();
	      h0_temp->Delete();
	      h1_temp->Delete();
	      // h2_temp->Delete();
	      h3_temp->Delete();
	      h4_temp->Delete();
	      // h5_temp->Delete();
	    }
	  }
	  output_file->Write();
	  output_file->Close();
	}
      }
    }
  }
  draw_map(lv2_err_map_mean, run_num, "bin_test", "DefaultPedestalMean");
  draw_map(lv2_err_map_rms, run_num, "bin_test", "DefaultPedestalRMS");
  draw_map(lv2_err_map_ici_scan, run_num, "bin_test", "ICIscan");
  //draw_map(lv2_err_map_ici_pulse7, run_num, "bin_test", "ICIpulse7");
  draw_map(lv2_err_map_cid_rotation, run_num, "bin_test", "CapRotation");
  // draw_map(lv2_err_map_cid_align, run_num, "bin_test", "CapAlignment");
  draw_map(lv2_err_map_gen, run_num, "bin_test", "BurnInAll");

} // close function
