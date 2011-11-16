// @(#)root/tmva $Id: TMVAClassification.C 37399 2010-12-08 15:22:07Z evt $
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void TMVAClassificationCompact()
{

  // ---------------------------------------------------------------
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassification" << std::endl;
  // --------------------------------------------------------------------------------------------------
  
  // --- Here the preparation phase begins
  
  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString TrainName = "test_tw_short_00";
  
  TString outfileName( "trainrootfiles/"+TrainName+".root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
  
  // Create the factory object. Later you can choose the methods
  // whose performance you'd like to investigate. The factory is 
  // the only TMVA object you have to interact with
  //
  // The first argument is the base of the name of all the
  // weightfiles in the directory weight/
  //
  // The second argument is the output file for the training results
  // All TMVA output can be suppressed by removing the "!" (not) in
  // front of the "Silent" argument in the option string
  TMVA::Factory *factory = new TMVA::Factory( TrainName, outputFile,
					      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
  
  // If you wish to modify default settings
  // (please check "src/Config.h" to see all available global options)
  //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
  //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
  
  // Define the input variables that shall be used for the MVA training
  // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
  // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
  
  /*factory->AddVariable ("jetpt", 'F');
  factory->AddVariable ("jeteta", 'F');
  factory->AddVariable ("jetphi", 'F');
  factory->AddVariable ("metpt", 'F');
  factory->AddVariable ("metpro",'F');
  factory->AddVariable ("lep0pt",'F');
  factory->AddVariable ("lep1pt",'F');
  factory->AddVariable ("lep0eta",'F');
  factory->AddVariable ("lep1eta",'F');
  factory->AddVariable ("lep0phi",'F');
  factory->AddVariable ("lep1phi",'F');*/
  factory->AddVariable ("ptsys",'F');
/*  factory->AddVariable ("ht",'F');
  factory->AddVariable ("oblateness", 'F');
  factory->AddVariable ("sphericity", 'F');
  factory->AddVariable ("aplanarity", 'F');
  factory->AddVariable ("njetw", 'F');
  factory->AddVariable ("sqrts", 'F');*/
  factory->AddVariable ("deltarleps", 'F');
/*  factory->AddVariable ("deltaphileps", 'F');
  factory->AddVariable ("deltaetaleps", 'F');
  factory->AddVariable ("philepmetclose", 'F');
  factory->AddVariable ("philepmetfar", 'F');*/
  factory->AddVariable ("rlepmetclose", 'F');
 /* factory->AddVariable ("rlepmetfar", 'F');
  factory->AddVariable ("philepjetclose", 'F');
  factory->AddVariable ("philepjetfar", 'F');*/
  factory->AddVariable ("rlepjetclose", 'F');
 /* factory->AddVariable ("rlepjetfar", 'F');
  factory->AddVariable ("phijetmet", 'F');*/
  factory->AddVariable ("rjetmet", 'F');
 /* factory->AddVariable ("mll", 'F');
  factory->AddVariable ("htnomet", 'F');
  factory->AddVariable ("ptsysnomet", 'F');
  factory->AddVariable ("metphi", 'F');
  factory->AddVariable ("metminusptsysnomet", 'F');*/
  
  //Load the signal and background event samples from ROOT trees
  
  TFile *inputSTrain(0);
  TFile *inputSTrain(0);
  
  TFile *inputBTrain(0);
  TFile *inputBTest(0);
  
  TString sigFileTrain  = "rootfiles/tmva_train_0_tw_dr.root";
  TString sigFileTest   = "rootfiles/tmva_test_0_tw_dr.root";
  
  TString bkgFileTrain = "rootfiles/tmva_train_0_tt.root";
  TString bkgFileTest  = "rootfiles/tmva_test_0_tt.root";
  
  inputSTrain  = TFile::Open( sigFileTrain );
  inputSTest   = TFile::Open( sigFileTest );
  
  inputBTrain = TFile::Open( bkgFileTrain );
  inputBTest  = TFile::Open( bkgFileTest );
  
  
  std::cout << "--- TMVAnalysis    : Accessing Signal Train: " << sigFileTrain << std::endl;
  std::cout << "--- TMVAnalysis    : Accessing Signal Test : " << sigFileTest << std::endl;
  std::cout << "--- TMVAnalysis    : Accessing Background Train: " << bkgFileTrain << std::endl;
  std::cout << "--- TMVAnalysis    : Accessing Background Test: " << bkgFileTest << std::endl;

  TTree *signalTrain      = (TTree*)inputSTrain->Get("myTree");
  TTree *signalTest       = (TTree*)inputSTest->Get("myTree");
  
  TTree *backgroundTrain = (TTree*)inputBTrain->Get("myTree");
  TTree *backgroundTest  = (TTree*)inputBTest->Get("myTree");
  
  factory->AddSignalTree( signalTrain, 1, "Training");
  factory->AddSignalTree( signalTest , 1, "Test");
  
  factory->AddBackgroundTree( backgroundTrain, 1, "Training");
  factory->AddBackgroundTree( backgroundTest,  1, "Test" );
  
  // Set xs-weight
  factory->SetSignalWeightExpression    ("weight");
  factory->SetBackgroundWeightExpression("weight");
  
  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = "";
  TCut mycutb = ""; 
  
  Int_t nSignalTrain = signalTrain->GetEntries();
  Int_t nSignalTest  = signalTest->GetEntries();
  
  Int_t nBackTrain  = backgroundTrain->GetEntries();
  Int_t nBackTest   = backgroundTest->GetEntries();
  
  factory->PrepareTrainingAndTestTree( mycuts, mycutb,
				       //":NSigTrain=:NBkgTrain=:NSigTest=:NBkgTest=:SplitMode=Alternate:!V" );  
				       //":nTrain_Signal=10000:nTest_Signal=2260:nTrain_Background=100000:nTest_Background=100000:SplitMode=Alternate:!V" );  
				       ":nTrain_Signal=nSignalTrain:nTest_Signal=nSignalTest:nTrain_Background=nBackTrain:nTest_Background=nBackTest:SplitMode=Block:!V");
  //":nTrain_Signal=nSignalTrain:nTest_Signal=nSignalTest:nTrain_Background=nBackTrain:nTest_Background=nBackTest:SplitMode=Block:!V");
  //":SplitMode=Alternate:!V");
  //":nTrain_Signal=:nTest_Signal=:nTrain_Background=:nTest_Background=:SplitMode=Block:!V");
  //":SplitMode=Alternate:!V" );  
  
  // ---- Book MVA methods
  factory->BookMethod( TMVA::Types::kBDT, "BDT",
		       "!H:!V:NTrees=400:nEventsMin=40:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=4.5" );
  
  
  
  // ---- Now you can tell the factory to train, test, and evaluate the MVAs
  
  // Train MVAs using the set of training events
  factory->TrainAllMethods();
  
   // ---- Evaluate all MVAs using the set of test events
  factory->TestAllMethods();
  
  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();
  
  // --------------------------------------------------------------
  
  // Save the output
  outputFile->Close();
  
  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;
  
  delete factory;
  
  // Launch the GUI for the root macros
  if (!gROOT->IsBatch()) TMVAGui( outfileName );
}

//SetSignalWeightExpression
