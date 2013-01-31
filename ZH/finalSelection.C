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
const double metcut = -10; 
const double mtcut = 85;
const double separationjj = 60; //60
const double phicut = 1.8; // 1.8

void finalSelection() {
  
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
  sprintf(output,"rootfiles/finalFile.root");     
  TFile* outFileNjets = new TFile(output,"recreate");
  
  //Histograms 
  
  double weight = 1;
  //Signal ZH -> 3l2j1nu
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
   
    if (signal.met_ < metcut) continue;
   
    if (mt > mtcut) continue;
   
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    
    //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
   
    
   
  }
  
  cout << endl;
  cout << "signal analyzed" << endl;
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
    
    if (background.met_ < metcut) continue;
  
    if (mt > mtcut) continue;
  
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
   
   //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
   
   

  }
  
  cout << endl;
  cout << " backgrounds done" << endl; 
  cout << endl;
  
  //data
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
 
    
    //2 same flavor, oppposite sign leptons + extra one
    if (data.lid3_ == data.lid2_ && data.lid3_ == data.lid1_) continue;
    if (data.lid3_ == data.lid2_ && fabs(data.lid3_) != fabs(data.lid1_)) continue;
    if (data.lid3_ == data.lid1_ && fabs(data.lid3_) != fabs(data.lid2_)) continue;
    if (data.lid2_ == data.lid1_ && fabs(data.lid2_) != fabs(data.lid3_)) continue;
    
    //At least 2 jets
    if (data.njets_ < 2) continue; 
  
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
    
    if (data.met_ < metcut) continue;
   
    if (mt > mtcut) continue;
    
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
      
    //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
    

  }
  
  cout << endl;
  cout << " data done" << endl; 
  cout << endl;
 
  outFileNjets->Write();
  outFileNjets->Close();
  
}

