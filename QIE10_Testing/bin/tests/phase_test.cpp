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

#include "../../src/draw_map_full.h"
#include "../../src/ActiveChannels.h"
#include "../../src/DETcoords2FEcoords.h"
#include "./ManageCoords.h"
#include "./HistogramTools.h"

// using namespace std;


void phase_test(Int_t run_num, Int_t SUITE_CODE, const char *Folder_NAME) {

  //////// DECLARATIONS AND INITIALIZATIONS
  char root_file_name[512];
  TFile *file0 =  new TFile();
  sprintf(root_file_name,"../../dat/QIE10testing_%i_%d.root",run_num, SUITE_CODE);
  file0 = TFile::Open(root_file_name);
  char dir_name[512];
  sprintf(dir_name,"mkdir -p ../../img/%i/%s/rootFiles",run_num,Folder_NAME);
  system(dir_name);
  char hist0_name[512];
  histData hist0;
  string sideName;
  int* coords;

  TH2F* hist2;
  TProfile* profile2;
  TF1* fit2;

  float offsetMax = 250;
  float offsetMin = 0;

  //////// full system PLOTS
  sprintf(hist0_name,"%s","phase_scan");
  printEV(hist0_name,run_num,file0,Folder_NAME);
  
  sprintf(hist0_name,"%s","T_abs");
  printEV(hist0_name,run_num,file0,Folder_NAME);

  TCanvas *canv = new TCanvas("canv","canv",100,100,1024,768);


  /////// INITIALIZE GENERAL ERROR MAP
  int**** lv2_err_map_gen = create_error_map();
  for (int i=0; i<numActiveChannels; i++) {
    coords = setCoords(i);
    lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
  }

  ////// PER TEST ERROR MAPS
  int**** lv2_err_map_slope = create_error_map();
  int**** lv2_err_map_offset = create_error_map();

  ///////// CH PLOTS
  for (int i=0; i<numActiveChannels; i++) {

    ///// GET COORDINATES
    coords = setCoords(i);

    ////// MainPeak  
    sprintf(hist0_name,"%s","phase_scan");

    sideName="M";
    if (coords[0]>0){sideName="P";}
    
    sprintf(hist0_name,"%s_CH/%s_CH_HF%s0%i_slot%i_channel%i","phase_scan","phase_scan",sideName.c_str(),coords[4],coords[5],coords[6]);

    hist2 = (TH2F*) file0->Get(hist0_name);
    profile2 = hist2->ProfileX();
    profile2->Fit("pol1","Q");
    fit2 = profile2->GetFunction("pol1");
    

    if ( fabs(fit2->GetParameter(1)+1)>0.01) {
      lv2_err_map_slope [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      cout << "Phase Slope ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << fit2->GetParameter(1) << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << fit2->GetParameter(1)<< endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist2->Draw();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"PhaseScan",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);
    } else {
      lv2_err_map_slope[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;    
    }
   
    if ( fit2->GetParameter(0) > offsetMax || fit2->GetParameter(0) < offsetMin ) {
      lv2_err_map_offset [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      cout << "Phase Offset ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << fit2->GetParameter(0) << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << fit2->GetParameter(0) << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist2->Draw();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"PhaseScan",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);
    } else {
      lv2_err_map_offset[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;    
    }
   

  } // close ch plots
  
  
  int creationdate = file0->GetCreationDate().GetDate();

  ///// DRAW ERROR MAPS
  draw_map(lv2_err_map_gen, run_num, Folder_NAME, "Phase Test" , creationdate);
  draw_map(lv2_err_map_slope, run_num, Folder_NAME, "Phase Slope" , creationdate);
  draw_map(lv2_err_map_offset, run_num, Folder_NAME, "Phase Offset" , creationdate);

} // close function
