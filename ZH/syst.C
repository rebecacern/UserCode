// rebeca@cern.ch
// Attempt to optimize the work
#include "inputs.h"


void syst(int cem = 8, int nsel = 1, int mh = 125, int syst = 0, bool isUp = true, int mode = 0){
  
  char plotName[300];
  sprintf(plotName,"test");
  bool isBackground = true;
  bool isData = false;
  
  if (nsel == 1)   			{sprintf(plotName,"ZH");	isBackground = false;}
  else if (nsel == 2)   		{sprintf(plotName,"WZ");}
  else if (nsel == 3)   		{sprintf(plotName,"ZZ");}
  else if (nsel == 4)   		{sprintf(plotName,"VVV");}
  else if (nsel == 5)			{sprintf(plotName,"Wjets");}
  
  char systName[300], direction[300];
  sprintf(systName,"test");
  
  if (syst == 1) 		sprintf(systName,"Stat");
  else if (syst == 2) { 	sprintf(systName,"JES"); 	}
  else if (syst == 3) { 	sprintf(systName,"PU");  	}
  else if (syst == 4) { 	sprintf(systName,"METRes");	}
  else if (syst == 5) { 	sprintf(systName,"LepRes");  	}
  else if (syst == 6) { 	sprintf(systName,"LepEff");  	}
  else if (syst == 7) { 	sprintf(systName,"WZ");  	}
  else if (syst == 8) { 	sprintf(systName,"W");  }
  
  if (isUp) sprintf(direction,"Up");
  else sprintf(direction,"Down");
  
  char myRootFile[300];
  
  if (cem != 7 && cem !=8) cem = 8;
  double lumi = lumi8;
  if (cem == 8){
    if (nsel > 1) sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/backgroundA_3l.root");
    else if (nsel == 1 && mh == 125) sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/zhww125.root");
    else if (nsel == 1) sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/hww%d.root", mh);
    else sprintf(myRootFile,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/mitf-alljets/data_3l.root");
  } else {
    lumi = lumi7;
    if (nsel > 1) sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/backgroundA_3l.root");
    else if (nsel == 1 && mh == 125) sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/zhww125.root");
    else if (nsel == 1) sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/hww%d.root", mh);
    else sprintf(myRootFile,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/data_3l.root");
  }
  
  cout << "[Info:] "<< cem <<  "TeV, " << plotName << ", Higgs mass " << mh << ","  ;
  if (mode == 1) cout << " eee channel" << endl ;
  else if (mode == 2) cout << " eem channel" << endl ; 
  else if (mode == 3) cout << " emm channel" << endl ; 
  else if (mode == 4) cout << " mmm channel" << endl ; 
  else cout << " all final states" << endl;
  
  cout << "[Info:] Systematic calculation of " << systName << endl;
  cout << "[Info:] Systematic "<< direction << endl;
  
  //Load datasets
  SmurfTree sample;
  sample.LoadTree(myRootFile,-1);
  sample.InitTree(0);
  
  // Prepare putput file
  char rootFile[300];
  if (mode == 1) sprintf(rootFile,"%d/zh3l2j_input_eee_%dTeV.root", mh, cem);
  else if (mode == 2) sprintf(rootFile,"%d/zh3l2j_input_eem_%dTeV.root", mh, cem);
  else if (mode == 3) sprintf(rootFile,"%d/zh3l2j_input_emm_%dTeV.root", mh, cem);
  else if (mode == 4) sprintf(rootFile,"%d/zh3l2j_input_mmm_%dTeV.root", mh, cem);
  else sprintf(rootFile,"%d/zh3l2j_input_%dTeV.root", mh, cem);
  
  TFile f_root(rootFile, "UPDATE");
  
  // Prepare histograms
  char title[300];
  if (syst == 1 && cem == 7) sprintf(title,"histo_%s_CMS_%s%sBound7TeV%s",plotName, plotName, systName, direction);
  else if (syst == 1 && cem == 8) sprintf(title,"histo_%s_CMS_%s%sBound8TeV%s",plotName, plotName, systName, direction);
  else sprintf(title,"histo_%s_CMS_%sBound%s",plotName, systName, direction);
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
    
     //Modes, 0 = all, 1 = eee, 2 = eem, 3 = emm, 4 = mmm
    if (mode == 1 && (abs(sample.lid1_)!= 11 || abs(sample.lid2_) != 11 || abs(sample.lid3_) != 11)) continue;
    if (mode == 2 && 
       ((abs(sample.lid1_)!= abs(sample.lid2_) && abs(sample.lid1_) != abs(sample.lid3_) && abs(sample.lid1_) == 11) ||
        (abs(sample.lid2_)!= abs(sample.lid1_) && abs(sample.lid2_) != abs(sample.lid3_) && abs(sample.lid2_) == 11) ||
	(abs(sample.lid3_)!= abs(sample.lid1_) && abs(sample.lid3_) != abs(sample.lid2_) && abs(sample.lid3_) == 11) ||
	(abs(sample.lid1_) == abs(sample.lid2_) && abs(sample.lid1_) == abs(sample.lid3_)))) continue;
    if (mode == 3 && 
       ((abs(sample.lid1_)!= abs(sample.lid2_) && abs(sample.lid1_) != abs(sample.lid3_) && abs(sample.lid1_) == 13) ||
        (abs(sample.lid2_)!= abs(sample.lid1_) && abs(sample.lid2_) != abs(sample.lid3_) && abs(sample.lid2_) == 13) ||
	(abs(sample.lid3_)!= abs(sample.lid1_) && abs(sample.lid3_) != abs(sample.lid2_) && abs(sample.lid3_) == 13) ||
	(abs(sample.lid1_) == abs(sample.lid2_) && abs(sample.lid1_) == abs(sample.lid3_)))) continue;
    if (mode == 4 && (abs(sample.lid1_)!= 13 || abs(sample.lid2_) != 13 || abs(sample.lid3_) != 13)) continue;
   
    
    weight = 1;
    double puweight = sample.sfWeightPU_;
    
    //PU syst
    if (!isData && sample.dstype_ != SmurfTree::data && syst == 3){
      double corrPU = 0;
      if (isUp) corrPU = sample.sfWeightPU_*0.01;
      else corrPU = -sample.sfWeightPU_*0.01;
      puweight = puweight + corrPU;
    }
    
    if (!isData && sample.dstype_ != SmurfTree::data) weight = lumi*sample.scale1fb_*puweight*sample.sfWeightEff_*sample.sfWeightTrig_;    
    
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
    if (nFake !=0) ntype = 61;
    
    
    //Fake Systematic
    double fakecorr = 1;
    if (syst == 8 && ntype == 61){
      //Fake rate systematics
      char fakepath[300];
      sprintf(fakepath,"/data/smurf/data/Run2012_Summer12_SmurfV9_53X/auxiliar/summary_fakes_Moriond2012.root");
      if (cem == 7) sprintf(fakepath,"/data/smurf/data/Run2011_Fall11_SmurfV9_42X/auxiliar/FakeRates_CutBasedMuon_BDTGWithIPInfoElectron.root");
      TFile *fLeptonFRFileSyst = TFile::Open(fakepath);
      TH2D *fhDFRMuSyst = (TH2D*)(fLeptonFRFileSyst->Get("MuonFakeRate_M2_ptThreshold30_PtEta"));
      TH2D *fhDFRElSyst = (TH2D*)(fLeptonFRFileSyst->Get("ElectronFakeRate_V4_ptThreshold50_PtEta"));
      assert(fhDFRMuSyst);
      assert(fhDFRElSyst);
      fhDFRMuSyst->SetDirectory(0);
      fhDFRElSyst->SetDirectory(0);
      
      if(((sample.cuts_ & SmurfTree::Lep1LooseMuV2)  == SmurfTree::Lep1LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection){
        fakecorr =  fhDFRMuSyst->GetBinContent(fhDFRMuSyst->FindBin(sample.lep1_.Pt(),sample.lep1_.Eta()));
      }
      if(((sample.cuts_ & SmurfTree::Lep2LooseMuV2)  == SmurfTree::Lep2LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection){
        fakecorr =  fhDFRMuSyst->GetBinContent(fhDFRMuSyst->FindBin(sample.lep2_.Pt(),sample.lep2_.Eta()));
      }
      if(((sample.cuts_ & SmurfTree::Lep3LooseMuV2)  == SmurfTree::Lep3LooseMuV2)  && (sample.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection){
        fakecorr =  fhDFRMuSyst->GetBinContent(fhDFRMuSyst->FindBin(sample.lep3_.Pt(),sample.lep3_.Eta()));
      }
      if(((sample.cuts_ & SmurfTree::Lep1LooseEleV4) == SmurfTree::Lep1LooseEleV4) && (sample.cuts_ & SmurfTree::Lep1FullSelection) != SmurfTree::Lep1FullSelection){
        fakecorr =  fhDFRElSyst->GetBinContent(fhDFRElSyst->FindBin(sample.lep1_.Pt(),sample.lep1_.Eta()));
      }
      if(((sample.cuts_ & SmurfTree::Lep2LooseEleV4) == SmurfTree::Lep2LooseEleV4) && (sample.cuts_ & SmurfTree::Lep2FullSelection) != SmurfTree::Lep2FullSelection){
        fakecorr =  fhDFRElSyst->GetBinContent(fhDFRElSyst->FindBin(sample.lep2_.Pt(),sample.lep2_.Eta()));
      }
      if(((sample.cuts_ & SmurfTree::Lep3LooseEleV4) == SmurfTree::Lep3LooseEleV4) && (sample.cuts_ & SmurfTree::Lep3FullSelection) != SmurfTree::Lep3FullSelection) {
        fakecorr =  fhDFRElSyst->GetBinContent(fhDFRElSyst->FindBin(sample.lep3_.Pt(),sample.lep3_.Eta()));
      }
      fLeptonFRFileSyst->Close();
      delete fLeptonFRFileSyst;
      
    }
    
    
    if (nFake !=0){ 
      if (syst == 8)   weight*= fakecorr*factor;
      else weight*= sample.sfWeightFR_*factor; 
    }
    
    
    //2 same flavor, oppposite sign leptons + extra one
    if (sample.lid3_ == sample.lid2_ && sample.lid3_ == sample.lid1_) continue;
    if (sample.lid3_ == sample.lid2_ && fabs(sample.lid3_) != fabs(sample.lid1_)) continue;
    if (sample.lid3_ == sample.lid1_ && fabs(sample.lid3_) != fabs(sample.lid2_)) continue;
    if (sample.lid2_ == sample.lid1_ && fabs(sample.lid2_) != fabs(sample.lid3_)) continue;
    
    // At least 2 jets 
    if (sample.njets_ < 2) continue; 
    
    double jescorr = 1;
    
    ///JER -> to be checked!
    if (syst == 2 && isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ) jescorr = 1 + JESerr;
    if (syst == 2 && !isUp && sample.dstype_ != SmurfTree::data && ntype !=61 ) jescorr = 1 - JESerr;
    if (sample.jet1_.Pt()*jescorr < ptleadingcut) continue; 
    
    //MET 
    double metcorr = 1;
    if (syst == 4 && isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ) metcorr = 1 + 0.10;
    if (syst == 4 && !isUp && sample.dstype_ != SmurfTree::data && ntype !=61 ) metcorr = 1 - 0.10;
    
    //Lepton Scale
    double rndMon[12] = {gRandom->Gaus(0.00,0.010),gRandom->Gaus(0.00,0.017),gRandom->Gaus(0.00,0.015),gRandom->Gaus(0.00,0.030),
			 gRandom->Gaus(0.00,0.010),gRandom->Gaus(0.00,0.017),gRandom->Gaus(0.00,0.015),gRandom->Gaus(0.00,0.030),
			 gRandom->Gaus(0.00,0.010),gRandom->Gaus(0.00,0.017),gRandom->Gaus(0.00,0.015),gRandom->Gaus(0.00,0.030)};
    
    
    double corr[3] = {1.0, 1.0, 1.0};
    if (cem == 8){
      if (syst == 5 && isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
	if (TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1./0.99920 + rndMon[0];
	}
	else if(TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1./0.99934 + rndMon[1];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1./0.99807 + rndMon[2];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1./0.99952 + rndMon[3];
	}
	if (TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1./0.99920 + rndMon[4];
	}
	else if(TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1./0.99934 + rndMon[5];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1./0.99807 + rndMon[6];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1./0.99952 + rndMon[7];
	}
	if (TMath::Abs(sample.lid3_) == 13 && TMath::Abs(sample.lep3_.eta()) <  1.479){
	  corr[2] = 1./0.99920 + rndMon[8];
	}
	else if(TMath::Abs(sample.lid3_) == 13 && TMath::Abs(sample.lep3_.eta()) >= 1.479){
	  corr[2] = 1./0.99934 + rndMon[9];
	}
	else if(TMath::Abs(sample.lid3_) == 11 && TMath::Abs(sample.lep3_.eta()) <  1.479){
	  corr[2] = 1./0.99807 + rndMon[10];
	}
	else if(TMath::Abs(sample.lid3_) == 11 && TMath::Abs(sample.lep3_.eta()) >= 1.479){
	  corr[2] = 1./0.99952 + rndMon[11];
	}    
      
      } 
    
      if (syst == 5 && !isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
	if (TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 0.99920 - rndMon[0];
	}
	else if(TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 0.99934 - rndMon[1];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 0.99807 - rndMon[2];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 0.99952 - rndMon[3];
	}
	if (TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 0.99920 - rndMon[4];
	}
	else if(TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 0.99934 - rndMon[5];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 0.99807 - rndMon[6];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 0.99952 - rndMon[7];
	}
	if (TMath::Abs(sample.lid3_) == 13 && TMath::Abs(sample.lep3_.eta()) <  1.479){
	  corr[2] = 0.99920 - rndMon[8];
	}
	else if(TMath::Abs(sample.lid3_) == 13 && TMath::Abs(sample.lep3_.eta()) >= 1.479){
	  corr[2] = 0.99934 - rndMon[9];
	}
	else if(TMath::Abs(sample.lid3_) == 11 && TMath::Abs(sample.lep3_.eta()) <  1.479){
	  corr[2] = 0.99807 - rndMon[10];
	}
	else if(TMath::Abs(sample.lid3_) == 11 && TMath::Abs(sample.lep3_.eta()) >= 1.479){
	  corr[2] = 0.99952 - rndMon[10];
	} 
      }
    } else {
      if (syst == 5 && isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
    
	if	 (TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1.01 + rndMon[0];
	}
	else if(TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1.01 + rndMon[1];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1.01 + rndMon[2];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1.06 + rndMon[3];
	}
	if	 (TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1.01 + rndMon[4];
	}
	else if(TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1.01 + rndMon[5];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1.01 + rndMon[6];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1.06 + rndMon[7];
	}
    
    
     
      } 
    
      if (syst == 5 && !isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
    
	if	 (TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1.01 + rndMon[0];
	}
	else if(TMath::Abs(sample.lid1_) == 13 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1.01 + rndMon[1];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) <  1.479){
	  corr[0] = 1.01 + rndMon[2];
	}
	else if(TMath::Abs(sample.lid1_) == 11 && TMath::Abs(sample.lep1_.eta()) >= 1.479){
	  corr[0] = 1.06 + rndMon[3];
	}
	if	 (TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1.01 + rndMon[4];
	}
	else if(TMath::Abs(sample.lid2_) == 13 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1.01 + rndMon[5];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) <  1.479){
	  corr[1] = 1.01 + rndMon[6];
	}
	else if(TMath::Abs(sample.lid2_) == 11 && TMath::Abs(sample.lep2_.eta()) >= 1.479){
	  corr[1] = 1.06 + rndMon[7];
	}
    
   
      }
    }
    
    
    // Eff fixed in 1.5 for muons and 2.0 for electrons (tbc)
    if (syst == 6 && isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
      if (TMath::Abs(sample.lid1_) == 13){
	corr[0] = 1 + 0.015;
      } 
      if (TMath::Abs(sample.lid2_) == 13){
	corr[1] = 1 + 0.015;
      } 
      if (TMath::Abs(sample.lid3_) == 13){
	corr[2] = 1 + 0.015;
      } 
      if (TMath::Abs(sample.lid1_) == 11){
        corr[0] = 1 + 0.02;
      }
      if (TMath::Abs(sample.lid2_) == 11){
        corr[1] = 1 + 0.02;
      }
      if (TMath::Abs(sample.lid3_) == 11){
        corr[2] = 1 + 0.02;
      }
    }
    if (syst == 6 && !isUp  && sample.dstype_ != SmurfTree::data && ntype !=61 ){
      if (TMath::Abs(sample.lid1_) == 13){
	corr[0] = 1 - 0.015;
      } 
      if (TMath::Abs(sample.lid2_) == 13){
	corr[1] = 1 - 0.015;
      } 
      if (TMath::Abs(sample.lid3_) == 13){
	corr[2] = 1 - 0.015;
      } 
      if (TMath::Abs(sample.lid1_) == 11){
        corr[0] = 1 - 0.02;
      }
      if (TMath::Abs(sample.lid2_) == 11){
        corr[1] = 1 - 0.02;
      }
      if (TMath::Abs(sample.lid3_) == 11){
        corr[2] = 1 - 0.02;
      }
    }
 
    //check migrations 
    if (sample.lep1_.Pt()*corr[0] < 10) continue;
    if (sample.lep2_.Pt()*corr[1] < 10) continue;
    if (sample.lep2_.Pt()*corr[2] < 10) continue;
    if (sample.lep1_.Pt()*corr[0] < 20 && sample.lep2_.Pt()*corr[1] < 20 && sample.lep3_.Pt()*corr[2] < 20) continue;
    
    //Make z-compatible pairs
    double m[3] = {0, 0, 0};
    LorentzVector pair1, pair2, pair3;
    if (fabs(sample.lid1_) == fabs(sample.lid2_) && sample.lq1_*sample.lq2_ < 0){
      pair1 = sample.lep1_*corr[0] + sample.lep2_*corr[1] ;
      m[0] = pair1.M();
    }
    if (fabs(sample.lid2_) == fabs(sample.lid3_) && sample.lq2_*sample.lq3_ < 0){
      pair2 = sample.lep2_*corr[1] + sample.lep3_*corr[2] ;
      m[1] = pair2.M();
    }
    if (fabs(sample.lid1_) == fabs(sample.lid3_) && sample.lq1_*sample.lq3_ < 0){
      pair3 = sample.lep1_*corr[0] + sample.lep3_*corr[2];
      m[2] = pair3.M();
    }
    
    //Get the closest to the Z mass
    double min = TMath::Min(TMath::Min(fabs(mz -m[0]), fabs(mz-m[1])), TMath::Min(fabs(mz -m[0]), fabs(mz-m[2])));
    
    //Select the different things: Z pair, extra lepton, Higgs system
    LorentzVector pair, tlepton, pairjet;
    double mt = 0;
    // double dR = 0; //dR = fabs(ROOT::Math::VectorUtil::DeltaR(sample.lep1_ ,sample.lep2_)) etc
    if (min == fabs(mz - m[0])) {  pair = pair1; mt =  sample.mt3_*sqrt(corr[2])*sqrt(metcorr); tlepton = sample.lep3_*corr[2];} 
    else if (min == fabs(mz - m[1])){  pair = pair2;  mt =  sample.mt1_*sqrt(corr[0])*sqrt(metcorr); tlepton = sample.lep1_*corr[0];} 
    else if (min == fabs(mz - m[2])){  pair = pair3;  mt =  sample.mt2_*sqrt(corr[1])*sqrt(metcorr); tlepton = sample.lep2_*corr[1];} 
    
    pairjet = sample.jet1_*jescorr+ sample.jet2_*jescorr;
    
    LorentzVector metvector(sample.met_*metcorr*cos(sample.metPhi_), sample.met_*metcorr*sin(sample.metPhi_), 0, 0);
    LorentzVector higgsSystem = tlepton + metvector + sample.jet1_+ sample.jet2_;
    LorentzVector lm = tlepton + metvector;
    
    
    double hp[5];
    hp[0] = tlepton.Px() + sample.jet1_.Px()*jescorr+ sample.jet2_.Px()*jescorr+ metvector.Px()*metcorr;
    hp[1] = tlepton.Py() + sample.jet1_.Py()*jescorr+ sample.jet2_.Py()*jescorr+ metvector.Py()*metcorr;
    /// calculations not needed
    hp[4] = tlepton.Pt() + sample.jet1_.Pt()*jescorr+ sample.jet2_.Pt()*jescorr+ sample.met_*metcorr;
    
    double recomth = hp[4]*hp[4]-hp[0]*hp[0]-hp[1]*hp[1]; if(recomth > 0) recomth = sqrt(recomth); else recomth  = 0.0;
    
    
    //Kinematic cuts
    if (pair.M() < (mz - separation)|| pair.M() > (mz + separation)) continue; 
    if (sample.met_*metcorr < metcut) continue;
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
    
    
    
    //histo->Fill(recomth, weight);
    histo->Fill(higgsSystem.M(), weight);
    eventsPass+= weight;
    
    
  }    
  
  cout << "[Info:] (" << plotName << ") " <<  eventsPass << " events pass " << endl;
  
  if (syst == 1){
    for (int i = 1; i < nbins+1; i ++){
      double content = 0;
      if (isUp) content = histo->GetBinContent(i) + histo->GetBinError(i);
      else content = histo->GetBinContent(i) - histo->GetBinError(i);
      if (content < 0) content = histo->GetBinContent(i);
      histo->SetBinContent(i,content);
    }
  }
  
  if (syst == 7 && !isUp){
    // read the new file
    char extraRootFile[300];
    sprintf(extraRootFile,"WZ%dTeV.root", cem);
    TFile *_file0 = TFile::Open(extraRootFile);
    TH1F* h;
    h = (TH1F*) _file0->Get("histogram");
    
    for (int i = 1; i < nbins+1; i ++){
      double content = 0;
      content = 2*h->GetBinContent(i) - histo->GetBinContent(i);
      if (content < 0) content = h->GetBinContent(i);
      histo->SetBinContent(i,content);
    }
    delete h;
    _file0->Close();
    
  }
  
  
  if (syst == 8){
    // read the new file
    char extraRootFile[300];
    sprintf(extraRootFile,"fakesAux%dTeV.root", cem);
    TFile *_file0 = TFile::Open(extraRootFile);
    TH1F* h;
    h = (TH1F*) _file0->Get("histogram");
    
    if(!isUp){
      for (int i = 1; i < nbins+1; i ++){
        double content = 0;
	content = 2*h->GetBinContent(i) - histo->GetBinContent(i);
	if (content < 0) content = h->GetBinContent(i);
	histo->SetBinContent(i,content);
      }
    }
    double scaleHisto = h->Integral(1,nbins)/histo->Integral(1,nbins);
    histo->Scale(scaleHisto);
      

    delete h;
    _file0->Close();
    
  }
  
  f_root.Write();
  f_root.Close();
  
}
