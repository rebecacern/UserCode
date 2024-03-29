// rebeca@cern.ch
// Basic plotter - comparing quantities
// November 2012

void compare(){
  
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
  
  labelcms  = new TPaveText(0.12,0.88,0.5,0.94,"NDCBR");
  labelcms->SetTextAlign(12);
  labelcms->SetTextSize(0.045);
  labelcms->SetFillColor(kWhite);
  labelcms->AddText("Simulation, #sqrt{s} = 8 TeV");
  labelcms->SetBorderSize(0);
    
  
  char myRootFile[300];
  sprintf(myRootFile,"rootfiles/signal_study.root");
  
  TFile *_file0 = TFile::Open(myRootFile);
  
  const int nPlots = 12;
  TString cutLabel[nPlots] =     { "met", "mllz", "mt", "ptjet", "mH", "mjj", "dphiljj", "dphilmjj", "mH_3d", "mH_2d", "minmet", "mlll"};
  int rebinHisto[nPlots] =       { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
  TString cutTitle[nPlots] =     { "Missing E_{T}", "Invariant Mass", "Transverse mass (W)", "P_{T} of the leading jet",
  				   "Reconstructed mass m_{H}",  "M_{jj}", "#Delta#Phi_{jjl}", "#Delta#Phi_{jjlMET}", "Reconstructed mass m_{H}",
				   "Reconstructed mass m_{H}", "Min(MET, Tracker MET)", "m_{lll}"};
  TH1F*  h [2][nPlots];
  
  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
    h[0][iPlot] = (TH1F*) _file0->Get("sig_" + cutLabel[iPlot]);
    h[1][iPlot] = (TH1F*) _file0->Get("bck_" + cutLabel[iPlot]);
    
    h[0][iPlot]->SetLineColor(kRed);
    h[0][iPlot]->SetLineWidth(2);
    h[0][iPlot]->SetNormFactor(1);
    h[0][iPlot]->Rebin(rebinHisto[iPlot]);
    
    h[1][iPlot]->SetFillColor(39);
    h[1][iPlot]->SetFillStyle(3001);
    h[1][iPlot]->SetLineWidth(2);
    h[1][iPlot]->SetNormFactor(1);
    h[1][iPlot]->Rebin(rebinHisto[iPlot]);
    
    leg = new TLegend(0.70,0.80,0.99,0.99);
    leg ->SetFillStyle(1001);
    leg ->SetFillColor(kWhite);
    leg ->SetBorderSize(1);
    leg->AddEntry(h[0][iPlot], "m_{H} = 125", "l");
    leg->AddEntry(h[1][iPlot], "Background", "f");
    
    double max = TMath::Max(h[0][iPlot]->GetMaximum(), h[1][iPlot]->GetMaximum());
    TCanvas *c1 = new TCanvas();
    h[0][iPlot]->Draw("histo");
    h[1][iPlot]->Draw("histo,sames");
    h[0][iPlot]->Draw("histo,sames");
    h[0][iPlot]->GetYaxis()->SetTitle("Normalized to 1");
    h[0][iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    h[0][iPlot]->GetYaxis()->SetLimits(0,0.5);
    h[0][iPlot]->GetYaxis()->CenterTitle(); 
    h[0][iPlot]->GetYaxis()->SetTitleOffset(1.5);
    leg->Draw();
    labelcms->Draw();
    
    c1->SaveAs("plots/nocuts_norm_"+cutLabel[iPlot]+".pdf");
    c1->SetLogy();
    c1->SaveAs("plots/nocuts_norm_"+cutLabel[iPlot]+"_log.pdf");
  }
  
}
