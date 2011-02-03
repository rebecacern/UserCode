#include "TStyle.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

//user code
#include "../../TopBrussels/TopTreeProducer/interface/TRootRun.h"
#include "../../TopBrussels/TopTreeProducer/interface/TRootEvent.h"
#include "../Selection/interface/SelectionTable.h"
#include "../Tools/interface/PlottingTools.h"
#include "../Tools/interface/JetTools.h"
#include "../Tools/interface/MultiSamplePlot.h"
#include "../Tools/interface/TTreeLoader.h"
#include "../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../Content/interface/AnalysisEnvironment.h"
#include "../Content/interface/Dataset.h"
#include "../MCInformation/interface/MCWeighter.h"
#include "../Selection/interface/ElectronPlotter.h"
#include "../Selection/interface/MuonPlotter.h"
#include "../Selection/interface/JetPlotter.h"
#include "../Selection/interface/VertexPlotter.h"
#include "../Tools/interface/MVATrainer.h"
#include "../Tools/interface/MVAComputer.h"
#include "../MCInformation/interface/ResolutionFit.h"
#include "../JESMeasurement/interface/FullKinFit.h"
#include "../JESMeasurement/interface/JetCombiner.h"
#include "Style.C"

using namespace std;
using namespace TopTree;

int main() {

  clock_t start = clock();

  cout << "*************************************************" << endl;
  cout << " Welcome to the e+jets analysis " << endl;
  cout << "*************************************************" << endl;
  
  
  setMyStyle();

  /////////////////////
  // Configuration
  /////////////////////

  //xml file
  string xmlfile ="../config/myBT.xml";

  //Output ROOT file
  string rootFileName ("outputs/out_vqq.root");
      
  //double weight = 36.1*157.5/1306182; //tt
  //double weight = 36.1*31314/14805546; //wjets
  //double weight = 36.1*3048/2543727; //zjets
  //double weight = 36.1*20.93/484060; //st tchan
  //double weight = 36.1*10.6/494961; //tW tchan
  //double weight = 36.1*132160/2205812; // qcd bc 20 - 30 
  //double weight = 36.1*136804/1975502; // qcd bc 30 - 80 
  //double weight = 36.1*9360/1043390; // qcd bc 80 - 170 
  //double weight = 36.1*2454400/36616469; // qcd em 20 - 30 
  //double weight = 36.1*3866200/71748368; // qcd em 30 - 80 
  //double weight = 36.1*139500/8049720; // qcd em 80 - 170 
  double weight = 36.1*36/720613; // vqq
  
  //double weight = 1;

  //Configuration output format
  TTree *configTree = new TTree("configTree","configuration Tree");
  TClonesArray* tcdatasets = new TClonesArray("Dataset",1000);
  configTree->Branch("Datasets","TClonesArray",&tcdatasets);
  TClonesArray* tcAnaEnv = new TClonesArray("AnalysisEnvironment",1000);
  configTree->Branch("AnaEnv","TClonesArray",&tcAnaEnv);

  ////////////////////////////////////
  /// AnalysisEnvironment  
  ////////////////////////////////////

  AnalysisEnvironment anaEnv;
  AnalysisEnvironmentLoader anaLoad(anaEnv,xmlfile.c_str());
  new ((*tcAnaEnv)[0]) AnalysisEnvironment(anaEnv);
  
  /////////////////////
  // Load Datasets
  /////////////////////

  TTreeLoader treeLoader;
  vector < Dataset* > datasets;
  treeLoader.LoadDatasets (datasets, xmlfile.c_str());
  for(unsigned int i=0;i<datasets.size();i++) new ((*tcdatasets)[i]) Dataset(*datasets[i]);


  //vector of objects
  vector < TRootVertex* > vertex;
  vector < TRootMuon* > init_muons;
  vector < TRootElectron* > init_electrons;
  vector < TRootJet* > init_jets;
  vector < TRootMET* > mets;


  TFile *fout = new TFile (rootFileName.c_str(), "RECREATE");
  
  TRootEvent* event = 0;
  
  ////////////////////////////////////
  /// Plots and Trees (TH1F* and TH2F*)
  ////////////////////////////////////

  
  TH1F* cutflow = new TH1F("cutflow", " ", 31,  -0.5, 30.5 );
  TH1F* cutflow_raw = new TH1F("cutflow_raw", " ", 31,  -0.5, 30.5 );
  
  cutflow->Sumw2();
  cutflow_raw->Sumw2();
  
  TH1F* jet_multiplicity = new TH1F("jet_multiplicity", " ", 4,  0.5, 4.5 );
  TH1F* jet_multiplicity_bt = new TH1F("jet_multiplicity_bt", " ", 4,  0.5, 4.5 );
  TH1F* jet_multiplicity_sel = new TH1F("jet_multiplicity_sel", " ", 4,  0.5, 4.5 );
  TH1F* jet_multiplicity_sel_bt = new TH1F("jet_multiplicity_sel_bt", " ", 4,  0.5, 4.5 );
  
  jet_multiplicity->Sumw2();
  jet_multiplicity_bt->Sumw2();
  jet_multiplicity_sel->Sumw2();
  jet_multiplicity_sel_bt->Sumw2();
  
  
  double xlweight; 
  
  double et;
  double eta;
  double phi;
  
  double met;
  
  std::vector<double> *ptjet;
  std::vector<double> *etajet;
  std::vector<double> *btjet;

  
  TTree* myTree = new TTree("myTree", "   ");
  
  myTree->Branch("xlweight", &xlweight, "xlweight/D");
  
  myTree->Branch("et", &et, "et/D");
  myTree->Branch("eta", &eta, "eta/D");
  myTree->Branch("phi", &phi, "phi/D");
  
  myTree->Branch("met", &met, "met/D");
  
  myTree->Branch("ptjet","std::vector<double>",&ptjet);
  myTree->Branch("etajet","std::vector<double>",&etajet);
  myTree->Branch("btjet","std::vector<double>",&btjet);
  
  ////////////////////////////////////
  //	Loop on datasets
  ////////////////////////////////////
  
 
 for (unsigned int d = 0; d < datasets.size (); d++)
  {
    treeLoader.LoadDataset (datasets[d], anaEnv);
    string dataSetName = datasets[d]->Name();

    ////////////////////////////////////
    //	Loop on events
    ////////////////////////////////////
    
    cout << dataSetName << endl;
    cout << datasets[d]->NofEvtsToRunOver() << endl;
    
    int itrigger = -1, previousRun = -1;
    
    for (unsigned int ievt = 0; ievt < datasets[d]->NofEvtsToRunOver(); ievt++)
    {
    

      if(ievt%500 == 0) std::cout<<"Processing the "<<ievt<<"th event" <<flush<<"\r";
	
      event = treeLoader.LoadEvent (ievt, vertex, init_muons, init_electrons, init_jets, mets);
     
      xlweight = weight;
      et = -45;
      eta = -45;
      phi = -45;
      met = -45;
      
      ptjet = new std::vector<double>;
      etajet = new std::vector<double>;
      btjet = new std::vector<double>;


      /////////////
      // TRIGGER //
      /////////////
      
      bool trigged=false;

      trigged=false;
      /*if(dataSetName == "Data" || dataSetName == "data" || dataSetName == "DATA") {
	  
	  if (event->runId() < 140040 && event->trigHLT(treeLoader.iTrigger("HLT_Ele10_LW_L1R", event->runId())))
	    trigged=true;
	  else if (event->runId() >= 140041 && event->runId() <= 143962 && event->trigHLT(treeLoader.iTrigger("HLT_Ele15_SW_L1R", event->runId())))
	    trigged=true;
	  else if (event->runId() >= 143963 && event->runId() <= 146427 && event->trigHLT(treeLoader.iTrigger("HLT_Ele15_SW_CaloEleId_L1R", event->runId()) ))
	    trigged=true;
	  else if (event->runId() >= 146428 && event->runId() <= 147116 && event->trigHLT(treeLoader.iTrigger("HLT_Ele17_SW_CaloEleId_L1R", event->runId()) ))
	    trigged=true;
	  else if (event->runId() >= 147117 && event->runId() <= 148818 && event->trigHLT(treeLoader.iTrigger("HLT_Ele17_SW_TightEleId_L1R", event->runId()) ))
	    trigged=true;
	  else if (event->runId() >= 148819 && event->runId() <= 149180 && event->trigHLT(treeLoader.iTrigger("HLT_Ele22_SW_TighterEleId_L1R_v2", event->runId()) ))
	    trigged=true;
	  else if (event->runId() >= 1149181 && event->trigHLT(treeLoader.iTrigger("HLT_Ele22_SW_TighterEleId_L1R_v3", event->runId()) ))
	    trigged=true;
	  else
	    trigged=false;
	}  else trigged = true;*/
	
	 if(dataSetName == "Data" || dataSetName == "data" || dataSetName == "DATA") {
	  
	  std::map<std::string,std::vector<double> > filters= event->getTriggerFilters();
	  
	  if (event->runId() < 140041 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 140041 && event->runId() <= 143962 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 143963 && event->runId() <= 146427 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt15CaloEleIdPixelMatchFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 146428 && event->runId() <= 147116 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt17CaloEleIdPixelMatchFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 147117 && event->runId() <= 148818 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt17TightEleIdDphiFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 148819 && event->runId() <= 149180 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterEleIdDphiFilter") != filters.end())
	    trigged=true;
	  else if (event->runId() >= 149181 && filters.find("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterEleIdDphiFilter") != filters.end())
	    trigged=true;
	  else
	    trigged=false;
	}  else trigged = true;
	

      /////////////////////////////
      //   Selection
      /////////////////////////////
      
      
      Selection selection(init_jets, init_muons, init_electrons, mets);
      
      
      bool isGoodPV = isGoodPV = selection.isPVSelected(vertex, 4,24,2.);  
	
      // Selection Cut Flow//
	
      cutflow->Fill(1, weight);
      cutflow_raw->Fill(1);
      if(trigged){
        cutflow->Fill(2, weight);
        cutflow_raw->Fill(2);
        if(isGoodPV){
          cutflow->Fill(3, weight);
          cutflow_raw->Fill(3);
          
          vector<TRootElectron*> selectedElectrons = selection.GetSelectedElectrons(vertex[0]);
          vector<TRootJet*> selectedJets = selection.GetSelectedJets(selectedElectrons,true);
          vector<TRootElectron*> looseElectrons = selection.GetSelectedLooseElectrons(20,2.5,1.,true);
          vector<TRootMuon*> vetoMuons = selection.GetSelectedLooseMuons();
	  
	  bool passZVeto = true;
          if (selectedElectrons.size()==1){
            for (unsigned int e=0;e<looseElectrons.size();e++) {
              TRootElectron* el = (TRootElectron*) looseElectrons[e];
              if (fabs(el->superClusterEta()) > 1.5660 || fabs(el->superClusterEta()) < 1.4442) {
                TLorentzVector Zcand = *looseElectrons[e]+*selectedElectrons[0];
                if (Zcand.M() > 76 && Zcand.M() < 106) passZVeto = false;
              }
            }
          }
	  
	  
	  bool BTjet = false;
	  for (int i =0; i < selectedJets.size(); i ++){
	    if (selectedJets[i]->btag_simpleSecondaryVertexHighEffBJetTags() > 1.74) BTjet = true;
	  }
	    
	  // Simple Jet Multiplicity
          if (selectedJets.size() < 4) jet_multiplicity->Fill(selectedJets.size(), weight);
          else jet_multiplicity->Fill(4, weight);
          if (selectedJets.size() < 4 && BTjet) jet_multiplicity_bt->Fill(selectedJets.size(), weight);
          else if (BTjet) jet_multiplicity_bt->Fill(4, weight);       
	 

	    
	    if (selectedElectrons.size()==1) {
	      cutflow->Fill(4, weight);
              cutflow_raw->Fill(4);
	      
	      TRootElectron* electron = (TRootElectron*) selectedElectrons[0];
	      
	      if (vetoMuons.size()==0) {
	        cutflow->Fill(5, weight);
                cutflow_raw->Fill(5);
		
		if (passZVeto) {
	          cutflow->Fill(6, weight);
                  cutflow_raw->Fill(6);
		  
		  if (selectedElectrons[0]->missingHits() == 0) {
	            cutflow->Fill(7, weight);
                    cutflow_raw->Fill(7);
		    		    
		    if (fabs(selectedElectrons[0]->Dist()) >= 0.02 || fabs(selectedElectrons[0]->DCot()) >= 0.02) {
	              cutflow->Fill(8, weight);
                      cutflow_raw->Fill(8);
		      
		      xlweight = weight;
		      
		      et = electron->Et();
		      eta = electron->Eta();
		      phi = electron->Phi();
		      
		      for (int i =0; i < selectedJets.size(); i ++){
		      TRootJet* jet = (TRootJet*) selectedJets[i];
	                ptjet->push_back(jet->Pt());
			etajet->push_back(jet->Eta());
			btjet->push_back(jet->btag_simpleSecondaryVertexHighEffBJetTags() > 1.74);
	              }
	              
		      met = mets[0]->Pt();
		      
		      if (selectedJets.size() < 4) jet_multiplicity_sel->Fill(selectedJets.size(), weight);
         	      else jet_multiplicity_sel->Fill(4, weight);
         	      if (selectedJets.size() < 4 && BTjet) jet_multiplicity_sel_bt->Fill(selectedJets.size(), weight);
         	      else if (BTjet) jet_multiplicity_sel_bt->Fill(4, weight);
		      
		      if(selectedJets.size()> 0) cutflow->Fill(9, weight);
		      if(selectedJets.size()> 1) cutflow->Fill(10, weight);
		      if(selectedJets.size()> 0) cutflow_raw->Fill(9);
		      if(selectedJets.size()> 1) cutflow_raw->Fill(10);
		      if(selectedJets.size()> 2) {
		        cutflow->Fill(11, weight);
                        cutflow_raw->Fill(11);
		        if (BTjet) cutflow->Fill(13, weight);
		        if (BTjet) cutflow_raw->Fill(13);
			
	              } 
		      if(selectedJets.size()> 3) {
		      
		        cutflow->Fill(12, weight);
		        cutflow_raw->Fill(12);
		        if (BTjet) cutflow->Fill(14, weight);
		        if (BTjet) cutflow_raw->Fill(14);
	              }
		      
		      
		      myTree->Fill();
		      delete ptjet;
		      delete etajet;
		      delete btjet;
		      
		    }
		  }
		}	
	      }
	    }
	  }
	}
	
	
    } // event loop
  } // dataset loop

  //Selection tables
 
  cout << "------------------------------------------" << endl;
  cout << "Results RAW: " <<  endl;
  cout << "------------------------------------------" << endl;  
  cout << "All:       " <<  cutflow_raw->GetBinContent(2) << endl;
  
  cout << "Electron:  " <<  cutflow_raw->GetBinContent(5) << endl;
  cout << "Muon veto: " <<  cutflow_raw->GetBinContent(6) << endl;
  cout << "Z veto:    " <<  cutflow_raw->GetBinContent(7) << endl;
  cout << "Conv. veto 2:      " <<  cutflow_raw->GetBinContent(9) << endl;
  cout << "3 + jets:  " <<  cutflow_raw->GetBinContent(12) << endl;
  cout << "SSVHEM 3+j:   " <<  cutflow_raw->GetBinContent(14) << endl;
  cout << "4 + jets:	 " <<  cutflow_raw->GetBinContent(13) << endl;
  cout << "SSVHEM 4+j:   " <<  cutflow_raw->GetBinContent(15) << endl;
  
  cout << "------------------------------------------" << endl;
  cout << "Results: " <<  endl;
  cout << "------------------------------------------" << endl;  
  cout << "All:       " <<  cutflow->GetBinContent(2) << " +/- " << cutflow->GetBinError(2) << endl;
  
  cout << "Electron:  " <<  cutflow->GetBinContent(5) << " +/- " << cutflow->GetBinError(5) << endl;
  cout << "Muon veto: " <<  cutflow->GetBinContent(6) << " +/- " << cutflow->GetBinError(6) << endl;
  cout << "Z veto:    " <<  cutflow->GetBinContent(7) << " +/- " << cutflow->GetBinError(7) << endl;
  cout << "Conv. veto 2:      " <<  cutflow->GetBinContent(9) << " +/- " << cutflow->GetBinError(9) << endl;
  cout << "3 + jets:  " <<  cutflow->GetBinContent(12) << " +/- " << cutflow->GetBinError(12) << endl;
  cout << "SSVHEM 3+j:   " <<  cutflow->GetBinContent(14) << " +/- " << cutflow->GetBinError(14) << endl;
  cout << "4 + jets:	 " <<  cutflow->GetBinContent(13) << " +/- " << cutflow->GetBinError(13) << endl;
  cout << "SSVHEM 4+j:   " <<  cutflow->GetBinContent(15) << " +/- " << cutflow->GetBinError(15) << endl;
  
  cout << "------------------------------------------" << endl; 
  
  fout->Write();
  fout->Close();


  cout << "It took you " << ((double)clock() - start) / CLOCKS_PER_SEC << " to run the program" << endl;

}
 
