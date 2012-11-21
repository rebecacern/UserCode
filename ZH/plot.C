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
  sprintf(myRootFile,"histo_test.root");
  
  TFile *_file0 = TFile::Open(myRootFile);
  
  const int nPlots = 7;
  TString cutLabel[nPlots] =     { "met", "mllz", "mt", "ptjet", "mH", "mjj", "dphill"};
  int rebinHisto[nPlots] =       { 10, 10, 10, 10, 10, 10, 10};
  TString cutTitle[nPlots] =     { "Missing E_{T}", "Invariant Mass", "Transverse mass (W)", "P_{T} of the leading jet", "Reconstructed mass",  "M_{jj}", "#Delta#Phi_{jjl}"};
  TH1F*  h [2][nPlots];
  
  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
    h[0][iPlot] = (TH1F*) _file0->Get("sig_" + cutLabel[iPlot]);
    h[1][iPlot] = (TH1F*) _file0->Get("bckg_" + cutLabel[iPlot]);
    
    h[0][iPlot]->SetLineColor(kRed);
    h[0][iPlot]->SetLineWidth(2);
    h[0][iPlot]->SetNormFactor(1);
    h[0][iPlot]->Rebin(rebinHisto[iPlot]);
    
    h[1][iPlot]->SetFillColor(kBlue);
    h[1][iPlot]->SetFillStyle(3001);
    h[1][iPlot]->SetLineWidth(2);
    h[1][iPlot]->SetNormFactor(1);
    h[1][iPlot]->Rebin(rebinHisto[iPlot]);
    
    leg = new TLegend(0.80,0.85,0.99,0.99);
    leg ->SetFillStyle(1001);
    leg ->SetFillColor(kWhite);
    leg ->SetBorderSize(1);
    leg->AddEntry(h[0][iPlot], "m_{H} = 125", "l");
    leg->AddEntry(h[1][iPlot], "Background", "f");
    
    double max = TMath::Max(h[0][iPlot]->GetMaximum(), h[1][iPlot]->GetMaximum());
    TCanvas *c1 = new TCanvas();
    h[0][iPlot]->Draw("histo");
   // h[1][iPlot]->SetMaximum(0.2);
   // h[0][iPlot]->SetMinimum(1);
    h[1][iPlot]->Draw("histo,sames");
    h[0][iPlot]->Draw("histo,sames");
    h[0][iPlot]->GetYaxis()->SetTitle("Normalized to 1");
    h[0][iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    h[0][iPlot]->GetYaxis()->SetLimits(0,0.5);
    h[0][iPlot]->GetYaxis()->CenterTitle(); 
    leg->Draw();
    
    c1->SaveAs("plots/"+cutLabel[iPlot]+".png");
    c1->SetLogy();
    c1->SaveAs("plots/"+cutLabel[iPlot]+"_log.png");
  }
  
}
