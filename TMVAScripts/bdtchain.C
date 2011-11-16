// rebeca@cern.ch
#include "TLorentzVector.h"
#include "TVector3.h"
#include "inputs.h"


void bdtchain(int nsel = 1, int mode = 0, bool silent = false){
  
  
  double SFval = 0.95;
  bool SFplus = false;
  bool SFminus = false;
  // samples used
  double x_sec = 0.;
  char plotName[300];
  sprintf(plotName,"test");
  
  if (nsel == 0)                	{sprintf(plotName,"tt");}
  else if (nsel == 1)   		{sprintf(plotName,"twdr");}
  else if (nsel == -1)   		{sprintf(plotName,"twds");}
  else if (nsel == 2)			{sprintf(plotName, "st");}
  else if (nsel == 3)   		{sprintf(plotName,"wjets");}
  else if (nsel == 4)   		{sprintf(plotName,"zjets");}
  else if (nsel == 5)   		{sprintf(plotName,"dy");}
  else if (nsel == 6)   		{sprintf(plotName,"qcd_mu");}
  else if (nsel == 7)   		{sprintf(plotName,"di");}
  else if (nsel == 77)                	{sprintf(plotName,"non_tt");}
  else if (nsel == 66)                	{sprintf(plotName,"others");}
  else if (nsel == 666)                	{sprintf(plotName,"data");}
  else if (nsel == 555)                	{sprintf(plotName,"mc");}
  else if (nsel == 777)                 {sprintf(plotName,"all");}
  
  // if (mode != 0 &&  mode !=1 && mode !=2) mode = 0;
  if (!silent){
    if      (mode == 0) 	cout << " Electron-Muon Mixed channel " << endl;
    else if (mode == 1) 	cout << " Di-Muon channel " << endl;
    else if (mode == 2) 	cout << " Di-Electron channel " << endl;
    cout << "*************************************************" << endl;
  }
  
  bool nosf = true;
  char myRootFile[300];
  sprintf(myRootFile,"rootfiles/tmva_test_%d_%s.root", mode, plotName);
  TFile *input = TFile::Open(myRootFile);
  
  // tree variables
  /////
  double weight; 
  double met, ht, ptsys, mll;
  
  TTree*Tree = (TTree*) gROOT->FindObject("myTree");
  
  Tree->SetBranchAddress("weight", &weight);
  Tree->SetBranchAddress("metpt", &met);
  Tree->SetBranchAddress("ht", &ht);
  Tree->SetBranchAddress("ptsys", &ptsys);
  Tree->SetBranchAddress("mll", &mll);
  
  
  double bdtvalue;
  TTree* BDTTree = (TTree*) gROOT->FindObject("BDT_test_tw_short_00");
  BDTTree->SetBranchAddress("BDT",&bdtvalue);
  
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
  

  double nFinal = 0;
  //////////
  for(int event = 0; event<nEvents; event++){
    
    Tree->GetEntry(event);
   
    
   
   
     BDTTree->GetEntry(event);
    
    if (mode!=0 || (ptsys < 60 && ht > 160)){
    
    if (bdtvalue >  -0.018) nFinal += weight;
    }
  } // events
  
  
  if (!silent){ 
    cout << "------------------------------------------" << endl;
    cout << "Results: " << nFinal << " " << plotName << " events pass the bdt cut " <<  endl;
    
    }
    cout << "------------------------------------------" << endl; 
  
 
  
  
}


