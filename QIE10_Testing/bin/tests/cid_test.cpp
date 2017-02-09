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
#include "./HistogramTools.h"

using namespace std;

void cid_test(Int_t run_num) {

  //////// DECLARATIONS AND INITIALIZATIONS
  char root_file_name[512];
  TFile *file0 =  new TFile();
  sprintf(root_file_name,"../../dat/QIE10testing_%i_1.root",run_num);
  file0 = TFile::Open(root_file_name);
  char dir_name[512];
  sprintf(dir_name,"mkdir -p ../../img/%i/capid_test/rootFiles",run_num);
  system(dir_name);
  char hist0_name[512];
  histData hist0;
  int* coords;

  //////// EV PLOTS
  sprintf(hist0_name,"%s","CapIDrot");
  printEV(hist0_name,run_num,file0);

  /////// INITIALIZE GENERAL ERROR MAP
  int**** lv2_err_map_gen = create_error_map();
  for (int i=0; i<numActiveChannels; i++) {
    coords = setCoords(i);
    lv2_err_map_gen[side][coords[3]-1][coords[4]-1][coords[5]-1] = 1;
  }

  ////// PER TEST ERROR MAPS
  int**** lv2_err_map_rot = create_error_map();

  ///////// CH PLOTS
  for (int i=0; i<numActiveChannels; i++) {

    ///// GET COORDINATES
    coords = setCoords(i);

    ///// CID rotation
    sprintf(hist0_name,"%s","CapIDrot");
    hist0 = processCH(hist0_name,run_num,coords,file0);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean() != 0 ) || ( hist0.hist->GetRMS() != 0 ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_rot[side][coords[3]-1][coords[4]-1][coords[5]-1] = 0;
      lv2_err_map_gen[side][coords[3]-1][coords[4]-1][coords[5]-1] = 0;
    } else {
      lv2_err_map_rot[side][coords[3]-1][coords[4]-1][coords[5]-1] = 1;    
    }

  } // close ch plots

  ///// DRAW ERROR MAPS
  draw_map(lv2_err_map_rot, run_num, "capid_test", "CapIDrotation");
  draw_map(lv2_err_map_gen, run_num, "capid_test", "All");

} // close function

