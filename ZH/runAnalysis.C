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
const double lumi = 17.6;
void runAnalysis() {
  
  TString bgdInputFile    = "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root";
  TString sigInputFile   =  "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/hww125.root";
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
  sprintf(output,"rootfiles/analysis_%dfb.root", (int) lumi);     
  TFile* outFileNjets = new TFile(output,"recreate");
  
  //Histograms 
  TH1D* sig_cuts = new TH1D("sig_cuts", "cuts", 10, 0, 10);
  sig_cuts->Sumw2();
  
  TH1D* bck_cuts = new TH1D("bck_cuts", "cuts", 10, 0, 10);
  bck_cuts->Sumw2();
  
  TH1D* data_cuts = new TH1D("data_cuts", "cuts", 10, 0, 10);
  data_cuts->Sumw2();
  
  double weight = 1;
  //Signal ZH -> 3l2j1nu

  int nSig=signal.tree_->GetEntries();
  for (int i=0; i<nSig; ++i) {
    
    signal.tree_->GetEntry(i);

    weight = 1;
    weight = lumi*signal.scale1fb_*signal.sfWeightPU_*signal.sfWeightEff_*signal.sfWeightTrig_;    
    
    //if(signal.processId_!=24) continue;
    
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
    
    //2 same flavor, oppposite sign leptons + extra one
    if (signal.lid3_ == signal.lid2_ && signal.lid3_ == signal.lid1_) continue;
    if (signal.lid3_ == signal.lid2_ && fabs(signal.lid3_) != fabs(signal.lid1_)) continue;
    if (signal.lid3_ == signal.lid1_ && fabs(signal.lid3_) != fabs(signal.lid2_)) continue;
    if (signal.lid2_ == signal.lid1_ && fabs(signal.lid2_) != fabs(signal.lid3_)) continue;
    sig_cuts->Fill(1., weight);
    
    //At least 2 jets
    if (signal.njets_ < 2) continue; 
    sig_cuts->Fill(2., weight);

    
    //Make z-compatible pairs
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
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  signal.mt3_; tlepton = signal.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  signal.mt1_; tlepton = signal.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  signal.mt2_; tlepton = signal.lep2_;} 
    pairjet = signal.jet1_+ signal.jet2_;
    LorentzVector metvector(signal.met_*cos(signal.metPhi_), signal.met_*sin(signal.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + signal.jet1_+ signal.jet2_;
    
    //Kinematic cuts
    if (pair.M() < 80 || pair.M() > 100) continue; 
    sig_cuts->Fill(3., weight);
    
    if (signal.met_ < 25) continue;
    sig_cuts->Fill(4., weight);
    
    if (mt < 30) continue;
    sig_cuts->Fill(5., weight);
 
    if (pairjet.M() < 65 || pairjet.M() > 95) continue;
    sig_cuts->Fill(6., weight);

  }
  
  //Backgrounds
  double bckType[60] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  double weiType[60] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
			0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  
  TString bckName[60] = {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null",
                         "null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"};
  
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
  
  
  int nBck=background.tree_->GetEntries();
  for (int i=0; i<nBck; ++i) {
    
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
    if (nFake) weight*= background.sfWeightFR_*0.5;
    bck_cuts->Fill(0., weight);
    
    //2 same flavor, oppposite sign leptons + extra one
    if (background.lid3_ == background.lid2_ && background.lid3_ == background.lid1_) continue;
    if (background.lid3_ == background.lid2_ && fabs(background.lid3_) != fabs(background.lid1_)) continue;
    if (background.lid3_ == background.lid1_ && fabs(background.lid3_) != fabs(background.lid2_)) continue;
    if (background.lid2_ == background.lid1_ && fabs(background.lid2_) != fabs(background.lid3_)) continue;
   
    bck_cuts->Fill(1., weight);
    
    //At least 2 jets
    if (background.njets_ < 2) continue; 
    bck_cuts->Fill(2., weight);

    
    //Make z-compatible pairs
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
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  background.mt3_; tlepton = background.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  background.mt1_; tlepton = background.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  background.mt2_; tlepton = background.lep2_;} 
    pairjet = background.jet1_+ background.jet2_;
    LorentzVector metvector(background.met_*cos(background.metPhi_), background.met_*sin(background.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + background.jet1_+ background.jet2_;
    
    //Kinematic cuts
    if (pair.M() < 80 || pair.M() > 100) continue; 
    bck_cuts->Fill(3., weight);
    
    if (background.met_ < 25) continue;
    bck_cuts->Fill(4., weight);
    
    if (mt < 30) continue;
    bck_cuts->Fill(5., weight);
 
    if (pairjet.M() < 65 || pairjet.M() > 95) continue;
    bck_cuts->Fill(6., weight);
        
    bckType[(int)nsel] += weight;
    weiType[(int)nsel] += weight*weight;

  }
  
  //data
  int nData=data.tree_->GetEntries();
  for (int i=0; i<nData; ++i) {
      
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
    data_cuts->Fill(1., weight);
    
    //At least 2 jets
    if (data.njets_ < 2) continue; 
    data_cuts->Fill(2., weight);

    
    //Make z-compatible pairs
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
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  data.mt3_; tlepton = data.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  data.mt1_; tlepton = data.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  data.mt2_; tlepton = data.lep2_;} 
    pairjet = data.jet1_+ data.jet2_;

    //Kinematic cuts
    if (pair.M() < 80 || pair.M() > 100) continue; 
    data_cuts->Fill(3., weight);
    
    if (data.met_ < 25) continue;
    data_cuts->Fill(4., weight);
    
    if (mt < 30) continue;
    data_cuts->Fill(5., weight);
 
    if (pairjet.M() < 65 || pairjet.M() > 95) continue;
    data_cuts->Fill(6., weight);

  }
  
  
  if (verboseLevel){ 
    cout << "------------------------------------------" << endl;
    cout << "[Signal HWW 125 GeV:] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 2; i < 9; i++){
      if (i == 2) cout << " 3 lep:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
      if (i == 4) cout << " mll:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
      if (i == 5) cout << " met:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
      if (i == 6) cout << " mt:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
      if (i == 7) cout << " mjj:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i)  << endl;
    }
    cout << "------------------------------------------" << endl;  
    cout << "[Backgrounds (All mixed):] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 2; i < 9; i++){
      if (i == 2) cout << " 3 lep:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 4) cout << " mll:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 5) cout << " met:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 6) cout << " mt:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
      if (i == 7) cout << " mjj:\t\t" <<  bck_cuts->GetBinContent(i) << " +/-  " <<  bck_cuts->GetBinError(i)  << endl;
    }
    cout << endl;
    cout << "[Breakdown:] " << endl;
    for(int i=0; i<60; i++){
      if(bckType[i] != 0 )
	cout << i <<"\t" << bckName[i] << ":\t\t" << bckType[i] << "+-" << sqrt(weiType[i]) <<endl;
    }
    cout << "------------------------------------------" << endl; 
    cout << "[Data:] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 2; i < 9; i++){
      if (i == 2) cout << " 3 lep:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 4) cout << " mll:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 5) cout << " met:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 6) cout << " mt:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
      if (i == 7) cout << " mjj:\t\t" <<  data_cuts->GetBinContent(i) << " +/-  " <<  data_cuts->GetBinError(i)  << endl;
    }
    cout << "------------------------------------------" << endl; 
  }
  
  outFileNjets->Write();
  outFileNjets->Close();
  
}
