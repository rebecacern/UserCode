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

const double separation = 150000; //15 is the chosen cut
const double metcut = -10; //10 is the proposed
const double mtcut = 850000;
const double separationjj = 600000; //60
const double phicut = 18000000000; // 1.8

void Content() {
  
  bool signal = false;
  TString bgdInputFile    = "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root";
  //TString bgdInputFile   =  "/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/zhww125.root";

  
  //Load datasets
  SmurfTree background;
  background.LoadTree(bgdInputFile,-1);
  background.InitTree(0);
  
  char output[200];
  sprintf(output,"rootfiles/composition_study.root");     
  TFile* outFileNjets = new TFile(output,"recreate");
 
 
  TH1D* bck_cuts = new TH1D("bck_cuts", "cuts", 10, 0, 10);
  
  bck_cuts->Sumw2();
   double eventsPassBck = 0;
   double weight = 1;
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
 
 
    if(signal && background.processId_ != 24) continue;

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
    if (signal & nFake) continue;
    
    
    
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
    
    //At least 2 jets
    if (background.njets_ < 2 ) continue; 
    bck_cuts->Fill(2., weight);
   
    
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
       
    
    if (background.met_ < metcut) continue;
    bck_cuts->Fill(4., weight);
       
     
    
    if (mt > mtcut) continue;
    bck_cuts->Fill(5., weight);
    
    
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    bck_cuts->Fill(6., weight);
  
   //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
    bck_cuts->Fill(7., weight);
  
    eventsPassBck += weight;
    
	     
    bckType[(int)nsel] += weight;
    weiType[(int)nsel] += weight*weight;	
   

  }
  
  cout << endl;
  cout << eventsPassBck << " background events in " << lumi << " fb" << endl; 
  cout << endl;


  
  if (verboseLevel){ 
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
  }
  
  outFileNjets->Write();
  outFileNjets->Close();
  
}

