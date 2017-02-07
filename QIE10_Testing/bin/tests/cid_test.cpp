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

//#include "../../src/mask.h"
#include "../../src/draw_map.h"

#include "../../src/ActiveChannels.h"
#include "../../src/DETcoords2FEcoords.h"

using namespace std;

void cid_test(Int_t run_num) {

  char hist0_name[512];
  char hist0_full_name[512];
  char root_file_name[512];
  char dir_name[512];
  char dir0_name[512];
  char figure0_name[512];
  //char outputfile0_name[512];

  sprintf(dir_name,"mkdir ../../img/%i",run_num);  
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/capid_test",run_num);  
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/capid_test/rootFiles",run_num);
  system(dir_name);

  TH1F *h0_temp = new TH1F();
  TFile *_file0 =  new TFile();
  TDirectory *_dir0 =  new TDirectory();
  //TFile *output_file =  new TFile();

  sprintf(root_file_name,"../../dat/QIE10testing_%i_1.root",run_num);
  _file0 = TFile::Open(root_file_name);

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  sprintf(hist0_name,"%s/%s","CIDvsTS_EV","CIDvsTS_EV");
  sprintf(figure0_name,"../../img/%i/capid_test/Pulse.png",run_num);
  h0_temp = (TH1F*)_file0->Get(hist0_name);
  h0_temp->Draw("box");
  c1->SaveAs(figure0_name);
  c1->Clear();
  h0_temp->Delete();

  int**** lv2_err_map_rot = create_error_map();
  //bool*** lv2_err_map_all = create_error_map(); 
  //bool*** lv2_err_map_gen = create_error_map();
  int DET_ieta   = -999;
  int DET_ieta_signed = -999;
  int DET_iphi   = -999;
  int DET_depth  = -999;
  int FE_crate   = -999;
  int FE_slot    = -999;
  int FE_channel = -999;
  int side       = -999;
  bool exists_flag = 0;
  for (int i=0; i<numActiveChannels; i++) {

    DET_ieta  = activeChannels[i][0];
    DET_iphi  = activeChannels[i][1];
    DET_depth = activeChannels[i][2];
    DET_ieta_signed = DET_ieta;
    if (DET_ieta>0) {
      side = 1;
    } else {
      side = 0;
      DET_ieta *= -1;
    }
    //cout << "side: " << side << " iEta: " << DET_ieta << " iPhi: " << DET_iphi << " depth: " << DET_depth << " -- Detcoords2FEcoords[" << side << "][" << DET_ieta-29 << "][" << (DET_iphi-1)/2 <<"][" << DET_depth-1 << "] = [" << DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][0] << "][" << DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][1] << "][" << DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][2] << "]" << endl;
    FE_crate   = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][0];
    FE_slot    = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][1];
    FE_channel = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][2];

    sprintf(dir0_name,"%s","CapIDrot_CH");
    _dir0 = (TDirectory*)_file0->Get(dir0_name);
    sprintf(hist0_name,"%s_depth%i_iphi%i_ieta%i",dir0_name,DET_depth,DET_iphi,DET_ieta_signed);
    //cout << "side: " << side << " iEta: " << DET_ieta_signed << " iPhi: " << DET_iphi << " depth: " << DET_depth << endl;
    //cout << _dir0->GetListOfKeys()->Contains(hist0_name) << endl;
    exists_flag = _dir0->GetListOfKeys()->Contains(hist0_name);
    sprintf(hist0_full_name,"%s/%s",dir0_name,hist0_name);
    if ( exists_flag == 1) {
      //cout << hist0_full_name << endl;
      h0_temp = (TH1F*)_file0->Get(hist0_full_name);
    }
    if ( ( exists_flag == 0 )  ||  ( h0_temp->GetMean() != 0 ) || ( h0_temp->GetRMS() != 0 ) || ( h0_temp->GetEntries() < 10) ) {
      //cout << "CHANNEL FAILED! (CID ROT) -- crate: " <<  FE_crate << " slot:" << FE_slot << " channel: " << FE_channel << endl;
      lv2_err_map_rot[side][FE_crate-1][FE_slot-1][FE_channel-1] = 0;
    } else {
      lv2_err_map_rot[side][FE_crate-1][FE_slot-1][FE_channel-1] = 1;
      //cout << "        passed! (CID ROT) -- crate: " <<  FE_crate << " slot:" << FE_slot << " channel: " << FE_channel << endl;
    }
  }

  //for (Int_t d = 1; d <=4; d++) {
    //if (lv0_mask[h] == 1) {
  //for (Int_t p = 3 ; p <= 71; p+=2) {
	//if (lv1_mask[h][s] == 1) {
	//sprintf(outputfile0_name,"../../img/%i/capid_test/rootFiles/CapIDTest_HF%i_Slot%i.root",run_num,h+1,s+1);
	//output_file = new TFile(outputfile0_name,"RECREATE");
  //for (Int_t e = -41; e <= 41; e++) {
	    //if (lv2_mask[h][s][q] == 1) {
	    //sprintf(dir_name,"QIE%i",q+1);
	      //output_file->mkdir(dir_name);

	      //sprintf(hist_name,"%s/%s_HF%i_Slot%i_QIE%i","ADC_ped_CH","ADC_ped_CH",lv0_map[h],lv1_map[h][s],lv2_map[h][s][q]);
	      //cout << "processing d: " << d << " p:" << p << " e: " << e << endl;

	    
	      //sprintf(hist0_name,"%s/%s_depth%i_iphi%i_ieta%i","CapIDrot_CH","CapIDrot_CH",d,p,e);
	      //sprintf(hist1_name,"%s/%s_depth%i_iphi%i_ieta%i","CapAllign_CH","CapAllign_CH",d,p,e);
	      //sprintf(hist2_name,"%s/%s_depth%i_iphi%i_ieta%i","CIDvsTS_CH","CIDvsTS_CH",d,p,e);
	      //h0_temp = (TH1F*)_file0->Get(hist0_name);
	      //h1_temp = (TH1F*)_file0->Get(hist1_name);
	      //h2_temp = (TH1F*)_file0->Get(hist2_name);
	      //if ( ( h0_temp->GetMean() != 0 ) || ( h0_temp->GetRMS() != 0 ) || (h0_temp->GetEntries() < 10) ) {
  //cout << "CHANNEL FAILED  CID ROT -- d: " << d << " p:" << p << " e: " << e << endl;
		//lv2_err_map_rot[h][s][q] = 0;
		//lv2_err_map_gen[h][s][q] = 0;
  //}	
	      //if ( ( h1_temp->GetMean() != 0 ) || ( h1_temp->GetRMS() != 0 ) || (h1_temp->GetEntries() < 10) ) {
		//lv2_err_map_all[h][s][q] = 0;
		//lv2_err_map_gen[h][s][q] = 0;
	      //}	
	      //output_file->cd();	      
	      //sprintf(dir_name,"QIE%i",q+1);
	      //output_file->cd(dir_name);
	      //h0_temp->Write();
	      //h1_temp->Write();
	      //h2_temp->Write();
	      //h0_temp->Delete();
	      //h1_temp->Delete();
	      //h2_temp->Delete();
	      //}
	      //if (e == -29) {
  //e = 29;
  //}
  // }
	  //output_file->Write();
	  //output_file->Close();
	  //}
  //  }
      //}
  // }
  draw_map(lv2_err_map_rot, run_num, "capid_test", "CapIDrotation");
  //draw_map(lv2_err_map_all, run_num, "capid_test", "CapIDalligment");
  //draw_map(lv2_err_map_gen, run_num, "capid_test", "CapIDgeneral");

} // close function
