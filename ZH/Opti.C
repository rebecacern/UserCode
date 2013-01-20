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
const double lumi = 19.467;
const double separation = 15;
const double metcut = -25;
const double mtcut = -30;
const double lowpair = -55;
const double highpair = 105000000000;
const double phicut = 22.5;

void Opti() {
  
  TString bgdInputFile    = "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root";
  TString sigInputFile   =  "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/zhww125.root";
  
  //Load datasets
  SmurfTree background;
  background.LoadTree(bgdInputFile,-1);
  background.InitTree(0);
  
  SmurfTree signal;
  signal.LoadTree(sigInputFile,-1);
  signal.InitTree(0);
 
  
  
  double weight = 1;
  //Signal ZH -> 3l2j1nu
  double cut[20] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
 
  int nSig=signal.tree_->GetEntries();
  for (int i=0; i<nSig; ++i) {
  
   signal.tree_->GetEntry(i);

    weight = 1;
    weight = lumi*signal.scale1fb_*signal.sfWeightPU_*signal.sfWeightEff_*signal.sfWeightTrig_;    
    

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
    
    //2 same flavor, oppposite sign leptons + extra one
    if (signal.lid3_ == signal.lid2_ && signal.lid3_ == signal.lid1_) continue;
    if (signal.lid3_ == signal.lid2_ && fabs(signal.lid3_) != fabs(signal.lid1_)) continue;
    if (signal.lid3_ == signal.lid1_ && fabs(signal.lid3_) != fabs(signal.lid2_)) continue;
    if (signal.lid2_ == signal.lid1_ && fabs(signal.lid2_) != fabs(signal.lid3_)) continue;
    
    //At least 2 jets
    if (signal.njets_ < 2) continue; 
 
    
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
    LorentzVector lm = tlepton + metvector;
   // double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));

    
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
   

    if (signal.met_ < metcut) continue;
    
    if (mt < mtcut) continue;
   
    if (pairjet.M() < lowpair || pairjet.M() > highpair) continue;
    
    if (deltaPhi > phicut) continue;
    
 
    for (int j=0; j < 20; j++){
       if (deltaPhi <= j*0.2) cut[j]+=weight;
    }
    
   
  }
  
   double cutb[20] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
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
    if (nFake){ 
      weight*= background.sfWeightFR_;
    }
    
       
    //2 same flavor, oppposite sign leptons + extra one
    if (background.lid3_ == background.lid2_ && background.lid3_ == background.lid1_) continue;
    if (background.lid3_ == background.lid2_ && fabs(background.lid3_) != fabs(background.lid1_)) continue;
    if (background.lid3_ == background.lid1_ && fabs(background.lid3_) != fabs(background.lid2_)) continue;
    if (background.lid2_ == background.lid1_ && fabs(background.lid2_) != fabs(background.lid3_)) continue;
   
    //At least 2 jets
    if (background.njets_ < 2) continue; 
    
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
     LorentzVector lm = tlepton + metvector;
   // double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));

    
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    
    if (background.met_ < metcut) continue;
    
    if (mt < mtcut) continue;
    
    if (pairjet.M() < lowpair || pairjet.M() > highpair) continue;
     
    if (deltaPhi > phicut) continue;
    
    
    
    for (int j=0; j < 20; j++){
       if (deltaPhi <= j*0.2) cutb[j]+=weight;
    }

  }
  cout << " Cut optimization " << endl;
  cout << endl;
  cout << "Phi <  " << endl;
  for (int i=1; i < 20; i++){
  
  cout << i*0.2 << "\tS: " << cut[i] << "\tB: " << cutb[i] << "\tS/B:" << cut[i]/cutb[i] << "\tS/sqrt(B):" << cut[i]/sqrt(cutb[i]) <<endl; 
  }
  cout << endl;
  
}
