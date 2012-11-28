// rebeca@cern.ch
// Trying to understand the signal
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
const double lumi = 12.1;
void Signal() {
  
  TString bgdInputFile    = "samples/backgroundA_3l.root";
  TString sigInputFile   =  "samples/hww125.root";
  TString dataInputFile   = "samples/data_3l.root";
  
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
  sprintf(output,"signal_study.root");     
  TFile* outFileNjets = new TFile(output,"recreate");
  
  //Histograms 
  TH1F* types = new TH1F("types", "types", 80, -0.5 , 79.5);
  types->Sumw2();

  TH1D* sig_met = new TH1D("sig_met", "MET", 200, 0, 200);
  TH1D* sig_mllz = new TH1D("sig_mllz", "m_{ll}", 200, 0, 200);
  TH1D* sig_mt = new TH1D("sig_mt", "m_t", 200, 0, 200);
  TH1D* sig_ptjet = new TH1D("sig_ptjet", "P_t of leading jet", 200, 0, 200);
  TH1D* sig_mH = new TH1D("sig_mH", "m_H", 200, 0, 400);
  TH1D* sig_mjj = new TH1D("sig_mjj", "m_jj", 200, 0, 400);
  TH1D* sig_dphiljj = new TH1D("sig_dphiljj", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* sig_njets = new TH1D("sig_njets", "Number of jets", 30, -0.5, 29.5);
  TH1D* sig_nfakes = new TH1D("sig_fakes", "Number of fakes", 30, -0.5, 29.5);

  sig_met->Sumw2();
  sig_mllz->Sumw2();
  sig_mt->Sumw2();
  sig_ptjet->Sumw2();
  sig_mH->Sumw2();
  sig_mjj->Sumw2();  
  sig_dphiljj->Sumw2();
  sig_njets->Sumw2();
  sig_nfakes->Sumw2();
  
  TH1D* bck_met = new TH1D("bck_met", "MET", 200, 0, 200);
  TH1D* bck_mllz = new TH1D("bck_mllz", "m_{ll}", 200, 0, 200);
  TH1D* bck_mt = new TH1D("bck_mt", "m_t", 200, 0, 200);
  TH1D* bck_ptjet = new TH1D("bck_ptjet", "P_t of leading jet", 200, 0, 200);
  TH1D* bck_mH = new TH1D("bck_mH", "m_H", 200, 0, 400);
  TH1D* bck_mjj = new TH1D("bck_mjj", "m_jj", 200, 0, 400);
  TH1D* bck_dphiljj = new TH1D("bck_dphiljj", "#Delta#phi_{ljj}", 200, 0, 3.5);
  TH1D* bck_njets = new TH1D("bck_njets", "Number of jets", 30, -0.5, 29.5);
  TH1D* bck_nfakes = new TH1D("bck_fakes", "Number of fakes", 30, -0.5, 29.5);

  bck_met->Sumw2();
  bck_mllz->Sumw2();
  bck_mt->Sumw2();
  bck_ptjet->Sumw2();
  bck_mH->Sumw2();
  bck_mjj->Sumw2();  
  bck_dphiljj->Sumw2();
  bck_njets->Sumw2();
  bck_nfakes->Sumw2();
  
  double weight = 1;
  //Signal ZH -> 3l2j1nu
  int nTotal = 0;
  int nZH = 0;
  double eventsPassSig = 0;

  int nSig=signal.tree_->GetEntries();
  for (int i=0; i<nSig; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nSig);
    signal.tree_->GetEntry(i);

    weight = 1;
    weight = lumi*signal.scale1fb_*signal.sfWeightPU_*signal.sfWeightEff_*signal.sfWeightTrig_;    
    
    nTotal++;
    //if(signal.processId_!=24) continue;
    nZH++;
    
    sig_njets->Fill(signal.njets_, weight);
    if (signal.njets_ < 2) continue; // At least one jet

    //Three real leptons
    bool isRealLepton = false;
    if((TMath::Abs(signal.lid1_) == 11 || TMath::Abs(signal.lid1_)) &&
       (TMath::Abs(signal.lid2_) == 11 || TMath::Abs(signal.lid2_)) &&
       (TMath::Abs(signal.lid3_) == 11 || TMath::Abs(signal.lid3_))) isRealLepton = true; 
    if (!isRealLepton) continue;
    
     
    int nFake = 0;
    if(((signal.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (signal.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (signal.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (signal.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((signal.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (signal.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    sig_nfakes->Fill(nFake, weight);
    if (nFake !=0) continue; //No fakes allowed
    
    
    //2 same flavor, oppposite sign leptons + extra one
    if (signal.lid3_ == signal.lid2_ && signal.lid3_ == signal.lid1_) continue;
    if (signal.lid3_ == signal.lid2_ && fabs(signal.lid3_) != fabs(signal.lid1_)) continue;
    if (signal.lid3_ == signal.lid1_ && fabs(signal.lid3_) != fabs(signal.lid2_)) continue;
    if (signal.lid2_ == signal.lid1_ && fabs(signal.lid2_) != fabs(signal.lid3_)) continue;
    
    
    //Make pairs
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
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    //Select the different things
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  signal.mt3_; tlepton = signal.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  signal.mt1_; tlepton = signal.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  signal.mt2_; tlepton = signal.lep2_;} 
    pairjet = signal.jet1_+ signal.jet2_;
  
    if (pair.M() < 80 || pair.M() > 100) continue; 
    if (signal.met_ < 25) continue;
    if (mt < 30) continue;
  /*
 
    if (pairjet.M() < 65 || pairjet.M() > 95) continue;
   
    */
    eventsPassSig += weight;
    
    
    //Fill histos
    types->Fill(signal.dstype_);
    sig_met->Fill(signal.met_, weight);
    sig_mllz->Fill(pair.M(), weight);
    sig_mt->Fill(mt, weight);
    sig_ptjet->Fill(signal.jet1_.Pt(), weight);  
    LorentzVector metvector(signal.met_*cos(signal.metPhi_), signal.met_*sin(signal.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + signal.jet1_+ signal.jet2_;
    sig_mH->Fill(higgsSystem.M(), weight);
    sig_mjj->Fill(pairjet.M(), weight);
    sig_dphiljj->Fill(DeltaPhi(pairjet.Phi(),tlepton.Phi()), weight);
    

  }
  
  cout << endl;
  cout << eventsPassSig << " signal events in " << lumi << " fb" << endl; 
  cout << nTotal << " events, from which " << nZH << " are ZH" << endl;


  //Backgrounds
  double eventsPassBck = 0;
  int before_type = -1;
  int nBck=background.tree_->GetEntries();
  for (int i=0; i<nBck; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nBck);
    background.tree_->GetEntry(i);

    weight = 1;
    weight = lumi*background.scale1fb_*background.sfWeightPU_*background.sfWeightEff_*background.sfWeightTrig_;    
    
    bck_njets->Fill(background.njets_, weight);
    if (background.njets_ < 2) continue; // At least one jet

    //Three real leptons
    bool isRealLepton = false;
    if((TMath::Abs(background.lid1_) == 11 || TMath::Abs(background.lid1_)) &&
       (TMath::Abs(background.lid2_) == 11 || TMath::Abs(background.lid2_)) &&
       (TMath::Abs(background.lid3_) == 11 || TMath::Abs(background.lid3_))) isRealLepton = true; 
    if (!isRealLepton) continue;
    
   // if (before_type != background.dstype_ )cout << background.dstype_ << endl;
    before_type = background.dstype_;
      
    int nFake = 0;
    if(((background.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (background.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (background.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (background.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (background.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (background.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((background.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (background.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake) weight*= background.sfWeightFR_*0.5;
    bck_nfakes->Fill(nFake, weight);
    //if (nFake !=0) continue;
      
    //2 same flavor, oppposite sign leptons + extra one
    if (background.lid3_ == background.lid2_ && background.lid3_ == background.lid1_) continue;
    if (background.lid3_ == background.lid2_ && fabs(background.lid3_) != fabs(background.lid1_)) continue;
    if (background.lid3_ == background.lid1_ && fabs(background.lid3_) != fabs(background.lid2_)) continue;
    if (background.lid2_ == background.lid1_ && fabs(background.lid2_) != fabs(background.lid3_)) continue;
  
    
    //Make pairs
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
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  background.mt3_; tlepton = background.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  background.mt1_; tlepton = background.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  background.mt2_; tlepton = background.lep2_;} 
    pairjet = background.jet1_+ background.jet2_;
    
     if (pair.M() < 80 || pair.M() > 100) continue;
    if (background.met_ < 25) continue;
    if (mt < 30) continue;
    
/*
   
    if (pairjet.M() < 65 || pairjet.M() > 95) continue;*/
    
    eventsPassBck += weight;
    
    
    //Fill histos
    types->Fill(background.dstype_);
    bck_met->Fill(background.met_, weight);
    bck_mllz->Fill(pair.M(), weight);
    bck_mt->Fill(mt, weight);
    bck_ptjet->Fill(background.jet1_.Pt(), weight);  
    LorentzVector metvector(background.met_*cos(background.metPhi_), background.met_*sin(background.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + background.jet1_+ background.jet2_;
    bck_mH->Fill(higgsSystem.M(), weight);
    bck_mjj->Fill(pairjet.M(), weight);
    bck_dphiljj->Fill(DeltaPhi(pairjet.Phi(),tlepton.Phi()), weight);

  }
  
  cout << endl;
  cout << eventsPassBck << " background events in " << lumi << " fb" << endl; 

  //data
  double eventsPassData = 0;
 
  int nData=data.tree_->GetEntries();
  for (int i=0; i<nBck; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nData);
    data.tree_->GetEntry(i);

    weight = 1;

 
    if (data.njets_ < 2) continue; // At least one jet
      
    int nFake = 0;
    if(((data.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (data.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (data.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (data.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (data.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (data.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((data.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (data.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake) weight*= data.sfWeightFR_;
    if (nFake !=0) continue; 
    
      
    //2 same flavor, oppposite sign leptons + extra one
    if (data.lid3_ == data.lid2_ && data.lid3_ == data.lid1_) continue;
    if (data.lid3_ == data.lid2_ && fabs(data.lid3_) != fabs(data.lid1_)) continue;
    if (data.lid3_ == data.lid1_ && fabs(data.lid3_) != fabs(data.lid2_)) continue;
    if (data.lid2_ == data.lid1_ && fabs(data.lid2_) != fabs(data.lid3_)) continue;
    
    
    //Make pairs
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
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  data.mt3_; tlepton = data.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  data.mt1_; tlepton = data.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  data.mt2_; tlepton = data.lep2_;} 
    pairjet = data.jet1_+ signal.jet2_;
    if (pair.M() < 80 || pair.M() > 100) continue;
    if (data.met_ < 25) continue;
    if (mt < 30) continue;
     /*  
     if (pairjet.M() < 65 || pairjet.M() > 95) continue;
  */  
    eventsPassData += weight;
    
    if (data.njets_ > 1)  pairjet = data.jet1_+ data.jet2_;
    else if (data.njets_ !=0) pairjet = data.jet1_;
    
  

  }
  
  cout << endl;
  cout << eventsPassData << " data events in " << lumi << " fb" << endl; 

  outFileNjets->Write();
  outFileNjets->Close();
  
}
