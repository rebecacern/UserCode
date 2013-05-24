// rebeca@cern.ch
// Trying to understand the signal
// November 2012

#include "../Smurf/Core/SmurfTree.h"
#include "../Smurf/Analysis/HWWlvlv/factors.h"
#include "../Smurf/Core/LeptonScaleLookup.h"
#include "Math/VectorUtil.h"
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <fstream>
#include "TLegend.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TSystem.h"

const int verboseLevel =   1;
const double mz = 91.1876;
const double mw = 80.4;
const double mmu = 0.105;
const double lumi = 19.467;

const double separation = 15; //15 is the chosen cut
const double metcut = -10; //10 is the proposed
const double mtcut = 85;
const double separationjj = 60; //60
const double phicut = 1.8; // 1.8

void Signal() {
  
  TString bgdInputFile    = "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root";
 // TString sigInputFile   =  "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/hww125.root";
  TString sigInputFile   =  "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/zhww125.root";
  TString dataInputFile   = "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/data_3l.root";
  
  //Load datasets
  SmurfTree background;
  background.LoadTree(bgdInputFile,-1);
  background.InitTree(0);
  
  SmurfTree signal;
  signal.LoadTree(sigInputFile,-1);
  signal.InitTree(0);
  
  SmurfTree data;
  data.LoadTree(dataInputFile,-1);
  data.InitTree(0);
  
  
  //Prepare output
  char output[200];
  sprintf(output,"rootfiles/signal_study.root");     
  TFile* outFileNjets = new TFile(output,"recreate");
  
  //Histograms 
  TH1F* types = new TH1F("types", "types", 80, -0.5 , 79.5);
  types->Sumw2();
  
  TH1D* sig_cuts = new TH1D("sig_cuts", "cuts", 10, 0, 10);
  TH1D* sig_cuts_zh = new TH1D("sig_cuts_zh", "cuts", 10, 0, 10);
  
  TH1D* sig_met = new TH1D("sig_met", "MET", 200, 0, 200);
  TH1D* sig_mllz = new TH1D("sig_mllz", "m_{ll}", 200, 0, 200);
  TH1D* sig_mt = new TH1D("sig_mt", "m_t", 200, 0, 200);
  TH1D* sig_ptjet = new TH1D("sig_ptjet", "P_t of leading jet", 200, 0, 200);
  TH1D* sig_mH = new TH1D("sig_mH", "m_H", 200, 0, 400);
  TH1D* sig_mH_v2 = new TH1D("sig_mH_v2", "m_H", 200, 0, 400);
  TH1D* sig_mH_3d = new TH1D("sig_mH_3d", "m_H", 200, 0, 400);
  TH1D* sig_mH_2d = new TH1D("sig_mH_2d", "m_H", 200, 0, 400);
  TH1D* sig_mjj = new TH1D("sig_mjj", "m_jj", 200, 0, 400);
  TH1D* sig_dphiljj = new TH1D("sig_dphiljj", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* sig_dphilmjj = new TH1D("sig_dphilmjj", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* sig_njets = new TH1D("sig_njets", "Number of jets", 30, -0.5, 29.5);
  TH1D* sig_dRll = new TH1D("sig_dRll", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* sig_tmet = new TH1D("sig_tmet", "tracker MET", 200, 0, 200);
  TH1D* sig_minmet = new TH1D("sig_minmet", "min(MET, tracker MET)", 200, 0, 200);
  TH1D* sig_minmll = new TH1D("sig_minmll", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* sig_id = new TH1D("sig_id", "lepton content", 4, 0, 4);
  TH1D* sig_mlll = new TH1D("sig_mlll", "m_{lll}", 200, 0, 400);

  sig_cuts->Sumw2();
  sig_cuts_zh->Sumw2();
  
  sig_met->Sumw2();
  sig_mllz->Sumw2();
  sig_mt->Sumw2();
  sig_ptjet->Sumw2();
  sig_mH->Sumw2();
  sig_mH_v2->Sumw2();
  sig_mH_3d->Sumw2();
  sig_mH_2d->Sumw2();
  sig_mjj->Sumw2();  
  sig_dphiljj->Sumw2();
  sig_dphilmjj->Sumw2();
  sig_njets->Sumw2();
  sig_dRll->Sumw2();
  sig_tmet->Sumw2();
  sig_minmet->Sumw2();
  sig_minmll->Sumw2();
  sig_id->Sumw2();
  sig_mlll->Sumw2();
  
  //Backgrounds
  TH1D* bck_cuts = new TH1D("bck_cuts", "cuts", 10, 0, 10);
  TH1D* bck_cuts_wz = new TH1D("bck_cuts_wz", "cuts", 10, 0, 10);
  TH1D* bck_cuts_www = new TH1D("bck_cuts_www", "cuts", 10, 0, 10);
  TH1D* bck_cuts_data = new TH1D("bck_cuts_data", "cuts", 10, 0, 10);
  TH1D* bck_cuts_zz = new TH1D("bck_cuts_zz", "cuts", 10, 0, 10);
  TH1D* bck_cuts_fakes = new TH1D("bck_cuts_fakes", "cuts", 10, 0, 10);
  TH1D* bck_cuts_other = new TH1D("bck_cuts_other", "cuts", 10, 0, 10);

  bck_cuts->Sumw2();
  bck_cuts_wz->Sumw2();
  bck_cuts_www->Sumw2();
  bck_cuts_data->Sumw2();
  bck_cuts_zz->Sumw2();
  bck_cuts_fakes->Sumw2();
  bck_cuts_other->Sumw2();
  
  TH1D* bck_met = new TH1D("bck_met", "MET", 200, 0, 200);
  TH1D* bck_mllz = new TH1D("bck_mllz", "m_{ll}", 200, 0, 200);
  TH1D* bck_mt = new TH1D("bck_mt", "m_t", 200, 0, 200);
  TH1D* bck_ptjet = new TH1D("bck_ptjet", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_mH = new TH1D("bck_mH", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2 = new TH1D("bck_mH_v2", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d = new TH1D("bck_mH_3d", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d = new TH1D("bck_mH_2d", "m_H", 200, 0, 400);
  TH1D* bck_mjj = new TH1D("bck_mjj", "m_jj", 200, 0, 400);
  TH1D* bck_dphiljj = new TH1D("bck_dphiljj", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj = new TH1D("bck_dphilmjj", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_njets = new TH1D("bck_njets", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_dRll = new TH1D("bck_dRll", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_tmet = new TH1D("bck_tmet", "tracker MET", 200, 0, 200);
  TH1D* bck_minmet = new TH1D("bck_minmet", "min(MET, tracker MET)", 200, 0, 200);
  TH1D* bck_minmll = new TH1D("bck_minmll", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_id = new TH1D("bck_id", "lepton content", 4, 0, 4);
  TH1D* bck_mlll = new TH1D("bck_mlll", "m_{lll}", 200, 0, 400);

  bck_met->Sumw2();
  bck_mllz->Sumw2();
  bck_mt->Sumw2();
  bck_ptjet->Sumw2();
  bck_mH->Sumw2();
  bck_mH_v2->Sumw2();
  bck_mH_3d->Sumw2();
  bck_mH_2d->Sumw2();
  bck_mjj->Sumw2();  
  bck_dphiljj->Sumw2();
  bck_dphilmjj->Sumw2();
  bck_njets->Sumw2();
  bck_dRll->Sumw2();
  bck_tmet->Sumw2();
  bck_minmet->Sumw2();
  bck_minmll->Sumw2();
  bck_id->Sumw2();
  bck_mlll->Sumw2();
  
  //Variables separated
  TH1D* bck_met_wz =  new TH1D("bck_met_wz", "MET", 200, 0, 200);
  TH1D* bck_met_www = new TH1D("bck_met_www", "MET", 200, 0, 200);
  TH1D* bck_met_data = new TH1D("bck_met_data", "MET", 200, 0, 200);
  TH1D* bck_met_zz =  new TH1D("bck_met_zz", "MET", 200, 0, 200);
  TH1D* bck_met_tt =  new TH1D("bck_met_tt", "MET", 200, 0, 200);
  TH1D* bck_met_fakes =  new TH1D("bck_met_fakes", "MET", 200, 0, 200);

  bck_met_wz->Sumw2(); 
  bck_met_www->Sumw2();
  bck_met_data->Sumw2();
  bck_met_zz->Sumw2();  
  bck_met_tt->Sumw2(); 
  bck_met_fakes->Sumw2(); 
  
  TH1D* bck_mllz_wz = new TH1D("bck_mllz_wz", "m_{ll}", 200, 0, 200);
  TH1D* bck_mllz_www = new TH1D("bck_mllz_www", "m_{ll}", 200, 0, 200);
  TH1D* bck_mllz_data = new TH1D("bck_mllz_data", "m_{ll}", 200, 0, 200);
  TH1D* bck_mllz_zz = new TH1D("bck_mllz_zz", "m_{ll}", 200, 0, 200);
  TH1D* bck_mllz_tt = new TH1D("bck_mllz_tt", "m_{ll}", 200, 0, 200);
  TH1D* bck_mllz_fakes = new TH1D("bck_mllz_fakes", "m_{ll}", 200, 0, 200);
  
  bck_mllz_wz->Sumw2(); 
  bck_mllz_www->Sumw2();
  bck_mllz_data->Sumw2();
  bck_mllz_zz->Sumw2();  
  bck_mllz_tt->Sumw2(); 
  bck_mllz_fakes->Sumw2(); 

  
  TH1D* bck_mt_wz = new TH1D("bck_mt_wz", "m_t", 200, 0, 200);
  TH1D* bck_mt_www = new TH1D("bck_mt_www", "m_t", 200, 0, 200);
  TH1D* bck_mt_data = new TH1D("bck_mt_data", "m_t", 200, 0, 200);
  TH1D* bck_mt_zz = new TH1D("bck_mt_zz", "m_t", 200, 0, 200);
  TH1D* bck_mt_tt = new TH1D("bck_mt_tt", "m_t", 200, 0, 200);
  TH1D* bck_mt_fakes = new TH1D("bck_mt_fakes", "m_t", 200, 0, 200);
  
  bck_mt_wz->Sumw2(); 
  bck_mt_www->Sumw2();
  bck_mt_data->Sumw2();
  bck_mt_zz->Sumw2();  
  bck_mt_tt->Sumw2(); 
  bck_mt_fakes->Sumw2(); 
  
  TH1D* bck_ptjet_wz = new TH1D("bck_ptjet_wz", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_ptjet_www = new TH1D("bck_ptjet_www", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_ptjet_data = new TH1D("bck_ptjet_data", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_ptjet_zz = new TH1D("bck_ptjet_zz", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_ptjet_tt = new TH1D("bck_ptjet_tt", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_ptjet_fakes = new TH1D("bck_ptjet_fakes", "P_t of leading jet", 200, 0, 200);
  
  bck_ptjet_wz->Sumw2(); 
  bck_ptjet_www->Sumw2();
  bck_ptjet_data->Sumw2();
  bck_ptjet_zz->Sumw2();  
  bck_ptjet_tt->Sumw2(); 
  bck_ptjet_fakes->Sumw2(); 
  
  TH1D* bck_mH_wz = new TH1D("bck_mH_wz", "m_H", 200, 0, 400);
  TH1D* bck_mH_www = new TH1D("bck_mH_www", "m_H", 200, 0, 400);
  TH1D* bck_mH_data = new TH1D("bck_mH_data", "m_H", 200, 0, 400);
  TH1D* bck_mH_zz = new TH1D("bck_mH_zz", "m_H", 200, 0, 400);
  TH1D* bck_mH_tt = new TH1D("bck_mH_tt", "m_H", 200, 0, 400);
  TH1D* bck_mH_fakes = new TH1D("bck_mH_fakes", "m_H", 200, 0, 400);
  
  bck_mH_wz->Sumw2();
  bck_mH_www->Sumw2();
  bck_mH_data->Sumw2(); 
  bck_mH_zz->Sumw2();
  bck_mH_tt->Sumw2();
  bck_mH_fakes->Sumw2();
  
  TH1D* bck_mH_v2_wz = new TH1D("bck_mH_v2_wz", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2_www = new TH1D("bck_mH_v2_www", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2_data = new TH1D("bck_mH_v2_data", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2_zz = new TH1D("bck_mH_v2_zz", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2_tt = new TH1D("bck_mH_v2_tt", "m_H", 200, 0, 400);
  TH1D* bck_mH_v2_fakes = new TH1D("bck_mH_v2_fakes", "m_H", 200, 0, 400);
  
  bck_mH_v2_wz->Sumw2();
  bck_mH_v2_www->Sumw2();
  bck_mH_v2_data->Sumw2(); 
  bck_mH_v2_zz->Sumw2();
  bck_mH_v2_tt->Sumw2();
  bck_mH_v2_fakes->Sumw2();
  
  TH1D* bck_mH_3d_wz = new TH1D("bck_mH_3d_wz", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d_www = new TH1D("bck_mH_3d_www", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d_data = new TH1D("bck_mH_3d_data", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d_zz = new TH1D("bck_mH_3d_zz", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d_tt = new TH1D("bck_mH_3d_tt", "m_H", 200, 0, 400);
  TH1D* bck_mH_3d_fakes = new TH1D("bck_mH_3d_fakes", "m_H", 200, 0, 400);
  
  bck_mH_3d_wz->Sumw2();
  bck_mH_3d_www->Sumw2();
  bck_mH_3d_data->Sumw2(); 
  bck_mH_3d_zz->Sumw2();
  bck_mH_3d_tt->Sumw2();
  bck_mH_3d_fakes->Sumw2();
  
  TH1D* bck_mH_2d_wz = new TH1D("bck_mH_2d_wz", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d_www = new TH1D("bck_mH_2d_www", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d_data = new TH1D("bck_mH_2d_data", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d_zz = new TH1D("bck_mH_2d_zz", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d_tt = new TH1D("bck_mH_2d_tt", "m_H", 200, 0, 400);
  TH1D* bck_mH_2d_fakes = new TH1D("bck_mH_2d_fakes", "m_H", 200, 0, 400);
  
  bck_mH_2d_wz->Sumw2();
  bck_mH_2d_www->Sumw2();
  bck_mH_2d_data->Sumw2(); 
  bck_mH_2d_zz->Sumw2();
  bck_mH_2d_tt->Sumw2();
  bck_mH_2d_fakes->Sumw2();
  
  TH1D* bck_mjj_wz = new TH1D("bck_mjj_wz", "m_{jj}", 200, 0, 400);
  TH1D* bck_mjj_www = new TH1D("bck_mjj_www", "m_{jj}", 200, 0, 400);
  TH1D* bck_mjj_data = new TH1D("bck_mjj_data", "m_{jj}", 200, 0, 400);
  TH1D* bck_mjj_zz = new TH1D("bck_mjj_zz", "m_{jj}", 200, 0, 400);
  TH1D* bck_mjj_tt = new TH1D("bck_mjj_tt", "m_{jj}", 200, 0, 400);
  TH1D* bck_mjj_fakes = new TH1D("bck_mjj_fakes", "m_{jj}", 200, 0, 400);
  
  bck_mjj_wz->Sumw2();
  bck_mjj_www->Sumw2();
  bck_mjj_data->Sumw2(); 
  bck_mjj_zz->Sumw2();
  bck_mjj_tt->Sumw2();
  bck_mjj_fakes->Sumw2();
  
  TH1D* bck_dphiljj_wz = new TH1D("bck_dphiljj_wz", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphiljj_www = new TH1D("bck_dphiljj_www", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphiljj_data = new TH1D("bck_dphiljj_data", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphiljj_zz = new TH1D("bck_dphiljj_zz", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphiljj_tt = new TH1D("bck_dphiljj_tt", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_dphiljj_fakes = new TH1D("bck_dphiljj_fakes", "#Delta#phi_{ljj}", 200, 0, 3.5);
 
  bck_dphiljj_wz->Sumw2();
  bck_dphiljj_www->Sumw2();
  bck_dphiljj_data->Sumw2(); 
  bck_dphiljj_zz->Sumw2();
  bck_dphiljj_tt->Sumw2();
  bck_dphiljj_fakes->Sumw2();
  
  TH1D* bck_dphilmjj_wz = new TH1D("bck_dphilmjj_wz", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj_www = new TH1D("bck_dphilmjj_www", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj_data = new TH1D("bck_dphilmjj_data", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj_zz = new TH1D("bck_dphilmjj_zz", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj_tt = new TH1D("bck_dphilmjj_tt", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* bck_dphilmjj_fakes = new TH1D("bck_dphilmjj_fakes", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
 
  bck_dphilmjj_wz->Sumw2();
  bck_dphilmjj_www->Sumw2();
  bck_dphilmjj_data->Sumw2(); 
  bck_dphilmjj_zz->Sumw2();
  bck_dphilmjj_tt->Sumw2();
  bck_dphilmjj_fakes->Sumw2();
  
  TH1D* bck_njets_wz = new TH1D("bck_njets_wz", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_njets_www = new TH1D("bck_njets_www", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_njets_data = new TH1D("bck_njets_data", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_njets_zz = new TH1D("bck_njets_zz", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_njets_tt = new TH1D("bck_njets_tt", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_njets_fakes = new TH1D("bck_njets_fakes","Number of jets", 30, -0.5, 29.5);
 
  bck_njets_wz->Sumw2();
  bck_njets_www->Sumw2();
  bck_njets_data->Sumw2(); 
  bck_njets_zz->Sumw2();
  bck_njets_tt->Sumw2();
  bck_njets_fakes->Sumw2();
  
  TH1D* bck_dRll_wz = new TH1D("bck_dRll_wz", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_dRll_www = new TH1D("bck_dRll_www", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_dRll_data = new TH1D("bck_dRll_data", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_dRll_zz = new TH1D("bck_dRll_zz", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_dRll_tt = new TH1D("bck_dRll_tt", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* bck_dRll_fakes = new TH1D("bck_dRll_fakes","#DeltaR_{ll}", 200, 0, 3.5);
 
  bck_dRll_wz->Sumw2();
  bck_dRll_www->Sumw2();
  bck_dRll_data->Sumw2(); 
  bck_dRll_zz->Sumw2();
  bck_dRll_tt->Sumw2();
  bck_dRll_fakes->Sumw2();
  
  TH1D* bck_tmet_wz =  new TH1D("bck_tmet_wz", "tmet", 200, 0, 200);
  TH1D* bck_tmet_www = new TH1D("bck_tmet_www", "tmet", 200, 0, 200);
  TH1D* bck_tmet_data = new TH1D("bck_tmet_data", "tmet", 200, 0, 200);
  TH1D* bck_tmet_zz =  new TH1D("bck_tmet_zz", "tmet", 200, 0, 200);
  TH1D* bck_tmet_tt =  new TH1D("bck_tmet_tt", "tmet", 200, 0, 200);
  TH1D* bck_tmet_fakes =  new TH1D("bck_tmet_fakes", "tmet", 200, 0, 200);

  bck_tmet_wz->Sumw2(); 
  bck_tmet_www->Sumw2();
  bck_tmet_data->Sumw2();
  bck_tmet_zz->Sumw2();  
  bck_tmet_tt->Sumw2(); 
  bck_tmet_fakes->Sumw2(); 
  
  TH1D* bck_minmet_wz =  new TH1D("bck_minmet_wz", "MET", 200, 0, 200);
  TH1D* bck_minmet_www = new TH1D("bck_minmet_www", "MET", 200, 0, 200);
  TH1D* bck_minmet_data = new TH1D("bck_minmet_data", "MET", 200, 0, 200);
  TH1D* bck_minmet_zz =  new TH1D("bck_minmet_zz", "MET", 200, 0, 200);
  TH1D* bck_minmet_tt =  new TH1D("bck_minmet_tt", "MET", 200, 0, 200);
  TH1D* bck_minmet_fakes =  new TH1D("bck_minmet_fakes", "MET", 200, 0, 200);

  bck_minmet_wz->Sumw2(); 
  bck_minmet_www->Sumw2();
  bck_minmet_data->Sumw2();
  bck_minmet_zz->Sumw2();  
  bck_minmet_tt->Sumw2(); 
  bck_minmet_fakes->Sumw2(); 
  
  TH1D* bck_minmll_wz =  new TH1D("bck_minmll_wz", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_minmll_www = new TH1D("bck_minmll_www", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_minmll_data = new TH1D("bck_minmll_data", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_minmll_zz =  new TH1D("bck_minmll_zz", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_minmll_tt =  new TH1D("bck_minmll_tt", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* bck_minmll_fakes =  new TH1D("bck_minmll_fakes", "|m_{ll} - m_{Z}|", 200, 0, 100);

  bck_minmll_wz->Sumw2(); 
  bck_minmll_www->Sumw2();
  bck_minmll_data->Sumw2();
  bck_minmll_zz->Sumw2();  
  bck_minmll_tt->Sumw2(); 
  bck_minmll_fakes->Sumw2(); 
  
  
  TH1D* bck_id_wz = new TH1D("bck_id_wz", "lepton content", 4, 0, 4);
  TH1D* bck_id_www = new TH1D("bck_id_www", "lepton content", 4, 0, 4);
  TH1D* bck_id_data = new TH1D("bck_id_data", "lepton content", 4, 0, 4);
  TH1D* bck_id_zz = new TH1D("bck_id_zz", "lepton content", 4, 0, 4);
  TH1D* bck_id_tt = new TH1D("bck_id_tt", "lepton content", 4, 0, 4);
  TH1D* bck_id_fakes = new TH1D("bck_id_fakes", "lepton content", 4, 0, 4);

  bck_id_wz->Sumw2(); 
  bck_id_www->Sumw2();
  bck_id_data->Sumw2();
  bck_id_zz->Sumw2();  
  bck_id_tt->Sumw2(); 
  bck_id_fakes->Sumw2(); 
  
   
  TH1D* bck_mlll_wz = new TH1D("bck_mlll_wz", "m_{lll}", 200, 0, 400);
  TH1D* bck_mlll_www = new TH1D("bck_mlll_www", "m_{lll}", 200, 0, 400);
  TH1D* bck_mlll_data = new TH1D("bck_mlll_data", "m_{lll}", 200, 0, 400);
  TH1D* bck_mlll_zz = new TH1D("bck_mlll_zz", "m_{lll}", 200, 0, 400);
  TH1D* bck_mlll_tt = new TH1D("bck_mlll_tt", "m_{lll}", 200, 0, 400);
  TH1D* bck_mlll_fakes = new TH1D("bck_mlll_fakes", "m_{lll}", 200, 0, 400);
  
  bck_mlll_wz->Sumw2();
  bck_mlll_www->Sumw2();
  bck_mlll_data->Sumw2(); 
  bck_mlll_zz->Sumw2();
  bck_mlll_tt->Sumw2();
  bck_mlll_fakes->Sumw2();
 
  
  //data
  TH1D* data_cuts = new TH1D("data_cuts", "cuts", 10, 0, 10);
  TH1D* data_met = new TH1D("data_met", "MET", 200, 0, 200);
  TH1D* data_mllz = new TH1D("data_mllz", "m_{ll}", 200, 0, 200);
  TH1D* data_mt = new TH1D("data_mt", "m_t", 200, 0, 200);
  TH1D* data_ptjet = new TH1D("data_ptjet", "P_t of leading jet", 200, 0, 200);
  TH1D* data_mH = new TH1D("data_mH", "m_H", 200, 0, 400);
  TH1D* data_mH_v2 = new TH1D("data_mH_v2", "m_H", 200, 0, 400);
  TH1D* data_mH_3d = new TH1D("data_mH_3d", "m_H", 200, 0, 400);
  TH1D* data_mH_2d = new TH1D("data_mH_2d", "m_H", 200, 0, 400);
  TH1D* data_mjj = new TH1D("data_mjj", "m_jj", 200, 0, 400);
  TH1D* data_dphiljj = new TH1D("data_dphiljj", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* data_dphilmjj = new TH1D("data_dphilmjj", "#Delta#phi_{lMETjj}", 200, 0, 3.5);
  TH1D* data_njets = new TH1D("data_njets", "Number of jets", 30, -0.5, 29.5);
  TH1D* data_dRll = new TH1D("data_dRll", "#DeltaR_{ll}", 200, 0, 3.5);
  TH1D* data_tmet = new TH1D("data_tmet", "tracker MET", 200, 0, 200);
  TH1D* data_minmet = new TH1D("data_minmet", "min(MET, tracker MET)", 200, 0, 200);
  TH1D* data_minmll =  new TH1D("data_minmll", "|m_{ll} - m_{Z}|", 200, 0, 100);
  TH1D* data_id = new TH1D("data_id", "lepton content", 4, 0, 4);
  TH1D* data_mlll = new TH1D("data_mlll", "m_{lll}", 200, 0, 400);

  data_cuts->Sumw2();
  data_met->Sumw2();
  data_mllz->Sumw2();
  data_mt->Sumw2();
  data_ptjet->Sumw2();
  data_mH->Sumw2();
  data_mH_v2->Sumw2();
  data_mH_3d->Sumw2();
  data_mH_2d->Sumw2();
  data_mjj->Sumw2();  
  data_dphiljj->Sumw2();
  data_dphilmjj->Sumw2();
  data_njets->Sumw2();
  data_dRll->Sumw2();
  data_tmet->Sumw2();
  data_minmet->Sumw2();
  data_minmll->Sumw2();
  data_id->Sumw2();
  data_mlll->Sumw2();
  
  double weight = 1;
  //Signal ZH -> 3l2j1nu
  double eventsPassSig = 0;
  double eventsZH = 0;
  int eventstouse = 0;
 
  int nSig=signal.tree_->GetEntries();
  for (int i=0; i<nSig; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nSig);
    signal.tree_->GetEntry(i);

    weight = 1;
    weight = lumi*signal.scale1fb_*signal.sfWeightPU_*signal.sfWeightEff_*signal.sfWeightTrig_;    
    
    //if(signal.processId_ != 24) continue;
    
    //Three real leptons MC level
    bool isRealLepton = false;
    if((TMath::Abs(signal.lep1McId_) == 11 || TMath::Abs(signal.lep1McId_) == 13) &&
       (TMath::Abs(signal.lep2McId_) == 11 || TMath::Abs(signal.lep2McId_) == 13) &&
       (TMath::Abs(signal.lep3McId_) == 11 || TMath::Abs(signal.lep3McId_) == 13)) isRealLepton = true; 
    if (!isRealLepton) continue;
     
    //Check for fakes
    int nFake = 0;
    if(((signal.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (signal.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (signal.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (signal.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake !=0) continue; //No fakes allowed
    sig_cuts->Fill(0., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(0., weight);
    
    //2 same flavor, oppposite sign leptons + extra one
    if (signal.lid3_ == signal.lid2_ && signal.lid3_ == signal.lid1_) continue;
    if (signal.lid3_ == signal.lid2_ && fabs(signal.lid3_) != fabs(signal.lid1_)) continue;
    if (signal.lid3_ == signal.lid1_ && fabs(signal.lid3_) != fabs(signal.lid2_)) continue;
    if (signal.lid2_ == signal.lid1_ && fabs(signal.lid2_) != fabs(signal.lid3_)) continue;
    sig_cuts->Fill(1., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(1., weight);

    //At least 2 jets
    if (signal.njets_ < 2) continue; 
    sig_cuts->Fill(2., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(2., weight);

    //Select the different things: Z pair, extra lepton, Higgs system
    int idcat = 20; // 0 = eee, 1 = eemu, 2 = mumue, 3 = mumumu
    if (fabs(signal.lid1_) == 11 && fabs(signal.lid2_) == 11 && fabs(signal.lid3_) == 11) idcat = 0;
    else if ((fabs(signal.lid1_) == 11 && fabs(signal.lid2_) == 11 && fabs(signal.lid3_) == 13) 
          || (fabs(signal.lid1_) == 11 && fabs(signal.lid2_) == 13 && fabs(signal.lid3_) == 11) 
	  || (fabs(signal.lid1_) == 13 && fabs(signal.lid2_) == 11 && fabs(signal.lid3_) == 11)) idcat = 1;
    else if ((fabs(signal.lid1_) == 11 && fabs(signal.lid2_) == 13 && fabs(signal.lid3_) == 13) 
          || (fabs(signal.lid1_) == 13 && fabs(signal.lid2_) == 13 && fabs(signal.lid3_) == 11) 
	  || (fabs(signal.lid1_) == 13 && fabs(signal.lid2_) == 11 && fabs(signal.lid3_) == 13)) idcat = 2;
    else  if (fabs(signal.lid1_) == 13 && fabs(signal.lid2_) == 13 && fabs(signal.lid3_) == 13)  idcat = 3;
    
    //Make z-compatible pairs
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3, trilep;
    if (fabs(signal.lid1_) == fabs(signal.lid2_) && signal.lq1_*signal.lq2_ < 0){
      pair1 = signal.lep1_ + signal.lep2_ ;
      m[0] = pair1.M();
    }
    if (fabs(signal.lid2_) == fabs(signal.lid3_) && signal.lq2_*signal.lq3_ < 0){
      pair2 = signal.lep2_ + signal.lep3_ ;
      m[1] = pair2.M();
    }
    if (fabs(signal.lid1_) == fabs(signal.lid3_) && signal.lq1_*signal.lq3_ < 0){
      pair3 = signal.lep1_ + signal.lep3_ ;
      m[2] = pair3.M();
    }
    trilep = signal.lep1_ + signal.lep2_ + signal.lep3_ ;
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    double dR = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  signal.mt3_; tlepton = signal.lep3_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(signal.lep1_ ,signal.lep2_));} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  signal.mt1_; tlepton = signal.lep1_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(signal.lep2_ ,signal.lep3_));} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  signal.mt2_; tlepton = signal.lep2_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(signal.lep1_ ,signal.lep3_));} 
    pairjet = signal.jet1_+ signal.jet2_;
    LorentzVector metvector(signal.met_*cos(signal.metPhi_), signal.met_*sin(signal.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + signal.jet1_+ signal.jet2_;
    LorentzVector lm = tlepton + metvector;
    
    double hp[5];
    hp[0] = tlepton.Px() + signal.jet1_.Px()+ signal.jet2_.Px()+ metvector.Px();
    hp[1] = tlepton.Py() + signal.jet1_.Py()+ signal.jet2_.Py()+ metvector.Py();
    hp[2] = tlepton.Pz() + signal.jet1_.Pz()+ signal.jet2_.Pz()+ metvector.Pz();
    
    //Calculate p of the neutrino using Maria's code
    double metp = 0;
   // double otherSol = 0;
    double alpha=(mw*mw-mmu*mmu)/2/tlepton.P()+(tlepton.Px()*signal.met_*cos(signal.metPhi_)+tlepton.Py()*signal.met_*sin(signal.metPhi_))/tlepton.P();
    double A=tlepton.Pz()*tlepton.Pz()/tlepton.P()/tlepton.P()-1;
    double B=2*alpha*tlepton.Pz()/tlepton.P();
    double C=alpha*alpha-(signal.met_*cos(signal.metPhi_)*signal.met_*cos(signal.metPhi_) + signal.met_*sin(signal.metPhi_)*signal.met_*sin(signal.metPhi_));
    double tmproot = B*B - 4.0*A*C;
      if (tmproot<0) { 
       // isComplex= true;
        metp = - B/(2*A); 
	//otherSol = metp;
      } else {
       // isComplex = false;
	double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
	double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) {
	  metp = tmpsol1; //otherSol = tmpsol2; 
	} else { metp = tmpsol2; //otherSol = tmpsol1; 
	}
     }
   
    
   // hp[3] = tlepton.P() + signal.jet1_.P()+ signal.jet2_.P()+ metvector.P(); //crappy solution
    hp[3] = tlepton.P() + signal.jet1_.P()+ signal.jet2_.P()+ metp;
    hp[4] = tlepton.Pt() + signal.jet1_.Pt()+ signal.jet2_.Pt()+ signal.met_;
    
    double recomh  = hp[3]*hp[3]-hp[0]*hp[0]-hp[1]*hp[1]-hp[2]*hp[2]; if(recomh  > 0) recomh  = sqrt(recomh);else recomh   = 0.0;
    double recomth = hp[4]*hp[4]-hp[0]*hp[0]-hp[1]*hp[1]; if(recomth > 0) recomth = sqrt(recomth); else recomth  = 0.0;
   
   
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    sig_cuts->Fill(3., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(3., weight);

    if (signal.met_ < metcut) continue;
    sig_cuts->Fill(4., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(4., weight);
    
    if (mt > mtcut) continue;
    sig_cuts->Fill(5., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(5., weight);
   
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    sig_cuts->Fill(6., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(6., weight);
    
    //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
    sig_cuts->Fill(7., weight);
    if(signal.processId_ == 24)sig_cuts_zh->Fill(7., weight);
    
    //Fill histos    
    sig_njets->Fill(signal.njets_, weight);
    sig_met->Fill(signal.met_, weight);
    sig_mllz->Fill(pair.M(), weight);
    sig_mt->Fill(mt, weight);
    sig_ptjet->Fill(signal.jet1_.Pt(), weight);  
    if (higgsSystem.M() > 0) sig_mH_v2->Fill(higgsSystem.M(), weight); 
    else sig_mH_v2->Fill(0.0, weight);
    sig_mH->Fill(higgsSystem.M(), weight);
    sig_mH_3d->Fill(recomh, weight);
    sig_mH_2d->Fill(recomth, weight);
    sig_mjj->Fill(pairjet.M(), weight);
    sig_dphiljj->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
    sig_dphilmjj->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
    sig_dRll->Fill(dR);
    sig_tmet->Fill(signal.trackMet_, weight);
    sig_minmet->Fill(TMath::Min(signal.met_, signal.trackMet_), weight);
    sig_minmll->Fill(fabs(mz - pair.M()), weight);
    sig_id->Fill(idcat, weight);
    sig_mlll->Fill(trilep.M(), weight);
    
    eventstouse++;
    
    eventsPassSig += weight;
    if(signal.processId_ == 24) eventsZH+= weight;
    
   
  }
  
  cout << endl;
  cout << eventsPassSig << " signal events in " << lumi << " fb" << endl; 
  cout << eventstouse << " raw events" << endl;
  cout << endl;

  //Backgrounds
  double bckType[62] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  double weiType[62] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  
  TString bckName[62] = {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null", "null", "fakes"};
  
  bckName[0] = "data";
  bckName[1] = "qqww";
  bckName[2] = "ggww";
  bckName[43] = "ttbar";
  bckName[44] = "tw";
  bckName[46] = "dymm";
  bckName[49] = "wz";
  bckName[50] = "zz";
  bckName[51] = "wgamma";
  bckName[59] = "www";
  bckName[60] = "dyttdd";    
  
  double eventsPassBck = 0;
  int previous = -1;
   double neach[8]{.0, .0, .0, .0, .0, .0, .0, .0};

  int nBck=background.tree_->GetEntries();
  for (int i=0; i<nBck; ++i) {
    
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nBck);
    background.tree_->GetEntry(i);

    weight = 1;
    if (background.dstype_ != SmurfTree::data) weight = lumi*background.scale1fb_*background.sfWeightPU_*background.sfWeightEff_*background.sfWeightTrig_;    

    int nsel = background.dstype_;
  
    //Three real leptons MC level
    bool isRealLepton = false;
    if((TMath::Abs(background.lep1McId_) == 11 || TMath::Abs(background.lep1McId_) == 13) &&
       (TMath::Abs(background.lep2McId_) == 11 || TMath::Abs(background.lep2McId_) == 13) &&
       (TMath::Abs(background.lep3McId_) == 11 || TMath::Abs(background.lep3McId_) == 13)) isRealLepton = true; 
    if (!isRealLepton && background.dstype_ != SmurfTree::data) continue;
     
    //Check for fakes
    int nFake = 0;
    if(((background.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (background.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (background.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (background.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (background.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (background.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (background.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake){ 
      nsel = 61;
      double factor = 1;
      weight*= background.sfWeightFR_*factor;
      //if (background.dstype_ != SmurfTree::data) weight *=-1;
    }
    
    bck_cuts->Fill(0., weight);
       
    if (nsel == 49) bck_cuts_wz->Fill(0., weight);
    else if (nsel == 59) bck_cuts_www->Fill(0., weight);
    else if (nsel == 0) bck_cuts_data->Fill(0., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(0., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(0., weight);
    else bck_cuts_other->Fill(0., weight);
    
    //2 same flavor, oppposite sign leptons + extra one
    if (background.lid3_ == background.lid2_ && background.lid3_ == background.lid1_) continue;
    if (background.lid3_ == background.lid2_ && fabs(background.lid3_) != fabs(background.lid1_)) continue;
    if (background.lid3_ == background.lid1_ && fabs(background.lid3_) != fabs(background.lid2_)) continue;
    if (background.lid2_ == background.lid1_ && fabs(background.lid2_) != fabs(background.lid3_)) continue;
    
    //Select the different things: Z pair, extra lepton, Higgs system
    int idcat = 20; // 0 = eee, 1 = eemu, 2 = mumue, 3 = mumumu
    if (fabs(background.lid1_) == 11 && fabs(background.lid2_) == 11 && fabs(background.lid3_) == 11) idcat = 0;
    else if ((fabs(background.lid1_) == 11 && fabs(background.lid2_) == 11 && fabs(background.lid3_) == 13) 
          || (fabs(background.lid1_) == 11 && fabs(background.lid2_) == 13 && fabs(background.lid3_) == 11) 
	  || (fabs(background.lid1_) == 13 && fabs(background.lid2_) == 11 && fabs(background.lid3_) == 11)) idcat = 1;
    else if ((fabs(background.lid1_) == 11 && fabs(background.lid2_) == 13 && fabs(background.lid3_) == 13) 
          || (fabs(background.lid1_) == 13 && fabs(background.lid2_) == 13 && fabs(background.lid3_) == 11) 
	  || (fabs(background.lid1_) == 13 && fabs(background.lid2_) == 11 && fabs(background.lid3_) == 13)) idcat = 2;
    else  if (fabs(background.lid1_) == 13 && fabs(background.lid2_) == 13 && fabs(background.lid3_) == 13)  idcat = 3;
    
    bck_cuts->Fill(1., weight);
       
    if (nsel == 49) bck_cuts_wz->Fill(1., weight);
    else if (nsel == 59) bck_cuts_www->Fill(1., weight);
    else if (nsel == 0) bck_cuts_data->Fill(1., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(1., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(1., weight);
    else bck_cuts_other->Fill(1., weight);
    
    //At least 2 jets
    if (background.njets_ < 2) continue; 
    bck_cuts->Fill(2., weight);
   
    if (nsel == 49) bck_cuts_wz->Fill(2., weight);
    else if (nsel == 59) bck_cuts_www->Fill(2., weight);
    else if (nsel == 0) bck_cuts_data->Fill(2., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(2., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(2., weight);
    else bck_cuts_other->Fill(2., weight);
    
    //Make z-compatible pairs
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3, trilep;
    if (fabs(background.lid1_) == fabs(background.lid2_) && background.lq1_*background.lq2_ < 0){
      pair1 = background.lep1_ + background.lep2_ ;
      m[0] = pair1.M();
    }
    if (fabs(background.lid2_) == fabs(background.lid3_) && background.lq2_*background.lq3_ < 0){
      pair2 = background.lep2_ + background.lep3_ ;
      m[1] = pair2.M();
    }
    if (fabs(background.lid1_) == fabs(background.lid3_) && background.lq1_*background.lq3_ < 0){
      pair3 = background.lep1_ + background.lep3_ ;
      m[2] = pair3.M();
    }
    trilep = background.lep1_ + background.lep2_ + background.lep3_ ;

    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    double dR = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  background.mt3_; tlepton = background.lep3_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(background.lep1_ ,background.lep2_));} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  background.mt1_; tlepton = background.lep1_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(background.lep2_ ,background.lep3_));} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  background.mt2_; tlepton = background.lep2_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(background.lep1_ ,background.lep3_));} 
    pairjet = background.jet1_+ background.jet2_;
    LorentzVector metvector(background.met_*cos(background.metPhi_), background.met_*sin(background.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + background.jet1_+ background.jet2_;
    LorentzVector lm = tlepton + metvector;
   
      
    double hp[5];
    hp[0] = tlepton.Px() + background.jet1_.Px()+ background.jet2_.Px()+ metvector.Px();
    hp[1] = tlepton.Py() + background.jet1_.Py()+ background.jet2_.Py()+ metvector.Py();
    hp[2] = tlepton.Pz() + background.jet1_.Pz()+ background.jet2_.Pz()+ metvector.Pz();
    
    //Calculate p of the neutrino using Maria's code
    double metp = 0;
    double otherSol = 0;
    double alpha=(mw*mw-mmu*mmu)/2/tlepton.P()+(tlepton.Px()*background.met_*cos(background.metPhi_)+tlepton.Py()*background.met_*sin(background.metPhi_))/tlepton.P();
    double A=tlepton.Pz()*tlepton.Pz()/tlepton.P()/tlepton.P()-1;
    double B=2*alpha*tlepton.Pz()/tlepton.P();
    double C=alpha*alpha-(background.met_*cos(background.metPhi_)*background.met_*cos(background.metPhi_) + background.met_*sin(background.metPhi_)*background.met_*sin(background.metPhi_));
    bool isComplex = false;
    double tmproot = B*B - 4.0*A*C;
      if (tmproot<0) { 
        isComplex= true;
        metp = - B/(2*A); 
	otherSol = metp;
      } else {
        isComplex = false;
	double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
	double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) {
	  metp = tmpsol1; otherSol = tmpsol2; 
	} else { metp = tmpsol2; otherSol = tmpsol1; }
     }
   
    
   // hp[3] = tlepton.P() + background.jet1_.P()+ background.jet2_.P()+ metvector.P(); //crappy solution
    hp[3] = tlepton.P() + background.jet1_.P()+ background.jet2_.P()+ metp;
    hp[4] = tlepton.Pt() + background.jet1_.Pt()+ background.jet2_.Pt()+ background.met_;
    
    double recomh  = hp[3]*hp[3]-hp[0]*hp[0]-hp[1]*hp[1]-hp[2]*hp[2]; if(recomh  > 0) recomh  = sqrt(recomh);else recomh   = 0.0;
    double recomth = hp[4]*hp[4]-hp[0]*hp[0]-hp[1]*hp[1]; if(recomth > 0) recomth = sqrt(recomth); else recomth  = 0.0;
    
 
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    bck_cuts->Fill(3., weight); 
       
    if (nsel == 49) bck_cuts_wz->Fill(3., weight);
    else if (nsel == 59) bck_cuts_www->Fill(3., weight);
    else if (nsel == 0) bck_cuts_data->Fill(3., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(3., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(3., weight);
    else bck_cuts_other->Fill(3., weight);
    
    if (background.met_ < metcut) continue;
    bck_cuts->Fill(4., weight);
       
    if (nsel == 49) bck_cuts_wz->Fill(4., weight);
    else if (nsel == 59) bck_cuts_www->Fill(4., weight);
    else if (nsel == 0) bck_cuts_data->Fill(4., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(4., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(4., weight);
    else bck_cuts_other->Fill(4., weight);
    
    
    if (mt > mtcut) continue;
    bck_cuts->Fill(5., weight);
    
    if (nsel == 49) bck_cuts_wz->Fill(5., weight);
    else if (nsel == 59) bck_cuts_www->Fill(5., weight);
    else if (nsel == 0) bck_cuts_data->Fill(5., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(5., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(5., weight);
    else bck_cuts_other->Fill(5., weight);
    
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    bck_cuts->Fill(6., weight);
       
    if (nsel == 49) bck_cuts_wz->Fill(6., weight);
    else if (nsel == 59) bck_cuts_www->Fill(6., weight);
    else if (nsel == 0) bck_cuts_data->Fill(6., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(6., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(6., weight);
    else bck_cuts_other->Fill(6., weight);
    
     
   //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
    bck_cuts->Fill(7., weight);
    if (nsel == 49) bck_cuts_wz->Fill(7., weight);
    else if (nsel == 59) bck_cuts_www->Fill(7., weight);
    else if (nsel == 0) bck_cuts_data->Fill(7., weight);
    else if (nsel == 50) bck_cuts_zz->Fill(7., weight);
    else if (nsel == 61) bck_cuts_fakes->Fill(7., weight);
    else bck_cuts_other->Fill(7., weight);
    
    //Fill histos
    types->Fill(background.dstype_);
    bck_njets->Fill(background.njets_, weight);
    bck_met->Fill(background.met_, weight);
    bck_mllz->Fill(pair.M(), weight);
    bck_mt->Fill(mt, weight);
    bck_ptjet->Fill(background.jet1_.Pt(), weight);  
    if (higgsSystem.M() > 0) bck_mH_v2->Fill(higgsSystem.M(), weight); 
    else bck_mH_v2->Fill(0.0, weight);
    bck_mH->Fill(higgsSystem.M(), weight); 
    bck_mH_3d->Fill(recomh, weight);
    bck_mH_2d->Fill(recomth, weight);
    bck_mjj->Fill(pairjet.M(), weight);
    bck_dphiljj->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
    bck_dphilmjj->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
    bck_dRll->Fill(dR);
    bck_tmet->Fill(background.trackMet_, weight);
    bck_minmet->Fill(TMath::Min(background.met_, background.trackMet_), weight);
    bck_minmll->Fill(fabs(mz - pair.M()), weight);
    bck_id->Fill(idcat, weight);
    bck_mlll->Fill(trilep.M(), weight);
    
    if (nsel == 49){
      bck_njets_wz->Fill(background.njets_, weight);
      bck_met_wz->Fill(background.met_, weight);
      bck_mllz_wz->Fill(pair.M(), weight);
      bck_mt_wz->Fill(mt, weight);
      bck_ptjet_wz->Fill(background.jet1_.Pt(), weight);  
      if (higgsSystem.M() > 0) bck_mH_v2_wz->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_wz->Fill(0.0, weight);
      bck_mH_wz->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_wz->Fill(recomh, weight);
      bck_mH_2d_wz->Fill(recomth, weight);
      bck_mjj_wz->Fill(pairjet.M(), weight);
      bck_dphiljj_wz->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dphilmjj_wz->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
      bck_dRll_wz->Fill(dR);
      bck_tmet_wz->Fill(background.trackMet_, weight);
      bck_minmet_wz->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_wz->Fill(fabs(mz - pair.M()), weight);
      bck_id_wz->Fill(idcat, weight);
      bck_mlll_wz->Fill(trilep.M(), weight);
    } else if (nsel == 59){
      bck_njets_www->Fill(background.njets_, weight);
      bck_met_www->Fill(background.met_, weight);
      bck_mllz_www->Fill(pair.M(), weight);
      bck_mt_www->Fill(mt, weight);
      bck_ptjet_www->Fill(background.jet1_.Pt(), weight); 
      if (higgsSystem.M() > 0) bck_mH_v2_www->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_www->Fill(0.0, weight);
      bck_mH_www->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_www->Fill(recomh, weight);
      bck_mH_2d_www->Fill(recomth, weight);
      bck_mjj_www->Fill(pairjet.M(), weight);
      bck_dphiljj_www->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dphilmjj_www->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
      bck_dRll_www->Fill(dR);
      bck_tmet_www->Fill(background.trackMet_, weight);
      bck_minmet_www->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_www->Fill(fabs(mz - pair.M()), weight);
      bck_id_www->Fill(idcat, weight);
      bck_mlll_www->Fill(trilep.M(), weight);
    } else if (nsel == 0){
      bck_njets_data->Fill(background.njets_, weight);
      bck_met_data->Fill(background.met_, weight);
      bck_mllz_data->Fill(pair.M(), weight);
      bck_mt_data->Fill(mt, weight);
      bck_ptjet_data->Fill(background.jet1_.Pt(), weight); 
      if (higgsSystem.M() > 0) bck_mH_v2_data->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_data->Fill(0.0, weight);
      bck_mH_data->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_data->Fill(recomh, weight);
      bck_mH_2d_data->Fill(recomth, weight);
      bck_mjj_data->Fill(pairjet.M(), weight);
      bck_dphiljj_data->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dRll_data->Fill(dR);
      bck_tmet_data->Fill(background.trackMet_, weight);
      bck_minmet_data->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_data->Fill(fabs(mz - pair.M()), weight);
      bck_id_data->Fill(idcat, weight);
      bck_mlll_data->Fill(trilep.M(), weight);
    } else if (nsel == 50){
      bck_njets_zz->Fill(background.njets_, weight);
      bck_met_zz->Fill(background.met_, weight);
      bck_mllz_zz->Fill(pair.M(), weight);
      bck_mt_zz->Fill(mt, weight);
      bck_ptjet_zz->Fill(background.jet1_.Pt(), weight);  
      if (higgsSystem.M() > 0) bck_mH_v2_zz->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_zz->Fill(0.0, weight);
      bck_mH_zz->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_zz->Fill(recomh, weight);
      bck_mH_2d_zz->Fill(recomth, weight);
      bck_mjj_zz->Fill(pairjet.M(), weight);
      bck_dphiljj_zz->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dphilmjj_zz->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
      bck_dRll_zz->Fill(dR);
      bck_tmet_zz->Fill(background.trackMet_, weight);
      bck_minmet_zz->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_zz->Fill(fabs(mz - pair.M()), weight);
      bck_id_zz->Fill(idcat, weight);
      bck_mlll_zz->Fill(trilep.M(), weight);
    } else if (nsel == 43){
      bck_njets_tt->Fill(background.njets_, weight);
      bck_met_tt->Fill(background.met_, weight);
      bck_mllz_tt->Fill(pair.M(), weight);
      bck_mt_tt->Fill(mt, weight);
      bck_ptjet_tt->Fill(background.jet1_.Pt(), weight);
      if (higgsSystem.M() > 0) bck_mH_v2_tt->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_tt->Fill(0.0, weight);
      bck_mH_tt->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_tt->Fill(recomh, weight);
      bck_mH_2d_tt->Fill(recomth, weight);
      bck_mjj_tt->Fill(pairjet.M(), weight);
      bck_dphiljj_tt->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dphilmjj_tt->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
      bck_dRll_tt->Fill(dR);
      bck_tmet_tt->Fill(background.trackMet_, weight);
      bck_minmet_tt->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_tt->Fill(fabs(mz - pair.M()), weight);
      bck_id_tt->Fill(idcat, weight);
      bck_mlll_tt->Fill(trilep.M(), weight);
    } 
    else if (nsel == 61) {
      bck_njets_fakes->Fill(background.njets_, weight);
      bck_met_fakes->Fill(background.met_, weight);
      bck_mllz_fakes->Fill(pair.M(), weight);
      bck_mt_fakes->Fill(mt, weight);
      bck_ptjet_fakes->Fill(background.jet1_.Pt(), weight);  
      if (higgsSystem.M() > 0) bck_mH_v2_fakes->Fill(higgsSystem.M(), weight); 
      else bck_mH_v2_fakes->Fill(0.0, weight);
      bck_mH_fakes->Fill(higgsSystem.M(), weight); 
      bck_mH_3d_fakes->Fill(recomh, weight);
      bck_mH_2d_fakes->Fill(recomth, weight);
      bck_mjj_fakes->Fill(pairjet.M(), weight);
      bck_dphiljj_fakes->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
      bck_dphilmjj_fakes->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
      bck_dRll_fakes->Fill(dR);
      bck_tmet_fakes->Fill(background.trackMet_, weight);
      bck_minmet_fakes->Fill(TMath::Min(background.met_, background.trackMet_), weight);
      bck_minmll_fakes->Fill(fabs(mz - pair.M()), weight);
      bck_id_fakes->Fill(idcat, weight);
      bck_mlll_fakes->Fill(trilep.M(), weight);
    } 
  
 if (nsel == 61){
   if  (background.dstype_ == 0) neach[0]+= weight;
   if  (background.dstype_ == 43) neach[1]+= weight;
   if  (background.dstype_ == 46) neach[2]+= weight;
   if  (background.dstype_ == 49) neach[3]+= weight;
   if  (background.dstype_ == 50) neach[4]+= weight;
   if  (background.dstype_ == 59) neach[5]+= weight;
   if  (background.dstype_ == 61) neach[6]+= weight;
   if  (background.dstype_ == 89) neach[7]+= weight;
 }  
 if (nsel == 61 && background.dstype_ != previous) { cout << "fake comes from:" <<  background.dstype_ << endl; previous = background.dstype_; }
  
    eventsPassBck += weight;
        
    bckType[(int)nsel] += weight;
    weiType[(int)nsel] += weight*weight;
		
   

  }
  
  cout << endl;
  cout << eventsPassBck << " background events in " << lumi << " fb" << endl; 
  cout << endl;
  for (int i = 0; i < 8; i++){
  cout << neach[i] << ", ";
  }
  cout << endl;
  
  //data
  double eventsPassData = 0;
  int nData=data.tree_->GetEntries();
  for (int i=0; i<nData; ++i) {
      
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nData);
    data.tree_->GetEntry(i);

    weight = 1;

    //Check for fakes
    int nFake = 0;
    if(((data.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (data.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (data.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (data.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (data.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (data.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (data.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake !=0) continue; //No fakes allowed
    data_cuts->Fill(0., weight);
    
    //2 same flavor, oppposite sign leptons + extra one
    if (data.lid3_ == data.lid2_ && data.lid3_ == data.lid1_) continue;
    if (data.lid3_ == data.lid2_ && fabs(data.lid3_) != fabs(data.lid1_)) continue;
    if (data.lid3_ == data.lid1_ && fabs(data.lid3_) != fabs(data.lid2_)) continue;
    if (data.lid2_ == data.lid1_ && fabs(data.lid2_) != fabs(data.lid3_)) continue;
    
    //Select the different things: Z pair, extra lepton, Higgs system
    int idcat = 20; // 0 = eee, 1 = eemu, 2 = mumue, 3 = mumumu
    if (fabs(data.lid1_) == 11 && fabs(data.lid2_) == 11 && fabs(data.lid3_) == 11) idcat = 0;
    else if ((fabs(data.lid1_) == 11 && fabs(data.lid2_) == 11 && fabs(data.lid3_) == 13) 
          || (fabs(data.lid1_) == 11 && fabs(data.lid2_) == 13 && fabs(data.lid3_) == 11) 
	  || (fabs(data.lid1_) == 13 && fabs(data.lid2_) == 11 && fabs(data.lid3_) == 11)) idcat = 1;
    else if ((fabs(data.lid1_) == 11 && fabs(data.lid2_) == 13 && fabs(data.lid3_) == 13) 
          || (fabs(data.lid1_) == 13 && fabs(data.lid2_) == 13 && fabs(data.lid3_) == 11) 
	  || (fabs(data.lid1_) == 13 && fabs(data.lid2_) == 11 && fabs(data.lid3_) == 13)) idcat = 2;
    else  if (fabs(data.lid1_) == 13 && fabs(data.lid2_) == 13 && fabs(data.lid3_) == 13)  idcat = 3;
    
    data_cuts->Fill(1., weight);
    
    //At least 2 jets
    if (data.njets_ < 2) continue; 
    data_cuts->Fill(2., weight);

    
    //Make z-compatible pairs
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3, trilep;
    if (fabs(data.lid1_) == fabs(data.lid2_) && data.lq1_*data.lq2_ < 0){
      pair1 = data.lep1_ + data.lep2_ ;
      m[0] = pair1.M();
    }
    if (fabs(data.lid2_) == fabs(data.lid3_) && data.lq2_*data.lq3_ < 0){
      pair2 = data.lep2_ + data.lep3_ ;
      m[1] = pair2.M();
    }
    if (fabs(data.lid1_) == fabs(data.lid3_) && data.lq1_*data.lq3_ < 0){
      pair3 = data.lep1_ + data.lep3_ ;
      m[2] = pair3.M();
    }
    trilep = data.lep1_ + data.lep2_ + data.lep3_ ;

    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    double dR = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  data.mt3_; tlepton = data.lep3_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(data.lep1_ ,data.lep2_));} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  data.mt1_; tlepton = data.lep1_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(data.lep2_ ,data.lep3_));} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  data.mt2_; tlepton = data.lep2_; dR = fabs(ROOT::Math::VectorUtil::DeltaR(data.lep1_ ,data.lep3_));} 
    pairjet = data.jet1_+ data.jet2_;

    LorentzVector metvector(data.met_*cos(data.metPhi_), data.met_*sin(data.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + data.jet1_+ data.jet2_;
    LorentzVector lm = tlepton + metvector;
    
    double hp[5];
    hp[0] = tlepton.Px() + data.jet1_.Px()+ data.jet2_.Px()+ metvector.Px();
    hp[1] = tlepton.Py() + data.jet1_.Py()+ data.jet2_.Py()+ metvector.Py();
    hp[2] = tlepton.Pz() + data.jet1_.Pz()+ data.jet2_.Pz()+ metvector.Pz();
    
    //Calculate p of the neutrino using Maria's code
    double metp = 0;
    double otherSol = 0;
    double alpha=(mw*mw-mmu*mmu)/2/tlepton.P()+(tlepton.Px()*data.met_*cos(data.metPhi_)+tlepton.Py()*data.met_*sin(data.metPhi_))/tlepton.P();
    double A=tlepton.Pz()*tlepton.Pz()/tlepton.P()/tlepton.P()-1;
    double B=2*alpha*tlepton.Pz()/tlepton.P();
    double C=alpha*alpha-(data.met_*cos(data.metPhi_)*data.met_*cos(data.metPhi_) + data.met_*sin(data.metPhi_)*data.met_*sin(data.metPhi_));
    bool isComplex = false;
    double tmproot = B*B - 4.0*A*C;
      if (tmproot<0) { 
        isComplex= true;
        metp = - B/(2*A); 
	otherSol = metp;
      } else {
        isComplex = false;
	double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
	double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) {
	  metp = tmpsol1; otherSol = tmpsol2; 
	} else { metp = tmpsol2; otherSol = tmpsol1; }
     }
   
    
   // hp[3] = tlepton.P() + data.jet1_.P()+ data.jet2_.P()+ metvector.P(); //crappy solution
    hp[3] = tlepton.P() + data.jet1_.P()+ data.jet2_.P()+ metp;
    hp[4] = tlepton.Pt() + data.jet1_.Pt()+ data.jet2_.Pt()+ data.met_;
    
    double recomh  = hp[3]*hp[3]-hp[0]*hp[0]-hp[1]*hp[1]-hp[2]*hp[2]; if(recomh  > 0) recomh  = sqrt(recomh);else recomh   = 0.0;
    double recomth = hp[4]*hp[4]-hp[0]*hp[0]-hp[1]*hp[1]; if(recomth > 0) recomth = sqrt(recomth); else recomth  = 0.0;
    
 
    
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    data_cuts->Fill(3., weight);
    
    if (data.met_ < metcut) continue;
    data_cuts->Fill(4., weight);
    
    if (mt > mtcut) continue;
    data_cuts->Fill(5., weight);
 
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    data_cuts->Fill(6., weight);
      
    //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
    data_cuts->Fill(7., weight);
    
    //Fill histos
    data_njets->Fill(data.njets_, weight);
    data_met->Fill(data.met_, weight);
    data_mllz->Fill(pair.M(), weight);
    data_mt->Fill(mt, weight);
    data_ptjet->Fill(data.jet1_.Pt(), weight);  
    if (higgsSystem.M() > 0) data_mH_v2->Fill(higgsSystem.M(), weight); 
    else data_mH_v2->Fill(0.0, weight);
    data_mH->Fill(higgsSystem.M(), weight);
    data_mH_3d->Fill(recomh, weight);
    data_mH_2d->Fill(recomth, weight);
    data_mjj->Fill(pairjet.M(), weight);
    data_dphiljj->Fill(fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi())), weight);
    data_dphilmjj->Fill(fabs(DeltaPhi(pairjet.Phi(),lm.Phi())), weight);
    data_dRll->Fill(dR, weight);
    data_tmet->Fill(data.trackMet_, weight);
    data_minmet->Fill(TMath::Min(data.met_, data.trackMet_), weight);
    data_minmll->Fill(fabs(mz - pair.M()), weight);
    data_id->Fill(idcat, weight);
    data_mlll->Fill(trilep.M(), weight);
    
    eventsPassData += weight;
    
  

  }
  
  cout << endl;
  cout << eventsPassData << " data events in " << lumi << " fb" << endl; 
  cout << endl;
  
  if (verboseLevel){ 
    cout << "------------------------------------------" << endl;
    cout << "[Signal HWW 125 GeV:] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " 3 lep:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 2) cout << " OSSF:\t\t"  <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 4) cout << " mll:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 5) cout << " met:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 6) cout << " mt:\t\t"    <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 7) cout << " mjj:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;  if (i == 7) cout << " mjj:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 8) cout << " phi:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
  
    }
    cout << "(from which " << eventsZH << " are genuine ZH events)" << endl;
    cout << "------------------------------------------" << endl;  
    cout << "[Backgrounds (All mixed):] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " 3 lep:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 2) cout << " OSSF:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 4) cout << " mll:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 5) cout << " met:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 6) cout << " mt:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 7) cout << " mjj:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 8) cout << " phi:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
    }
    cout << endl;
    cout << "[Breakdown:] " << endl;
    for(int i=0; i<62; i++){
      if(bckType[i] != 0 )
	cout << i <<"\t" << bckName[i] << ":\t\t" << bckType[i] << "+-" << sqrt(weiType[i]) <<endl;
    }
    cout << "------------------------------------------" << endl; 
    cout << "[Data:] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " 3 lep:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 2) cout << " OSSF:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 4) cout << " mll:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 5) cout << " met:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 6) cout << " mt:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 7) cout << " mjj:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 8) cout << " phi:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
    }
    cout << "------------------------------------------" << endl; 
  }
  
  outFileNjets->Write();
  outFileNjets->Close();
  
}

