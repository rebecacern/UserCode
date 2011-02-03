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

void plots(){
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetErrorX(0);
  setTDRStyle();
  
  bool corrector = true;
  
  char myRootFile[300];
  char myName[100];
  
  sprintf(myName,"2010");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file0 = TFile::Open(myRootFile);
  TH1F* histo_0 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"tt");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file1 = TFile::Open(myRootFile);
  TH1F* histo_1 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"wjets");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file2 = TFile::Open(myRootFile);
  TH1F* histo_2 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"qcd");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file3 = TFile::Open(myRootFile);
  TH1F* histo_3 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"zjets");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file4 = TFile::Open(myRootFile);
  TH1F* histo_4 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"st_t");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file5a = TFile::Open(myRootFile);
  TH1F* histo_5a = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"st_tw");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file5 = TFile::Open(myRootFile);
  TH1F* histo_5 = jet_multiplicity_sel->Clone();
  
  sprintf(myName,"vqq");
  sprintf(myRootFile,"outputs/out_%s.root",myName);
  TFile *_file6 = TFile::Open(myRootFile);
  TH1F* histo_6 = jet_multiplicity_sel->Clone();
  
 
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
  

  histo_1->SetFillColor(kRed);
  histo_2->SetFillColor(kGreen);
  histo_3->SetFillColor(kYellow);
  histo_4->SetFillColor(kBlue);
  histo_5->SetFillColor(kMagenta);histo_6->SetFillColor(10);
  
  
  histo_0->SetMarkerStyle(20);
  histo_0->SetMarkerSize(1);
  histo_0->SetMarkerColor(kBlack);
  
  THStack *hs = new THStack("hs","");
  hs->Add(histo_6);
  hs->Add(histo_3);
  hs->Add(histo_4);
  hs->Add(histo_2);
  hs->Add(histo_5);
  hs->Add(histo_1);
  
  TCanvas *c1 = new TCanvas();
  
  histo_0->Draw("e");
  hs->Draw("sames, histo");  
  histo_0->Draw("sames, e"); 
  histo_0->GetXaxis()->SetTitle("Jet Multiplicity");
  histo_0->GetYaxis()->SetTitle("events / 36.1 pb^{-1}");
 // histo_0->GetXaxis()->SetTitle("Cut Flow");
  histo_0->GetXaxis()->SetRangeUser(1,14);
  histo_0->GetYaxis()->SetTitleOffset(1.4);
  c1->SetLogy();
  histo_0->GetYaxis()->CenterTitle(); 
  /*
  histo_0->GetXaxis()->SetBinLabel(2,"Skim");
  histo_0->GetXaxis()->SetBinLabel(3,"HLT");
  histo_0->GetXaxis()->SetBinLabel(4,"PV");
  histo_0->GetXaxis()->SetBinLabel(5,"1 e");
  histo_0->GetXaxis()->SetBinLabel(6,"#mu veto");
  histo_0->GetXaxis()->SetBinLabel(7,"Z veto");
  histo_0->GetXaxis()->SetBinLabel(8,"conv. vet 1");
  histo_0->GetXaxis()->SetBinLabel(9,"conv veto 2");
  histo_0->GetXaxis()->SetBinLabel(10,"> 0 j");
  histo_0->GetXaxis()->SetBinLabel(11,"> 1j");
  histo_0->GetXaxis()->SetBinLabel(12,"> 2j");
  histo_0->GetXaxis()->SetBinLabel(13,"> 3j");
  histo_0->GetXaxis()->SetBinLabel(14,"> 2j bt");
  histo_0->GetXaxis()->SetBinLabel(15,"> 3j bt");
*/
  
  TLegend* leg;
  leg = new TLegend(0.55,0.70,0.90,0.90);
  leg ->SetFillStyle(0);
  leg ->SetFillColor(kWhite);
  leg ->SetBorderSize(0);
  leg ->SetTextSize(0.030);
  leg->AddEntry(histo_0, "Data, 36.1 pb^{-1}", "pl");
  leg->AddEntry(histo_1, "t#bar{t}", "f");
  leg->AddEntry(histo_5, "Single t", "f");
  leg->AddEntry(histo_2, "W+jets", "f");
  leg->AddEntry(histo_4, "Z+jets", "f");
  leg->AddEntry(histo_3, "QCD", "f");
  leg->AddEntry(histo_6, "Vqq", "f");
  leg->Draw();
  
  c1->SaveAs("plots/jet_multiplicity_sel_corr.png");
  
    
}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600);
  tdrStyle->SetCanvasDefW(600);
  tdrStyle->SetCanvasDefX(0);
  tdrStyle->SetCanvasDefY(0);

  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);

  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);

  tdrStyle->SetEndErrorSize(2);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  tdrStyle->SetMarkerSize(2);

  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  tdrStyle->SetOptDate(0);

  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); 
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);

  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.10);
  tdrStyle->SetPadRightMargin(0.05);

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);

 
  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.03, "XYZ");

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  
  tdrStyle->SetPadTickY(1);


  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  tdrStyle->SetPaperSize(20.,20.);

  tdrStyle->cd();
}


