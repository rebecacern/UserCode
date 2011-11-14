/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void TMVAClassificationApplication_tW(TString signal = "DATA") 
{   
#ifdef __CINT__
  gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif
  
  //---------------------------------------------------------------
  // This loads the library
  TMVA::Tools::Instance();
  // --------------------------------------------------------------------------------------------------
  
  // --- Create the Reader object
  
  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    
  
  // Create a set of variables and declare them to the reader
  // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
  
  Float_t jetpt;
  Float_t jeteta;
  Float_t jetphi;
  Float_t metpt;
  Float_t metpro;
  Float_t lep0pt;
  Float_t lep1pt;
  Float_t lep0eta;
  Float_t lep1eta;
  Float_t lep0phi;
  Float_t lep1phi;
  Float_t ptsys;
  Float_t ht;
  Float_t oblateness;
  Float_t sphericity;
  Float_t aplanarity;
  Float_t njetw;
  Float_t sqrts;
  Float_t deltarleps;
  Float_t deltaphileps;
  Float_t deltaetaleps;
  Float_t philepmetclose;
  Float_t philepmetfar;
  Float_t rlepmetclose;
  Float_t rlepmetfar;
  Float_t philepjetclose;
  Float_t philepjetfar;
  Float_t rlepjetclose;
  Float_t rlepjetfar;
  Float_t phijetmet;
  Float_t rjetmet;
  
  reader->AddVariable ("jetpt", &jetpt);
  reader->AddVariable ("jeteta", &jeteta);
  reader->AddVariable ("jetphi", &jetphi);
  reader->AddVariable ("metpt", &metpt);
  reader->AddVariable ("metpro",&metpro);
  reader->AddVariable ("lep0pt",&lep0pt);
  reader->AddVariable ("lep1pt",&lep1pt);
  reader->AddVariable ("lep0eta",&lep0eta);
  reader->AddVariable ("lep1eta",&lep1eta);
  reader->AddVariable ("lep0phi",&lep0phi);
  reader->AddVariable ("lep1phi",&lep1phi);
  reader->AddVariable ("ptsys",&ptsys);
  reader->AddVariable ("ht",&ht);
  reader->AddVariable ("oblateness", &oblateness);
  reader->AddVariable ("sphericity", &sphericity);
  reader->AddVariable ("aplanarity", &aplanarity);
  reader->AddVariable ("njetw", &njetw);
  reader->AddVariable ("sqrts", &sqrts);
  reader->AddVariable ("deltarleps", &deltarleps);
  reader->AddVariable ("deltaphileps", &deltaphileps);
  reader->AddVariable ("deltaetaleps", &deltaetaleps);
  reader->AddVariable ("philepmetclose", &philepmetclose);
  reader->AddVariable ("philepmetfar", &philepmetfar);
  reader->AddVariable ("rlepmetclose", &rlepmetclose);
  reader->AddVariable ("rlepmetfar", &rlepmetfar);
  reader->AddVariable ("philepjetclose", &philepjetclose);
  reader->AddVariable ("philepjetfar", &philepjetfar);
  reader->AddVariable ("rlepjetclose", &rlepjetclose);
  reader->AddVariable ("rlepjetfar", &rlepjetfar);
  reader->AddVariable ("phijetmet", &phijetmet);
  reader->AddVariable ("rjetmet", &rjetmet);
  
  
  // *************************************************
  
  // --- Book the MVA methods
  
  TString dir    = "weights/";
  
  TString prefix = "test_tw_00";
  TString name = "BDT_"+prefix;
  std::cout<<"********* name = "<<name<<std::endl;
  
  //
  // book the MVA methods
  //
  
  reader->BookMVA( "BDT method", dir + prefix + "_BDT.weights.xml" );   
  
  // book output histograms
  Int_t nbin = 100;
  histBdt   = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -1.0, 1.0 );
  
  // book example histogram for probability (the other methods are done similarly)
  probHistBDT   = new TH1F( "Probability_MVA_BDT", "Probability_MVA_BDT", nbin, -1, 1);
  rarityHistBDT = new TH1F( "Rarity_MVA_BDT", "Rarity_MVA_BDT", nbin, -1, 1);
  
  // Prepare input tree (this must be replaced by your data source)
  // in this example, there is a toy tree with signal and one with background events
  // we'll later on use only the "signal" events for the test in this example.
  //   
  TFile *input(0);
  
  TString folder = "rootfiles/";
  
  if(signal == "data") { TString fname = folder+"tmva_test_0_data.root"; }
  
  if(signal == "tw"){ TString fname = folder+"tmva_test_0_twdr.root"; }
  if(signal == "ww"){ TString fname = folder+"tmva_test_0_ww.root"; }
  if(signal == "qcd"){ TString fname = folder+"tmva_test_0_qcd_mu.root"; }
  if(signal == "wz"){ TString fname = folder+"tmva_test_0_wz.root"; }
  if(signal == "zz"){ TString fname = folder+"tmva_test_0_zz.root"; }
  if(signal == "st"){ TString fname = folder+"tmva_test_0_st.root"; }
  if(signal == "tt"){ TString fname = folder+"tmva_test_0_tt.root"; }
  if(signal == "wjets"){ TString fname = folder+"tmva_test_0_wjets.root"; }
  if(signal == "zjets"){ TString fname = folder+"tmva_test_0_zjets.root"; }
  
  input = TFile::Open( fname,"UPDATE");   
  
  if (!input) {
    cout << "ERROR: could not open data file: " << fname << endl;
    exit(1);
  }
  
  // --- Event loop
  
  // Prepare the event tree
  // - here the variable names have to corresponds to your tree
  // - you can use the same variables as above which is slightly faster,
  //   but of course you can use different ones and copy the values inside the event loop
  //
  
  input->cd();
  TTree* theTree = (TTree*)input->Get("myTree");
  cout << "--- Select signal sample" << endl;
  
  Float_t userjetpt;
  Float_t userjeteta;
  Float_t userjetphi;
  Float_t usermetpt;
  Float_t usermetpro;
  Float_t userlep0pt;
  Float_t userlep1pt;
  Float_t userlep0eta;
  Float_t userlep1eta;
  Float_t userlep0phi;
  Float_t userlep1phi;
  Float_t userptsys;
  Float_t userht;
  Float_t useroblateness;
  Float_t usersphericity;
  Float_t useraplanarity;
  Float_t usernjetw;
  Float_t usersqrts;
  Float_t userdeltarleps;
  Float_t userdeltaphileps;
  Float_t userdeltaetaleps;
  Float_t userphilepmetclose;
  Float_t userphilepmetfar;
  Float_t userrlepmetclose;
  Float_t userrlepmetfar;
  Float_t userphilepjetclose;
  Float_t userphilepjetfar;
  Float_t userrlepjetclose;
  Float_t userrlepjetfar;
  Float_t userphijetmet;
  Float_t userrjetmet;
  
  theTree->SetBranchAddress ("jetpt", &userjetpt);
  theTree->SetBranchAddress ("jeteta", &userjeteta);
  theTree->SetBranchAddress ("jetphi", &userjetphi);
  theTree->SetBranchAddress ("metpt", &usermetpt);
  theTree->SetBranchAddress ("metpro",&usermetpro);
  theTree->SetBranchAddress ("lep0pt",&userlep0pt);
  theTree->SetBranchAddress ("lep1pt",&userlep1pt);
  theTree->SetBranchAddress ("lep0eta",&userlep0eta);
  theTree->SetBranchAddress ("lep1eta",&userlep1eta);
  theTree->SetBranchAddress ("lep0phi",&userlep0phi);
  theTree->SetBranchAddress ("lep1phi",&userlep1phi);
  theTree->SetBranchAddress ("ptsys",&userptsys);
  theTree->SetBranchAddress ("ht",&userht);
  theTree->SetBranchAddress ("oblateness", &useroblateness);
  theTree->SetBranchAddress ("sphericity", &usersphericity);
  theTree->SetBranchAddress ("aplanarity", &useraplanarity);
  theTree->SetBranchAddress ("njetw", &usernjetw);
  theTree->SetBranchAddress ("sqrts", &usersqrts);
  theTree->SetBranchAddress ("deltarleps", &userdeltarleps);
  theTree->SetBranchAddress ("deltaphileps", &userdeltaphileps);
  theTree->SetBranchAddress ("deltaetaleps", &userdeltaetaleps);
  theTree->SetBranchAddress ("philepmetclose", &userphilepmetclose);
  theTree->SetBranchAddress ("philepmetfar", &userphilepmetfar);
  theTree->SetBranchAddress ("rlepmetclose", &userrlepmetclose);
  theTree->SetBranchAddress ("rlepmetfar", &userrlepmetfar);
  theTree->SetBranchAddress ("philepjetclose", &userphilepjetclose);
  theTree->SetBranchAddress ("philepjetfar", &userphilepjetfar);
  theTree->SetBranchAddress ("rlepjetclose", &userrlepjetclose);
  theTree->SetBranchAddress ("rlepjetfar", &userrlepjetfar);
  theTree->SetBranchAddress ("phijetmet", &userphijetmet);
  theTree->SetBranchAddress ("rjetmet", &userrjetmet);
  
  Double_t tBDT;    
  TTree* BDTTree = new TTree(name,"");
  BDTTree->Branch("BDT",&tBDT,"BDT/D");
  
  Float_t tweight;
  
  theTree->SetBranchAddress("weight",&tweight);
  
  std::cout<<" ... opening file : "<<fname<<std::endl;
  cout << "--- Processing: " << theTree->GetEntries() << " events" << endl;
  
  TH1F *hBDT = new TH1F("hBDT","",100,-1.0,1.0);
  
  TStopwatch sw;
  sw.Start();
  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
    
    if (ievt%1000 == 0) cout << "--- ... Processing event: " << ievt << endl;
    
    theTree->GetEntry(ievt);
    
    jetpt = userjetpt;
    jeteta = userjeteta;
    jetphi = userjetphi;
    metpt = usermetpt;
    metpro = usermetpro;
    lep0pt = userlep0pt;
    lep1pt = userlep1pt;
    lep0eta = userlep0eta;
    lep1eta = userlep1eta;
    lep0phi = userlep0phi;
    lep1phi = userlep1phi;
    ptsys = userptsys;
    ht = userht;
    oblateness = useroblateness;
    sphericity = usersphericity;
    aplanarity = useraplanarity;
    njetw = usernjetw;
    sqrts = usersqrts;
    deltarleps = userdeltarleps;
    deltaphileps = userdeltaphileps;
    deltaetaleps = userdeltaetaleps;
    philepmetclose = userphilepmetclose;
    philepmetfar = userphilepmetfar;
    rlepmetclose = userrlepmetclose;
    rlepmetfar = userrlepmetfar;
    philepjetclose = userphilepjetclose;
    philepjetfar = userphilepjetfar;
    rlepjetclose = userrlepjetclose;
    rlepjetfar = userrlepjetfar;
    phijetmet = userphijetmet;
    rjetmet = userrjetmet;
    
    double bdt         = reader->EvaluateMVA("BDT method");
    
    tBDT = bdt;
    BDTTree->Fill();
    hBDT->Fill(bdt,tweight);
    
    double bdt_     = reader->EvaluateMVA("BDT method");
    histBdt   ->Fill(bdt_);	
  
  }

  // Get elapsed time
  sw.Stop();
  std::cout << "--- End of event loop: "; sw.Print();
  
  input->cd();
  hBDT->Write();
  BDTTree->Write();
  
  input->Close();
  
  
  // --- Write histograms
  
  TFile *target  = new TFile( "TMVApp.root","RECREATE" );
  
  histBdt    ->Write();
  
  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
  
  
  delete reader;
  
  std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;
} 
