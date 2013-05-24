// rebeca@cern.ch
// Basic plotter - comparing quantities
// November 2012

void ratioplot(int cem = 8){
  
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
  
  labelcms  = new TPaveText(0.12,0.92,0.5,0.94,"NDCBR");
  labelcms->SetTextAlign(12);
  labelcms->SetTextSize(0.045);
  labelcms->SetFillColor(kWhite);
  if (cem !=8) labelcms->AddText("CMS Preliminary, #sqrt{s} = 7 TeV");
  else labelcms->AddText("CMS Preliminary, #sqrt{s} = 8 TeV");
  labelcms->SetBorderSize(0);
   
  char myRootFile[300];
  if (cem !=8) sprintf(myRootFile,"rootfiles/signal_study_7TeV.root");
  else sprintf(myRootFile,"rootfiles/signal_study.root");
  
  TFile *_file0 = TFile::Open(myRootFile);
  
  const int nProcess = 4;
  TString processName[nProcess] =  { "wz", "www", "fakes","zz"};
  TString processTitle[nProcess] = { "WZ", "tri-boson", "fakes", "ZZ"};
  Color_t color[nProcess] =        { kBlue+2, kBlue-9, kGray, kGray+2};
  
  const int nPlots = 16;
  TString cutLabel[nPlots] =     { "mH_2d", "dphiljj", "dphilmjj", "met", "mllz", "mt", "ptjet", "mjj", "dRll", "tmet", "minmet", "njets", "minmll", "id", "mlll", "mH"};
  int rebinHisto[nPlots] =       { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 1, 10, 1, 10, 10};
  TString cutTitle[nPlots] =     { "Reconstructed mass m_{H}", "#Delta#Phi_{jjl}", "#Delta#Phi_{jjlMET}", "MET", "m_{ll} Z", "m_{T}", "P_{T} of the leading jet",
                                   "m_{jj}", "#DeltaR_{ll}", "tracker MET", "min(MET, tracker MET)", "# of jets", "|m_{ll} - m{Z}|", "lepton content", "m_{lll}", "Reconstructed mass m_{H}"  }; 
  
  TH1D*  h [nPlots][nProcess];
  TH1D*  h0 [nPlots];
  TH1D*  h1 [nPlots];
  TH1D*  h2 [nPlots];
  TH1D*  h3 [nPlots];
  THStack* hStack[nPlots];
  
  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
    h0[iPlot] = (TH1D*) _file0->Get("sig_" + cutLabel[iPlot]);
    
    h0[iPlot]->SetLineColor(kMagenta-4);
    h0[iPlot]->SetLineWidth(2);
    h0[iPlot]->Rebin(rebinHisto[iPlot]);
    
    leg = new TLegend(0.70,0.78,0.99,0.99);
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
    
   
    h1[iPlot] = (TH1D*) _file0->Get("data_" + cutLabel[iPlot]);
    h1[iPlot]->Rebin(rebinHisto[iPlot]);
    h1[iPlot]->SetMarkerStyle(20);
    h1[iPlot]->SetMarkerSize(1.2);
    h1[iPlot]->SetLineWidth(1);
    h1[iPlot]->SetMarkerColor(kBlack);
    h1[iPlot]->SetLineColor(kBlack);
    leg->AddEntry( h1[iPlot], "data", "p");
    
    double max = TMath::Max(h1[iPlot]->GetMaximum(), hStack[iPlot]->GetMaximum());
    
    TCanvas *c1 = new TCanvas("c1","example",600,700);
    c1->SetGrid(0,1);
    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1->SetBottomMargin(0);
    pad1->Draw();
    pad1->cd();
    
    hStack[iPlot]->Draw("histo");
    hStack[iPlot]->SetMaximum(max*1.5);
    hStack[iPlot]->SetMinimum(0.01);
    hStack[iPlot]->GetXaxis()->SetLabelFont(63); //font in pixels
    hStack[iPlot]->GetXaxis()->SetLabelSize(16); //in pixels
    hStack[iPlot]->GetYaxis()->SetLabelFont(63); //font in pixels
    hStack[iPlot]->GetYaxis()->SetLabelSize(16); //in pixels
    h0[iPlot]->Draw("histo,sames");
    
    //h1[iPlot]->Draw("e,sames");
    TGraphAsymmErrors * g = new TGraphAsymmErrors(h1[iPlot]);
    bool plotCorrectErrorBars = true;
    if(plotCorrectErrorBars == true) {
      
      for (int i = 0; i < g->GetN(); ++i) {
	double N = g->GetY()[i];
	double alpha=(1-0.6827);
	double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
	double U =  (N==0) ?  ( ROOT::Math::gamma_quantile_c(alpha,N+1,1.) ) :
	  ( ROOT::Math::gamma_quantile_c(alpha/2,N+1,1.) );
	g->SetPointEYlow(i,double(N)-L);
	if(N > 0)
	  g->SetPointEYhigh(i, U-double(N));
	else
	  g->SetPointEYhigh(i, 0.0);
      }
      g->Draw("P");
    }
    else {
      h1[iPlot]->Draw("ep,same");
    }
    
    if (cem !=8) hStack[iPlot]->GetYaxis()->SetTitle("events / 4.924 fb^{-1}");
    else hStack[iPlot]->GetYaxis()->SetTitle("events / 19.46 fb^{-1}");
    
    
    if (cutLabel[iPlot] == "id") {
      hStack[iPlot]->GetXaxis()->SetBinLabel(1,"eee");
      hStack[iPlot]->GetXaxis()->SetBinLabel(2,"ee#mu");
      hStack[iPlot]->GetXaxis()->SetBinLabel(3,"#mu#mue");
      hStack[iPlot]->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
    } else  hStack[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);

    hStack[iPlot]->GetYaxis()->SetLimits(0,0.5);
    hStack[iPlot]->GetYaxis()->CenterTitle(); 
    hStack[iPlot]->GetYaxis()->SetTitleOffset(1.3);
    if (iPlot == 1 || iPlot == 2) hStack[iPlot]->GetXaxis()->SetRangeUser(0.0, 3.14);
    
    
    
    leg->Draw();
    labelcms->Draw();
    
    c1->cd();
    TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->Draw();
    pad2->cd();
    pad2->SetGridx();
    pad2->SetGridy();
    
    h2[iPlot]=(TH1D*)hStack[iPlot]->GetStack()->Last()->Clone();
    h3[iPlot]=(TH1D*)h1[iPlot]->Clone();
    h3[iPlot]->Divide(h2[iPlot]);
    
    if (cutLabel[iPlot] == "id") {
      h3[iPlot]->GetXaxis()->SetBinLabel(1,"eee");
      h3[iPlot]->GetXaxis()->SetBinLabel(2,"ee#mu");
      h3[iPlot]->GetXaxis()->SetBinLabel(3,"#mu#mue");
      h3[iPlot]->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
      
    } else  h3[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
      
    h3[iPlot]->GetYaxis()->CenterTitle();
    h3[iPlot]->GetYaxis()->SetTitle("data/MC");
    h3[iPlot]->GetYaxis()->SetLabelFont(63); //font in pixels
    h3[iPlot]->GetYaxis()->SetLabelSize(16); //in pixels
    h3[iPlot]->GetXaxis()->SetLabelFont(63); //font in pixels
    h3[iPlot]->GetXaxis()->SetLabelSize(16); //in pixels
    h3[iPlot]->GetYaxis()->SetTitleFont(63); //font in pixels
    h3[iPlot]->GetYaxis()->SetTitleSize(16); //in pixels
    h3[iPlot]->GetXaxis()->SetTitleFont(63); //font in pixels
    h3[iPlot]->GetXaxis()->SetTitleSize(16); //in pixels
    h3[iPlot]->SetMarkerStyle(20);
    h3[iPlot]->SetMarkerSize(1.2);
    h3[iPlot]->GetYaxis()->SetLimits(0,0.5);
    h3[iPlot]->GetYaxis()->CenterTitle(); 
    h3[iPlot]->GetYaxis()->SetTitleOffset(2);
    h3[iPlot]->GetXaxis()->SetTitleOffset(3.5);
    h3[iPlot]->Draw("ep");
    c1->cd();
    
    if (cem != 8){
      c1->SaveAs("plots/ratio_all_7_"+cutLabel[iPlot]+".pdf");
      pad1->cd();
      pad1->SetLogy();
      c1->cd();
      c1->SaveAs("plots/ratio_all_7_"+cutLabel[iPlot]+"_log.pdf");
    } else {
      c1->SaveAs("plots/ratio_all_8_"+cutLabel[iPlot]+".pdf");
      pad1->cd();
      pad1->SetLogy();
      c1->cd();
      c1->SaveAs("plots/ratio_all_8_"+cutLabel[iPlot]+"_log.pdf");
    }
    
    
    
  }
  
}


   
