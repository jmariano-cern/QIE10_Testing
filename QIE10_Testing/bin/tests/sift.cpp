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

void sift(Int_t run0_num, Int_t run1_num) {

  char hist0_name[512];
  char hist1_name[512];
  char hist2_name[512];
  char hist3_name[512];
  char hist4_name[512];
  char hist5_name[512];
  char hist6_name[512];
  char file0_name[512];
  char file1_name[512];
  char file2_name[512];
  char output_file_name[512];
  char dir0_name[512];
  char dir1_name[512];
  char dir2_name[512];
  char dir3_name[512];
  char dir4_name[512];

  TH1F *h0_temp = new TH1F();
  TH1F *h1_temp = new TH1F();
  TH1F *h2_temp = new TH1F();
  TH1F *h3_temp = new TH1F();
  TH1F *h4_temp = new TH1F();
  TH1F *h5_temp = new TH1F();
  TH1F *h6_temp = new TH1F();
  TFile *_file0 =  new TFile();
  TFile *_file1 =  new TFile();
  TFile *_file2 =  new TFile();
  TFile *output_file = new TFile();
  TDirectory *dir0 = new TDirectory();
  TDirectory *dir1 = new TDirectory();
  TDirectory *dir2 = new TDirectory();
  TDirectory *dir3 = new TDirectory();
  TDirectory *dir4 = new TDirectory();

  sprintf(file0_name,"../../dat/QIE10testing_%i_1.root",run0_num);
  _file0 = TFile::Open(file0_name);
  sprintf(file1_name,"../../dat/QIE10testing_%i_4.root",run0_num);
  _file1 = TFile::Open(file1_name);
  sprintf(file2_name,"../../dat/QIE10testing_%i_6.root",run1_num);
  _file2 = TFile::Open(file2_name);
  
  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  sprintf(output_file_name,"../../dat/output/HF%i_Slot%i.root",h+1,s+1);
	  output_file = new TFile(output_file_name,"RECREATE");
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      
	      sprintf(dir0_name,"QIE%i",q+1);
	      dir0 = output_file->mkdir(dir0_name);

	      sprintf(dir1_name,"%s","cid_test");
	      dir1 = dir0->mkdir(dir1_name);
	      dir1->cd();
	      
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","CIDvsTS_CH","CIDvsTS_CH",h+1,s+1,q+1);
	      h0_temp = (TH1F*)_file0->Get(hist0_name);
 	      h0_temp->Write();
	      h0_temp->Delete();

	      sprintf(dir2_name,"%s","ped_test");
	      dir2 = dir0->mkdir(dir2_name);
	      dir2->cd();
	      
	      sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","DefaultPed_CH","DefaultPed_CH",h+1,s+1,q+1);
	      h1_temp = (TH1F*)_file1->Get(hist1_name);
 	      h1_temp->Write();
	      h1_temp->Delete();

	      sprintf(hist2_name,"%s/%s_HF%i_Slot%i_QIE%i","PedScan_qav_CH","PedScan_qav_CH",h+1,s+1,q+1);
	      h2_temp = (TH1F*)_file1->Get(hist2_name);
 	      h2_temp->Write();
	      h2_temp->Delete();

	      sprintf(dir3_name,"%s","ici_test");
	      dir3 = dir0->mkdir(dir3_name);
	      dir3->cd();

	      sprintf(hist3_name,"%s/%s_HF%i_Slot%i_QIE%i","ici_scan_CH","ici_scan_CH",h+1,s+1,q+1);
	      h3_temp = (TH1F*)_file2->Get(hist3_name);
 	      h3_temp->Write();
	      h3_temp->Delete();

	      sprintf(hist4_name,"%s/%s_HF%i_Slot%i_QIE%i","qratio_CH","qratio_CH",h+1,s+1,q+1);
	      h4_temp = (TH1F*)_file2->Get(hist4_name);
 	      h4_temp->Write();
	      h4_temp->Delete();

	      sprintf(hist5_name,"%s/%s_HF%i_Slot%i_QIE%i","T_abs_CH","T_abs_CH",h+1,s+1,q+1);
	      h5_temp = (TH1F*)_file2->Get(hist5_name);
 	      h5_temp->Write();
	      h5_temp->Delete();

	      sprintf(dir4_name,"%s","pulse");
	      dir4 = dir3->mkdir(dir4_name);
	      dir4->cd();

	      for (int ici=0 ; ici<8 ; ici++) {
		sprintf(hist6_name,"%s%i%s/%s%i%s_HF%i_Slot%i_QIE%i","pulse_ICI",ici,"_CH","pulse_ICI",ici,"_CH",h+1,s+1,q+1);
		h6_temp = (TH1F*)_file2->Get(hist6_name);
		h6_temp->Write();
		h6_temp->Delete();
	      }
	    }
	  }

	  output_file->Write();
	  output_file->Close();

	}
      }
    }
  }
} // close function
