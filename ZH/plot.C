// rebeca@cern.ch
// Basic plotter - comparing quantities
// November 2012

void plot(){
  
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
  
  const int nProcess = 4;
  TString processName[nProcess] =  { "wz", "www", "fakes","zz"};
  TString processTitle[nProcess] = { "WZ", "WWW", "fakes", "ZZ"};
  Color_t color[nProcess] =        { kBlue+2, kBlue-9, kGray, kGray+2};
 
  const int nPlots = 2;
  TString cutLabel[nPlots] =     { "mH", "dphiljj"};
  int rebinHisto[nPlots] =       { 10, 10};
  TString cutTitle[nPlots] =     { "Reconstructed mass m_{H}", "#Delta#Phi_{jjl}"}; 
 
  TH1D*  h [nPlots][nProcess];
  TH1D*  h0 [nPlots];
  TH1D*  h1 [nPlots];
  THStack* hStack[nPlots];
  
  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
    h0[iPlot] = (TH1D*) _file0->Get("sig_" + cutLabel[iPlot]);
       
    h0[iPlot]->SetLineColor(kMagenta-4);
    h0[iPlot]->SetLineWidth(2);
    h0[iPlot]->Rebin(rebinHisto[iPlot]);

    leg = new TLegend(0.70,0.80,0.99,0.99);
    leg ->SetFillStyle(1001);
    leg ->SetFillColor(kWhite);
    leg ->SetBorderSize(1);
    leg->AddEntry( h0[iPlot], "(ZH) m_{H} = 125", "l");
    
    for (int iProcess = 0; iProcess < nProcess; iProcess++){
      h[iPlot][iProcess] = (TH1D*) _file0->Get("bck_" + cutLabel[iPlot]+ "_" + processName[iProcess]);
      h[iPlot][iProcess]->Rebin(rebinHisto[iPlot]);
      h[iPlot][iProcess]->SetFillColor(color[iProcess]);
      h[iPlot][iProcess]->SetLineColor(kBlack);
      h[iPlot][iProcess]->SetLineWidth(1);
      leg->AddEntry(h[iPlot][iProcess], processTitle[iProcess], "f");
      h[iPlot][iProcess]->Draw();
    }
    
    hStack[iPlot] = new THStack(cutLabel[iPlot],cutLabel[iPlot]);
    for (int iProcess = nProcess-1; iProcess > -1; iProcess--){
      hStack[iPlot]->Add(h[iPlot][iProcess]);
    }
 
    double max = TMath::Max(h0[iPlot]->GetMaximum(), hStack[iPlot]->GetMaximum());
    TCanvas *c1 = new TCanvas();
    hStack[iPlot]->Draw("histo");
    hStack[iPlot]->SetMaximum(max*1.25);
    hStack[iPlot]->SetMinimum(0.01);
    h0[iPlot]->Draw("histo,sames");
    hStack[iPlot]->GetYaxis()->SetTitle("events / 19.46fb^{-1}");
    hStack[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    hStack[iPlot]->GetYaxis()->SetLimits(0,0.5);
    hStack[iPlot]->GetYaxis()->CenterTitle(); 
    hStack[iPlot]->GetYaxis()->SetTitleOffset(1.3);
    if (iPlot == 1) hStack[iPlot]->GetXaxis()->SetRangeUser(0.0, 3.1415);
    leg->Draw();
    
    c1->SaveAs("plots/"+cutLabel[iPlot]+".png");
    c1->SetLogy();
    c1->SaveAs("plots/"+cutLabel[iPlot]+"_log.png");
    
    h1[iPlot] = (TH1D*) _file0->Get("data_" + cutLabel[iPlot]);
    h1[iPlot]->Rebin(rebinHisto[iPlot]);
    h1[iPlot]->SetMarkerStyle(20);
    h1[iPlot]->SetMarkerSize(1.2);
    h1[iPlot]->SetLineWidth(1);
    h1[iPlot]->SetMarkerColor(kBlack);
    h1[iPlot]->SetLineColor(kBlack);
    leg->AddEntry( h1[iPlot], "data", "p");
    
    max = TMath::Max(h1[iPlot]->GetMaximum(), hStack[iPlot]->GetMaximum());
    TCanvas *c1 = new TCanvas();
    hStack[iPlot]->Draw("histo");
    hStack[iPlot]->SetMaximum(max*1.5);
    hStack[iPlot]->SetMinimum(0.01);
    h0[iPlot]->Draw("histo,sames");
    h1[iPlot]->Draw("e,sames");
    hStack[iPlot]->GetYaxis()->SetTitle("events / 19.46 fb^{-1}");
    hStack[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    hStack[iPlot]->GetYaxis()->SetLimits(0,0.5);
    hStack[iPlot]->GetYaxis()->CenterTitle(); 
    hStack[iPlot]->GetYaxis()->SetTitleOffset(1.3);
    leg->Draw();
    
    c1->SaveAs("plots/data_"+cutLabel[iPlot]+".png");
    c1->SetLogy();
    c1->SaveAs("plots/data_"+cutLabel[iPlot]+"_log.png");
    
    
    
  }
  
}
