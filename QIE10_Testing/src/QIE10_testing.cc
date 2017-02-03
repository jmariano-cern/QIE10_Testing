// -*- C++ -*-
//
// Package:    HFanalyzer
// Class:      HFanalyzer
// 
//class HFanalyzer HFanalyzer.cc UserCode/HFanalyzer/src/HFanalyzer.cc
/*
   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"

#include "DataFormats/HcalDetId/interface/HcalOtherDetId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDigi/interface/QIE10DataFrame.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"

#include "RecoTBCalo/HcalTBObjectUnpacker/interface/HcalTBSlowDataUnpacker.h"

#include "TBDataFormats/HcalTBObjects/interface/HcalTBRunData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"

//#include "HFcommissioning/Analysis/interface/ADC_Conversion.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TProfile.h"
#include "TFile.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>

#include "adc2q.h"
#include "QIE10_init.h"
#include "QIE10_loop.h"
#include "unpack_name.h"

using namespace std;


class QIE10_testing : public edm::EDAnalyzer {

public:
  explicit QIE10_testing(const edm::ParameterSet&);
  ~QIE10_testing();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  void getData(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  TFile *_file;

  int _event_num;

  string _outFileName;
  int _verbosity;
  int _suite_code;
  int _sequencer_flag;
  
  char histoName[100];
  char dirName[100];

  TQIE10Info _qie10Info;

  // THESE CAN BE AUTO GENERATED

  ofstream *loggers;
  int _num_loggers;
  vector<string> logger_name;
  vector<string> logger_log_file;

  TTree **_trees;
  int _num_TTrees;
  vector<string> TTree_name;

  vector<TH1F*> TH1F_perEVs;
  int _num_TH1F_perEVs;
  vector<string> TH1F_perEV_name;
  vector<int> TH1F_perEV_nbinsx;
  vector<float> TH1F_perEV_lowx;
  vector<float> TH1F_perEV_highx;
  vector<string> TH1F_perEV_titlex;

  vector<vector<TH1F*> > TH1F_perCHs;
  int _num_TH1F_perCHs;
  vector<string> TH1F_perCH_name;
  vector<int> TH1F_perCH_nbinsx;
  vector<float> TH1F_perCH_lowx;
  vector<float> TH1F_perCH_highx;
  vector<string> TH1F_perCH_titlex;

  vector<vector<vector<TH1F*> > > TH1F_PerTSs;
  int _num_TH1F_PerTSs;
  vector<string> TH1F_PerTS_name;
  vector<int> TH1F_PerTS_nbinsx;
  vector<float> TH1F_PerTS_lowx;
  vector<float> TH1F_PerTS_highx;
  vector<string> TH1F_PerTS_titlex;

  vector<TH2F*> TH2F_perEVs;
  int _num_TH2F_perEVs;
  vector<string> TH2F_perEV_name;
  vector<int> TH2F_perEV_nbinsx;
  vector<float> TH2F_perEV_lowx;
  vector<float> TH2F_perEV_highx;
  vector<string> TH2F_perEV_titlex;
  vector<int> TH2F_perEV_nbinsy;
  vector<float> TH2F_perEV_lowy;
  vector<float> TH2F_perEV_highy;
  vector<string> TH2F_perEV_titley;

  vector<vector<TH2F*> > TH2F_perCHs;
  int _num_TH2F_perCHs;
  vector<string> TH2F_perCH_name;
  vector<int> TH2F_perCH_nbinsx;
  vector<float> TH2F_perCH_lowx;
  vector<float> TH2F_perCH_highx;
  vector<string> TH2F_perCH_titlex;
  vector<int> TH2F_perCH_nbinsy;
  vector<float> TH2F_perCH_lowy;
  vector<float> TH2F_perCH_highy;
  vector<string> TH2F_perCH_titley;

  vector<vector<vector<TH2F*> > > TH2F_PerTSs;
  int _num_TH2F_PerTSs;
  vector<string> TH2F_PerTS_name;
  vector<int> TH2F_PerTS_nbinsx;
  vector<float> TH2F_PerTS_lowx;
  vector<float> TH2F_PerTS_highx;
  vector<string> TH2F_PerTS_titlex;
  vector<int> TH2F_PerTS_nbinsy;
  vector<float> TH2F_PerTS_lowy;
  vector<float> TH2F_PerTS_highy;
  vector<string> TH2F_PerTS_titley;

  vector<vector<TH2F*> > TProfiles;
  vector<vector<TProfile*> > TProfiles_real;
  int _num_TProfiles;
  vector<string> TProfile_name;
  vector<int> TProfile_nbinsx;
  vector<float> TProfile_lowx;
  vector<float> TProfile_highx;
  vector<string> TProfile_titlex;
  vector<int> TProfile_nbinsy;
  vector<float> TProfile_lowy;
  vector<float> TProfile_highy;
  vector<string> TProfile_titley;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  edm::EDGetTokenT<HcalDataFrameContainer<QIE10DataFrame> > tok_QIE10DigiCollection_;
  edm::EDGetTokenT<HFDigiCollection> hf_token;
  edm::EDGetTokenT<FEDRawDataCollection> raw_token;  
  edm::Handle<QIE10DigiCollection> qie10DigiCollection;
  edm::Handle<FEDRawDataCollection> raw_collection;  

  hcaltb::HcalTBSlowDataUnpacker sdp;
  //  std::map<std::string,std::string> strings;
  //  std::map<std::string,double> numerics;

};


QIE10_testing::QIE10_testing(const edm::ParameterSet& iConfig) :
  _outFileName(iConfig.getUntrackedParameter<string>("OutFileName")),
  _verbosity(iConfig.getUntrackedParameter<int>("Verbosity")),
  _suite_code(iConfig.getUntrackedParameter<int>("Suite_Code")),
  _sequencer_flag(iConfig.getUntrackedParameter<int>("Sequencer_Flag"))

{

  tok_QIE10DigiCollection_ = consumes<HcalDataFrameContainer<QIE10DataFrame> >(edm::InputTag("hcalDigis"));
  hf_token = consumes<HFDigiCollection>(edm::InputTag("hcalDigis"));
  raw_token = consumes<FEDRawDataCollection>(edm::InputTag("source"));

  _file = new TFile(_outFileName.c_str(), "recreate");

  _event_num = 0;

}

QIE10_testing::~QIE10_testing()
{

  // COLLAPSE INTO 'WRITE' FUNCTION

  _file->cd(); // WHAT DOES THIS LINE DO?

  for (int i = 0 ; i < _num_TH1F_perEVs ; i++) {
    _file->mkdir(TH1F_perEV_name[i].c_str());
    _file->cd(TH1F_perEV_name[i].c_str());
    TH1F_perEVs[i]->Write();
  }

  for (int i = 0 ; i < _num_TH1F_perCHs ; i++) {
    _file->mkdir(TH1F_perCH_name[i].c_str());
    _file->cd(TH1F_perCH_name[i].c_str());
    for( unsigned int j = 0 ; j < TH1F_perCHs[i].size() ; j++ ){
      TH1F_perCHs[i][j]->Write();
    }
  }

  for (int i = 0 ; i < _num_TH1F_PerTSs ; i++) {
    for (unsigned int j = 0 ; j < TH1F_PerTSs[i].size() ; j++) {
      sprintf(dirName,"%s_perTS/%s_TS%i",TH1F_PerTS_name[i].c_str(),TH1F_PerTS_name[i].c_str(),j);
      _file->mkdir(dirName);
      _file->cd(dirName);
      for( unsigned int k = 0 ; k < TH1F_PerTSs[i][j].size() ; k++ ){
	TH1F_PerTSs[i][j][k]->Write();
      }
    }
  }

  for (int i = 0 ; i < _num_TH2F_perEVs ; i++) {
    _file->mkdir(TH2F_perEV_name[i].c_str());
    _file->cd(TH2F_perEV_name[i].c_str());
    TH2F_perEVs[i]->Write();
  }

  for (int i = 0 ; i < _num_TH2F_perCHs ; i++) {
    _file->mkdir(TH2F_perCH_name[i].c_str());
    _file->cd(TH2F_perCH_name[i].c_str());
    for( unsigned int j = 0 ; j < TH2F_perCHs[i].size() ; j++ ){    
      TH2F_perCHs[i][j]->Write();
    }
  }

  for (int i = 0 ; i < _num_TH2F_PerTSs ; i++) {
    for (unsigned int j = 0 ; j < TH2F_PerTSs[i].size() ; j++) {
      sprintf(dirName,"%s_PerTS/%s_TS%i",TH2F_PerTS_name[i].c_str(),TH2F_PerTS_name[i].c_str(),j);
      _file->mkdir(dirName);
      _file->cd(dirName);
      for( unsigned int k = 0 ; k < TH2F_PerTSs[i][j].size() ; k++ ){
	TH2F_PerTSs[i][j][k]->Write();
      }
    }
  }

  for (int i = 0 ; i < _num_TProfiles ; i++) {
    _file->mkdir(TProfile_name[i].c_str());
    _file->cd(TProfile_name[i].c_str());
    vector<TProfile*> temp_TProfile_real_vector;
    TProfiles_real.push_back(temp_TProfile_real_vector);
    for( unsigned int j = 0 ; j < TProfiles[i].size() ; j++ ){
      TProfiles_real[i].push_back(TProfiles[i][j]->ProfileX(TProfiles[i][j]->GetTitle(),1,-1,"s"));      
      TProfiles_real[i][j]->GetXaxis()->SetTitle(TProfile_titlex.at(i).c_str());
      TProfiles_real[i][j]->GetYaxis()->SetTitle(TProfile_titley.at(i).c_str());      
      TProfiles_real[i][j]->Write();
    }
  }

  //  _file->cd(); // WHAT DOES THIS LINE DO?

  //  for (int i = 0 ; i < _num_TTrees ; i++) {
  //    _trees[i]->Write();
  //  }

  _file->Write();
  _file->Close();

}
	
void QIE10_testing::getData(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  using namespace edm;

  //  Extracting All the Collections containing useful Info
  iEvent.getByToken(tok_QIE10DigiCollection_,qie10DigiCollection);

  iEvent.getByToken(raw_token,raw_collection);

  name_data slow_data;
  if (_sequencer_flag > 0) { 
    slow_data = unpack_name(raw_collection,sdp,_event_num,_sequencer_flag);
  }
  else {
    slow_data.parameter.push_back("NULL");
    slow_data.val.push_back(-999.0);    
  }

  const QIE10DigiCollection& qie10dc=*(qie10DigiCollection);

  if (_verbosity>0){
      cout << "### Before Loop: " << endl;
      cout << "### QIE10 Digis=" << qie10dc.size() << endl;
  }

  QIE10DataFrame qie10df_0 = static_cast<QIE10DataFrame>(qie10dc[0]);
  int tTS_0 = qie10df_0.samples();
  unsigned int nCH = qie10dc.size();

  if (_event_num == 0){
    init(_suite_code,tTS_0,nCH,
	 loggers,_num_loggers, logger_name, logger_log_file, 
	 _file,_trees,_qie10Info,TTree_name,_num_TTrees,
	 TH1F_perEVs,_num_TH1F_perEVs,TH1F_perEV_name,TH1F_perEV_nbinsx,TH1F_perEV_lowx,TH1F_perEV_highx,TH1F_perEV_titlex,
	 TH1F_perCHs,_num_TH1F_perCHs,TH1F_perCH_name,TH1F_perCH_nbinsx,TH1F_perCH_lowx,TH1F_perCH_highx,TH1F_perCH_titlex,
	 TH1F_PerTSs,_num_TH1F_PerTSs,TH1F_PerTS_name,TH1F_PerTS_nbinsx,TH1F_PerTS_lowx,TH1F_PerTS_highx,TH1F_PerTS_titlex,
	 TH2F_perEVs,_num_TH2F_perEVs,TH2F_perEV_name,TH2F_perEV_nbinsx,TH2F_perEV_lowx,TH2F_perEV_highx,TH2F_perEV_titlex,TH2F_perEV_nbinsy,TH2F_perEV_lowy,TH2F_perEV_highy,TH2F_perEV_titley,
	 TH2F_perCHs,_num_TH2F_perCHs,TH2F_perCH_name,TH2F_perCH_nbinsx,TH2F_perCH_lowx,TH2F_perCH_highx,TH2F_perCH_titlex,TH2F_perCH_nbinsy,TH2F_perCH_lowy,TH2F_perCH_highy,TH2F_perCH_titley,
	 TH2F_PerTSs,_num_TH2F_PerTSs,TH2F_PerTS_name,TH2F_PerTS_nbinsx,TH2F_PerTS_lowx,TH2F_PerTS_highx,TH2F_PerTS_titlex,TH2F_PerTS_nbinsy,TH2F_PerTS_lowy,TH2F_PerTS_highy,TH2F_PerTS_titley,
	 TProfiles,_num_TProfiles,TProfile_name,TProfile_nbinsx,TProfile_lowx,TProfile_highx,TProfile_titlex,TProfile_nbinsy,TProfile_lowy,TProfile_highy,TProfile_titley);

    for (int i = 0 ; i < _num_loggers ; i++) {
      loggers[i].open(logger_log_file[i],std::ios_base::app);
    }

    for (int i = 0 ; i < _num_TH1F_perEVs ; i++) {
      sprintf(histoName,"%s",TH1F_perEV_name.at(i).c_str());
      TH1F_perEVs.push_back(new TH1F(histoName,histoName,TH1F_perEV_nbinsx[i],TH1F_perEV_lowx[i],TH1F_perEV_highx[i]));
      TH1F_perEVs.back()->GetXaxis()->SetTitle(TH1F_perEV_titlex.at(i).c_str());
    }
    

    for (int i = 0 ; i < _num_TH2F_perEVs ; i++) {
      sprintf(histoName,"%s",TH2F_perEV_name.at(i).c_str());
      TH2F_perEVs.push_back(new TH2F(histoName,histoName,TH2F_perEV_nbinsx[i],TH2F_perEV_lowx[i],TH2F_perEV_highx[i],TH2F_perEV_nbinsy[i],TH2F_perEV_lowy[i],TH2F_perEV_highy[i]));
      TH2F_perEVs.back()->GetXaxis()->SetTitle(TH2F_perEV_titlex.at(i).c_str());
      TH2F_perEVs.back()->GetYaxis()->SetTitle(TH2F_perEV_titley.at(i).c_str());
    }

  }

  // ********* PRE EVENT LOOP ************

  loop_vars global;
  
  pre_event_loop(slow_data.parameter,slow_data.val,_suite_code,_event_num,_qie10Info,_trees,TH1F_perEVs,TH1F_perCHs,TH1F_PerTSs,TH2F_perEVs,TH2F_perCHs,TH2F_PerTSs,TProfiles,loggers);

  for (unsigned int j=0; j < nCH ; j++){

    QIE10DataFrame qie10df = static_cast<QIE10DataFrame>(qie10dc[j]);

    // Extract info on detector location
    DetId detid = qie10df.detid();
    HcalDetId hcaldetid = HcalDetId(detid);
    int iphi = hcaldetid.iphi();
    int depth = hcaldetid.depth();
    int ieta = hcaldetid.ieta();
    int nTS = qie10df.samples();

    // WHY  AM I DOING THIS HERE (AND NOT IN INIT)?
    if(_event_num == 0) { // SAME AS LOOP OVER nCH

      for (int i = 0 ; i < _num_TH1F_perCHs ; i++) {
	sprintf(histoName,"%s_HF%i_Slot%i_QIE%i",TH1F_perCH_name.at(i).c_str(),depth,iphi,ieta);
	TH1F_perCHs[i].push_back(new TH1F(histoName,histoName,TH1F_perCH_nbinsx[i],TH1F_perCH_lowx[i],TH1F_perCH_highx[i]));
	TH1F_perCHs[i].back()->GetXaxis()->SetTitle(TH1F_perCH_titlex.at(i).c_str());
      }

      for (int i = 0 ; i < _num_TH1F_PerTSs ; i++) {
	for(int k = 0; k < nTS ; k++) {
	  if (j == 0) { 
	    vector<TH1F*> temp_TH1F_vector;  
	    TH1F_PerTSs[i].push_back(temp_TH1F_vector);
	  }  
	  sprintf(histoName,"%s_TS%i_HF%i_Slot%i_QIE%i",TH1F_PerTS_name.at(i).c_str(),k,depth,iphi,ieta);
	  TH1F_PerTSs[i][k].push_back(new TH1F(histoName,histoName,TH1F_PerTS_nbinsx[i],TH1F_PerTS_lowx[i],TH1F_PerTS_highx[i]));
	  TH1F_PerTSs[i][k].back()->GetXaxis()->SetTitle(TH1F_PerTS_titlex.at(i).c_str());
	}
      }

      for (int i = 0 ; i < _num_TH2F_perCHs ; i++) {
	sprintf(histoName,"%s_HF%i_Slot%i_QIE%i",TH2F_perCH_name.at(i).c_str(),depth,iphi,ieta);
	TH2F_perCHs[i].push_back(new TH2F(histoName,histoName,TH2F_perCH_nbinsx[i],TH2F_perCH_lowx[i],TH2F_perCH_highx[i],TH2F_perCH_nbinsy[i],TH2F_perCH_lowy[i],TH2F_perCH_highy[i]));      
	TH2F_perCHs[i].back()->GetXaxis()->SetTitle(TH2F_perCH_titlex.at(i).c_str());
	TH2F_perCHs[i].back()->GetYaxis()->SetTitle(TH2F_perCH_titley.at(i).c_str());
      }

      for (int i = 0 ; i < _num_TH2F_PerTSs ; i++) {
	for(int k = 0; k < nTS ; k++) {
	  if (j == 0) {
	    vector<TH2F*> temp_TH2F_vector;
	    TH2F_PerTSs[i].push_back(temp_TH2F_vector);
	  }
	  sprintf(histoName,"%s_TS%i_HF%i_Slot%i_QIE%i",TH2F_PerTS_name.at(i).c_str(),k,depth,iphi,ieta);
	  TH2F_PerTSs[i][k].push_back(new TH2F(histoName,histoName,TH2F_PerTS_nbinsx[i],TH2F_PerTS_lowx[i],TH2F_PerTS_highx[i],TH2F_PerTS_nbinsy[i],TH2F_PerTS_lowy[i],TH2F_PerTS_highy[i]));      
	  TH2F_PerTSs[i][k].back()->GetXaxis()->SetTitle(TH2F_PerTS_titlex.at(i).c_str());
	  TH2F_PerTSs[i][k].back()->GetYaxis()->SetTitle(TH2F_PerTS_titley.at(i).c_str());
	}      
      }

      for (int i = 0 ; i < _num_TProfiles ; i++) {
	sprintf(histoName,"%s_HF%i_Slot%i_QIE%i",TProfile_name.at(i).c_str(),depth,iphi,ieta);
	TProfiles[i].push_back(new TH2F(histoName,histoName,TProfile_nbinsx[i],TProfile_lowx[i],TProfile_highx[i],TProfile_nbinsy[i],TProfile_lowy[i],TProfile_highy[i]));      
	TProfiles[i].back()->GetXaxis()->SetTitle(TProfile_titlex.at(i).c_str());
	TProfiles[i].back()->GetYaxis()->SetTitle(TProfile_titley.at(i).c_str());
      }

    }

    if (_verbosity>0){
      std::cout << "Printing raw dataframe" << std::endl;
      std::cout << qie10df << std::endl;
            
      std::cout << "Printing content of samples() method" << std::endl;
      std::cout << qie10df.samples() << std::endl;
    }
        
    if (_verbosity>0){
      std::cout << " detid: " << detid.rawId() << std::endl;
      std::cout << " ieta: " << ieta << "\n"
		<< " iphi: " << iphi << "\n"
		<< " depth: " << depth << std::endl;
    }
        
    //******** PRELOOP ***********

    global = pre_loop(slow_data.parameter,slow_data.val,_suite_code,global,qie10df,j,_event_num,_qie10Info,_trees,TH1F_perEVs,TH1F_perCHs,TH1F_PerTSs,TH2F_perEVs,TH2F_perCHs,TH2F_PerTSs,TProfiles,loggers);

    //********** LOOP ************
	
    for(int i=0; i<nTS; ++i)
      {

	global = loop(slow_data.parameter,slow_data.val,_suite_code,global,qie10df,i,j,_event_num,_qie10Info,_trees,TH1F_perEVs,TH1F_perCHs,TH1F_PerTSs,TH2F_perEVs,TH2F_perCHs,TH2F_PerTSs,TProfiles,loggers);

      }

    //******** POSTLOOP ********

    post_loop(slow_data.parameter,slow_data.val,_suite_code,global,qie10df,j,_event_num,_qie10Info,_trees,TH1F_perEVs,TH1F_perCHs,TH1F_PerTSs,TH2F_perEVs,TH2F_perCHs,TH2F_PerTSs,TProfiles,loggers);

    if (_verbosity>0)
      std::cout << "The pedestal for this channel is " << global.adcped << "ADC counts and " << global.qped << " fC" << std::endl;
    
  }

  //maybe one channel not filled?
  //  for (int k = 0; k < 10 ; k++){
  //    cout << _qie10Info.pulse_adc[0][k] << " ";
  //  }
  //  cout << endl;
  //_trees[0]->Fill();

  // ********* POST_EVENT_LOOP *************

  post_event_loop(slow_data.parameter,slow_data.val,_suite_code,global,_event_num,_qie10Info,_trees,TH1F_perEVs,TH1F_perCHs,TH1F_PerTSs,TH2F_perEVs,TH2F_perCHs,TH2F_PerTSs,TProfiles,loggers);

  _event_num++;

  return;
}


// ------------ method called for each event  ------------
void 
QIE10_testing::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  getData(iEvent, iSetup);

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
QIE10_testing::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
QIE10_testing::endJob() 
{
  //      _file->Write();
  //      _file->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
QIE10_testing::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
QIE10_testing::endRun(edm::Run const&, edm::EventSetup const&)
{
  //      _file->Write();
  //      _file->Close();
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
QIE10_testing::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
QIE10_testing::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
QIE10_testing::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(QIE10_testing);
