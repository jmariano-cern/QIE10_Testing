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

#include "../../src/draw_map_full.h"
#include "../../src/ActiveChannels.h"
#include "../../src/DETcoords2FEcoords.h"
#include "./ManageCoords.h"
#include "./HistogramTools.h"

// using namespace std;


void ici_test(Int_t run_num, Int_t SUITE_CODE, const char *Folder_NAME) {

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
 
  int peakTS = 4;
  int ADCPeakMin = 140;
  int ADCPeakMax = 150;
  int ADCRefMin = 65;
  int ADCRefMax = 100;
  int ADCEchoMax = 25;
  

  //////// full system PLOTS
  sprintf(hist0_name,"%s","ADCvsTS");
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
  int**** lv2_err_map_mainpeak = create_error_map();
  int**** lv2_err_map_reflection = create_error_map();
  int**** lv2_err_map_echo = create_error_map();


  ///////// CH PLOTS
  for (int i=0; i<numActiveChannels; i++) {

    ///// GET COORDINATES
    coords = setCoords(i);

    ////// MainPeak  
    sprintf(hist0_name,"%s","ADCvsTS");
    hist0 = processCH(hist0_name,run_num,coords,file0);	
    hist0.hist->GetXaxis()->SetRange(peakTS+1,peakTS+1);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean(2) < ADCPeakMin ) || (hist0.hist->GetMean(2) > ADCPeakMax ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_mainpeak [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "MainPeak ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " MainPeak Value: " << hist0.hist->GetMean(2) << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " MainPeak Value: " << hist0.hist->GetMean(2) << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->GetXaxis()->SetRange();
      hist0.hist->Draw();
      //      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"ADCvsTS",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_mainpeak[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1;    
      hist0.hist->Delete();
    }
   
 
    ////// Reflection
    sprintf(hist0_name,"%s","ADCvsTS");
    hist0 = processCH(hist0_name,run_num,coords,file0); 
    hist0.hist->GetXaxis()->SetRange(peakTS+3,peakTS+3);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean(2) < ADCRefMin ) || (hist0.hist->GetMean(2) > ADCRefMax ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_reflection [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "Reflection ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " Reflection Value: " << hist0.hist->GetMean(2) << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " Reflection Value: " << hist0.hist->GetMean(2) << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->GetXaxis()->SetRange();
      hist0.hist->Draw();
      //      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"ADCvsTS",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_reflection[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1; 
      hist0.hist->Delete();
    }



    ////// Pulse Echo
    sprintf(hist0_name,"%s","ADCvsTS");
    hist0 = processCH(hist0_name,run_num,coords,file0); 
    hist0.hist->GetXaxis()->SetRange(peakTS+5,peakTS+5);
    if ((hist0.exists == 0) ||  ( hist0.hist->GetMean(2) > ADCEchoMax ) || ( hist0.hist->GetEntries() < 10)) {
      lv2_err_map_echo [coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      lv2_err_map_gen[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 0;
      if (hist0.exists == 1) {
      cout << "Pulse Echo ERROR !!!!!! "<< endl;
      cout << "SIDE:"<< coords[0] << " Crate:"<< coords[4] <<" Slot:"<< coords[5] <<" Channel:"<< coords[6] << " -------->" << " Reflection Value: " << hist0.hist->GetMean(2) << endl;
      cout << "SIDE:"<< coords[0]  << " Eta:"<< coords[1] <<" Phi:"<< coords[2] <<" Depth:"<< coords[3] << " -------->" << " Reflection Value: " << hist0.hist->GetMean(2) << endl;
      cout << "----------------------------------------------------------------ooo------------------------------------------------------------------------------------ "<< endl;
      canv->cd();
      hist0.hist->GetXaxis()->SetRange();
      hist0.hist->Draw();
      //      canv->SetLogy();
      sideName="M";
      if (coords[0]>0){sideName="P";}
      sprintf(hist0_name,"../../img/%i/%s/%s_HF%s0%i_slot%i_channel%i.png",run_num,Folder_NAME,"ADCvsTS",sideName.c_str(),coords[4],coords[5],coords[6]);
      canv->SaveAs(hist0_name);

      }
    } else {
      lv2_err_map_echo[coords[0]][coords[4]-1][coords[5]-1][coords[6]-1] = 1; 
      hist0.hist->Delete();
    }



 
    
  } // close ch plots
  
  
  int creationdate = file0->GetCreationDate().GetDate();

  ///// DRAW ERROR MAPS
  draw_map(lv2_err_map_gen, run_num, Folder_NAME, "IQI Test" , creationdate);
  draw_map(lv2_err_map_mainpeak, run_num, Folder_NAME, "IQI Main Peak" , creationdate);
  draw_map(lv2_err_map_reflection, run_num, Folder_NAME, "IQI Reflection" , creationdate);
  draw_map(lv2_err_map_echo, run_num, Folder_NAME, "IQI Pulse Echo" , creationdate);

} // close function
