// rebeca@cern.ch
// Attempt to optimize the work
#include "inputs.h"


void doFakes(){
  
  char plotName[300];
  sprintf(plotName,"test");
  
  sprintf(plotName,"Wjets");
  bool isBackground = true;
  bool isData = false;
  int nsel = 5;
  
  char myRootFile[300];
  
  sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root");
  
   //Load datasets
  SmurfTree sample;
  sample.LoadTree(myRootFile,-1);
  sample.InitTree(0);
  
  // Prepare putput file
  char rootFile[300];
  sprintf(rootFile,"fakesAux.root");
  
  TFile f_root(rootFile, "RECREATE");
  
  // Prepare histograms
  char title[300];
  
  sprintf(title,"histogram");
  TH1F* histo = new TH1F( title, " ", nbins, nbinlow, nbinhigh);
  histo->Sumw2();
  
 
  //Prepare useful things
  double weight = 1;
  double eventsPass = 0;
  
  int nSample=sample.tree_->GetEntries();
   for (int i=0; i<nSample; ++i) {
    
    if (i%100000 == 0 && verboseLevel > 0)
      printf("--- reading event %5d of %5d\n",i,nSample);
    sample.tree_->GetEntry(i);
    
    weight = 1;
    if (!isData && sample.dstype_ != SmurfTree::data) weight = lumi*sample.scale1fb_*sample.sfWeightPU_*sample.sfWeightEff_*sample.sfWeightTrig_;    
   
   //Three real leptons MC level
    if (!isData){
      bool isRealLepton = false;
      if((TMath::Abs(sample.lep1McId_) == 11 || TMath::Abs(sample.lep1McId_) == 13) &&
         (TMath::Abs(sample.lep2McId_) == 11 || TMath::Abs(sample.lep2McId_) == 13) &&
         (TMath::Abs(sample.lep3McId_) == 11 || TMath::Abs(sample.lep3McId_) == 13)) isRealLepton = true; 
      if (!isRealLepton && !isBackground) continue; //signal
      if (!isRealLepton && sample.dstype_ != SmurfTree::data) continue; //background
    }
    
    int ntype = sample.dstype_;
    
     //Check for fakes
    int nFake = 0;
    if(((sample.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((sample.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((sample.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if(((sample.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (sample.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection) nFake++;
    if(((sample.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (sample.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection) nFake++;
    if(((sample.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (sample.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) nFake++;
    if (nFake !=0 && !isBackground) continue; 
    if (nFake !=0){ 
      ntype = 61;
      weight*= sample.sfWeightFR_*factor;
      //if (sample.dstype_ != SmurfTree::data) weight *=-1;
    }
    
    
    //2 same flavor, oppposite sign leptons + extra one
    if (sample.lid3_ == sample.lid2_ && sample.lid3_ == sample.lid1_) continue;
    if (sample.lid3_ == sample.lid2_ && fabs(sample.lid3_) != fabs(sample.lid1_)) continue;
    if (sample.lid3_ == sample.lid1_ && fabs(sample.lid3_) != fabs(sample.lid2_)) continue;
    if (sample.lid2_ == sample.lid1_ && fabs(sample.lid2_) != fabs(sample.lid3_)) continue;
    
    // At least 2 jets 
    if (sample.njets_ < 2) continue; 
    
     //Make z-compatible pairs
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3;
    if (fabs(sample.lid1_) == fabs(sample.lid2_) && sample.lq1_*sample.lq2_ < 0){
      pair1 = sample.lep1_ + sample.lep2_ ;
      m[0] = pair1.M();
    }
    if (fabs(sample.lid2_) == fabs(sample.lid3_) && sample.lq2_*sample.lq3_ < 0){
      pair2 = sample.lep2_ + sample.lep3_ ;
      m[1] = pair2.M();
    }
    if (fabs(sample.lid1_) == fabs(sample.lid3_) && sample.lq1_*sample.lq3_ < 0){
      pair3 = sample.lep1_ + sample.lep3_ ;
      m[2] = pair3.M();
    }
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
   
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
   // double dR = 0; //dR = fabs(ROOT::Math::VectorUtil::DeltaR(sample.lep1_ ,sample.lep2_)) etc
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  sample.mt3_; tlepton = sample.lep3_;} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  sample.mt1_; tlepton = sample.lep1_;} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  sample.mt2_; tlepton = sample.lep2_;} 
    pairjet = sample.jet1_+ sample.jet2_;
    LorentzVector metvector(sample.met_*cos(sample.metPhi_), sample.met_*sin(sample.metPhi_), 0, 0);
   // LorentzVector higgsSystem = tlepton + metvector + sample.jet1_+ sample.jet2_;
    LorentzVector lm = tlepton + metvector;
   
      
    double hp[5];
    hp[0] = tlepton.Px() + sample.jet1_.Px()+ sample.jet2_.Px()+ metvector.Px();
    hp[1] = tlepton.Py() + sample.jet1_.Py()+ sample.jet2_.Py()+ metvector.Py();
    hp[2] = tlepton.Pz() + sample.jet1_.Pz()+ sample.jet2_.Pz()+ metvector.Pz();
    
    //Calculate p of the neutrino using Maria's code
    double metp = 0;
   // double otherSol = 0;
    double alpha=(mw*mw-mmu*mmu)/2/tlepton.P()+(tlepton.Px()*sample.met_*cos(sample.metPhi_)+tlepton.Py()*sample.met_*sin(sample.metPhi_))/tlepton.P();
    double A=tlepton.Pz()*tlepton.Pz()/tlepton.P()/tlepton.P()-1;
    double B=2*alpha*tlepton.Pz()/tlepton.P();
    double C=alpha*alpha-(sample.met_*cos(sample.metPhi_)*sample.met_*cos(sample.metPhi_) + sample.met_*sin(sample.metPhi_)*sample.met_*sin(sample.metPhi_));
   // bool isComplex = false;
    double tmproot = B*B - 4.0*A*C;
      if (tmproot<0) { 
        //isComplex= true;
        metp = - B/(2*A); 
	//otherSol = metp;
      } else {
       // isComplex = false;
	double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
	double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) {
	  metp = tmpsol1; 
	  //otherSol = tmpsol2; 
	} else { 
	  metp = tmpsol2; 
	  //otherSol = tmpsol1; 
	}
     }
   
    
   // hp[3] = tlepton.P() + sample.jet1_.P()+ sample.jet2_.P()+ metvector.P(); //crappy solution
    hp[3] = tlepton.P() + sample.jet1_.P()+ sample.jet2_.P()+ metp;
    hp[4] = tlepton.Pt() + sample.jet1_.Pt()+ sample.jet2_.Pt()+ sample.met_;
    
    double recomh  = hp[3]*hp[3]-hp[0]*hp[0]-hp[1]*hp[1]-hp[2]*hp[2]; if(recomh  > 0) recomh  = sqrt(recomh);else recomh   = 0.0;
    double recomth = hp[4]*hp[4]-hp[0]*hp[0]-hp[1]*hp[1]; if(recomth > 0) recomth = sqrt(recomth); else recomth  = 0.0;
    
   
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    if (sample.met_ < metcut) continue;
    if (mt > mtcut) continue;
    if (pairjet.M() < (mw - separationjj) || pairjet.M() > (mw + separationjj)) continue;
    
   //double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),tlepton.Phi()));
    double deltaPhi = fabs(DeltaPhi(pairjet.Phi(),lm.Phi()));
    if (deltaPhi > phicut) continue;
   
    
    if (nsel == 2 && ntype != 49) continue; //WZ
    if (nsel == 3 && ntype != 50) continue; //ZZ
    if (nsel == 4 && ntype != 59) continue; //VVV
    if (nsel == 5 && ntype != 61) continue; //fakes
    if (nsel == 0 && ntype != 0)  continue; //data
    
    histo->Fill(recomth, weight);
    eventsPass+= weight;
     
  
  }    
  
   cout << "[Info:] (" << plotName << ") " <<  eventsPass << " events pass " << endl;
  
  
    f_root.Write();
    f_root.Close();
  
}
