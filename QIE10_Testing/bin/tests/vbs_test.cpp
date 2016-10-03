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

void vbs_test(Int_t run_num) {

  gErrorIgnoreLevel = kWarning;

  float vbs_top_high = 1.4;
  float vbs_top_low = 0.4;
  float vbs_a_high = 25000.;
  float vbs_a_low = 100.;
  float vbs_b_high = 300.;
  float vbs_b_low = 0.;

  float vbs_top;
  float vbs_a;
  float vbs_b;

  char hist0_name[512]; 
  char hist1_name[512];
  char hist2_name[512];
  char hist3_name[512];
  char file0_name[512];
  char outputfile0_name[512];
  char dir_name[512];
  char figure0_name[512];
  char figure1_name[512];
  char figure2_name[512];
  char figure3_name[512];

  sprintf(dir_name,"mkdir $QIE10ROOT/img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir $QIE10ROOT/img/%i/vbs_test",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir $QIE10ROOT/img/%i/vbs_test/rootFiles",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TH1F *h3_temp = new TH1F();
  TFile *_file0 =  new TFile();
  TFile *output_file = new TFile();
  
  sprintf(file0_name,"$QIE10ROOT/dat/QIE10testing_%i_7.root",run_num);
  _file0 = TFile::Open(file0_name);

  gStyle->SetOptStat(0);


  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  //sprintf(hist2_name,"%s/%s","PedScan_qav_EV","PedScan_qav_EV");
  //sprintf(figure0_name,"$QIE10ROOT/img/%i/ped_test/Scan.png",run_num);
  //h2_temp = (TH1F*)_file0->Get(hist2_name);
  //h2_temp->Draw("box");
  //c1->SaveAs(figure0_name);
  //c1->Clear();
  //h2_temp->Delete();

  TF1 *vb_scan = new TF1();

  int ref_flag;
  
  bool*** lv2_err_map_top = create_error_map();
  bool*** lv2_err_map_a = create_error_map();
  bool*** lv2_err_map_b = create_error_map();
  bool*** lv2_err_map_refl = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  sprintf(outputfile0_name,"../../img/%i/vbs_test/rootFiles/VbsTest_HF%i_Slot%i.root",run_num,h+1,s+1);
	  output_file = new TFile(outputfile0_name,"RECREATE");
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->mkdir(dir_name);
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","Qsum_vs_Vb_PR","Qsum_vs_Vb_PR",h+1,s+1,q+1);
	      cout << hist0_name << endl;
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      vb_scan = new TF1("vb_scan",voltage_scan_fit,-0.5,2.55,3);
	      vb_scan->SetParNames("turn-on-point","a(x-top)^2","+b");
	      h0_temp->Fit("vb_scan","Q");
	      vbs_top = vb_scan->GetParameter(0);
	      vbs_a = vb_scan->GetParameter(1);
	      vbs_b = vb_scan->GetParameter(2);
	      //cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " -- TurnOnPoint: " << vbs_top << ", a(x-top)^2: " << vbs_a << ", +b: " << vbs_b << endl;
	      cout << "HF: " << h+1 << ", SL: " << s+1 << ", QI: " << q+1 << " --  a(x-top)^2: " << vbs_a ;
	      //cout << vbs_top << endl;
	      //cout << vbs_a << endl;
	      //cout << vbs_b << endl;
	      sprintf(figure0_name,"$QIE10ROOT/img/%i/vbs_test/scan_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      //h0_temp->SetTitle("Bias Voltage Scan with Sequencer");
	      h0_temp->GetXaxis()->SetTitle("LED Bias Voltage (V)");
	      h0_temp->GetYaxis()->SetTitle("Integrated Charge (fC)");
	      h0_temp->GetYaxis()->SetTitleOffset(1.5);

	      if ((vbs_top > vbs_top_high) || (vbs_top < vbs_top_low) || (h0_temp->GetEntries() < 10)) {
		lv2_err_map_top[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
		c1->SaveAs(figure0_name);
	      }	
	      if ((vbs_a > vbs_a_high) || (vbs_a < vbs_a_low) || (h0_temp->GetEntries() < 10)) {
		lv2_err_map_a[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ((vbs_b > vbs_b_high) || (vbs_b < vbs_b_low) || (h0_temp->GetEntries() < 10)) {
		lv2_err_map_b[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      c1->Clear();

	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","qratio_PR","qratio_PR",h+1,s+1,q+1);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      sprintf(figure1_name,"$QIE10ROOT/img/%i/vbs_test/ratio2_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      //h0_temp->SetTitle("Bias Voltage Scan with Sequencer");
	      h1_temp->GetXaxis()->SetTitle("LED Bias Voltage (V)");
	      h1_temp->GetYaxis()->SetTitle("(Qmax+Qnext)/Qsum");
	      h1_temp->GetYaxis()->SetTitleOffset(1.5);
	      h1_temp->Draw();

	      ref_flag = 0;
	      for (int binx=11 ; binx<27 ; binx ++) {
		if (h1_temp->GetBinContent(binx) - h1_temp->GetBinContent(binx-1) < -0.005 ) {
		  ref_flag = 1;
		  c1->SaveAs(figure1_name);
		}
	      }
	      c1->Clear();
	      sprintf(hist2_name,"%s/%s_HF%i_Slot%i_QIE%i","QvsTS_2.5V","QvsTS_2.5V",h+1,s+1,q+1);
	      sprintf(figure2_name,"$QIE10ROOT/img/%i/vbs_test/QvsTS_2.5V_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      h2_temp = (TH1F*)_file0->Get(hist2_name);
	      h2_temp->Draw("box");
	      if (ref_flag == 1) {
		cout << " <<< REFLECTION???" << endl;
		c1->SaveAs(figure2_name);
		c1->Clear();
		lv2_err_map_refl[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      } 
	      else {
		cout << endl;
	      }

	      sprintf(hist3_name,"%s/%s_HF%i_Slot%i_QIE%i","ADCvsTS_2.5V","ADCvsTS_2.5V",h+1,s+1,q+1);
	      sprintf(figure3_name,"$QIE10ROOT/img/%i/vbs_test/ADCvsTS_2.5V_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      h3_temp = (TH1F*)_file0->Get(hist3_name);
	      h3_temp->Draw("box");
	      if (lv2_err_map_gen[h][s][q] == 0){
		c1->SaveAs(figure3_name);
		c1->Clear();
	      }

	      /*
	      if ((ped_slope > ped_slope_high) || (ped_slope < ped_slope_low) || (h1_temp->GetEntries() < 10)) {
		lv2_err_map_slope[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      */
	      output_file->cd();
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->cd(dir_name);
	      h0_temp->Write();
	      h1_temp->Write();
	      h2_temp->Write();
	      h3_temp->Write();
	      vb_scan->Write();
	      h0_temp->Delete();
	      h1_temp->Delete();
	      h2_temp->Delete();
	      h3_temp->Delete();
	      vb_scan->Delete();

	    }
	  }
	  output_file->Write();
	  output_file->Close();
	}
      }
    }
  }
  
  draw_map(lv2_err_map_refl, run_num, "vbs_test", "VoltageScanRefl");
  draw_map(lv2_err_map_top, run_num, "vbs_test", "VoltageScanTOP");
  draw_map(lv2_err_map_a, run_num, "vbs_test", "VoltageScanA");
  draw_map(lv2_err_map_b, run_num, "vbs_test", "VoltageScanB");
  draw_map(lv2_err_map_gen, run_num, "vbs_test", "VoltageScanAll");
  
} // close function
