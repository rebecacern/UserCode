// rebeca@cern.ch
// Basic latex table
// December 2012
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TFile.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

void tables(){
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetErrorX(0);
  
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gROOT->SetBatch(1);
    
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetLabelFont(18,"");
  
  char myRootFile[300];
  sprintf(myRootFile,"rootfiles/signal_study.root");
  TFile *_file0 = TFile::Open(myRootFile);
  
  double lumi = 18.8;
  char myTexFile[300];
  sprintf(myTexFile,"tables/table_%dpb.tex", lumi);
  ofstream output(myTexFile); 
  
  const int nProcess = 7;
  TString processName[nProcess] =  { "sig_cuts_zh", "data_cuts", "bck_cuts", "bck_cuts_wz", "bck_cuts_fakes", "bck_cuts_zz","bck_cuts_www"};
  TString processTitle[nProcess] = { "ZH, HWW", "data", "background", "WZ", "fakes", "ZZ", "tri-bosons"};
 
  const int nCuts = 8;
  TString cutLabel[nCuts] = { "3 lep", "OSSF", "2 jets", "$m_{ll}$", "$E_{T}^{miss}$", "$m_{T}$", "$m_{jj}$", "$\\Delta\\Phi_{jj-l\\nu}$"};
 
   
  TH1F*  h [nProcess];
  for(int i=0; i<nProcess; i++){
    h[i] = (TH1F*) _file0->Get(processName[i]);
  }

 
  output << "\\documentclass{cmspaper}" << endl;
  output << "\\begin{document}" << endl;
  output << endl;
  output << endl;
  
  
  output << "  \\begin{table}" << endl;
  output << "  \\begin{center}" << endl;
  output << "  \\begin{tabular} {|l|c|c|c|c|c|c|c|c|c|}" << endl;
  output << "  \\hline " << endl;
 
  for (int i = 0; i < nProcess; i++){
    output << " & " << processTitle[i] ;
  }
  output << "  \\\\ " << endl; 
  output << "  \\hline " << endl;
  
  for (int i=1; i < nCuts+1; i++){
    output << cutLabel[i-1];
    for (int iProcess = 0; iProcess < nProcess; iProcess++){
    

      if (h[iProcess]->GetBinContent(i)!=0 && iProcess != 1){
	output << " & " << std::setiosflags(std::ios::fixed) << setprecision(precision(h[iProcess]->GetBinError(i))) << h[iProcess]->GetBinContent(i) ; 
	output << " $\\pm $"  << setprecision(precision(h[iProcess]->GetBinError(i))) << h[iProcess]->GetBinError(i);
       
      } else if (h[iProcess]->GetBinContent(i) !=0)
	output << " & " << std::setiosflags(std::ios::fixed) << setprecision(precision(h[iProcess]->GetBinError(i))) << h[iProcess]->GetBinContent(i) ; 
      else if (i > 1) { if (h[iProcess]->GetBinContent(i-1) )
	  output << " & $\\leq$ " << setprecision(h[iProcess]->GetBinError(i-1)) << 2*h[iProcess]->GetBinError(i-1);}
      else  output << " & - " ;
     
    }
    output <<  " \\\\  " << endl; 
  }
  output << "   \\hline " << endl;
  output << "  \\end{tabular}" << endl;
  output << "  \\end{center}" << endl;
  output << "  \\end{table}" << endl;
  
  output << endl;
  output << endl;
  
  
  output << "\\end{document}" << endl;
  
}



double normalization(double nevents, double xsec, double lumi){
  
  if (nevents !=0) return lumi*xsec/nevents;
  else return 1;
  
}

int precision(double error){
  
  int precisionValue;
  double factErr = 0; 
  int iN = 0;
  if (error == 0 || error >= 1) precisionValue = 0;
  else if (error < 1) {
    iN = 0;
    factErr = 0; 
    while (factErr < 1){
      factErr = error*(10**iN);
      iN++;  
    }
    precisionValue = iN-1;
  }
  
  if (factErr > 9.5) precisionValue-=1;
  
  return precisionValue;
  
}

double errorEfi (double efi, double errTotal, double totalEvents, double error, double number){
  
  if (totalEvents !=0 && number != 0) return efi*((errTotal/totalEvents)+(error/number));
  else return 0;
  
}

double efficiency (double finalevents, double totalevents){
  
  if (totalevents !=0) return finalevents*100/totalevents;
  else return 0;
  
}
