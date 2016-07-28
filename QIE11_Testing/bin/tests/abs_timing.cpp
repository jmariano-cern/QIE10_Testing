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

#include "904_maps.h"
#include "904_masks_C50.h"
#include "runs.h"

using namespace std;

void abs_timing() {

  ofstream outfile;
  //  outfile.open("../dat/absolute_timing.csv",std::ios_base::app);
  outfile.open("../dat/absolute_timing.csv",std::ios_base::out | std::ios_base::trunc);

  char hist_name[100];
  char figure_name[100];
  char root_file_name[100]; 

  float data[1][13][24][num_runs];
  float f_runs[num_runs];

  TH1F *h_temp = new TH1F();
  TFile *_file0 =  new TFile();

  outfile << "             HF:";
  for (Int_t h = 0; h < 6; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < 13; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < 24; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      outfile << ", " << setw(5) << lv0_map[h];
	    } // close if QIE_mask
	  } // close for over QIE
	} // close if slot_mask
      } // close for over slot
    } // close if HF_mask
  } // close for over HF
  outfile << endl;

  outfile << "             SL:";  
  for (Int_t h = 0; h < 6; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < 13; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < 24; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      outfile << ", " << setw(5) << lv1_map[h][s];
	    } // close if QIE_mask
	  } // close for over QIE
	} // close if slot_mask
      } // close for over slot
    } // close if HF_mask
  } // close for over HF
  outfile << endl;
  
  outfile << "             QI:";
  for (Int_t h = 0; h < 6; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < 13; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < 24; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      outfile << ", " << setw(5) << lv2_map[h][s][q];
	    } // close if QIE_mask
	  } // close for over QIE
	} // close if slot_mask
      } // close for over slot
    } // close if HF_mask
  } // close for over HF
  outfile << endl;
  
  for (Int_t r=0 ; r < num_runs ; r++) {

    f_runs[r] = (float)r;

    sprintf(root_file_name,"../dat/QIE11testing_%i_5.root",runs[r]);  

    _file0 = TFile::Open(root_file_name);
    
    outfile << runs[r] << " mean:";
    for (Int_t h = 0; h < 6; h++) {
      if (lv0_mask[h] == 1) {
	for (Int_t s = 0 ; s < 13; s++) {
	  if (lv1_mask[h][s] == 1) {
	    for (Int_t q = 0; q < 24; q++) {
	      if (lv2_mask[h][s][q] == 1) {
		
		sprintf(hist_name,"%s/%s_HF%i_Slot%i_QIE%i","T_abs_CH","T_abs_CH",lv0_map[h],lv1_map[h][s],lv2_map[h][s][q]); 
	      
		h_temp = (TH1F*)_file0->Get(hist_name);
	  
		//		outfile << "HF" << lv0_map[h] << ", Slot " << lv1_map[h][s] << ", QIE" << lv2_map[h][s][q] << ": " << h_temp->GetMean() << " +- " << h_temp->GetRMS() << endl;
		outfile << ", " << setw(5) << setiosflags(ios::fixed) << setprecision(1) << h_temp->GetMean();

		data[h][s][q][r] = h_temp->GetMean();

		h_temp->Delete();
	      
	      } // close if QIE_mask
	    } // close for over QIE
	  } // close if slot_mask
	} // close for over slot
      } // close if HF_mask
    } // close for over HF
    outfile << endl;

    outfile << "            rms:";
    for (Int_t h = 0; h < 6; h++) {
      if (lv0_mask[h] == 1) {
	for (Int_t s = 0 ; s < 13; s++) {
	  if (lv1_mask[h][s] == 1) {
	    for (Int_t q = 0; q < 24; q++) {
	      if (lv2_mask[h][s][q] == 1) {
		
		sprintf(hist_name,"%s/%s_HF%i_Slot%i_QIE%i","T_abs_CH","T_abs_CH",lv0_map[h],lv1_map[h][s],lv2_map[h][s][q]); 
	      
		h_temp = (TH1F*)_file0->Get(hist_name);
	  
		//		outfile << "HF" << lv0_map[h] << ", Slot " << lv1_map[h][s] << ", QIE" << lv2_map[h][s][q] << ": " << h_temp->GetMean() << " +- " << h_temp->GetRMS() << endl;
		outfile << ", " << setw(5) << setiosflags(ios::fixed) << setprecision(1) << h_temp->GetRMS();

		data[h][s][q][r] = h_temp->GetMean();

		h_temp->Delete();
	      
	      } // close if QIE_mask
	    } // close for over QIE
	  } // close if slot_mask
	} // close for over slot
      } // close if HF_mask
    } // close for over HF
    outfile << endl;


    _file0->Delete();

  } // close for over runs

  TCanvas *c1 = new TCanvas("c1","TitleHere",200,10,1024,768);
  vector<TGraph*> TGraphs;	

  gStyle->SetOptStat(0);

  TH2F* placeholder = new TH2F("placeholder","Timing vs Run",1000,0,(int)num_runs,1000,0,250);
  placeholder->GetXaxis()->SetTitle("RunNum");
  placeholder->GetYaxis()->SetTitle("TDC in ns from start of TS0");
  placeholder->Draw();

  //  TGraph *gr = new TGraph(num_runs,f_runs,data[0][9][0]);
  //  gr->SetLineColor(2);
  //  gr->Draw("ACPS");

  //  for (int r=0 ; r<num_runs ; r++) {
  //    cout << runs[r] << ": ";
  //    cout << data[0][9][0][r] << endl;
  //  }

  int counter = 0; 

  for (Int_t h = 0; h < 6; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < 13; s++) {
	if (lv1_mask[h][s] == 1) {
	  for (Int_t q = 0; q < 23; q++) {
	    if (lv2_mask[h][s][q] == 1) {

	      TGraphs.push_back(new TGraph(num_runs,f_runs,data[h][s][q]));

	      // TGraphs.back()->SetLineColor((counter%9)+1);

	      TGraphs.back()->SetMarkerStyle(4);

	      if (s == 8) {
		TGraphs.back()->SetLineColor(1);
	      }

	      if (s == 9) {
		if (q < 12) {
		  TGraphs.back()->SetLineColor(2);
		}
		else {
		  TGraphs.back()->SetLineColor(3);
		}
	      }

	      TGraphs.back()->SetLineWidth(2);
	      TGraphs.back()->Draw("LPS");
	      
	      counter++;
				  
	    } // close if QIE_mask
	  } // close for over QIE
	} // close if slot_mask
      } // close for over slot
    } // close if HF_mask
  } // close for over HF
  
    //  outfile << endl;

} // close function
