#include "../interface/Selection.h"

//____CONSTRUCTORS______________________________________________________//

Selection::Selection() {
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setPlainElectronCuts();
}

Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootMET*>& mets_){
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setPlainElectronCuts();
  for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
  for(unsigned int i=0;i<muons_.size();i++) muons.push_back(muons_[i]);
  for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);
}

Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_){
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setPlainElectronCuts();
  for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
  for(unsigned int i=0;i<muons_.size();i++) muons.push_back(muons_[i]);
  for(unsigned int i=0;i<electrons_.size();i++) electrons.push_back(electrons_[i]);
  for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);
}

Selection::Selection(const Selection& s) {
  // copy the objects
  jets = s.jets;
  electrons = s.electrons;
  muons = s.muons;
  mets = s.mets;
  // copy the cutvalues
  setJetCuts(s.JetPtThreshold_,s.JetEtaThreshold_,s.JetEMFThreshold_,s.n90HitsThreshold_,s.fHPDThreshold_,s.DRJetElectron_,s.DRJetMuon_);
  setMuonCuts(s.MuonPtThreshold_,s.MuonEtaThreshold_,s.MuonRelIso_,s.MuonNofValidHits_,s.Muond0Cut_,s.MuonDRJetsCut_,s.MuonNMatches_,s.MuonDistVzPVz_,s.MuonNPixelLayersWithMeasurement_);
  setDiMuonCuts(s.MuonPtThreshold_,s.MuonEtaThreshold_,s.MuonRelIso_,s.MuonNofValidHits_,s.Muond0Cut_,s.MuonDRJetsCut_);
  setLooseMuonCuts(s.JetPtThreshold_,s.JetEtaThreshold_,s.MuonRelIso_);
  setElectronCuts(s.ElectronEtThreshold_,s.ElectronEtaThreshold_,s.ElectronRelIso_,s.Electrond0Cut_,s.ElectronDistVzPVz_);
  setLooseElectronCuts(s.ElectronEtThreshold_,s.ElectronEtaThreshold_,s.ElectronRelIso_);
  setPlainElectronCuts(s.ElectronEtThreshold_,s.ElectronEtaThreshold_);
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

Selection::~Selection() {
  jets.clear();
  electrons.clear();
  muons.clear();
  mets.clear();
}

//______________________________________________________________________//


//____CUT INITIALISATION________________________________________________//

void Selection::setJetCuts(float Pt, float Eta, float EMF, float n90Hits, float fHPD, float dRJetElectron, float dRJetMuon) {
  JetPtThreshold_ = Pt;
  JetEtaThreshold_ = Eta;
  JetEMFThreshold_ = EMF;
  n90HitsThreshold_ = n90Hits;
  fHPDThreshold_ = fHPD;
  DRJetElectron_ = dRJetElectron;
  DRJetMuon_ = dRJetMuon; // only used for PFlow without PF2PAT
}

void Selection::setJetCuts() {
  setJetCuts(15.,2.4,0.01,1.,0.98,0.3,0.1); // refSelV4 values
}

void Selection::setElectronCuts(float Et, float Eta, float RelIso, float d0, float DistVzPVz) {
  ElectronEtThreshold_ = Et;
  ElectronEtaThreshold_ = Eta;
  ElectronRelIso_ = RelIso;
  Electrond0Cut_ = d0;
  ElectronDistVzPVz_ = DistVzPVz;

  // setup VBTF cuts (DO NOT TOUCH)
  cutsVBTFWP70["sigmaIEtaIEta_B"]=0.01;
  cutsVBTFWP70["deltaPhi_B"]=0.03;
  cutsVBTFWP70["deltaETA_B"]=0.004;
  cutsVBTFWP70["HoverE_B"]=0.025;
  cutsVBTFWP70["sigmaIEtaIEta_E"]=0.03;
  cutsVBTFWP70["deltaPhi_E"]=0.02;
  cutsVBTFWP70["deltaETA_E"]=0.005;
  cutsVBTFWP70["HoverE_E"]=0.025;

}

void Selection::setElectronCuts() {
  setElectronCuts(20,2.4,0.1,0.02,1);
}

void Selection::setLooseElectronCuts(float Et, float Eta, float RelIso) {
  ElectronLooseEtThreshold_ = Et;
  ElectronLooseEtaThreshold_ = Eta;
  ElectronLooseRelIso_ = RelIso;
  
  // setup VBTF cuts (DO NOT TOUCH)
  cutsVBTFWP95["sigmaIEtaIEta_B"]=0.01;
  cutsVBTFWP95["deltaPhi_B"]=0.8;
  cutsVBTFWP95["deltaETA_B"]=0.007;
  cutsVBTFWP95["HoverE_B"]=0.15;
  cutsVBTFWP95["sigmaIEtaIEta_E"]=0.03;
  cutsVBTFWP95["deltaPhi_E"]=0.7;
  cutsVBTFWP95["deltaETA_E"]=0.01;
  cutsVBTFWP95["HoverE_E"]=0.07;
}

void Selection::setLooseElectronCuts() {
  setLooseElectronCuts(15,2.5,0.2); // refSelV4 (mu) values
}

void Selection::setPlainElectronCuts(float Et, float Eta) {
  ElectronLooseEtThreshold_ = Et;
  ElectronLooseEtaThreshold_ = Eta;
}

void Selection::setPlainElectronCuts() {
  setPlainElectronCuts(10,2.5);
}

void Selection::setMuonCuts(float Pt, float Eta, float RelIso, int NValidHits, float d0, float DRJets, int NMatches, float DistVzPVz, int NPixelLayersWithMeas) {
  MuonPtThreshold_ = Pt;
  MuonEtaThreshold_ = Eta;
  MuonRelIso_ = RelIso;
  MuonNofValidHits_ = NValidHits;
  Muond0Cut_ = d0;
  MuonDRJetsCut_ = DRJets;
  MuonNMatches_ = NMatches;
  MuonDistVzPVz_ = DistVzPVz;
  MuonNPixelLayersWithMeasurement_ = NPixelLayersWithMeas;
}

void Selection::setMuonCuts() {
  setMuonCuts(20,2.4,0.05,10,0.02,0.3,1,1,1); // refSelV4 values
}

void Selection::setDiMuonCuts(float Pt, float Eta, float RelIso, int NValidHits, float d0, float DRJets) {
  MuonPtThreshold_ = Pt;
  MuonEtaThreshold_ = Eta;
  MuonRelIso_ = RelIso;
  MuonNofValidHits_ = NValidHits;
  Muond0Cut_ = d0;
  MuonDRJetsCut_ = DRJets;
}

void Selection::setDiMuonCuts() {
  setDiMuonCuts(20,2.4,0.15,10,0.02,0.3); // refSelV4 values
}

void Selection::setLooseMuonCuts(float Pt, float Eta, float RelIso) {
  MuonLoosePtThreshold_ = Pt;
  MuonLooseEtaThreshold_ = Eta;
  MuonLooseRelIso_ = RelIso;
}

void Selection::setLooseMuonCuts() {
  setLooseMuonCuts(10,2.5,0.2); // refSelV4 (mu+el) values
}

//______________________________________________________________________//

//__EXTRA METHODS_______________________________________________________//

bool passVBTFID(TRootElectron* el, std::map<std::string,float> cuts) {

  //try the cuts manually
  bool passvbtf = false;
  
  float sigmaiEtaiEta = el->sigmaIEtaIEta();
  float deltaPhi = el->deltaPhiIn();
  float deltaETA = el->deltaEtaIn();
  float HoverE = el->hadronicOverEm();

  if (fabs(el->superClusterEta()) < 1.4442)
    if (sigmaiEtaiEta <  cuts["sigmaIEtaIEta_B"])
      if (fabs(deltaPhi) < cuts["deltaPhi_B"])
	if (fabs(deltaETA) < cuts["deltaETA_B"])
	  if (HoverE < cuts["HoverE_B"])
	    passvbtf=true;
  
  if (fabs(el->superClusterEta()) > 1.5660)
    if (sigmaiEtaiEta <  cuts["sigmaIEtaIEta_E"])
      if (fabs(deltaPhi) < cuts["deltaPhi_E"])
	if (fabs(deltaETA) < cuts["deltaETA_E"])
	  if (HoverE < cuts["HoverE_E"])
	    passvbtf=true;

  return passvbtf;

}

//______________________________________________________________________//

//____SELECTION GETTERS_________________________________________________//

  // ______________JETS_________________________________________________//

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID) const{
  std::vector<TRootJet*> selectedJets;
  
  for(unsigned int i=0;i<jets.size();i++){

    TRootJet* init_jet = (TRootJet*) jets[i];

    if (init_jet->jetType() == 1) { // CaloJets

      const TRootCaloJet* CaloJet = static_cast<const TRootCaloJet*>(init_jet);
      
      if(fabs(CaloJet->Eta())<EtaThr && CaloJet->Pt()>PtThr) {

	if( applyJetID ) {
	  if ( CaloJet->ecalEnergyFraction() > JetEMFThreshold_) {
	    if (CaloJet->fHPD() < fHPDThreshold_ ) {
	      if (CaloJet->n90Hits()> n90HitsThreshold_ ) {
		selectedJets.push_back(init_jet);
	      }
	    }
	  }
	}
	else selectedJets.push_back(init_jet);
	  
      }
    }

    else if (init_jet->jetType() == 2) { // PFJets
      
      const TRootPFJet* PFJet = static_cast<const TRootPFJet*>(init_jet);

      if(fabs(PFJet->Eta())<JetEtaThreshold_ && PFJet->Pt()>JetPtThreshold_)
	if ( applyJetID ) {
	  if (PFJet->nConstituents() > 1 )
	    if (PFJet->chargedEmEnergyFraction() < 0.99 )
	      if (PFJet->neutralHadronEnergyFraction() < 0.99 )
		if (PFJet->neutralEmEnergyFraction() < 0.99 )
		  if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedHadronEnergyFraction() > 0) 
		    if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedMultiplicity() > 0) 
		      selectedJets.push_back(init_jet);
	} else selectedJets.push_back(init_jet);
    }

    else if (init_jet->jetType() == 3) { // JPTJets
      
      const TRootJPTJet* JPTJet = static_cast<const TRootJPTJet*>(init_jet);

      if(fabs(JPTJet->Eta())<JetEtaThreshold_ && JPTJet->Pt()>JetPtThreshold_)
	if( applyJetID ) {
	  if ( JPTJet->ecalEnergyFraction() > JetEMFThreshold_) {
	    if (JPTJet->fHPD() < fHPDThreshold_ ) {
	      if (JPTJet->n90Hits()> n90HitsThreshold_ ) {
		selectedJets.push_back(init_jet);
	      }
	    }
	  }
	}
	else selectedJets.push_back(init_jet);
    }
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TLorentzVector*> lepton, float dRLeptonJet, bool applyJetID) const {
  std::vector<TRootJet*> init_jets = GetSelectedJets(PtThr,EtaThr,applyJetID);
  std::vector<TRootJet*> selectedJets;
  for (unsigned int i=0; i<init_jets.size(); i++) {
    float minDrElJet = 9999999999.;
    for (unsigned int j=0; j<lepton.size(); j++) {
      TLorentzVector* l = (TLorentzVector*) lepton[j];
      float dr = l->DeltaR(*init_jets[i]);
      if ( dr < minDrElJet ) minDrElJet = dr;
    }
  
    if (minDrElJet >= dRLeptonJet)
      selectedJets.push_back(init_jets[i]);
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TRootMuon*> muons, float dRLeptonJet, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<muons.size(); i++)
    leptons.push_back((TLorentzVector*)muons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetMuon_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TRootElectron*> electrons, float dRLeptonJet, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<electrons.size(); i++)
    leptons.push_back((TLorentzVector*)electrons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetElectron_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(vector<TRootMuon*> muons, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<muons.size(); i++)
    leptons.push_back((TLorentzVector*)muons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetMuon_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(vector<TRootElectron*> electrons, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<electrons.size(); i++)
    leptons.push_back((TLorentzVector*)electrons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetElectron_,applyJetID);
}


std::vector<TRootJet*> Selection::GetSelectedJets(bool applyJetID) const{
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedBJets(const std::vector<TRootJet*>& seljets, int& btagAlgo, float& btagCut) const{
  std::vector<TRootJet*> bjets;
  Float_t btagDisc = 0;
  for(unsigned int i = 0; i<seljets.size(); i++) {
    if(     btagAlgo == 0) btagDisc = seljets[i]->btag_trackCountingHighEffBJetTags();
    else if(btagAlgo == 1) btagDisc = seljets[i]->btag_trackCountingHighPurBJetTags();
    else if(btagAlgo == 2) btagDisc = seljets[i]->btag_jetProbabilityBJetTags();
    else if(btagAlgo == 3) btagDisc = seljets[i]->btag_jetBProbabilityBJetTags();
    else if(btagAlgo == 4) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighEffBJetTags();
    else if(btagAlgo == 5) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighPurBJetTags();
    else if(btagAlgo == 6) btagDisc = seljets[i]->btag_combinedSecondaryVertexBJetTags();
    else                   btagDisc = -999999;
    if(btagDisc>btagCut) bjets.push_back(seljets[i]);
  }
  return bjets;
}

  // ______________MUONS________________________________________________//

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    if(muons[i]->isGlobalMuon() && muons[i]->isTrackerMuon() && muons[i]->idGlobalMuonPromptTight() 
       && muons[i]->Pt()>PtThr 
       && fabs(muons[i]->Eta())<EtaThr 
       && muons[i]->relativeIso03() < MuonRelIso 
       && fabs(muons[i]->d0()) <Muond0Cut_
       && muons[i]->nofValidHits()>MuonNofValidHits_ 
       && muons[i]->nofMatches()>MuonNMatches_ 
       && muons[i]->nofPixelLayersWithMeasurement() >= MuonNPixelLayersWithMeasurement_ 
       ) {

      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons() const{
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso, vector<TRootJet*> selJets) const{
  std::vector<TRootMuon*> selectedMuons;
  std::vector<TRootMuon*> init_muons=GetSelectedMuons(PtThr,EtaThr,MuonRelIso);
  for(unsigned int i=0;i<init_muons.size();i++){
    float mindRMuJet = 999999.;
    TRootJet* jet;
    for(unsigned int j=0;j<selJets.size();j++) {
      jet = selJets.at(j);
      float dRMuJet = init_muons[i]->DeltaR(*jet);
      if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    if(mindRMuJet > MuonDRJetsCut_){
      selectedMuons.push_back(init_muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, TRootVertex* vertex, vector<TRootJet*> selJets) const {
  std::vector<TRootMuon*> selectedMuons;
  std::vector<TRootMuon*> init_muons=GetSelectedMuons(PtThr,EtaThr,MuonRelIso,selJets);
  for(unsigned int i=0;i<init_muons.size();i++){
    TRootMuon* mu = (TRootMuon*) init_muons[i];
    if ( fabs(mu->vz() - vertex->Z()) < MuonDistVzPVz_) {
      selectedMuons.push_back(init_muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(TRootVertex* vertex) const {
  std::vector<TRootJet*> empty;
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_,vertex,empty);
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(TRootVertex* vertex, vector<TRootJet*> selJets) const {
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_,vertex,selJets);
}

std::vector<TRootMuon*> Selection::GetSelectedDiMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    if(muons[i]->isGlobalMuon() && muons[i]->isTrackerMuon() && muons[i]->idGlobalMuonPromptTight() 
       && muons[i]->Pt()>PtThr 
       && fabs(muons[i]->Eta())<EtaThr 
       && muons[i]->relativeIso03() < MuonRelIso 
       && fabs(muons[i]->d0()) <Muond0Cut_
       && muons[i]->nofValidHits()>MuonNofValidHits_ 
       ) {

      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedDiMuons() const{
  return GetSelectedDiMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedDiMuons(float PtThr, float EtaThr,float MuonRelIso, vector<TRootJet*> selJets) const{
  std::vector<TRootMuon*> selectedMuons;
  std::vector<TRootMuon*> init_muons=GetSelectedDiMuons(PtThr,EtaThr,MuonRelIso);
  for(unsigned int i=0;i<init_muons.size();i++){
    float mindRMuJet = 999999.;
    TRootJet* jet;
    for(unsigned int j=0;j<selJets.size();j++) {
      jet = selJets.at(j);
      float dRMuJet = init_muons[i]->DeltaR(*jet);
      if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    if(mindRMuJet > MuonDRJetsCut_){
      selectedMuons.push_back(init_muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}



std::vector<TRootMuon*> Selection::GetSelectedLooseMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    if(muons[i]->isGlobalMuon() && fabs(muons[i]->Eta())<EtaThr && muons[i]->Pt()>PtThr && muons[i]->relativeIso03() < MuonRelIso ) {

      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedLooseMuons() const{
  return GetSelectedLooseMuons(MuonLoosePtThreshold_, MuonLooseEtaThreshold_, MuonLooseRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsNoID(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    if(fabs(muons[i]->Eta())<EtaThr && muons[i]->Pt()>PtThr && muons[i]->relativeIso03() < MuonRelIso){
	selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsInvIso(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> init_muons = GetSelectedMuons(PtThr,EtaThr,9999.);
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<init_muons.size();i++){
    //Compute isolation
    if(init_muons[i]->relativeIso03() > MuonRelIso ) 
      selectedMuons.push_back(init_muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsInvIso(float PtThr, float EtaThr,float MuonRelIso, vector<TRootJet*> selJets) const{
  std::vector<TRootMuon*> init_muons = GetSelectedMuonsInvIso(PtThr,EtaThr,MuonRelIso);
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<init_muons.size();i++){
    float mindRMuJet = 999.;
    for(unsigned int j=0;j<selJets.size();j++) {
	float dRMuJet = init_muons[i]->DeltaR(*selJets.at(j));
	if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    if(mindRMuJet> MuonDRJetsCut_){
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsNonIso(float PtThr, float EtaThr, vector<TRootJet*> selJets) const{
    return GetSelectedMuons(PtThr,EtaThr,9999.,selJets);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsNonIso() const{
    return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,9999.);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsIsoRange(float PtThr, float EtaThr,float MuonRelIsoMin, float MuonRelIsoMax, vector<TRootJet*> selJets) const{
  std::vector<TRootMuon*> init_muons = GetSelectedMuons(PtThr,EtaThr,9999.,selJets);
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<init_muons.size();i++){
    //Compute isolation
    if( MuonRelIsoMin < init_muons[i]->relativeIso03() && init_muons[i]->relativeIso03() < MuonRelIsoMax )
      selectedMuons.push_back(muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}
  
  // ______________ELECTRONS____________________________________________//

std::vector<TRootElectron*> Selection::GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso) const {
  std::vector<TRootElectron*> selectedElectrons;
  //cout << ElectronRelIso << endl;
  for(unsigned int i=0;i<electrons.size();i++){
    TRootElectron* el = (TRootElectron*) electrons[i];
    //Compute isolation
    float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();

    //try the cuts manually
    bool passvbtf = passVBTFID(el,cutsVBTFWP70);
    
    // supercluster eta cut -> EB-EE transition region
    if(el->Et() > EtThr && fabs(el->Eta())< EtaThr)
      if ( fabs(el->superClusterEta()) > 1.5660 ||  fabs(el->superClusterEta()) < 1.4442 )
	if ( fabs(el->d0()) < Electrond0Cut_ )
	  if ( RelIso < ElectronRelIso )
	    if (passvbtf)
	      if((fabs(el->Dist()) >= 0.02 || fabs(el->DCot()) >= 0.02) && el->missingHits() == 0)  
	    
	        selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedElectrons() const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_);
}

std::vector<TRootElectron*> Selection::GetSelectedElectrons(TRootVertex* vertex) const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_,vertex);
}

std::vector<TRootElectron*> Selection::GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, TRootVertex* vertex) const {
  std::vector<TRootElectron*> init_electrons = GetSelectedElectrons(EtThr,EtaThr,ElectronRelIso);
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<init_electrons.size();i++){
    TRootElectron* el = (TRootElectron*) init_electrons[i];
    if ( fabs(el->vz() - vertex->Z()) < ElectronDistVzPVz_) {
      selectedElectrons.push_back(init_electrons[i]);
    }
  }

  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedLooseElectrons(float EtThr, float EtaThr, float ElectronRelIso, bool vbtfid) const {
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<electrons.size();i++){
    TRootElectron* el = (TRootElectron*) electrons[i];
    //Compute isolation
    float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();

    //try the cuts manually
    bool passvbtf = passVBTFID(el,cutsVBTFWP95);
    
    if(el->Et() > EtThr && fabs(el->Eta()) < EtaThr  && RelIso < ElectronRelIso)
      if ( !vbtfid || ( vbtfid && fabs(el->superClusterEta()) > 1.5660 ||  fabs(el->superClusterEta()) < 1.4442 ) )
	if (!vbtfid || (vbtfid && passvbtf))
	  selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedLooseElectrons(bool vbtfid) const {
  return GetSelectedLooseElectrons(ElectronLooseEtThreshold_, ElectronLooseEtaThreshold_, ElectronLooseRelIso_,vbtfid);
}


std::vector<TRootElectron*> Selection::GetSelectedPlainElectrons(float EtThr, float EtaThr) const {
  std::vector<TRootElectron*> selectedPlainElectrons;
  for(unsigned int i=0;i<electrons.size();i++){
    TRootElectron* el = (TRootElectron*) electrons[i];
    //Compute isolation
  
    
    if(el->Et() > EtThr && fabs(el->Eta()) < EtaThr)
   
	  selectedPlainElectrons.push_back(electrons[i]);
  }
  std::sort(selectedPlainElectrons.begin(),selectedPlainElectrons.end(),HighestPt());
  return selectedPlainElectrons;
}

//____________IS SELECTED_______________________________________________//

bool Selection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut){
  if(vertex.size()==0) return false;
  float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
  if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
  return false;
}

bool Selection::isSelected(unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
  	if(GetSelectedMuons().size()==1 && GetSelectedElectrons().size()==0 && GetSelectedJets().size()>= Njets) return true;
  }
  else{
  	if(GetSelectedMuons().size()>0 && GetSelectedJets().size()>= Njets) return true;
  }
  return false;
}

bool Selection::isSelected(float JetPtThr, unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
  	if(GetSelectedMuons().size()==1 && GetSelectedElectrons().size()==0 && GetSelectedJets(JetPtThr, JetEtaThreshold_, JetEMFThreshold_).size()>= Njets) return true;
  }
  else{
  	if(GetSelectedMuons().size()>0 && GetSelectedJets(JetPtThr, JetEtaThreshold_, JetEMFThreshold_).size()>= Njets) return true;
  }
  return false;
}

bool Selection::isSelected(float PtThrJets, float EtaThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float PtThrElectrons, float EtaThrElectrons, float ElectronRelIso, unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
  	if(GetSelectedMuons(PtThrMuons, EtaThrMuons, MuonRelIso, GetSelectedJets(PtThrJets, EtaThrJets, Njets)).size()==1 && GetSelectedElectrons(PtThrElectrons, EtaThrElectrons, ElectronRelIso).size()==0 && GetSelectedJets(PtThrJets, EtaThrJets, Njets).size()>=Njets) return true;
  }
  else{
  	if(GetSelectedMuons(PtThrMuons, EtaThrMuons, MuonRelIso, GetSelectedJets(PtThrJets, EtaThrJets, Njets)).size()>0 && GetSelectedJets(PtThrJets, EtaThrJets, Njets).size()>=Njets) return true;
  }
  return false;
}

//______________________________________________________________________//
