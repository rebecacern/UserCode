// rebeca@cern.ch
// First attempt to ZH code
// November 2012

#include "../Smurf/Core/SmurfTree.h"
#include "../Smurf/Analysis/HWWlvlv/factors.h"
#include "../Smurf/Core/LeptonScaleLookup.h"
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

void Test() {
  
  TString bgdInputFile    = "samples/backgroundA_3l.root";
  TString dataInputFile   = "samples/data_3l.root";
  TString sigInputFile   =  "samples/hww125.root";
  
  SmurfTree background;
  background.LoadTree(bgdInputFile,-1);
  background.InitTree(0);
  
  SmurfTree data;
  data.LoadTree(dataInputFile,-1);
  data.InitTree(0);
  
  SmurfTree signal;
  signal.LoadTree(sigInputFile,-1);
  signal.InitTree(0);
  
  char output[200];
  sprintf(output,"histo_test.root");     
  TFile* outFileNjets = new TFile(output,"recreate");
  
  TH1F* types = new TH1F("types", "types", 80, -0.5 , 79.5);
  types->Sumw2();
  
  TH1D* bckg_met = new TH1D("bckg_met", "MET", 200, 0, 200);
  bckg_met->Sumw2();
  TH1D* bckg_mllz = new TH1D("bckg_mllz", "m_{ll}", 200, 0, 200);
  bckg_mllz->Sumw2();
  TH1D* bckg_mt = new TH1D("bckg_mt", "m_t", 200, 0, 200);
  bckg_mt->Sumw2();
  TH1D* bckg_ptjet = new TH1D("bckg_ptjet", "P_t of leading jet", 200, 0, 200);
  bckg_ptjet->Sumw2();
  TH1D* bckg_mH = new TH1D("bckg_mH", "m_H", 200, 0, 400);
  bckg_mH->Sumw2();
  TH1D* bckg_mjj = new TH1D("bckg_mjj", "m_jj", 200, 0, 400);
  bckg_mjj->Sumw2();
  TH1D* bckg_dphill = new TH1D("bckg_dphill", "#Delta#phi_{ll}", 200, 0, 3.5);
  bckg_dphill->Sumw2();
  
  TH1D* sig_met = new TH1D("sig_met", "MET", 200, 0, 200);
  sig_met->Sumw2();
  TH1D* sig_mllz = new TH1D("sig_mllz", "m_{ll}", 200, 0, 200);
  sig_mllz->Sumw2();
  TH1D* sig_mt = new TH1D("sig_mt", "m_t", 200, 0, 200);
  sig_mt->Sumw2();
  TH1D* sig_ptjet = new TH1D("sig_ptjet", "P_t of leading jet", 200, 0, 200);
  sig_ptjet->Sumw2();
  TH1D* sig_mH = new TH1D("sig_mH", "m_H", 200, 0, 400);
  sig_mH->Sumw2();
  TH1D* sig_mjj = new TH1D("sig_mjj", "m_jj", 200, 0, 400);
  sig_mjj->Sumw2();
  TH1D* sig_dphill = new TH1D("sig_dphill", "#Delta#phi_{ll}", 200, 0, 3.5);
  sig_dphill->Sumw2();

  double lumi = 12.1;
  double weight = 1;
  double eventsPass = 0;
  int nBgd=background.tree_->GetEntries();
  for (int i=0; i<nBgd; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nBgd);
    background.tree_->GetEntry(i);
    
    weight = 1;
    weight *= background.scale1fb_*lumi;
    weight *= background.sfWeightPU_;
    weight *= background.sfWeightEff_;
    weight *= background.sfWeightTrig_;
    weight *= background.sfWeightFR_;
    
    
    if (background.njets_ != 2) continue;
    if (background.lid3_ == background.lid2_ && background.lid3_ == background.lid1_) continue;
    if (background.lid3_ == background.lid2_ && fabs(background.lid3_) != fabs(background.lid1_)) continue;
    if (background.lid3_ == background.lid1_ && fabs(background.lid3_) != fabs(background.lid2_)) continue;
    if (background.lid2_ == background.lid1_ && fabs(background.lid2_) != fabs(background.lid3_)) continue;
    
    
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3;
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
    
    if ( (m[0] < 80 || m[0] > 100) &&  (m[1] < 80 || m[1] > 100) &&  (m[2] < 80 || m[2] > 100)) continue;
    
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
   
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])){  pair = pair1; mt =  background.mt3_; tlepton = background.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  background.mt1_; tlepton = background.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  background.mt2_; tlepton = background.lep2_;} 
    pairjet = background.jet1_+ background.jet2_;
    
    if (mt < 40 || background.met_ < 25) continue;
   
    types->Fill(background.dstype_);
    bckg_met->Fill(background.met_, weight);
    bckg_mllz->Fill(pair.M(), weight);
    bckg_mt->Fill(mt, weight);
    bckg_ptjet->Fill(background.jet1_.Pt(), weight);
    LorentzVector metvector(background.met_*cos(background.metPhi_), background.met_*sin(background.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + background.jet1_  + background.jet2_;
    bckg_mH->Fill(higgsSystem.M(), weight);
    bckg_mjj->Fill(pairjet.M(), weight);
    bckg_dphill->Fill(DeltaPhi(pairjet.Phi(),tlepton.Phi()), weight);
 eventsPass += weight;
 
  }
 cout << eventsPass << " background events in " << lumi << " fb" << endl; 
  
  
  int nSig=signal.tree_->GetEntries();
  double eventsPassSig = 0;
  for (int i=0; i<nSig; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nSig);
    signal.tree_->GetEntry(i);
    
    weight = 1;
    weight *= signal.scale1fb_*lumi;
    weight *= signal.sfWeightPU_;
    weight *= signal.sfWeightEff_;
    weight *= signal.sfWeightTrig_;
    
    if (signal.njets_ != 2) continue;
    if (signal.lid3_ == signal.lid2_ && signal.lid3_ == signal.lid1_) continue;
    if (signal.lid3_ == signal.lid2_ && fabs(signal.lid3_) != fabs(signal.lid1_)) continue;
    if (signal.lid3_ == signal.lid1_ && fabs(signal.lid3_) != fabs(signal.lid2_)) continue;
    if (signal.lid2_ == signal.lid1_ && fabs(signal.lid2_) != fabs(signal.lid3_)) continue;
    
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3;
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
    
    if ( (m[0] < 80 || m[0] > 100) &&  (m[1] < 80 || m[1] > 100) &&  (m[2] < 80 || m[2] > 100)) continue;
    
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])){  pair = pair1; mt =  signal.mt3_; tlepton = signal.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  signal.mt1_; tlepton = signal.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  signal.mt2_; tlepton = signal.lep2_;} 
    pairjet = signal.jet1_+ signal.jet2_;
        
    if (mt < 40 || signal.met_ < 25) continue;
    types->Fill(signal.dstype_);
    sig_met->Fill(signal.met_, weight);
    sig_mllz->Fill(pair.M(), weight);
    sig_mt->Fill(mt, weight);
    sig_ptjet->Fill(signal.jet1_.Pt(), weight);  
    LorentzVector metvector(signal.met_*cos(signal.metPhi_), signal.met_*sin(signal.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + signal.jet1_  + signal.jet2_;
    sig_mH->Fill(higgsSystem.M(), weight);
    sig_mjj->Fill(pairjet.M(), weight);
    sig_dphill->Fill(DeltaPhi(pairjet.Phi(),tlepton.Phi()), weight);
 eventsPassSig += weight;
  }
  
   cout << eventsPassSig << " signal events in " << lumi << " fb" << endl; 

  int nData=data.tree_->GetEntries();
  double eventsPassData = 0;
  for (int i=0; i<nData; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nData);
    data.tree_->GetEntry(i);
    
    weight = 1;

    
    if (data.njets_ != 2) continue;
    if (data.lid3_ == data.lid2_ && data.lid3_ == data.lid1_) continue;
    if (data.lid3_ == data.lid2_ && fabs(data.lid3_) != fabs(data.lid1_)) continue;
    if (data.lid3_ == data.lid1_ && fabs(data.lid3_) != fabs(data.lid2_)) continue;
    if (data.lid2_ == data.lid1_ && fabs(data.lid2_) != fabs(data.lid3_)) continue;
    
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3;
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
    
    if ( (m[0] < 80 || m[0] > 100) &&  (m[1] < 80 || m[1] > 100) &&  (m[2] < 80 || m[2] > 100)) continue;
    
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])){  pair = pair1; mt =  data.mt3_; tlepton = data.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  data.mt1_; tlepton = data.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  data.mt2_; tlepton = data.lep2_;} 
    pairjet = data.jet1_+ data.jet2_;
        
    if (mt < 40 || data.met_ < 25) continue;
   
 eventsPassData += weight;
  }
  
   cout << eventsPassData << " data events in " << lumi << " fb" << endl; 

  outFileNjets->Write();
  outFileNjets->Close();
  
  
}
