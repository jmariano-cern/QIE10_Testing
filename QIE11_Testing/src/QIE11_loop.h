// #############################################################
// #############################################################
// ###                                                       ###
// ###                     ./QIE11_loop.h                    ###
// ###                                                       ###
// ###           This file was written by a robot!           ###
// ###                  Do not edit by hand!                 ###
// ###                                                       ###
// ###        Instead, modify the configuration file:        ###
// ###                    QIE11_testing.cfg                  ###
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

  int adcped = 4;
  float qped = adc2fC_QIE11_refl[ adcped ];
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
  int ts_ref = -1;
  int ref_ns = 117;
  float t_abs = -1.0;
  float t_cid = -1.0;

};

// ###################
// # PRE_EVENT_LOOPS #
// ###################

loop_vars pre_event_loop(vector<std::string> parameter, vector<float> val, int suite_code, int _event_num, TQIE11Info &_qie11Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

  loop_vars global;

// --- Suite 0: "full" ---

  if (suite_code == 0) {


  }

// --- Suite 1: "PedestalData" ---

  if (suite_code == 1) {


  }

  return global;

}

// #############
// # PRE_LOOPS #
// #############

loop_vars pre_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE11DataFrame digis, int nCH, int _event_num, TQIE11Info &_qie11Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

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
      global.qsum += adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.t_abs = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].tdc()*0.5);
    global.t_cid = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].tdc()*0.5);
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if (global.ts_max > 0) {
      global.qmax2m = adc2fC_QIE11_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + (2*14.45);
    }
    if (global.ts_max < 9) {
      global.qmax2p = adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
    }
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax3 = adc2fC_QIE11_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }
    int ref_offset = 999;
    if (global.ts_TDC1 > -1) {
      ref_offset = ((int)(((float)digis[global.ts_TDC1].tdc()*0.5) + (float)global.ref_ns) - (int)(((float)digis[global.ts_TDC1].tdc()*0.5) + (float)global.ref_ns)%25)/25;
    }
    global.ts_ref = -1;
    if ((ref_offset + global.ts_TDC1) < 9) {
      global.ts_ref = ref_offset + global.ts_TDC1;
    }

  }

// --- Suite 1: "PedestalData" ---

  if (suite_code == 1) {

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
      global.qsum += adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45;
      if ((adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45) > global.qmax) {
        global.qmax = adc2fC_QIE11_refl[ digis[iTS].adc() ] + 14.45;
        global.ts_max = iTS;
      }
      if ((TDC1_flag == 0) && (digis[iTS].tdc() < 50)) {
        TDC1_flag = 1;
        global.ts_TDC1 = iTS;
        global.cid_TDC1 = digis[iTS].capid();
      }
      if ((TDC1_flag == 1) && (TDC2_flag == 0) && (digis[iTS].tdc() < 50)) {
        TDC2_flag = 1;
        global.ts_TDC2 = iTS;
        global.cid_TDC2 = digis[iTS].capid();
      }
    }
    global.t_abs = ((global.ts_TDC1)*25.0) + (digis[global.ts_TDC1].tdc()*0.5);
    global.t_cid = (global.cid_TDC1*25.0) + (digis[global.ts_TDC1].tdc()*0.5);
    global.qmax2p = -1;
    global.qmax2m = -1;
    global.qmax3 = -1;
    if (global.ts_max > 0) {
      global.qmax2m = adc2fC_QIE11_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + (2*14.45);
    }
    if (global.ts_max < 9) {
      global.qmax2p = adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max+1].adc() ] + (2*14.45);
    }
    if ((global.ts_max > 0) && (global.ts_max < 9)) {
      global.qmax3 = adc2fC_QIE11_refl[ digis[global.ts_max-1].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max].adc() ] + adc2fC_QIE11_refl[ digis[global.ts_max+1].adc() ] + (3*14.45);
    }
    int ref_offset = 999;
    if (global.ts_TDC1 > -1) {
      ref_offset = ((int)(((float)digis[global.ts_TDC1].tdc()*0.5) + (float)global.ref_ns) - (int)(((float)digis[global.ts_TDC1].tdc()*0.5) + (float)global.ref_ns)%25)/25;
    }
    global.ts_ref = -1;
    if ((ref_offset + global.ts_TDC1) < 9) {
      global.ts_ref = ref_offset + global.ts_TDC1;
    }

  }

  return global;

}

// #########
// # LOOPS #
// #########

loop_vars loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE11DataFrame digis, int nTS, int nCH, int _event_num, TQIE11Info &_qie11Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].tdc();
    le_tdc = le_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE11_refl[ adc ] + 14.45;
    charge = charge;
    TH1F_perEVs[0]->Fill ( adc );
    TH2F_perEVs[0]->Fill ( nTS, adc );
    TH2F_perEVs[1]->Fill ( nTS, charge );
    TH2F_perEVs[2]->Fill ( capid , adc );
    TH1F_perCHs[0][nCH]->Fill ( adc );
    TH2F_perCHs[0][nCH]->Fill ( nTS, adc );
    TH2F_perCHs[1][nCH]->Fill ( nTS, charge );
    TH2F_perCHs[2][nCH]->Fill ( capid , adc );

  }

// --- Suite 1: "PedestalData" ---

  if (suite_code == 1) {

    int tTS = digis.samples();
    tTS = tTS;
    int adc = digis[nTS].adc();
    adc = adc;
    int le_tdc = digis[nTS].tdc();
    le_tdc = le_tdc;
    int capid = digis[nTS].capid();
    capid = capid;
    int soi = digis[nTS].soi();
    soi = soi;
    float charge = adc2fC_QIE11_refl[ adc ] + 14.45;
    charge = charge;
    TH1F_perEVs[0]->Fill ( adc );
    TH2F_perEVs[0]->Fill ( nTS, adc );
    TH2F_perEVs[1]->Fill ( nTS, charge );
    TH2F_perEVs[2]->Fill ( capid , adc );
    TH1F_perCHs[0][nCH]->Fill ( adc );
    TH2F_perCHs[0][nCH]->Fill ( nTS, adc );
    TH2F_perCHs[1][nCH]->Fill ( nTS, charge );
    TH2F_perCHs[2][nCH]->Fill ( capid , adc );

  }

  return global;

}

// ##############
// # POST_LOOPS #
// ##############

loop_vars post_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE11DataFrame digis, int nCH, int _event_num, TQIE11Info &_qie11Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {

    TH1F_perEVs[1]->Fill ( global.qsum );
    if (global.t_abs > -1) {
      TH1F_perEVs[2]->Fill (global.t_abs);
    }
    TH1F_perCHs[1][nCH]->Fill ( global.qsum );
    if (global.t_abs > -1) {
      TH1F_perCHs[2][nCH]->Fill (global.t_abs);
    }

  }

// --- Suite 1: "PedestalData" ---

  if (suite_code == 1) {

    TH1F_perEVs[1]->Fill ( global.qsum );
    if (global.t_abs > -1) {
      TH1F_perEVs[2]->Fill (global.t_abs);
    }
    TH1F_perCHs[1][nCH]->Fill ( global.qsum );
    if (global.t_abs > -1) {
      TH1F_perCHs[2][nCH]->Fill (global.t_abs);
    }

  }

  return global;

}

// ####################
// # POST_EVENT_LOOPS #
// ####################

void post_event_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, int _event_num, TQIE11Info &_qie11Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {

// --- Suite 0: "full" ---

  if (suite_code == 0) {


  }

// --- Suite 1: "PedestalData" ---

  if (suite_code == 1) {


  }

}

