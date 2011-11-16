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
#include "setTDRStyle.C"
using namespace std;

void tmvaplot(int mode = 1){
  
  //gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetErrorX(0);
  setTDRStyle();
  gROOT->SetBatch(1);

  labelcms  = new TPaveText(0.12,0.88,0.5,0.94,"NDCBR");
  labelcms->SetTextAlign(12);
  labelcms->SetTextSize(0.045);
  labelcms->SetFillColor(kWhite);
  labelcms->AddText("CMS Preliminary, #sqrt{s} = 7 TeV");
  labelcms->SetBorderSize(0);
  
    
  labelcms2  = new TPaveText(0.12,0.85,0.5,0.88,"NDCBR");
  labelcms2->SetTextAlign(12);
  labelcms2->SetTextSize(0.045);
  labelcms2->SetFillColor(kWhite);
  if (mode == 0) labelcms2->AddText("2.1 fb^{-1}, e#mu channel  ");
  if (mode == 1) labelcms2->AddText("2.1 fb^{-1}, #mu#mu channel  ");
  if (mode == 2) labelcms2->AddText("2.1 fb^{-1}, ee channel  ");
  
  labelcms2->SetBorderSize(0);
  
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetLabelFont(18,"");

  gStyle->SetTitleXOffset(1.2);//1.5
  gStyle->SetTitleYOffset(1.2);//1.7
  
  
  char myRootFile[300];
  double lumi = 1000;
  if (mode == 0 )       lumi = 2121.307; 
  else if ( mode == 1)   lumi = 2110.25; 
  else if ( mode == 2)   lumi = 2096.434; 
  sprintf(myRootFile,"rootfiles/tmva_test_%d", mode);
  
  TString processName[7] =  { "twdr", "tt","di", "zjets", "wjets",  "qcd_mu", "data"};
  TString processTitle[7] = { "tW", "t#bar{t}", "WW/WZ/ZZ", "Z/#gamma*+jets", "W+jets",  "QCD", "data"};
  Color_t color[7] =        {kWhite,  kRed+1, kYellow-10,  kAzure-2, kGreen-3, 40, kBlack};
 
  
  
  TH1F*  h [7];
  THStack* hStack;
  
  leg = new TLegend(0.7,0.7,0.94,0.94);
  leg ->SetFillStyle(1);
  leg ->SetFillColor(kWhite);
  leg ->SetBorderSize(1);
  // leg ->SetTextSize(0.027);
  hStack = new THStack("BDT","BDT");
  for (int iProcess = 0; iProcess < 7; iProcess++){
  
  TFile *_file0 = TFile::Open("rootfiles/tmva_test_1_" + processName[iProcess] + ".root");
   cout << "rootfiles/tmva_test_1_" + processName[iProcess] + ".root" << endl;
    h[iProcess] = (TH1F*) _file0->Get("hBDT");
    h[iProcess]->SetFillColor(color[iProcess]);
    h[iProcess]->SetLineColor(kBlack);
    h[iProcess]->SetLineWidth(1);
    //if (iProcess !=6) h[iProcess]->Scale(2);
    if (iProcess == 0) h[iProcess]->SetLineColor(kBlack);
    }
    
    h[4]->Add(h[2]);
    h[4]->Add(h[5]);
    
    hStack->Add(h[4]);
    hStack->Add(h[3]);
    hStack->Add(h[1]);
    hStack->Add(h[0]);
    
    if (mode == 0) leg->AddEntry(h[6],  processTitle[6], "p");
    if (mode == 1) leg->AddEntry(h[6],  processTitle[6], "p");
    if (mode == 2) leg->AddEntry(h[6], processTitle[6], "p");
    leg->AddEntry(h[0], processTitle[0], "f");
    leg->AddEntry(h[1], processTitle[1], "f");
    leg->AddEntry(h[3], processTitle[3], "f");
    leg->AddEntry(h[4], "Other", "f");
    
    h[6]->SetMarkerStyle(20);
    h[6]->SetMarkerSize(1.2);
    h[6]->SetLineWidth(4);
    h[6]->SetMarkerColor(kBlack);
    h[6]->SetLineColor(kBlack);
    
    
    double max = TMath::Max(hStack->GetMaximum(), h[6]->GetMaximum());
    TCanvas *c1 = new TCanvas();
    hStack->Draw("histo");
    hStack->SetMaximum(max * 1.25);
    hStack->SetMinimum(1);
    hStack->GetXaxis()->SetTitle("BDT output");
    hStack->GetXaxis()->SetRangeUser(-0.2, 0.2);
    //hStack->GetYaxis()->SetTitle("events / 1.1 fb^{-1}");
    // hStack->GetYaxis()->SetTitle("events / 1.4 fb^{-1}");
    hStack->GetYaxis()->SetTitle("events / 2.1 fb^{-1}");
   
    hStack->GetYaxis()->CenterTitle(); 
    h[6]->Draw("e2, sames");
    leg->Draw();
    labelcms->Draw();
    labelcms2->Draw();
    
    if (mode == 1){
      c1->SaveAs("plots/tmva_plot_1_.png");
      c1->SetLogy();
      hStack->SetMaximum(max * 2);
      c1->SaveAs("plots/tmva_plot_1__log.png");
    } else if (mode == 2){
      c1->SaveAs("plots/tmva_plot_2_.png");
      c1->SetLogy();
      hStack->SetMaximum(max * 2);
      c1->SaveAs("plots/tmva_plot_2__log.png");
    } else if (mode == 0){
      c1->SaveAs("plots/tmva_plot_0_.png");
      c1->SetLogy();
      hStack->SetMaximum(max * 2);
      c1->SaveAs("plots/tmva_plot_0__log.png");
    }
  
 
}
