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
#include "TGraph.h"

#include "../../src/mask.h"
#include "../../src/draw_map.h"

#include "runs.h"

using namespace std;

void avc_scan() {

  //gErrorIgnoreLevel = kWarning;

  float avc_mean_high = 0.02;
  float avc_rms_high = 0.015;

  char hist0_name[512];
  char file0_name[512];
  //char dir_name[512];
  char title0_name[512];
  char figure0_name[512];

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);

  gStyle->SetOptStat(0);

  TH1F *h0_temp = new TH1F();
  TFile *_file0 =  new TFile();

  //sprintf(dir_name,"mkdir ../../img/%i",runs[0]);
  //system(dir_name);
  //sprintf(dir_name,"mkdir ../../img/%i/avc_test",runs[0]);
  //system(dir_name);

  float num_good[num_runs];

  for (Int_t r=0 ; r < num_runs ; r++) {

    sprintf(file0_name,"../../dat/QIE11testing_%i_6.root",runs[r]);
    _file0 = TFile::Open(file0_name); 

    num_good[r] = 24;
  
    for (Int_t h = 0; h < HF_num; h++) {
      if (lv0_mask[h] == 1) {
	for (Int_t s = 0 ; s < SL_num; s++) {
	  if (lv1_mask[h][s] == 1) {
	    for (Int_t q = 0; q < QI_num; q++) {
	      if (lv2_mask[h][s][q] == 1) {
		sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","BADratio_ICI7_CH","BADratio_ICI7_CH",h+1,s+1,q+1);
		h0_temp = (TH1F*)_file0->Get(hist0_name);
		//cout << "HF" << h+1 << ", SL" << s+1 << ", QI" << q+1 << " --- mean: " << h0_temp->GetMean() << " , rms: " << h0_temp->GetRMS();
		if ((h0_temp->GetMean()>avc_mean_high) || (h0_temp->GetRMS()>avc_rms_high)) {
		  num_good[r] -= 1;
		  //h0_temp->Draw("color");
		  //sprintf(figure0_name,"../../img/%i/ici_test/%i_ici_scan_HF%i_Slot%i_QIE%i.png",runs[0],runs[r],h+1,s+1,q+1);
		  //c1->SaveAs(figure0_name);
		  //c1->Clear();
		  //cout << " <<<<<<<<<<< ";
		}
		//		cout << endl;
		h0_temp->Delete();
		c1->Clear();
	      }
	    }
	  }
	}
      }
    }
    cout << num_good[r] << endl;;
  }

  TGraph *gr = new TGraph(num_runs,voltages,num_good);
  gr->SetMarkerStyle(20);
  gr->SetLineColor(2);
  sprintf(title0_name,"Good QIEs vs AVCC for SN: %i",SN);
  gr->SetTitle(title0_name);
  gr->GetXaxis()->SetTitle("AVCC (V)");
  gr->GetYaxis()->SetTitle("# Good QIEs");
  gr->Draw("APS");
  sprintf(figure0_name,"../../img/%i_AVCC_scan.png",SN);
  c1->SaveAs(figure0_name);
} // close function
