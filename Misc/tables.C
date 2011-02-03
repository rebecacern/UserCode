//Rebeca Gonzalez Suarez
//rebeca@cern.ch

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
  
  bool corrector = true;
  char myTexFile[300];
  sprintf(myTexFile,"tables/table_36pb_corrected.tex");
  
  char myName[300];
  sprintf(myName,"2010");
  
  char myRootFile[300];
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file0 = TFile::Open(myRootFile);
  TH1F* histo_0 = cutflow->Clone();
  
  sprintf(myName,"tt");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file1 = TFile::Open(myRootFile);
  TH1F* histo_1 = cutflow->Clone();
  
  sprintf(myName,"wjets");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file2 = TFile::Open(myRootFile);
  TH1F* histo_2 = cutflow->Clone();
  
  sprintf(myName,"qcd");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file3 = TFile::Open(myRootFile);
  TH1F* histo_3 = cutflow->Clone();
  
  sprintf(myName,"zjets");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file4 = TFile::Open(myRootFile);
  TH1F* histo_4 = cutflow->Clone();
  
  sprintf(myName,"st_t");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file5a = TFile::Open(myRootFile);
  TH1F* histo_5a = cutflow->Clone();
  
  sprintf(myName,"st_tw");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file5 = TFile::Open(myRootFile);
  TH1F* histo_5 = cutflow->Clone();
  
  sprintf(myName,"vqq");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file6 = TFile::Open(myRootFile);
  TH1F* histo_6 = cutflow->Clone();
  
  sprintf(myName,"mc");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file7 = TFile::Open(myRootFile);
  TH1F* histo_7 = cutflow->Clone();
  
 
  if(corrector){
    double factor = 0.982*0.950;
    double ttfactor = 0.108*9*0.676*1.5*0.982*0.950;
    double stfactor = 0.108*9*0.982*0.950;
    histo_1->Scale(ttfactor);
    histo_2->Scale(factor);
    histo_3->Scale(factor);
    histo_4->Scale(factor);
    histo_5->Scale(stfactor);
    histo_5->Add(histo_5a);
    histo_6->Scale(factor);
  } 
 
  double vec_tor[8][17][4];
  
  for (int i = 0; i < 16; i++){
    
    vec_tor[0][i][0] = histo_0->GetBinContent(i);
    double temp_error = histo_0->GetBinError(i);
    vec_tor[0][i][1] = precision(temp_error);
    vec_tor[0][i][2] = histo_0->GetBinError(i);
    
    vec_tor[1][i][0] = histo_1->GetBinContent(i);
    temp_error = histo_1->GetBinError(i);
    vec_tor[1][i][1] = precision(temp_error);
    vec_tor[1][i][2] = histo_1->GetBinError(i);
    
    vec_tor[2][i][0] = histo_2->GetBinContent(i);
    temp_error = histo_2->GetBinError(i);
    vec_tor[2][i][1] = precision(temp_error);
    vec_tor[2][i][2] = histo_2->GetBinError(i);
    
    vec_tor[3][i][0] = histo_3->GetBinContent(i);
    temp_error = histo_3->GetBinError(i);
    vec_tor[3][i][1] = precision(temp_error);
    vec_tor[3][i][2] = histo_3->GetBinError(i);
    
    vec_tor[4][i][0] = histo_4->GetBinContent(i);
    temp_error = histo_4->GetBinError(i);
    vec_tor[4][i][1] = precision(temp_error);
    vec_tor[4][i][2] = histo_4->GetBinError(i);
    
    vec_tor[5][i][0] = histo_5->GetBinContent(i);
    temp_error = histo_5->GetBinError(i);
    vec_tor[5][i][1] = precision(temp_error);
    vec_tor[5][i][2] = histo_5->GetBinError(i);
    
    vec_tor[6][i][0] = histo_6->GetBinContent(i);
    temp_error = histo_6->GetBinError(i);
    vec_tor[6][i][1] = precision(temp_error);
    vec_tor[6][i][2] = histo_6->GetBinError(i);
    
    vec_tor[7][i][0] = histo_7->GetBinContent(i);
    temp_error = histo_7->GetBinError(i);
    vec_tor[7][i][1] = precision(temp_error);
    vec_tor[7][i][2] = histo_7->GetBinError(i);
  }
  
  
  
  
  ofstream salida_tex(myTexFile); 
  
  salida_tex << "\\documentclass{cmspaper}" << endl;
  salida_tex << "\\begin{document}" << endl;
  salida_tex << endl;
  salida_tex << "  \\begin{table}" << endl;
  salida_tex << "  \\begin{center}" << endl;
  salida_tex << "  \\begin{tabular} {|l|c|c|c|c|c|c|c|c|}" << endl;
  salida_tex << "  \\hline " << endl;
  
  salida_tex << "    & \\textbf{$Data$} & \\textbf{$t \\bar{t}$} & \\textbf{$W+jets$} &  \\textbf{$QCD$} &\\textbf{$Z+jets$} & \\textbf{$single$ $top$} & \\textbf{$VQQ$} \\\\ " << endl; 
  
  
  salida_tex << "  \\hline " << endl;
  
  for (int i=1; i < 16; i++){
    
    char label[300];
    if (i==5) sprintf(label,"1 electron");
    if (i==6) sprintf(label,"Muon Veto");
    if (i==7) sprintf(label,"Z Veto");
    if (i==9) sprintf(label,"Conv. Veto");
    if (i==12) sprintf(label,"$\\geq$ 3 jets");
    if (i==13) sprintf(label,"$\\geq$ 4 jets");
    if (i==14) sprintf(label,"$\\geq$ 3 jets + bt");
    if (i==15) sprintf(label,"$\\geq$ 4 jets + bt");
    
    if (i == 5 || i == 6 || i == 7 || i == 9 || i == 12 || i == 13 || i == 14 || i == 15){
    if (i == 5 || i == 14 )salida_tex << " \\hline " << endl;
    salida_tex << label;
    
    for (int j = 0; j < 7; j++){
      if (i != 0 && vec_tor[j][i][0] == 0 && vec_tor[j][i-1][0] != 0){
        salida_tex << " & $\\leq$ " << setprecision(vec_tor[j][i-1][1]) << 2*vec_tor[j][i-1][2];
      } else if (i != 0 && vec_tor[j][i][0] == 0 && vec_tor[j][i-1][0] == 0){
        salida_tex << " & $-$ " ;
      } else {
	salida_tex << " & " << std::setiosflags(std::ios::fixed) << setprecision(vec_tor[j][i][1])<< vec_tor[j][i][0] ; 
	salida_tex << " $\\pm $"  << setprecision(vec_tor[j][i][1])<< vec_tor[j][i][2];

      }
    }
    salida_tex <<  " \\\\  " << endl; 
    }
  }
  
  salida_tex << "   \\hline " << endl;
  salida_tex << "  \\end{tabular}" << endl;
  salida_tex << "  \\end{center}" << endl;
  salida_tex << "  \\end{table}" << endl;
  
  salida_tex << endl;
  salida_tex << "  \\begin{table}" << endl;
  salida_tex << "  \\begin{center}" << endl;
  salida_tex << "  \\begin{tabular} {|l|c|c|}" << endl;
  salida_tex << "  \\hline " << endl;
  
  salida_tex << "    & \\textbf{$Data$} & \\textbf{$Monte$ $Carlo$} \\\\ " << endl; 
  
  
  salida_tex << "  \\hline " << endl;
  
  for (int i=1; i < 16; i++){
    
    char label[300];
    if (i==5) sprintf(label,"1 electron");
    if (i==6) sprintf(label,"Muon Veto");
    if (i==7) sprintf(label,"Z Veto");
    if (i==9) sprintf(label,"Conv. Veto");
    if (i==12) sprintf(label,"$\\geq$ 3 jets");
    if (i==13) sprintf(label,"$\\geq$ 4 jets");
    if (i==14) sprintf(label,"$\\geq$ 3 jets + bt");
    if (i==15) sprintf(label,"$\\geq$ 4 jets + bt");
    
    if (i == 5 || i == 6 || i == 7 || i == 9 || i == 12 || i == 13 || i == 14 || i == 15){
    if (i == 5 || i == 14 )salida_tex << " \\hline " << endl;
    salida_tex << label;
    
    for (int j = 0; j < 8; j++){
    if (j == 1) j = 7;
      if (i != 0 && vec_tor[j][i][0] == 0 && vec_tor[j][i-1][0] != 0){
        salida_tex << " & $\\leq$ " << setprecision(vec_tor[j][i-1][1]) << 2*vec_tor[j][i-1][2];
      } else if (i != 0 && vec_tor[j][i][0] == 0 && vec_tor[j][i-1][0] == 0){
        salida_tex << " & $-$ " ;
      } else {
	salida_tex << " & " << std::setiosflags(std::ios::fixed) << setprecision(vec_tor[j][i][1])<< vec_tor[j][i][0] ; 
	salida_tex << " $\\pm $"  << setprecision(vec_tor[j][i][1])<< vec_tor[j][i][2];

      } 
    }
    salida_tex <<  " \\\\  " << endl;
    
    }
  }
  
  salida_tex << "   \\hline " << endl;
  salida_tex << "  \\end{tabular}" << endl;
  salida_tex << "  \\end{center}" << endl;
  salida_tex << "  \\end{table}" << endl;
  
  salida_tex << "\\end{document}" << endl;
    
}


double normalization(double nevents, double xsec, double lumi){
  
  if (nevents !=0) return lumi*xsec/nevents;
  else return 1;
  
}

double precision(double error){
  
  int pre;
  double fact_err = 0; 
  int i_n = 0; 
  if (error == 0) {
    pre = 0;
  } 
  else if (error > 1) pre = 0;
  else if (error < 1) {
    i_n = 0;
    fact_err = 0; 
    while (fact_err < 1){
      fact_err = error*(10**i_n);
      i_n++;  
    }
    pre = i_n-1;
  }
  
  if (fact_err > 9.5) pre -=1;
  
  return pre;
  
}

double err_efi (double efi, double err_total, double total_events, double error, double number){
  
  if (total_events !=0 && number != 0) return efi*((err_total/total_events)+(error/number));
  else return 0;
  
}

double efficiency (double finalevents, double totalevents){
  
  if (totalevents !=0) return finalevents*100/totalevents;
  else return 0;
  
}
