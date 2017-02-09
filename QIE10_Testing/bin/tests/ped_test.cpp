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

void ped_test(Int_t run_num, int suite_code, char* test_name) {

  float ped_mean_low = 1.5;
  float ped_mean_high = 6.5;
  float ped_rms_low = 0.1;
  float ped_rms_high = 1.5;

  //////// DECLARATIONS AND INITIALIZATIONS
  char root_file_name[512];
  TFile *file0 =  new TFile();
  sprintf(root_file_name,"../../dat/QIE10testing_%i_%i.root",run_num,suite_code);
  file0 = TFile::Open(root_file_name);
  char dir_name[512];
  sprintf(dir_name,"mkdir -p ../../img/%i/%s/ped_test/rootFiles",run_num,test_name);
  system(dir_name);
  char hist0_name[512];
  histData hist0;
  int* coords;


  //////// EV PLOTS
  sprintf(hist0_name,"%s","ADC_spectrum");
  printEV(hist0_name,run_num,file0);



  /////// INITIALIZE GENERAL ERROR MAP
  int**** lv2_err_map_gen = create_error_map();
  for (int i=0; i<numActiveChannels; i++) {
    coords = setCoords(i);
    lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
  }

  ////// PER TEST ERROR MAPS
  int**** lv2_err_map_mean = create_error_map();
  int**** lv2_err_map_rms = create_error_map();


  ///////// CH PLOTS
  for (int i=0; i<numActiveChannels; i++) {

    ///// GET COORDINATES
    coords = setCoords(i);

    ///// Pedestal Mean
    sprintf(hist0_name,"%s","ADC_spectrum");
    hist0 = processCH(hist0_name,run_num,coords,file0);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean() < ped_mean_low ) || ( hist0.hist->GetMean() > ped_mean_high) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_mean[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
     
      cout << "Side: " << coords[0] << " / FE Crate: " << coords[4] << " / Slot: " << coords[5] << " / Channel: " << coords[6] << "   ............     Mean: " << hist0.hist->GetMean() << endl;       

    } else {
      lv2_err_map_mean[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
      hist0.hist->Delete();
    }

    ///// Pedestal RMS
    sprintf(hist0_name,"%s","ADC_spectrum");
    hist0 = processCH(hist0_name,run_num,coords,file0);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetRMS() < ped_rms_low ) || ( hist0.hist->GetRMS() >ped_rms_high) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_rms[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      cout << "Side: " << coords[0] << " / FE Crate: " << coords[4] << " / Slot: " << coords[5] << " / Channel: " << coords[6] << "   ............     RMS: " << hist0.hist->GetRMS() << endl;       
    } else {
      lv2_err_map_rms[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
      hist0.hist->Delete();
    }

} // close ch plots

   ///// DRAW ERROR MAPS
   draw_map(lv2_err_map_mean, run_num, test_name, "Ped_Mean");
   draw_map(lv2_err_map_rms, run_num, test_name, "Ped_RMS");
   draw_map(lv2_err_map_gen, run_num, test_name, "All");
} // close function


