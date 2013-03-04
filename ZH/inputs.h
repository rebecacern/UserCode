#include "../Smurf/Core/SmurfTree.h"
#include "../Smurf/Analysis/HWWlvlv/factors.h"
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

const int verboseLevel =   1;

//Fixed quantities
const double mz = 91.1876;
const double mw = 80.4;
const double mmu = 0.105;
const double lumi = 19.467;

//kinamtic cuts
const double ptleadingcut = 30; //leading jet
const double separation = 15; //15 is the chosen cut
const double metcut = -10; 
const double mtcut = 85;
const double separationjj = 60; //60
const double phicut = 1.8; // 1.8

//Other
const double factor = 1; //Fake factor

//Syst. values 
double JESerr = 0.10; // 0.05 also

//Histos
int nbins = 20;
double nbinlow = 0;
double nbinhigh = 400;


double precision(double value){
  
  int precisionValue;
  if (value == 0) precisionValue = 0;
  else if (value < 10) precisionValue = 4;
  else precisionValue = 5;
  
  return precisionValue;
  
}


