
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
#include "TProfile.h"

#include "../../src/mask.h"
#include "../../src/draw_map.h"

using namespace std;

void phase_test(Int_t run_num) {


  char hist0_name[512]; 
  char file0_name[512];
  char outputfile0_name[512];
  char dir_name[512];
  char figure0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/phase_test",run_num);
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/phase_test/rootFiles",run_num);
  system(dir_name);

  TH2F *h0_temp = new TH2F();
  TProfile *p0_temp = new TProfile();
  TF1 *fitFunction = new TF1();
  TFile *_file0 =  new TFile();
  TFile *output_file = new TFile();
  double phaseSlope = 0;

  double binContent = 0;
  double fitEval = 0;
  
  sprintf(file0_name,"../../dat/QIE10testing_%i_5.root",run_num);
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


  bool*** lv2_err_map_slope = create_error_map();
  bool*** lv2_err_map_residual = create_error_map();
  bool*** lv2_err_map_gen = create_error_map();
  double slopes[24];
  

  for (Int_t h = 0; h < HF_num; h++) {
    if (lv0_mask[h] == 1) {
      for (Int_t s = 0 ; s < SL_num; s++) {
	if (lv1_mask[h][s] == 1) {
	  sprintf(outputfile0_name,"../../img/%i/phase_test/rootFiles/PedTest_HF%i_Slot%i.root",run_num,h+1,s+1);
	  output_file = new TFile(outputfile0_name,"RECREATE");
	  memset(slopes,0.0,sizeof(slopes));//[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  for (Int_t q = 0; q < QI_num; q++) {
	    if (lv2_mask[h][s][q] == 1) {
	      sprintf(dir_name,"QIE%i",q+1);
	      output_file->mkdir(dir_name);
	      sprintf(hist0_name,"%s/%s_HF%i_Slot%i_QIE%i","phase_scan_CID_CH","phase_scan_CID_CH",h+1,s+1,q+1);
	      cout << hist0_name << endl;
	      h0_temp = (TH2F*)_file0->Get(hist0_name);
	      p0_temp = (TProfile*) h0_temp->ProfileX();
	      p0_temp->Fit("pol1","Q");
	      fitFunction = p0_temp->GetFunction("pol1");
	      phaseSlope = fitFunction->GetParameter(1);
	      slopes[q] = phaseSlope;
	      sprintf(figure0_name,"../../img/%i/phase_test/scan_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      //h0_temp->SetTitle("Bias Voltage Scan with Sequencer");
	      h0_temp->GetXaxis()->SetTitle("Phase Delay");
	      h0_temp->GetYaxis()->SetTitle("Absolute Timing (ns)");
	      h0_temp->GetYaxis()->SetTitleOffset(1.5);
	      c1->SaveAs(figure0_name);
	      c1->Clear();

	      if (abs(phaseSlope+1) > 0.01){
		lv2_err_map_slope[h][s][q] = 0;
		lv2_err_map_gen[h][s][q] = 0;
	      }	

	      for(Int_t bin = 1; bin < 101; bin++){
		binContent = p0_temp->GetBinContent(bin);
		fitEval = fitFunction->Eval(p0_temp->GetBinCenter(bin));
		if ( abs(binContent-fitEval) > 2){
		  lv2_err_map_residual[h][s][q] = 0;
		  lv2_err_map_gen[h][s][q] = 0;		  
		  break;
		}
	      }

	      
	      // sprintf(hist1_name,"%s/%s_HF%i_Slot%i_QIE%i","qratio2_vb_PR","qratio2_vb_PR",h+1,s+1,q+1);
	      // h1_temp = (TH1F*)_file0->Get(hist1_name);
	      // sprintf(figure1_name,"../../img/%i/vbs_test/ratio2_HF%i_SL%i_QI%i.png",run_num,h+1,s+1,q+1);
	      // //h0_temp->SetTitle("Bias Voltage Scan with Sequencer");
	      // h1_temp->GetXaxis()->SetTitle("LED Bias Voltage (V)");
	      // h1_temp->GetYaxis()->SetTitle("(Qmax+Qnext)/Qsum");
	      // h1_temp->GetYaxis()->SetTitleOffset(1.5);
	      // h1_temp->Draw();
	      // c1->SaveAs(figure1_name);
	      // c1->Clear();

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
	      p0_temp->Write();
	      h0_temp->Delete();
	      p0_temp->Delete();
	    }
	  }
	  char outputHistName[100];
	  sprintf(outputHistName,"PhaseSlopes_HF%i_Slot%i",h+1,s+1);
	  TH1F slopeDist = TH1F(outputHistName, outputHistName, 20,-1.01,-0.99);
	  for (int n=0; n<24;n++){
	    if (slopes[n]>-0.99){
	      slopeDist.Fill(-0.991);
	    } else if (slopes[n]>-1.01){
	      slopeDist.Fill(slopes[n]);
	    } else{
	      slopeDist.Fill(-1.009);
	    }
	  }
	  slopeDist.SetFillColor(kRed);
	  slopeDist.GetXaxis()->SetTitle("Phase Delay Slope");
	  slopeDist.GetYaxis()->SetTitle("Channels");
	  slopeDist.GetYaxis()->SetTitleOffset(1.5);
	  slopeDist.DrawCopy();
	  slopeDist.GetXaxis()->SetRange(6,14);
	  slopeDist.SetFillColor(kGreen);
	  slopeDist.DrawCopy("same");
	  sprintf(figure0_name,"../../img/%i/phase_test/PhaseDelaySlope_HF%i_SL%i.png",run_num,h+1,s+1);
	  c1->SaveAs(figure0_name);	  
	  slopeDist.Delete();
	  output_file->Write();
	  output_file->Close();
	}
      }
    }
  }
  
  draw_map(lv2_err_map_slope, run_num, "phase_test", "PhaseScanSlope");
  draw_map(lv2_err_map_residual, run_num, "phase_test", "PhaseScanResiduals");
  draw_map(lv2_err_map_gen, run_num, "phase_test", "PhaseScanAll");
  
} // close function

