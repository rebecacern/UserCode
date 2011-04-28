// rebeca@cern.ch
#include "TLorentzVector.h"
#include "TVector3.h"
#include "inputs.h"


void chain(int nsel, int mode = 0, bool silent = false){
  
  
  // samples used
  double x_sec = 0.;
  char plotName[300];
  sprintf(plotName,"test");
  
  if (nsel == 0)                	{sprintf(plotName,"tt");}
  else if (nsel == 1)   		{sprintf(plotName,"tw");}
  else if (nsel == 2)   		{sprintf(plotName,"st");}
  else if (nsel == 3)   		{sprintf(plotName,"wjets");}
  else if (nsel == 4)   		{sprintf(plotName,"zjets");}
  else if (nsel == 5)   		{sprintf(plotName,"vqq");}
  else if (nsel == 6)   		{sprintf(plotName,"qcd_mu");}
  else if (nsel == 7)   		{sprintf(plotName,"all");}
  else if (nsel == 8)   		{sprintf(plotName,"non_tt");}
  
  if (mode != 0 &&  mode !=1 && mode !=2) mode = 0;
  if (!silent){
    if      (mode == 0) 	cout << " Electron-Muon Mixed channel " << endl;
    else if (mode == 1) 	cout << " Di-Muon channel " << endl;
    else if (mode == 2) 	cout << " Di-Electron channel " << endl;
    cout << "*************************************************" << endl;
  }
  
  char myRootFile[300];
  sprintf(myRootFile,"outputs/out_%d_%s.root", mode, plotName);
  TFile *input = TFile::Open(myRootFile);
  
  // tree variables
  /////
  double xlWeight; 
  
  double metPt;
  double metPx;
  double metPy;
  
  std::vector<double> *ptLepton;
  std::vector<double> *pxLepton;
  std::vector<double> *pyLepton;
  std::vector<double> *pzLepton;
  std::vector<double> *eLepton;
  std::vector<double> *qLepton;
  
  std::vector<double> *ptJet;
  std::vector<double> *pxJet;
  std::vector<double> *pyJet;
  std::vector<double> *pzJet;
  std::vector<double> *eJet;
  std::vector<double> *qJet;
  std::vector<double> *btJet;
  
  TTree*Tree = (TTree*) gROOT->FindObject("myTree");
  
  Tree->SetBranchAddress("xlWeight", &xlWeight);
  
  Tree->SetBranchAddress("metPt", &metPt);
  Tree->SetBranchAddress("metPx", &metPx);
  Tree->SetBranchAddress("metPy", &metPy);
  
  Tree->SetBranchAddress("ptLepton",&ptLepton);
  Tree->SetBranchAddress("pxLepton",&pxLepton);
  Tree->SetBranchAddress("pyLepton",&pyLepton);
  Tree->SetBranchAddress("pzLepton",&pzLepton);
  Tree->SetBranchAddress("eLepton",&eLepton);
  Tree->SetBranchAddress("qLepton",&qLepton);
  
  Tree->SetBranchAddress("ptJet",&ptJet);
  Tree->SetBranchAddress("pxJet",&pxJet);
  Tree->SetBranchAddress("pyJet",&pyJet);
  Tree->SetBranchAddress("pzJet",&pzJet);
  Tree->SetBranchAddress("eJet",&eJet);
  Tree->SetBranchAddress("qJet",&qJet);
  Tree->SetBranchAddress("btJet",&btJet);
  
  int nEvents = Tree->GetEntries();
  if(!silent){
    cout << endl;
    cout << "******************************************" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Starting the analysis: " << plotName <<  endl;
    cout << "------------------------------------------" << endl;
    cout << "Number of Raw events: " <<  nEvents << endl;
    cout << "------------------------------------------" << endl;
    cout << "******************************************" << endl;
  }
  
  char newRootFile[300];
  sprintf(newRootFile,"results/an_%dpb_%d.root", luminosity, mode);
  TFile f_var(newRootFile, "UPDATE");
  
  
  //////////
  
  char title[300];
  sprintf(title,"cuts_%s",plotName);
  TH1F* histo = new TH1F( title, " ", 10,  0, 10 );
  histo->Sumw2();
  
  //////////
  
  for(int event = 0; event<nEvents; event++){
    
    Tree->GetEntry(event);
    
    if (luminosity != 1000) xlWeight*=(luminosity/1000);
    
    if(ptLepton->size() != 2) cout << "Something is wrong, your Tree is not correctly filled" << endl;
    else {
      
      histo->Fill(1, xlWeight);
      
      TLorentzVector lepton0(pxLepton->at(0),pyLepton->at(0), pzLepton->at(0), eLepton->at(0));
      TLorentzVector lepton1(pxLepton->at(1),pyLepton->at(1), pzLepton->at(1), eLepton->at(1)); 
      TLorentzVector pair = lepton0+lepton1;
      
      bool invMass = false;
      if      (mode == 0) invMass = true;
      else if (mode == 1 && (pair.M() > invMax || pair.M() < invMin)) invMass = true;
      else if (mode == 2 && (pair.M() > invMax || pair.M() < invMin)) invMass = true;
     
      if (invMass){
	histo->Fill(2, xlWeight);
	if (metPt >= metCut || mode ==0){
	  histo->Fill(3, xlWeight);
	  
	  int nJets = 0;
	  int nJetsSecond = 0;
	  int nJetsBt = 0;
	  bool bTagged = false;
	  int indexJet = -5;
	  for (int i=0; i < ptJet->size(); i++){
	    TLorentzVector tempJet(pxJet->at(i),pyJet->at(i), pzJet->at(i), eJet->at(i));
	    if (tempJet.Pt()  >= jetCut && TMath::Min(fabs(lepton0.DeltaR(tempJet)), fabs(lepton1.DeltaR(tempJet))) > 0.3){
	      nJets++;
	      indexJet = i;
	      if(btJet->at(i) > 1.74) bTagged = true;
	    } else if (tempJet.Pt()  >= secondJetCut) nJetsSecond++;
	    if (btJet->at(i) > 1.74) nJetsBt++;
	     
	  }
	  
	  if (nJets == 1){
	    histo->Fill(4, xlWeight);
	    if (nJetsBt == 1 && bTagged){
	   // if (bTagged && nJetsSecond == 0){
	      histo->Fill(5, xlWeight);
	      TLorentzVector jet(pxJet->at(indexJet),pyJet->at(indexJet), pzJet->at(indexJet), eJet->at(indexJet));
	      
	      double ptSysPx = lepton0.Px() + lepton1.Px() + jet.Px() + metPx;
	      double ptSysPy = lepton0.Py() + lepton1.Py() + jet.Py() + metPy;
	      double ptSystem = sqrt(ptSysPx*ptSysPx + ptSysPy*ptSysPy);
	      double ht = lepton0.Pt() + lepton1.Pt() + jet.Pt() + metPt; 
	      
	      if (ptSystem <= ptsysCut){
		histo->Fill(6, xlWeight);
		if (ht > htMin || mode !=0){
		  histo->Fill(7, xlWeight);
	       	}
	      }
	    }
	  }
	}
      }
      
    } // 2 leptons
  } // events
  
  
  if (!silent){ 
    cout << "------------------------------------------" << endl;
    cout << "Results: " << plotName <<  endl;
    cout << "------------------------------------------" << endl;  
    for (int i = 2; i < 9; i++){
      if (i == 2) cout << " leptons: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 3) cout << " inv. mass: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 4) cout << " met: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 5) cout << " jet: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 6) cout << " jet_bt: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 7) cout << " pt system: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
      if (i == 8) cout << " ht: " <<  histo->GetBinContent(i) << " +/-  " <<  histo->GetBinError(i)  << endl;
    }
    cout << "------------------------------------------" << endl; 
  }
  f_var.Write();
  f_var.Close();
  
  
}


