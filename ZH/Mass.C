// rebeca@cern.ch
// Trying to understand the signal
// November 2012

#include "../Smurf/Core/SmurfTree.h"
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
#include "inputs.h"


void Mass(int cem = 8,  int mh = 125) {
  
  
  char myRootFile[300];
  if (cem != 7 && cem !=8) cem = 8;
  double lumi = lumi8;
  if (cem == 8){
    if (mh == 125) sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/zhww125.root");
    else sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/hww%d.root", mh);
  } else {
    lumi = lumi7;
    if (mh == 125) sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/zhww125.root");
    else sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/hww%d.root", mh);
  }
  
  cout << "[Info:] "<< cem <<  "TeV, Higgs mass " << mh << ","  ;
  								    
  SmurfTree signal;
  signal.LoadTree(myRootFile,-1);
  signal.InitTree(0);
  
  
  //Prepare output
  char output[200];
  sprintf(output,"rootfiles/signal_%d.root", cem);     
  TFile* outFileNjets = new TFile(output,"update");
  
  //Histograms 
   char title[200];
   sprintf(title,"cuts_%d", mh);     
  TH1D* sig_cuts = new TH1D(title, "cuts", 10, 0, 10);
   sprintf(title,"cuts_zh_%d", mh);   
  TH1D* sig_cuts_zh = new TH1D(title, "cuts", 10, 0, 10);
  
 
  sig_cuts->Sumw2();
  sig_cuts_zh->Sumw2();
  
 
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
    double m[3] = {-1, -1, -1};
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

if ( (m[0] !=-1 && m[0] < 12) || (m[1] !=-1 && m[1] < 12) || (m[2] =!-1 && m[2] < 12)) continue;
			
   LorentzVector trelep = signal.lep1_ + signal.lep2_ + signal.lep3_;
   if (fabs(trelep.M() - mz) < 10) continue; 

    
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
    
    
    eventstouse++;
    
    eventsPassSig += weight;
    if(signal.processId_ == 24) eventsZH+= weight;
    
   
  } 
  if (verboseLevel){ 
    cout << "------------------------------------------" << endl;
    cout << "[Signal HWW" << mh << " 125 GeV:] " << endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " 3 lep:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 2) cout << " OSSF:\t\t"  <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 3) cout << " 2 jet:\t\t" <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 4) cout << " mll:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
     // if (i == 5) cout << " met:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 6) cout << " mt:\t\t"    <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 7) cout << " mjj:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;  if (i == 7) cout << " mjj:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
      if (i == 8) cout << " phi:\t\t"   <<  sig_cuts->GetBinContent(i) << " +/-  " <<  sig_cuts->GetBinError(i) << " (" << sig_cuts_zh->GetBinContent(i)<< " +/-  " <<  sig_cuts->GetBinError(i) << ")" << endl;
  
    }
    cout << "(from which " << eventsZH << " are genuine ZH events)" << endl;
    cout << "------------------------------------------" << endl; 
  }
  outFileNjets->Write();
  outFileNjets->Close();
  
}

