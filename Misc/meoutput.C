// rebeca@cern.ch
#include "TLorentzVector.h"
#include "TVector3.h"
#include "inputs.h"


void meoutput(int nsel = 1, int mode = 0, bool silent = false){
  
   
  
  // samples used
  double x_sec = 0.;
  char plotName[300];
  sprintf(plotName,"test");
  
  if (nsel == 0)                	{sprintf(plotName,"tt");}
  else if (nsel == 1)   		{sprintf(plotName,"tw");}
  
  
  if (mode != 0 &&  mode !=1 && mode !=2) mode = 0;
  if (!silent){
    if      (mode == 0) 	cout << " Electron-Muon Mixed channel " << endl;
    else if (mode == 1) 	cout << " Di-Muon channel " << endl;
    else if (mode == 2) 	cout << " Di-Electron channel " << endl;
  }
  
  char myRootFile[300];
  sprintf(myRootFile,"outputs/out_%d_%s.root", mode, plotName);
  TFile *input = TFile::Open(myRootFile);
  
  
  char myTexFile[300];
  sprintf(myTexFile,"out/%s_%d.lhco", plotName, mode);
  ofstream salida(myTexFile);
  
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
    cout << "------------------------------------------" << endl;
    cout << "Sample: " << plotName <<  endl;
    cout << "------------------------------------------" << endl;
    cout << "Number of Total events in the tuple: " <<  nEvents << endl;
    cout << "------------------------------------------" << endl;
  }
  
  
  //////////
  int n1Jet = 0;
  int nTotal = 0;
  int nUsed = 0;
  
  for(int event = 0; event<nEvents; event++){
    
    Tree->GetEntry(event);
    
    if (luminosity != 1000) xlWeight*=(luminosity/1000);
    
    if(ptLepton->size() != 2) cout << "Something is wrong, your Tree is not correctly filled" << endl;
    else {
      
      TLorentzVector lepton0(pxLepton->at(0),pyLepton->at(0), pzLepton->at(0), eLepton->at(0));
      TLorentzVector lepton1(pxLepton->at(1),pyLepton->at(1), pzLepton->at(1), eLepton->at(1)); 
      TLorentzVector pair = lepton0+lepton1;
      
      bool invMass = false;
      if      (mode == 0) invMass = true;
      else if (mode == 1 && (pair.M() > invMax || pair.M() < invMin)) invMass = true;
      else if (mode == 2 && (pair.M() > invMax || pair.M() < invMin)) invMass = true;
      
      if (invMass){
	if (metPt >= metCut || mode ==0){
	  
	  int nJets = 0;
	  int nSecondJets = 0;
	  int nJetsBt = 0;
	  bool bTagged = false;
	  int indexJet = -5;
	  for (int i=0; i < ptJet->size(); i++){
	    TLorentzVector tempJet(pxJet->at(i),pyJet->at(i), pzJet->at(i), eJet->at(i));
	    if (tempJet.Pt()  >= jetCut && TMath::Min(fabs(lepton0.DeltaR(tempJet)), fabs(lepton1.DeltaR(tempJet))) > 0.3){
	      nJets++;
	      indexJet = i;
	      if(btJet->at(i) > 1.74) bTagged = true;
	    } else if (tempJet.Pt()  >= secondJetCut && TMath::Min(fabs(lepton0.DeltaR(tempJet)), fabs(lepton1.DeltaR(tempJet))) > 0.3) nSecondJets++;
	    if (btJet->at(i) > 1.74) nJetsBt++;
	    
	  }
	  
	  if (nJets == 1){
	    if (nJetsBt == 1 && bTagged){
	      // if (bTagged && nSecondJets == 0){
	      TLorentzVector jet(pxJet->at(indexJet),pyJet->at(indexJet), pzJet->at(indexJet), eJet->at(indexJet));
	      
	      double ptSysPx = lepton0.Px() + lepton1.Px() + jet.Px() + metPx;
	      double ptSysPy = lepton0.Py() + lepton1.Py() + jet.Py() + metPy;
	      double ptSystem = sqrt(ptSysPx*ptSysPx + ptSysPy*ptSysPy);
	      double ht = lepton0.Pt() + lepton1.Pt() + jet.Pt() + metPt; 
	      
	      if (ptSystem <= ptsysCut){
		if (ht > htMin || mode !=0){
		  nTotal++;
		  if (nSecondJets != 0){
		    nUsed++;
		    int indexSecondJet = -5;
		    for (int i=0; i < ptJet->size(); i++){
		      TLorentzVector tempJet(pxJet->at(i),pyJet->at(i), pzJet->at(i), eJet->at(i));
		      if (i != indexJet && TMath::Min(fabs(lepton0.DeltaR(tempJet)), fabs(lepton1.DeltaR(tempJet))) > 0.3){
			indexSecondJet = i;
			break;
		      } 
		    }
		    
		    TLorentzVector jet2(pxJet->at(indexSecondJet),pyJet->at(indexSecondJet), pzJet->at(indexSecondJet), eJet->at(indexSecondJet));
		    TVector3 missingEt(metPx, metPy, 0);
		    
		    salida << "0 1 1 " << endl;
		    salida << "1 2 " << lepton0.Eta() << " " << lepton0.Phi() << " " << lepton0.Pt() <<" " << lepton0.M() << " " << qLepton->at(0) << " 0 0 0 0 " <<  endl;
		    salida << "2 2 " << lepton1.Eta() << " " << lepton1.Phi() << " " << lepton1.Pt() <<" " << lepton1.M() << " " << qLepton->at(1) << " 0 0 0 0 " <<  endl;
		    salida << "3 4 " << jet.Eta() << " " << jet.Phi() << " " << jet.Pt() <<" " << jet.M() << " " << qJet->at(indexJet) << " 2 0 0 0 " <<  endl;
		    salida << "4 4 " << jet2.Eta() << " " << jet2.Phi() << " " << jet2.Pt() <<" " << jet2.M() << " " << qJet->at(indexSecondJet) << " 2 0 0 0 " <<  endl;
		    salida << "5 6 " << missingEt.Eta() << " " << missingEt.Phi() << " " << metPt <<" " << " 0 0 0 0 0 0 " <<  endl;
		    
		  } else n1Jet++;
		  
		  
		  
	       	}
	      }
	    }
	  }
	}
      }
      
    } // 2 leptons
  } // events
  
  
  if (!silent){ 
    cout << "Results: "  <<  endl;
    cout << nTotal << " events passed the selection" << endl;
    cout << nUsed << " events were used in the lhco file " << endl;
    cout << n1Jet << " events were not used (only 1 jet exactly) " << endl;
    cout << "------------------------------------------" << endl; 
  }

  
  
}


