// #############################################################
// #############################################################
// ###                                                       ###
// ###                     ./QIE10_loop.h                    ###
// ###                                                       ###
// ###           This file was written by a robot!           ###
// ###                  Do not edit by hand!                 ###
// ###                                                       ###
// ###        Instead, modify the configuration file:        ###
// ###                    QIE10_testing.cfg                  ###
// ###                 and then rebuild with:                ###
// ###                       ./rebuild                       ###
// ###                                                       ###
// ###           Joseph Mariano -- jmariano@cern.ch          ###
// ###                       27-04-2016                      ###
// ###                                                       ###
// #############################################################
// #############################################################

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

using namespace std;

// ########################
// # LOOP_VARS STRUCTURES #
// ########################

// --- Suite 0: "full" ---

struct loop_vars {

  int pdac2poff[64] = {0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
  float ici2q[8] = {90, 180, 360, 720, 1440, 2880, 5760, 8640};
  int adcped = 4;
  float qped = adc2fC_QIE10_refl[ adcped ];
  bool corrupt = 0;
  float qsum = 0;
  float qmax = 0;
  int ts_max = 0;
  float qmax2p = 0;
  float qmax2m = 0;
  float qmax3 = 0;
  int ts_TDC1 = -1;
  int ts_TDC2 = -1;
  int cid_TDC1 = -1;
  int cid_TDC2 = -1;
  int* ch0_cid_ev = new int[10];
  int* ch0_cid_ch = new int[10];
  bool bad_flag;

};

// ###################
// # PRE_EVENT_LOOPS #
// ###################

loop_vars pre_event_loop(std::string parameter, float val, int suite_code, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

  loop_vars global;

// --- Suite 0: "full" ---

  if (suite_code == 0) {


  }

// --- Suite 1: "CapIDtesting" ---

  if (suite_code == 1) {


  }

// --- Suite 2: "PedestalData" ---

  if (suite_code == 2) {


  }

// --- Suite 3: "AbsoluteTiming" ---

  if (suite_code == 3) {


  }

// --- Suite 4: "PedScanner" ---

  if (suite_code == 4) {


  }

// --- Suite 5: "PhaseScanner" ---

  if (suite_code == 5) {


  }

// --- Suite 6: "ICIScanner" ---

  if (suite_code == 6) {


  }

// --- Suite 7: "LED_Vb_Scan" ---

  if (suite_code == 7) {


  }

// --- Suite 8: "RinSel_Scan" ---

  if (suite_code == 8) {


  }

// --- Suite 9: "BXscanner" ---

  if (suite_code == 9) {


  }

  return global;

}

// #############
// # PRE_LOOPS #
// #############

loop_vars pre_loop(std::string parameter, float val, int suite_code, loop_vars global, QIE10DataFrame digis, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }
    DetId detid_loc = digis.detid();
    HcalDetId hcaldetid_loc = HcalDetId(detid_loc);
    int iEta_loc = hcaldetid_loc.ieta();
    int iPhi_loc = hcaldetid_loc.iphi();
    int Depth_loc = hcaldetid_loc.depth();
    global.bad_flag = 0;
    for (int iTS=1 ; iTS<10 ; iTS++) {
      if ( (digis[iTS].capid() - (digis[iTS-1].capid()+1)%4) != 0 ) {
        global.bad_flag = 1;
      }
    }
    if (global.corrupt == 0) {
      if (global.bad_flag == 1) {
        loggers[0] << "#############" << endl;
        loggers[0] << "# BAD EVENT" << endl;
        loggers[0] << "# HF: " << Depth_loc << endl;
        loggers[0] << "# Slot: " << iPhi_loc << endl;
        loggers[0] << "# QIE: " << iEta_loc << endl;
        loggers[0] << "# nEV:" << _event_num << endl;
        loggers[0] << "#############" << endl;
        for (int iTS=0 ; iTS<10 ; iTS++) {
          if (( (digis[iTS].capid() - (digis[iTS-1].capid()+1)%4) != 0 ) && ( iTS != 0 )) {
            loggers[0] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << "  <<<<<<<<" << endl;
          }
          else {
            loggers[0] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << endl;
          }
        }
        loggers[0] << "#############" << endl;
      }
    }
    DetId detid_loc_z = digis.detid();
    HcalDetId hcaldetid_loc_z = HcalDetId(detid_loc_z);
    int iEta_loc_z = hcaldetid_loc_z.ieta();
    int iPhi_loc_z = hcaldetid_loc_z.iphi();
    int Depth_loc_z = hcaldetid_loc_z.depth();
    if (global.corrupt == 1) {
       loggers[1] << "#############" << endl;
       loggers[1] << "# BAD EVENT" << endl;
       loggers[1] << "# HF: " << Depth_loc_z << endl;
       loggers[1] << "# Slot: " << iPhi_loc_z << endl;
       loggers[1] << "# QIE: " << iEta_loc_z << endl;
       loggers[1] << "# nEV:" << _event_num << endl;
       loggers[1] << "#############" << endl;
       for (int iTS=0 ; iTS<10 ; iTS++) {
          loggers[1] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << endl;
       }
       loggers[1] << "#############" << endl;
    }

  }

// --- Suite 1: "CapIDtesting" ---

  if (suite_code == 1) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }
    DetId detid_loc = digis.detid();
    HcalDetId hcaldetid_loc = HcalDetId(detid_loc);
    int iEta_loc = hcaldetid_loc.ieta();
    int iPhi_loc = hcaldetid_loc.iphi();
    int Depth_loc = hcaldetid_loc.depth();
    global.bad_flag = 0;
    for (int iTS=1 ; iTS<10 ; iTS++) {
      if ( (digis[iTS].capid() - (digis[iTS-1].capid()+1)%4) != 0 ) {
        global.bad_flag = 1;
      }
    }
    if (global.corrupt == 0) {
      if (global.bad_flag == 1) {
        loggers[0] << "#############" << endl;
        loggers[0] << "# BAD EVENT" << endl;
        loggers[0] << "# HF: " << Depth_loc << endl;
        loggers[0] << "# Slot: " << iPhi_loc << endl;
        loggers[0] << "# QIE: " << iEta_loc << endl;
        loggers[0] << "# nEV:" << _event_num << endl;
        loggers[0] << "#############" << endl;
        for (int iTS=0 ; iTS<10 ; iTS++) {
          if (( (digis[iTS].capid() - (digis[iTS-1].capid()+1)%4) != 0 ) && ( iTS != 0 )) {
            loggers[0] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << "  <<<<<<<<" << endl;
          }
          else {
            loggers[0] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << endl;
          }
        }
        loggers[0] << "#############" << endl;
      }
    }
    DetId detid_loc_z = digis.detid();
    HcalDetId hcaldetid_loc_z = HcalDetId(detid_loc_z);
    int iEta_loc_z = hcaldetid_loc_z.ieta();
    int iPhi_loc_z = hcaldetid_loc_z.iphi();
    int Depth_loc_z = hcaldetid_loc_z.depth();
    if (global.corrupt == 1) {
       loggers[1] << "#############" << endl;
       loggers[1] << "# BAD EVENT" << endl;
       loggers[1] << "# HF: " << Depth_loc_z << endl;
       loggers[1] << "# Slot: " << iPhi_loc_z << endl;
       loggers[1] << "# QIE: " << iEta_loc_z << endl;
       loggers[1] << "# nEV:" << _event_num << endl;
       loggers[1] << "#############" << endl;
       for (int iTS=0 ; iTS<10 ; iTS++) {
          loggers[1] << "TS:" << iTS << "  --  CID " << digis[iTS].capid() << "  --  ADC " << digis[iTS].adc() << "  --  leTDC " << digis[iTS].le_tdc() << "  --  teTDC " << digis[iTS].te_tdc() << endl;
       }
       loggers[1] << "#############" << endl;
    }

  }

// --- Suite 2: "PedestalData" ---

  if (suite_code == 2) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 3: "AbsoluteTiming" ---

  if (suite_code == 3) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 4: "PedScanner" ---

  if (suite_code == 4) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 5: "PhaseScanner" ---

  if (suite_code == 5) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 6: "ICIScanner" ---

  if (suite_code == 6) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 7: "LED_Vb_Scan" ---

  if (suite_code == 7) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 8: "RinSel_Scan" ---

  if (suite_code == 8) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

// --- Suite 9: "BXscanner" ---

  if (suite_code == 9) {

    global.corrupt = 0;
    for (int iTS=0 ; iTS<10 ; iTS++) {
      if (digis[iTS].le_tdc() == 0) {
        global.corrupt = 1;
      }
    }
    global.qsum = 0;
    global.qmax = 0;
    global.ts_max = 0;
    global.ts_TDC1 = -1;
    global.ts_TDC2 = -1;
    global.cid_TDC1 = -1;
    global.cid_TDC2 = -1;
    bool TDC1_flag = 0;
    bool TDC2_flag = 0;
    for ( int iTS=0 ; iTS<digis.samples() ; iTS++) {
      global.qsum += adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE10_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].le_tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax2p = adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
      global.qmax2m = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + (2*14.45);
      global.qmax3 = adc2fC_QIE10_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE10_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }

  }

  return global;

}

// #########
// # LOOPS #
// #########

loop_vars loop(std::string parameter, float val, int suite_code, loop_vars global, QIE10DataFrame digis, int nTS, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (nTS != 0) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill (capid - (digis[nTS-1].capid()+1)%4 );
        }
      }
    }
    if (nCH == 0) {
      global.ch0_cid_ev[nTS] = digis[nTS].capid();
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[1]->Fill (capid - global.ch0_cid_ev[nTS]);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[2]->Fill ( adc );
      }
    }
    if ( val == 38.0 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[6]->Fill ( adc );
        }
      }
    }
    if ( val == 7 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[7]->Fill ( global.qmax / global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( nTS, capid );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( nTS, adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[2]->Fill ( capid , adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[3]->Fill ( nTS, le_tdc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[4]->Fill ( nTS, charge );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[5]->Fill ( global.pdac2poff[(int)val] , adc);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[6]->Fill ( global.pdac2poff[(int)val] , charge);
      }
    }
    if (nTS != 0) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill (capid - (digis[nTS-1].capid()+1)%4 );
        }
      }
    }
    if (nCH == 0) {
      global.ch0_cid_ch[nTS] = digis[nTS].capid();
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[1][nCH]->Fill (capid - global.ch0_cid_ch[nTS]);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[2][nCH]->Fill ( adc );
      }
    }
    if ( val == 38.0 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[6][nCH]->Fill ( adc );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, capid );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[1][nCH]->Fill ( nTS, adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[2][nCH]->Fill ( capid , adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[3][nCH]->Fill ( nTS, le_tdc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[4][nCH]->Fill ( nTS, charge );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[5][nCH]->Fill ( global.pdac2poff[(int)val] , adc);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[6][nCH]->Fill ( global.pdac2poff[(int)val] , charge);
      }
    }
    if (global.cid_TDC1 > -1) {
      if (global.cid_TDC1 == 0) {
        if (global.corrupt == 0) {
          if (_event_num > 2) {
            TH2F_perCHs[11][nCH]->Fill (nTS,adc);
          }
        }
      }
    }
    if ( val == 0 ) {
      TH2F_perCHs[16][nCH]->Fill ( nTS, adc );
    }
    if ( val == 1 ) {
      TH2F_perCHs[17][nCH]->Fill ( nTS, adc );
    }
    if ( val == 2 ) {
      TH2F_perCHs[18][nCH]->Fill ( nTS, adc );
    }
    if ( val == 3 ) {
      TH2F_perCHs[19][nCH]->Fill ( nTS, adc );
    }
    if ( val == 4 ) {
      TH2F_perCHs[20][nCH]->Fill ( nTS, adc );
    }
    if ( val == 5 ) {
      TH2F_perCHs[21][nCH]->Fill ( nTS, adc );
    }
    if ( val == 6 ) {
      TH2F_perCHs[22][nCH]->Fill ( nTS, adc );
    }
    if ( val == 7 ) {
      TH2F_perCHs[23][nCH]->Fill ( nTS, adc );
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[26][nCH]->Fill (nTS,charge);
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[27][nCH]->Fill (nTS,adc);
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[28][nCH]->Fill (nTS,le_tdc);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[29][nCH]->Fill (nTS,charge);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[30][nCH]->Fill (nTS,adc);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[31][nCH]->Fill (nTS,le_tdc);
    }
    if ( val == 0 ) {
      TH2F_perCHs[33][nCH]->Fill (nTS,adc);
    }
    if ( val == 1 ) {
      TH2F_perCHs[34][nCH]->Fill (nTS,adc);
    }
    if ( val == 2 ) {
      TH2F_perCHs[35][nCH]->Fill (nTS,adc);
    }
    if ( val == 3 ) {
      TH2F_perCHs[36][nCH]->Fill (nTS,adc);
    }
    if ( val == 4 ) {
      TH2F_perCHs[37][nCH]->Fill (nTS,adc);
    }
    if ( val == 5 ) {
      TH2F_perCHs[38][nCH]->Fill (nTS,adc);
    }
    if ( val == 6 ) {
      TH2F_perCHs[39][nCH]->Fill (nTS,adc);
    }
    if ( val == 7 ) {
      TH2F_perCHs[40][nCH]->Fill (nTS,adc);
    }
    if ( val == 8 ) {
      TH2F_perCHs[41][nCH]->Fill (nTS,adc);
    }
    if ( val == 9 ) {
      TH2F_perCHs[42][nCH]->Fill (nTS,adc);
    }
    if ( val == 10 ) {
      TH2F_perCHs[43][nCH]->Fill (nTS,adc);
    }
    if ( val == 11 ) {
      TH2F_perCHs[44][nCH]->Fill (nTS,adc);
    }
    if ( val == 12 ) {
      TH2F_perCHs[45][nCH]->Fill (nTS,adc);
    }
    if ( val == 13 ) {
      TH2F_perCHs[46][nCH]->Fill (nTS,adc);
    }
    if ( val == 14 ) {
      TH2F_perCHs[47][nCH]->Fill (nTS,adc);
    }
    if ( val == 15 ) {
      TH2F_perCHs[48][nCH]->Fill (nTS,adc);
    }

  }

// --- Suite 1: "CapIDtesting" ---

  if (suite_code == 1) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (nTS != 0) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill (capid - (digis[nTS-1].capid()+1)%4 );
        }
      }
    }
    if (nCH == 0) {
      global.ch0_cid_ev[nTS] = digis[nTS].capid();
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[1]->Fill (capid - global.ch0_cid_ev[nTS]);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( nTS, capid );
      }
    }
    if (nTS != 0) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill (capid - (digis[nTS-1].capid()+1)%4 );
        }
      }
    }
    if (nCH == 0) {
      global.ch0_cid_ch[nTS] = digis[nTS].capid();
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[1][nCH]->Fill (capid - global.ch0_cid_ch[nTS]);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, capid );
      }
    }

  }

// --- Suite 2: "PedestalData" ---

  if (suite_code == 2) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[0]->Fill ( adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( nTS, adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( capid , adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[0][nCH]->Fill ( adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, adc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[1][nCH]->Fill ( capid , adc );
      }
    }

  }

// --- Suite 3: "AbsoluteTiming" ---

  if (suite_code == 3) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( nTS, le_tdc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( nTS, charge );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, le_tdc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[1][nCH]->Fill ( nTS, charge );
      }
    }

  }

// --- Suite 4: "PedScanner" ---

  if (suite_code == 4) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if ( val == 38.0 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill ( adc );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( global.pdac2poff[(int)val] , adc);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( global.pdac2poff[(int)val] , charge);
      }
    }
    if ( val == 38.0 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill ( adc );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( global.pdac2poff[(int)val] , adc);
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[1][nCH]->Fill ( global.pdac2poff[(int)val] , charge);
      }
    }

  }

// --- Suite 5: "PhaseScanner" ---

  if (suite_code == 5) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (global.cid_TDC1 > -1) {
      if (global.cid_TDC1 == 0) {
        if (global.corrupt == 0) {
          if (_event_num > 2) {
            TH2F_perCHs[3][nCH]->Fill (nTS,adc);
          }
        }
      }
    }

  }

// --- Suite 6: "ICIScanner" ---

  if (suite_code == 6) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if ( val == 7 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[1]->Fill ( global.qmax / global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, le_tdc );
      }
    }
    if ( val == 0 ) {
      TH2F_perCHs[5][nCH]->Fill ( nTS, adc );
    }
    if ( val == 1 ) {
      TH2F_perCHs[6][nCH]->Fill ( nTS, adc );
    }
    if ( val == 2 ) {
      TH2F_perCHs[7][nCH]->Fill ( nTS, adc );
    }
    if ( val == 3 ) {
      TH2F_perCHs[8][nCH]->Fill ( nTS, adc );
    }
    if ( val == 4 ) {
      TH2F_perCHs[9][nCH]->Fill ( nTS, adc );
    }
    if ( val == 5 ) {
      TH2F_perCHs[10][nCH]->Fill ( nTS, adc );
    }
    if ( val == 6 ) {
      TH2F_perCHs[11][nCH]->Fill ( nTS, adc );
    }
    if ( val == 7 ) {
      TH2F_perCHs[12][nCH]->Fill ( nTS, adc );
    }

  }

// --- Suite 7: "LED_Vb_Scan" ---

  if (suite_code == 7) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[0]->Fill ( nTS, le_tdc );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( nTS, le_tdc );
      }
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[3][nCH]->Fill (nTS,charge);
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[4][nCH]->Fill (nTS,adc);
    }
    if ( val == 2.5 ) {
      TH2F_perCHs[5][nCH]->Fill (nTS,le_tdc);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[6][nCH]->Fill (nTS,charge);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[7][nCH]->Fill (nTS,adc);
    }
    if ( val == 2.0 ) {
      TH2F_perCHs[8][nCH]->Fill (nTS,le_tdc);
    }

  }

// --- Suite 8: "RinSel_Scan" ---

  if (suite_code == 8) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;
    if ( val == 0 ) {
      TH2F_perCHs[1][nCH]->Fill (nTS,adc);
    }
    if ( val == 1 ) {
      TH2F_perCHs[2][nCH]->Fill (nTS,adc);
    }
    if ( val == 2 ) {
      TH2F_perCHs[3][nCH]->Fill (nTS,adc);
    }
    if ( val == 3 ) {
      TH2F_perCHs[4][nCH]->Fill (nTS,adc);
    }
    if ( val == 4 ) {
      TH2F_perCHs[5][nCH]->Fill (nTS,adc);
    }
    if ( val == 5 ) {
      TH2F_perCHs[6][nCH]->Fill (nTS,adc);
    }
    if ( val == 6 ) {
      TH2F_perCHs[7][nCH]->Fill (nTS,adc);
    }
    if ( val == 7 ) {
      TH2F_perCHs[8][nCH]->Fill (nTS,adc);
    }
    if ( val == 8 ) {
      TH2F_perCHs[9][nCH]->Fill (nTS,adc);
    }
    if ( val == 9 ) {
      TH2F_perCHs[10][nCH]->Fill (nTS,adc);
    }
    if ( val == 10 ) {
      TH2F_perCHs[11][nCH]->Fill (nTS,adc);
    }
    if ( val == 11 ) {
      TH2F_perCHs[12][nCH]->Fill (nTS,adc);
    }
    if ( val == 12 ) {
      TH2F_perCHs[13][nCH]->Fill (nTS,adc);
    }
    if ( val == 13 ) {
      TH2F_perCHs[14][nCH]->Fill (nTS,adc);
    }
    if ( val == 14 ) {
      TH2F_perCHs[15][nCH]->Fill (nTS,adc);
    }
    if ( val == 15 ) {
      TH2F_perCHs[16][nCH]->Fill (nTS,adc);
    }

  }

// --- Suite 9: "BXscanner" ---

  if (suite_code == 9) {

    global.adcped = global.adcped;
    global.qped = global.qped;
    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].le_tdc();
    le_tdc = le_tdc;
    int te_tdc = digis[nTS].te_tdc();
    te_tdc = te_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE10_refl[ adc ] + 14.45;
    charge = charge;
    float acharge = charge - global.qped;
    acharge = acharge;

  }

  return global;

}

// ##############
// # POST_LOOPS #
// ##############

loop_vars post_loop(std::string parameter, float val, int suite_code, loop_vars global, QIE10DataFrame digis, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[3]->Fill ( global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[4]->Fill ( global.qsum / float(digis.samples()) );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[5]->Fill (t_abs_ev);
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[7]->Fill ( global.pdac2poff[(int)val] , global.qsum/(float)digis.samples() );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ph_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perEVs[8]->Fill (val/2.0,t_abs_ph_ev);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perEVs[8]->Fill ((val-14.0)/2.0,t_abs_ph_ev);
          }
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        if ( val < 50 ) {
          TH2F_perEVs[9]->Fill (val/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
        if (( val > 63 ) && ( val < 114 )) {
          TH2F_perEVs[9]->Fill ((val-14.0)/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      float t_abs_ph_cid_ev = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perEVs[10]->Fill (val/2.0,t_abs_ph_cid_ev);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perEVs[10]->Fill ((val-14.0)/2.0,t_abs_ph_cid_ev);
          }
        }
      }
    }
    if (val < 8 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH2F_perEVs[11]->Fill ( global.ici2q[(int)val] , global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[12]->Fill ( val , global.qmax / global.qsum );
      }
    }
    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TH2F_perEVs[13]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perEVs[14]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perEVs[15]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[3][nCH]->Fill ( global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[4][nCH]->Fill ( global.qsum / float(digis.samples()) );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[5][nCH]->Fill (t_abs_ch);
        }
      }
    }
    if ( val == 7 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[7][nCH]->Fill ( global.qmax / global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[7][nCH]->Fill ( global.pdac2poff[(int)val] , global.qsum/(float)digis.samples() );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ph_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perCHs[8][nCH]->Fill (val/2.0,t_abs_ph_ch);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perCHs[8][nCH]->Fill ((val-14.0)/2.0,t_abs_ph_ch);
          }
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        if ( val < 50 ) {
          TH2F_perCHs[9][nCH]->Fill (val/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
        if (( val > 63 ) && ( val < 114 )) {
          TH2F_perCHs[9][nCH]->Fill ((val-14.0)/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      float t_abs_ph_cid_ch = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perCHs[10][nCH]->Fill (val/2.0,t_abs_ph_cid_ch);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perCHs[10][nCH]->Fill ((val-14.0)/2.0,t_abs_ph_cid_ch);
          }
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      if (global.cid_TDC1 == 0) {
        if (global.corrupt == 0) {
          if (_event_num > 2) {
            DetId detid_loc_WS = digis.detid();
            HcalDetId hcaldetid_loc_WS = HcalDetId(detid_loc_WS);
            int iEta_loc_WS = hcaldetid_loc_WS.ieta();
            int iPhi_loc_WS = hcaldetid_loc_WS.iphi();
            int Depth_loc_WS = hcaldetid_loc_WS.depth();
            cout << "##############" << endl;
            cout << "# WEIRD EVENT" << endl;
            cout << "# HF: " << Depth_loc_WS << endl;
            cout << "# Slot: " << iPhi_loc_WS << endl;
            cout << "# QIE: " << iEta_loc_WS << endl;
            cout << "# nEV:" << _event_num << endl;
  	  cout << "# phase: " << val << endl;
            cout << "#############" << endl;
          }
        }
      }
    }
    if (val < 8 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH2F_perCHs[12][nCH]->Fill ( global.ici2q[(int)val] , global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[13][nCH]->Fill ( val , global.qmax / global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[14][nCH]->Fill ( val , (adc2fC_QIE10_refl[ digis[7].adc() ] + 14.45) / global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[15][nCH]->Fill ( global.ici2q[(int)val] , (adc2fC_QIE10_refl[ digis[7].adc() ] + 14.45) / global.qsum );
      }
    }
    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TH2F_perCHs[24][nCH]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perCHs[25][nCH]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perCHs[32][nCH]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[49][nCH]->Fill ( val , adc2fC_QIE10_refl[ digis[5].adc() ] / global.qsum );
      }
    }
    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TProfiles[0][nCH]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TProfiles[1][nCH]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TProfiles[2][nCH]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }

  }

// --- Suite 1: "CapIDtesting" ---

  if (suite_code == 1) {


  }

// --- Suite 2: "PedestalData" ---

  if (suite_code == 2) {

    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[1]->Fill ( global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perEVs[2]->Fill ( global.qsum / float(digis.samples()) );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[3]->Fill (t_abs_ev);
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[1][nCH]->Fill ( global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH1F_perCHs[2][nCH]->Fill ( global.qsum / float(digis.samples()) );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[3][nCH]->Fill (t_abs_ch);
        }
      }
    }

  }

// --- Suite 3: "AbsoluteTiming" ---

  if (suite_code == 3) {

    if (global.ts_TDC1 > -1) {
      float t_abs_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill (t_abs_ev);
        }
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill (t_abs_ch);
        }
      }
    }

  }

// --- Suite 4: "PedScanner" ---

  if (suite_code == 4) {

    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[2]->Fill ( global.pdac2poff[(int)val] , global.qsum/(float)digis.samples() );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[2][nCH]->Fill ( global.pdac2poff[(int)val] , global.qsum/(float)digis.samples() );
      }
    }

  }

// --- Suite 5: "PhaseScanner" ---

  if (suite_code == 5) {

    if (global.ts_TDC1 > -1) {
      float t_abs_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill (t_abs_ev);
        }
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ph_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perEVs[0]->Fill (val/2.0,t_abs_ph_ev);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perEVs[0]->Fill ((val-14.0)/2.0,t_abs_ph_ev);
          }
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        if ( val < 50 ) {
          TH2F_perEVs[1]->Fill (val/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
        if (( val > 63 ) && ( val < 114 )) {
          TH2F_perEVs[1]->Fill ((val-14.0)/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      float t_abs_ph_cid_ev = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perEVs[2]->Fill (val/2.0,t_abs_ph_cid_ev);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perEVs[2]->Fill ((val-14.0)/2.0,t_abs_ph_cid_ev);
          }
        }
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill (t_abs_ch);
        }
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ph_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perCHs[0][nCH]->Fill (val/2.0,t_abs_ph_ch);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perCHs[0][nCH]->Fill ((val-14.0)/2.0,t_abs_ph_ch);
          }
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        if ( val < 50 ) {
          TH2F_perCHs[1][nCH]->Fill (val/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
        if (( val > 63 ) && ( val < 114 )) {
          TH2F_perCHs[1][nCH]->Fill ((val-14.0)/2.0,digis[global.ts_TDC1].le_tdc()*0.5);
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      float t_abs_ph_cid_ch = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          if ( val < 50 ) {
            TH2F_perCHs[2][nCH]->Fill (val/2.0,t_abs_ph_cid_ch);
          }
          if (( val > 63 ) && ( val < 114 )) {
            TH2F_perCHs[2][nCH]->Fill ((val-14.0)/2.0,t_abs_ph_cid_ch);
          }
        }
      }
    }
    if (global.cid_TDC1 > -1) {
      if (global.cid_TDC1 == 0) {
        if (global.corrupt == 0) {
          if (_event_num > 2) {
            DetId detid_loc_WS = digis.detid();
            HcalDetId hcaldetid_loc_WS = HcalDetId(detid_loc_WS);
            int iEta_loc_WS = hcaldetid_loc_WS.ieta();
            int iPhi_loc_WS = hcaldetid_loc_WS.iphi();
            int Depth_loc_WS = hcaldetid_loc_WS.depth();
            cout << "##############" << endl;
            cout << "# WEIRD EVENT" << endl;
            cout << "# HF: " << Depth_loc_WS << endl;
            cout << "# Slot: " << iPhi_loc_WS << endl;
            cout << "# QIE: " << iEta_loc_WS << endl;
            cout << "# nEV:" << _event_num << endl;
  	  cout << "# phase: " << val << endl;
            cout << "#############" << endl;
          }
        }
      }
    }

  }

// --- Suite 6: "ICIScanner" ---

  if (suite_code == 6) {

    if (global.ts_TDC1 > -1) {
      float t_abs_ev = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perEVs[0]->Fill (t_abs_ev);
        }
      }
    }
    if (val < 8 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH2F_perEVs[0]->Fill ( global.ici2q[(int)val] , global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( val , global.qmax / global.qsum );
      }
    }
    if (global.ts_TDC1 > -1) {
      float t_abs_ch = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].le_tdc()*0.5);
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[0][nCH]->Fill (t_abs_ch);
        }
      }
    }
    if ( val == 7 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH1F_perCHs[1][nCH]->Fill ( global.qmax / global.qsum );
        }
      }
    }
    if (val < 8 ) {
      if (global.corrupt == 0) {
        if (_event_num > 2) {
          TH2F_perCHs[1][nCH]->Fill ( global.ici2q[(int)val] , global.qsum );
        }
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[2][nCH]->Fill ( val , global.qmax / global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[3][nCH]->Fill ( val , (adc2fC_QIE10_refl[ digis[7].adc() ] + 14.45) / global.qsum );
      }
    }
    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[4][nCH]->Fill ( global.ici2q[(int)val] , (adc2fC_QIE10_refl[ digis[7].adc() ] + 14.45) / global.qsum );
      }
    }

  }

// --- Suite 7: "LED_Vb_Scan" ---

  if (suite_code == 7) {

    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TH2F_perEVs[1]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perEVs[2]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }
    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TH2F_perCHs[1][nCH]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perCHs[2][nCH]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }
    if ( global.corrupt == 0 ) {
      if (_event_num > 2) {
        TProfiles[0][nCH]->Fill ( val , global.qsum );
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TProfiles[1][nCH]->Fill ( val , global.qmax3 / global.qsum );
        }
      }
    }

  }

// --- Suite 8: "RinSel_Scan" ---

  if (suite_code == 8) {

    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perEVs[0]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TH2F_perCHs[0][nCH]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }
    if (global.qmax3 > -1) {
      if ( global.corrupt == 0 ) {
        if (_event_num > 2) {
          TProfiles[0][nCH]->Fill ( val , (global.qmax3) / global.qsum );
        }
      }
    }

  }

// --- Suite 9: "BXscanner" ---

  if (suite_code == 9) {

    if (global.corrupt == 0) {
      if (_event_num > 2) {
        TH2F_perCHs[0][nCH]->Fill ( val , adc2fC_QIE10_refl[ digis[5].adc() ] / global.qsum );
      }
    }

  }

  return global;

}

// ####################
// # POST_EVENT_LOOPS #
// ####################

void post_event_loop(std::string parameter, float val, int suite_code, loop_vars global, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {


  }

// --- Suite 1: "CapIDtesting" ---

  if (suite_code == 1) {


  }

// --- Suite 2: "PedestalData" ---

  if (suite_code == 2) {


  }

// --- Suite 3: "AbsoluteTiming" ---

  if (suite_code == 3) {


  }

// --- Suite 4: "PedScanner" ---

  if (suite_code == 4) {


  }

// --- Suite 5: "PhaseScanner" ---

  if (suite_code == 5) {


  }

// --- Suite 6: "ICIScanner" ---

  if (suite_code == 6) {


  }

// --- Suite 7: "LED_Vb_Scan" ---

  if (suite_code == 7) {


  }

// --- Suite 8: "RinSel_Scan" ---

  if (suite_code == 8) {


  }

// --- Suite 9: "BXscanner" ---

  if (suite_code == 9) {


  }

}

