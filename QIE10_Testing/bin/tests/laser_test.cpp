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
#include "TH2.h"

#include "../../src/draw_map_full.h"
#include "../../src/ActiveChannels.h"
#include "../../src/DETcoords2FEcoords.h"
#include "./ManageCoords.h"
#include "./HistogramTools.h"
#include "./DetectorPlots.h"

using namespace std;


void laser_test(Int_t run_num, Int_t SUITE_CODE, const char *Folder_NAME) {

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
 
  float timing_rms_high= 20;
  float qsum_mean_low = 150;
  float qsum_mean_high = 10000;
  float qsum_rms_low = 1;
  float qsum_rms_high = 650;
  float qratio_mean_low = 0.4;
  float qratio_mean_high = 1;
  float qratio_rms_low = 0.001;
  float qratio_rms_high = 0.1;

  SquareHist timing_detector_hist("Absolute Timing (ns)",25,75);
  SquareHistInv timing_inverted_detector_hist("Absolute Timing (ns)",25,75);
  PolarHist timing_polar_hist("Absolute Timing (ns)",25,75);
  PolarHistInv timing_inverted_polar_hist("Absolute Timing (ns)",25,75);

  SquareHist Qsum_detector("Integrated Charge (fC)",0,5000);
  PolarHistInv Qsum_detector_polar("Integrated Charge (fC)",0,5000);

  //////// full system PLOTS
  sprintf(hist0_name,"%s","T_abs");
  printEV(hist0_name,run_num,file0,Folder_NAME);
  
  sprintf(hist0_name,"%s","QSum");
  printEV(hist0_name,run_num,file0,Folder_NAME);

  sprintf(hist0_name,"%s","Qratio");
  printEV(hist0_name,run_num,file0,Folder_NAME); 

  TCanvas *canv = new TCanvas("canv","canv",100,100,1024,768);


  /////// INITIALIZE GENERAL ERROR MAP
  int**** lv2_err_map_gen = create_error_map();
  for (int i=0; i<numActiveChannels; i++) {
    coords = setCoords(i);
    lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
  }

  ////// PER TEST ERROR MAPS
  int**** lv2_err_map_timing_rms= create_error_map();
  int**** lv2_err_map_qsum_mean = create_error_map();
  int**** lv2_err_map_qsum_rms = create_error_map();
  int**** lv2_err_map_qratio_mean = create_error_map();
  int**** lv2_err_map_qratio_rms = create_error_map();



  ///////// CH PLOTS
  for (int i=0; i<numActiveChannels; i++) {

    ///// GET COORDINATES
    coords = setCoords(i);

    ////// Timing RMS  
    sprintf(hist0_name,"%s","T_abs");
    hist0 = processCH(hist0_name,run_num,coords,file0);	
    timing_detector_hist.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    timing_inverted_detector_hist.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    timing_polar_hist.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    timing_inverted_polar_hist.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    if ((hist0.exists == 0) || (hist0.hist->GetRMS() > timing_rms_high ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_timing_rms [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "Timing RMS ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " Timing RMS Value: " << hist0.hist->GetRMS() << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " Timing RMS Value: " << hist0.hist->GetRMS() << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->Draw();
      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"T_abs",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_timing_rms[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;    
      hist0.hist->Delete();
    }
   
 
    ////// QSum Mean 
    sprintf(hist0_name,"%s","QSum");
    hist0 = processCH(hist0_name,run_num,coords,file0);
    Qsum_detector.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    Qsum_detector_polar.Fill(coords[1],coords[2],coords[3],hist0.hist->GetMean());
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean() < qsum_mean_low  ) || (hist0.hist->GetMean() > qsum_mean_high ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_qsum_mean [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "QSum Mean ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " QSum Mean Value: " << hist0.hist->GetMean() << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " QSum Mean Value: " << hist0.hist->GetMean() << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->Draw();
      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"QSum",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_qsum_mean[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1; 
      hist0.hist->Delete();
    }


    ////// QSum RMS
    sprintf(hist0_name,"%s","QSum");
    hist0 = processCH(hist0_name,run_num,coords,file0);    
    if ((hist0.exists == 0) ||  ( hist0.hist->GetRMS() < qsum_rms_low  ) || (hist0.hist->GetRMS() > qsum_rms_high ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_qsum_rms [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "QSum RMS ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " QSum RMS Value: " << hist0.hist->GetRMS() << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " QSum RMS Value: " << hist0.hist->GetRMS() << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->Draw();
      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"QSum",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_qsum_rms[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
      hist0.hist->Delete();
    }


    ////// Qratio Mean
    sprintf(hist0_name,"%s","Qratio");
    hist0 = processCH(hist0_name,run_num,coords,file0);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean() < qratio_mean_low  ) || (hist0.hist->GetMean() > qratio_mean_high ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_qratio_mean [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "Qratio Mean ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " Qratio Mean Value: " << hist0.hist->GetMean() << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " Qratio Mean Value: " << hist0.hist->GetMean() << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->Draw();
      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"Qratio",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_qratio_mean[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
      hist0.hist->Delete();
    }


    ////// Qratio RMS
    sprintf(hist0_name,"%s","Qratio");
    hist0 = processCH(hist0_name,run_num,coords,file0);  
    if ((hist0.exists == 0) ||  ( hist0.hist->GetRMS() < qratio_rms_low  ) || (hist0.hist->GetRMS() > qratio_rms_high ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_qratio_rms [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "Qratio RMS ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " Qratio RMS Value: " << hist0.hist->GetRMS() << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " Qratio RMS Value: " << hist0.hist->GetRMS() << endl;
        cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->Draw();
      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"Qratio",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_qratio_rms[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;
      hist0.hist->Delete();
    }
 
    
  } // close ch plots
        
  int creationdate = file0->GetCreationDate().GetDate();
  ///// DRAW ERROR MAPS
  draw_map(lv2_err_map_gen, run_num, Folder_NAME, "LASER Test" , creationdate);
  draw_map(lv2_err_map_timing_rms, run_num, Folder_NAME, "Timing RMS" , creationdate);
  draw_map(lv2_err_map_qsum_mean, run_num, Folder_NAME, "QSum Mean" , creationdate);
  draw_map(lv2_err_map_qsum_rms, run_num, Folder_NAME, "QSum RMS" , creationdate);
  draw_map(lv2_err_map_qratio_mean, run_num, Folder_NAME, "Qratio Mean" , creationdate);
  draw_map(lv2_err_map_qratio_rms, run_num, Folder_NAME, "Qratio RMS" , creationdate);

  timing_detector_hist.Draw(run_num,Folder_NAME,"timing_detector_hist");
  timing_inverted_detector_hist.Draw(run_num,Folder_NAME,"timing_inverted_detector_hist");
  timing_polar_hist.Draw(run_num,Folder_NAME,"timing_polar_hist");
  timing_inverted_polar_hist.Draw(run_num,Folder_NAME,"timing_inverted_polar_hist");

  Qsum_detector.Draw(run_num,Folder_NAME,"Qsum_detector");
  Qsum_detector_polar.Draw(run_num,Folder_NAME,"Qsum_detector_polar");

} // close function
