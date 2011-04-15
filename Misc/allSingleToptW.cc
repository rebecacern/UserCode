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
  cout << " Welcome to the single top tW analysis " << endl;
  cout << "*************************************************" << endl;
  
  setMyStyle();
  string xmlfile ="../config/tWSt.xml";
  
  double lumi = 1000;
  
  //modes: 0 emu, 1mumu, 2ee 
  int  mode = 0;  
  if (mode != 0 &&  mode !=1 && mode !=2) mode = 0;
  
  if      (mode == 0) 	cout << " Electron-Muon Mixed channel " << endl;
  else if (mode == 1) 	cout << " Di-Muon channel " << endl;
  else if (mode == 2) 	cout << " Di-Electron channel " << endl;
  cout << "*************************************************" << endl;
  
  
  int sample = 0; 
  
  char name[100];
  double weight;
  if (sample == 0){		sprintf(name, "tt");			weight = lumi*157.5/1306182;}
  else if (sample == 1){	sprintf(name, "tw");			weight = lumi*10.6/494961;}
  else if (sample == 2){	sprintf(name, "t");			weight = lumi*20.93/484060;}
  else if (sample == 3){	sprintf(name, "s");			weight = lumi*4.6/494961;}
  else if (sample == 4){	sprintf(name, "wjets"); 		weight = lumi*31314/14805546;}
  else if (sample == 5){	sprintf(name, "zjets"); 		weight = lumi*3048/2543727;}
  else if (sample == 6){	sprintf(name, "vqq");			weight = lumi*36/720613;}
  else if (sample == 7){	sprintf(name, "qcd_mu");		weight = lumi*84679.3/28884866;}
  else if (sample == 8){	sprintf(name, "qcd_bc_2030");		weight = lumi*132160/2205812;}
  else if (sample == 9){	sprintf(name, "qcd_bc_3080");  		weight = lumi*136804/1975502;}
  else if (sample == 10){	sprintf(name, "qcd_bc_80170"); 	 	weight = lumi*9360/1043390;}
  else if (sample == 11){	sprintf(name, "qcd_em_2030"); 	 	weight = lumi*2454400/36616469;}
  else if (sample == 12){	sprintf(name, "qcd_em_3080"); 	 	weight = lumi*3866200/71748368;}
  else if (sample == 13){	sprintf(name, "qcd_em_80170"); 	 	weight = lumi*139500/8049720;}
  else {			sprintf(name, "test"); 	 		weight = 1;}
  
 
  char rootFileName[100];
  sprintf(rootFileName,"outputs/out_%d_%s.root", mode, name);
  
  cout << " " << lumi << " pb: " << name << " - " << weight << endl;
  
  TTree *configTree = new TTree("configTree","configuration Tree");
  TClonesArray* tcdatasets = new TClonesArray("Dataset",1000);
  configTree->Branch("Datasets","TClonesArray",&tcdatasets);
  TClonesArray* tcAnaEnv = new TClonesArray("AnalysisEnvironment",1000);
  configTree->Branch("AnaEnv","TClonesArray",&tcAnaEnv);

  AnalysisEnvironment anaEnv;
  AnalysisEnvironmentLoader anaLoad(anaEnv,xmlfile.c_str());
  new ((*tcAnaEnv)[0]) AnalysisEnvironment(anaEnv);
  
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


  TFile *fout = new TFile (rootFileName, "RECREATE");
  
  TRootEvent* event = 0;
  
  TH1F* cutflow = new TH1F("cutflow", " ", 31,  -0.5, 30.5 );
  TH1F* cutflow_raw = new TH1F("cutflow_raw", " ", 31,  -0.5, 30.5 );
  
  cutflow->Sumw2();
  cutflow_raw->Sumw2();
  
  
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
  
  
  TTree* myTree = new TTree("myTree", "   ");
  
  myTree->Branch("xlWeight", &xlWeight, "xlWeight/D");
 
  myTree->Branch("metPt", &metPt, "metPt/D");
  myTree->Branch("metPx", &metPx, "metPx/D");
  myTree->Branch("metPy", &metPy, "metPy/D");
  
  myTree->Branch("ptLepton","std::vector<double>",&ptLepton);
  myTree->Branch("pxLepton","std::vector<double>",&pxLepton);
  myTree->Branch("pyLepton","std::vector<double>",&pyLepton);
  myTree->Branch("pzLepton","std::vector<double>",&pzLepton);
  myTree->Branch("eLepton","std::vector<double>",&eLepton);
  myTree->Branch("qLepton","std::vector<double>",&qLepton);
  
  myTree->Branch("ptJet","std::vector<double>",&ptJet);
  myTree->Branch("pxJet","std::vector<double>",&pxJet);
  myTree->Branch("pyJet","std::vector<double>",&pyJet);
  myTree->Branch("pzJet","std::vector<double>",&pzJet);
  myTree->Branch("eJet","std::vector<double>",&eJet);
  myTree->Branch("qJet","std::vector<double>",&qJet);
  myTree->Branch("btJet","std::vector<double>",&btJet);
  
  for (unsigned int d = 0; d < datasets.size (); d++)
    {
      treeLoader.LoadDataset (datasets[d], anaEnv);
      string dataSetName = datasets[d]->Name();
      
      cout << " " << dataSetName << endl;
      cout << " " << datasets[d]->NofEvtsToRunOver() << endl;
      
      int itrigger = -1, previousRun = -1;
      
      for (unsigned int ievt = 0; ievt < datasets[d]->NofEvtsToRunOver(); ievt++)
	{
	  if(ievt%500 == 0) std::cout<<"Processing the "<<ievt<<"th event" <<flush<<"\r";
	  
	  event = treeLoader.LoadEvent (ievt, vertex, init_muons, init_electrons, init_jets, mets);
	  
	  bool trigged = true;
	  
	  Selection selection(init_jets, init_muons, init_electrons, mets);
	  bool isGoodPV = isGoodPV = selection.isPVSelected(vertex, 4,24,2.);  
	  
	  cutflow->Fill(1, weight);
	  cutflow_raw->Fill(1);
	  if(trigged){
	    cutflow->Fill(2, weight);
	    cutflow_raw->Fill(2);
	    if(isGoodPV){
	      cutflow->Fill(3, weight);
	      cutflow_raw->Fill(3);
	      
	      vector<TRootElectron*> selectedElectrons = selection.GetSelectedElectrons(vertex[0]);
	      vector<TRootMuon*> selectedMuons = selection.GetSelectedDiMuons();
	      vector<TRootElectron*> looseElectrons = selection.GetSelectedLooseElectrons(true);
	      vector<TRootMuon*> looseMuons = selection.GetSelectedLooseMuons();
	      vector<TRootJet*> selectedJets = selection.GetSelectedJets(selectedElectrons,true);
	      
	      bool BTjet = false;
	      for (int i =0; i < selectedJets.size(); i ++){
		if (selectedJets[i]->btag_simpleSecondaryVertexHighEffBJetTags() > 1.74) BTjet = true;      
	      }
	      
	      bool leptonSelection = false;
	      if 	(mode == 0 && selectedElectrons.size()== 1 && selectedMuons.size()== 1) leptonSelection = true;
	      else if 	(mode == 1 && selectedElectrons.size()== 0 && selectedMuons.size()== 2) leptonSelection = true;
	      else if 	(mode == 2 && selectedElectrons.size()== 2 && selectedMuons.size()== 0) leptonSelection = true;
	      
	      if (leptonSelection) {
		
		bool charge = false;
		double q0, q1;
		TLorentzVector lepton0, lepton1;
		
		if (mode == 0){
		  TRootElectron* electron = (TRootElectron*) selectedElectrons[0];
		  TRootMuon* muon = (TRootMuon*) selectedMuons[0];
		  if (electron->charge()*muon->charge() < 0) charge = true;
		  if (electron->Pt() > muon->Pt()){
		    lepton0.SetPxPyPzE(electron->Px(), electron->Py(), electron->Pz(), electron->Energy());
		    lepton1.SetPxPyPzE(muon->Px(), muon->Py(), muon->Pz(), muon->Energy());
		    q0 = electron->charge();
		    q1 = muon->charge();
		  } else {
		    lepton0.SetPxPyPzE(muon->Px(), muon->Py(), muon->Pz(), muon->Energy());
		    lepton1.SetPxPyPzE(electron->Px(), electron->Py(), electron->Pz(), electron->Energy());
		    q0 = muon->charge();
		    q1 = electron->charge();
		  }
		} else if (mode == 1){
		  TRootMuon* muon0 = (TRootMuon*) selectedMuons[0];
		  TRootMuon* muon1 = (TRootMuon*) selectedMuons[1];
		  if (muon0->charge()*muon1->charge() < 0) charge = true;
		  lepton0.SetPxPyPzE(muon0->Px(), muon0->Py(), muon0->Pz(), muon0->Energy());
		  lepton1.SetPxPyPzE(muon1->Px(), muon1->Py(), muon1->Pz(), muon1->Energy());
		  q0 = muon0->charge();
		  q1 = muon1->charge();
		} else {
		  TRootElectron* electron0 = (TRootElectron*) selectedElectrons[0];
		  TRootElectron* electron1 = (TRootElectron*) selectedElectrons[1];
		  if (electron0->charge()*electron1->charge() < 0) charge = true;
		  lepton0.SetPxPyPzE(electron0->Px(), electron0->Py(), electron0->Pz(), electron0->Energy());
		  lepton1.SetPxPyPzE(electron1->Px(), electron1->Py(), electron1->Pz(), electron1->Energy());
		  q0 = electron0->charge();
		  q1 = electron0->charge();
		}
		
		if (charge){
		  cutflow->Fill(4, weight);
		  cutflow_raw->Fill(4);
		  
		  bool leptonVeto = false;
		  if 	  (mode == 0 && looseMuons.size()== 1 && looseElectrons.size() == 1) leptonVeto = true;
		  else if (mode == 1 && looseMuons.size()== 2 && looseElectrons.size() == 0) leptonVeto = true;
		  else if (mode == 2 && looseMuons.size()== 0 && looseElectrons.size() == 2) leptonVeto = true;
		  
		  if (leptonVeto) {
		    cutflow->Fill(5, weight);
		    cutflow_raw->Fill(5);
		    
		    TLorentzVector pair = lepton0 + lepton1;   
		    
		    xlWeight = weight;
		    
		    metPt = mets[0]->Pt();
		    metPx = mets[0]->Px();
		    metPy = mets[0]->Py();
		    
		    ptLepton = new std::vector<double>; 
		    pxLepton = new std::vector<double>; 
		    pyLepton = new std::vector<double>; 
		    pzLepton = new std::vector<double>; 
		    eLepton = new std::vector<double>; 
		    qLepton = new std::vector<double>;
		     
		    ptJet = new std::vector<double>; 
		    pxJet = new std::vector<double>; 
		    pyJet = new std::vector<double>; 
		    pzJet = new std::vector<double>; 
		    eJet = new std::vector<double>; 
		    qJet = new std::vector<double>; 
		    btJet = new std::vector<double>; 
		    
		    ptLepton->push_back(lepton0.Pt());
		    ptLepton->push_back(lepton1.Pt());
		     
		    pxLepton->push_back(lepton0.Px());
		    pxLepton->push_back(lepton1.Px());
		     
		    pyLepton->push_back(lepton0.Py());
		    pyLepton->push_back(lepton1.Py());
		     
		    pzLepton->push_back(lepton0.Pz());
		    pzLepton->push_back(lepton1.Pz());
		     
		    eLepton->push_back(lepton0.Energy());
		    eLepton->push_back(lepton1.Energy());
		    
		    qLepton->push_back(q0);
		    qLepton->push_back(q1);
		    
		    for (int i =0; i < selectedJets.size(); i ++){
		       TRootJet* tempJet = (TRootJet*) selectedJets[i];
		       ptJet->push_back(tempJet->Pt());
		       pxJet->push_back(tempJet->Px());
		       pyJet->push_back(tempJet->Py());
		       pzJet->push_back(tempJet->Pz());
		       eJet->push_back(tempJet->Energy());
		       qJet->push_back(tempJet->charge());
		       btJet->push_back(tempJet->btag_simpleSecondaryVertexHighEffBJetTags() );
		     }
		    
		    myTree->Fill();
		    
		    delete ptLepton;
		    delete pxLepton;
		    delete pyLepton;
		    delete pzLepton;
		    delete eLepton;
		    delete qLepton;
		    
		    delete ptJet;
		    delete pxJet;
		    delete pyJet;
		    delete pzJet;
		    delete eJet;
		    delete qJet;
		    delete btJet;
		    
		    if (pair.M() > 101 || pair.M() < 81){
		      cutflow->Fill(6, weight);
		      cutflow_raw->Fill(6);
		      if (mets[0]->Pt() > 20){
			cutflow->Fill(7, weight);
			cutflow_raw->Fill(7);
			if(selectedJets.size()==1){
			  TRootJet* jet = (TRootJet*) selectedJets[0];
			  if (jet->Pt() > 30){
			    cutflow->Fill(8, weight);
			    cutflow_raw->Fill(8);
			    if (BTjet){
			      cutflow->Fill(9,weight);
			      cutflow_raw->Fill(9);
			      
			      double ptSysPx = lepton0.Px() + lepton1.Px() + jet->Px() + mets[0]->Px();
			      double ptSysPy = lepton0.Py() + lepton1.Py() + jet->Py() + mets[0]->Py();
			      double ptSys = sqrt(ptSysPx*ptSysPx + ptSysPy*ptSysPy);
			      double Ht = lepton0.Pt() + lepton1.Pt() + jet->Pt() + mets[0]->Pt(); 
			      
			      if (ptSys < 60){
				cutflow->Fill(10, weight);
				cutflow_raw->Fill(10);
				if (Ht > 160){
				  cutflow->Fill(11, weight);
				  cutflow_raw->Fill(11);
				}
			      }
			    }
			  }
			}
		      }
		    }      
		  }
		}
	      }
	    }
	  }
	  
	  
	} // event loop
    } // dataset loop
  
  
  cout << "*************************************************" << endl;
  cout << "Results: " <<  endl;
  cout << "*************************************************" << endl;
  cout << "All:       " <<  cutflow->GetBinContent(2) << " +/- "  << cutflow->GetBinError(2) << endl;
  cout << "HLT:       " <<  cutflow->GetBinContent(3) << " +/- "  << cutflow->GetBinError(3) << endl;
  cout << "PV:        " <<  cutflow->GetBinContent(4) << " +/- "  << cutflow->GetBinError(4) << endl;
  cout << "Lep. Sel:  " <<  cutflow->GetBinContent(5) << " +/- "  << cutflow->GetBinError(5) << endl;
  cout << "Lep. Veto: " <<  cutflow->GetBinContent(6) << " +/- "  << cutflow->GetBinError(6) << endl;
  cout << "mll:       " <<  cutflow->GetBinContent(7) << " +/- "  << cutflow->GetBinError(7) << endl;
  cout << "MET:       " <<  cutflow->GetBinContent(8) << " +/- "  << cutflow->GetBinError(8) << endl;
  cout << "1 jet:     " <<  cutflow->GetBinContent(9) << " +/- "  << cutflow->GetBinError(9) << endl;
  cout << "1 jet BT:  " <<  cutflow->GetBinContent(10) << " +/- " << cutflow->GetBinError(10) << endl;
  cout << "Pt tW:     " <<  cutflow->GetBinContent(11) << " +/- " << cutflow->GetBinError(11) << endl;
  cout << "Ht:        " <<  cutflow->GetBinContent(12) << " +/- " << cutflow->GetBinError(12) << endl;
  cout << "*************************************************" << endl; 
  
  fout->Write();
  fout->Close();
  
  
  cout << "It took you " << ((double)clock() - start) / CLOCKS_PER_SEC << " to run the program" << endl;
  
}

