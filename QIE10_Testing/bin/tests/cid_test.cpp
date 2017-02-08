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

#include "../../src/draw_map.h"

#include "../../src/ActiveChannels.h"
#include "../../src/DETcoords2FEcoords.h"

#include "./ManageCoords.h"

using namespace std;

void cid_test(Int_t run_num) {

  //////// DECLARATIONS

  char root_file_name[512];
  TFile *_file0 =  new TFile();
  sprintf(root_file_name,"../../dat/QIE10testing_%i_1.root",run_num);
  _file0 = TFile::Open(root_file_name);

  char dir_name[512];
  sprintf(dir_name,"mkdir ../../img/%i",run_num);  
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/capid_test",run_num);  
  system(dir_name);
  sprintf(dir_name,"mkdir ../../img/%i/capid_test/rootFiles",run_num);
  system(dir_name);

  char dir0_name[512];
  TDirectory *_dir0 =  new TDirectory();

  char hist0_name[512];
  char hist0_full_name[512];
  bool exists_flag = 0; //exists0_flag
  TH1F *h0_temp = new TH1F();

  char figure0_name[512];

  //////// EV PLOTS

  TCanvas *c1 = new TCanvas("c1","c1",100,100,1024,768);
  sprintf(hist0_name,"%s/%s","CIDvsTS_EV","CIDvsTS_EV");
  sprintf(figure0_name,"../../img/%i/capid_test/Pulse.png",run_num);
  h0_temp = (TH1F*)_file0->Get(hist0_name);
  h0_temp->Draw("box");
  c1->SaveAs(figure0_name);
  c1->Clear();
  h0_temp->Delete();

  ///////// CH PLOTS

  int**** lv2_err_map_rot = create_error_map();
  for (int i=0; i<numActiveChannels; i++) {

    setCoords(i);

    sprintf(dir0_name,"%s","CapIDrot_CH");
    _dir0 = (TDirectory*)_file0->Get(dir0_name);
    sprintf(hist0_name,"%s_depth%i_iphi%i_ieta%i",dir0_name,DET_depth,DET_iphi,DET_ieta_signed);
    exists_flag = _dir0->GetListOfKeys()->Contains(hist0_name);
    sprintf(hist0_full_name,"%s/%s",dir0_name,hist0_name);

    if ( exists_flag == 0 ) {
      cout << "iEta: " << DET_ieta; 
      cout << " iPhi: " << DET_iphi; 
      cout << " Depth: " << DET_depth; 
      cout << " FE_crate: " << FE_crate; 
      cout << " FE_slot: " << FE_slot; 
      cout << " FE_channel: " << FE_channel;
      cout << "   <<<<<<<<<< MISSING HISTOGRAM";
    }
    cout << endl;

    if ( exists_flag == 1) {
      h0_temp = (TH1F*)_file0->Get(hist0_full_name);
    }
    if ( ( exists_flag == 0 )  ||  ( h0_temp->GetMean() != 0 ) || ( h0_temp->GetRMS() != 0 ) || ( h0_temp->GetEntries() < 10) ) {
      lv2_err_map_rot[side][FE_crate-1][FE_slot-1][FE_channel-1] = 0;
    } else {
      lv2_err_map_rot[side][FE_crate-1][FE_slot-1][FE_channel-1] = 1;
    }



  }

  draw_map(lv2_err_map_rot, run_num, "capid_test", "CapIDrotation");

} // close function
