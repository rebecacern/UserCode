// rebeca@cern.ch
// Basic plotter - comparing quantities
// November 2012

void histos(int cem = 8, int iPlot = 0){
  
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
 
  const int nPlots = 17;
  TString cutLabel[nPlots] =     { "mH_2d", "dphiljj", "dphilmjj", "met", "mllz", "mt", "ptjet", "mjj", "dRll", "tmet", "minmet", "njets", "minmll", "id", "mlll", "mH", "njets"};
  int rebinHisto[nPlots] =       { 10, 10, 10, 0, 10, 10, 10, 10, 10, 10, 10, 1, 10, 1, 10, 10, 1};
  TString cutTitle[nPlots] =     { "Reconstructed mass m_{H}", "#Delta#Phi_{jjl}", "#Delta#Phi_{jjlMET}", "MET", "m_{ll} Z", "m_{T}", "P_{T} of the leading jet",
                                   "m_{jj}", "#DeltaR_{ll}", "tracker MET", "min(MET, tracker MET)", "# of jets", "|m_{jj} - m{Z}|", "lepton content", "m_{lll}", "Reconstructed mass m_{H}", 
				   "Number of jets"  }; 

 TString cm[2] = {"8TeV", "7TeV"};
 
  TFile f_var("rootfiles/histo_" + cm[8 - cem]  + "_" + cutLabel[iPlot] + ".root", "RECREATE");
  
  
  TH1D*  h [nPlots][nProcess];
  TH1D*  h0 [nPlots];
  TH1D*  h1 [nPlots];
  THStack* hStack[nPlots];
  
 
    h0[iPlot] = (TH1D*) _file0->Get("sig_" + cutLabel[iPlot]);
       
   // h0[iPlot]->SetLineColor(kMagenta-4);
    //h0[iPlot]->SetLineWidth(2);
    h0[iPlot]->Rebin(rebinHisto[iPlot]);

    
    for (int iProcess = 0; iProcess < nProcess; iProcess++){
      h[iPlot][iProcess] = (TH1D*) _file0->Get("bck_" + cutLabel[iPlot]+ "_" + processName[iProcess]);
      h[iPlot][iProcess]->Rebin(rebinHisto[iPlot]);
     // h[iPlot][iProcess]->SetFillColor(color[iProcess]);
     // h[iPlot][iProcess]->SetLineColor(kBlack);
     // h[iPlot][iProcess]->SetLineWidth(1);
     
    }
    
    hStack[iPlot] = new THStack(cutLabel[iPlot],cutLabel[iPlot]);
    for (int iProcess = nProcess-1; iProcess > -1; iProcess--){
      hStack[iPlot]->Add(h[iPlot][iProcess]);
    }
  
    h1[iPlot] = (TH1D*) _file0->Get("data_" + cutLabel[iPlot]);
    h1[iPlot]->Rebin(rebinHisto[iPlot]);
    //h1[iPlot]->SetMarkerStyle(20);
    //h1[iPlot]->SetMarkerSize(1.2);
    //h1[iPlot]->SetLineWidth(1);
    //h1[iPlot]->SetMarkerColor(kBlack);
    //h1[iPlot]->SetLineColor(kBlack);
 
    
    TH1F* histo2 = h[iPlot][3]->Clone(); //ZZ
    histo2->SetName("histo2");
    TH1F* histo3 = h[iPlot][0]->Clone(); //WZ
    histo3->Add(h[iPlot][1]); //VVV added
    histo3->SetName("histo3");
    TH1F* histo4 = h[iPlot][2]->Clone(); //fakes
    histo4->SetName("histo4");
    TH1F* histo5 = h1[iPlot]->Clone(); //data
     histo5->SetName("histo5");
    TH1F* histos = h0[iPlot]->Clone(); //signal
     histos->SetName("histos");
  
  f_var.Write();
  f_var.Close();
  
}
