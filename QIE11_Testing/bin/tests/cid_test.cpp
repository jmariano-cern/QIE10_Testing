#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TF1.h"

#include "../../src/mask.h"
#include "../../src/draw_map.h"

using namespace std;

void cid_test(Int_t run_num) {

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char root_file_name[512];
  char dir_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);  
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/capid_test",run_num);  
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TFile *_file0 =  new TFile();

  sprintf(root_file_name,"../../dat/QIE11testing_%i_1.root",run_num);
  _file0 = TFile::Open(root_file_name);

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  sprintf(hist2_name,"%s/%s","CIDvsTS_EV","CIDvsTS_EV");
  sprintf(figure0_name,"../../img/%i/capid_test/Pulse.png",run_num);
  h2_temp = (TH1F*)_file0->Get(hist2_name);
  h2_temp->Draw("box");
  c1->SaveAs(figure0_name);
  c1->Clear();
  h2_temp->Delete();

  bool*** lv2_err_map_rot = create_error_map();
  bool*** lv2_err_map_all = create_error_map(); 
  bool*** lv2_err_map_gen = create_error_map();
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      //sprintf(hist_name,"%s/%s_HF%i_Slot%i_QIE%i","ADC_ped_CH","ADC_ped_CH",lv0_map[h],lv1_map[h][s],lv2_map[h][s][q]);
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","CapIDrot_CH","CapIDrot_CH",h+1,s+1,q+1);
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","CapAllign_CH","CapAllign_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
	      h1_temp = (TH1F*)_file0->Get(hist1_name);
	      if ( ( h0_temp->GetMean() != 0 ) || ( h0_temp->GetRMS() != 0 ) || (h0_temp->GetEntries() < 10) ) {
		lv2_err_map_rot[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      if ( ( h1_temp->GetMean() != 0 ) || ( h1_temp->GetRMS() != 0 ) || (h1_temp->GetEntries() < 10) ) {
		lv2_err_map_all[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	
	      h0_temp->Delete();
	      h1_temp->Delete();
	    }
	  }
	}
      }
    }
  }
  draw_map(lv2_err_map_rot, run_num, "capid_test", "CapIDrotation");
  draw_map(lv2_err_map_all, run_num, "capid_test", "CapIDalligment");
  draw_map(lv2_err_map_gen, run_num, "capid_test", "CapIDgeneral");

} // close function
