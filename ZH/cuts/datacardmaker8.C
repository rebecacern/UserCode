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
#include "inputs.h"

using namespace std;
void datacardmaker8(int mh = 125, int mode = 0){

  const int np = 6;
  TString processName[np] =  { "ZH", "WZ","ZZ", "VVV", "Wjets", "Data"};
 
  char rootFile[300];
  if (mode == 1) sprintf(rootFile,"%d/zh3l2j_input_cut_eee_8TeV.root", mh);
  else if (mode == 2) sprintf(rootFile,"%d/zh3l2j_input_cut_eem_8TeV.root", mh );
  else if (mode == 3) sprintf(rootFile,"%d/zh3l2j_input_cut_emm_8TeV.root", mh );
  else if (mode == 4) sprintf(rootFile,"%d/zh3l2j_input_cut_mmm_8TeV.root", mh);
  else sprintf(rootFile,"%d/zh3l2j_input_cut_8TeV.root", mh);
  
  TFile *_file0 = TFile::Open(rootFile);
  
  TH1F*  h[np];
  for (int i = 0; i < np; i++){
    h[i] = (TH1F*) _file0->Get("histo_"+ processName[i]);
  }
 
  char datacardFile[300];
  if (mode == 1) sprintf(datacardFile,"%d/zh3l2j_cut_eee_8TeV.txt", mh);
  else if (mode == 2) sprintf(datacardFile,"%d/zh3l2j_cut_eem_8TeV.txt", mh );
  else if (mode == 3) sprintf(datacardFile,"%d/zh3l2j_cut_emm_8TeV.txt", mh );
  else if (mode == 4) sprintf(datacardFile,"%d/zh3l2j_cut_mmm_8TeV.txt", mh);
  else sprintf(datacardFile,"%d/zh3l2j_cut_8TeV.txt", mh);

  ofstream datacard(datacardFile); 
  
  datacard << "imax 1 number of channels" << endl;
  datacard << "jmax * number of background" << endl;
  datacard << "kmax * number of nuisance parameters" << endl;
  datacard << "Observation " << h[5]->Integral(1,nbins) << endl;
  datacard << "shapes *   *  "  ;
  if (mode == 1){
    datacard << "zh3l2j_input_cut_eee_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
    datacard << "shapes data_obs * zh3l2j_input_cut_eee_8TeV.root  histo_Data " << endl;
  } else if (mode == 2){
    datacard << "zh3l2j_input_cut_eem_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
    datacard << "shapes data_obs * zh3l2j_input_cut_eem_8TeV.root  histo_Data " << endl;
  } else if (mode == 3){
    datacard << "zh3l2j_input_cut_emm_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
    datacard << "shapes data_obs * zh3l2j_input_cut_emm_8TeV.root  histo_Data " << endl;
  } else if (mode == 4){
    datacard << "zh3l2j_input_cut_mmm_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
    datacard << "shapes data_obs * zh3l2j_input_cut_mmm_8TeV.root  histo_Data " << endl;
  } else {
    datacard << "zh3l2j_input_cut_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
    datacard << "shapes data_obs * zh3l2j_input_cut_8TeV.root  histo_Data " << endl;
  }
 
  datacard << "bin " ;
  for (int i = 0; i < np -1; i++) { datacard << "1 ";} 
  datacard << endl;
  datacard << "process ";
  for (int i = 0; i < np -1; i++) { datacard << processName[i] << " " ;} 
  datacard << endl;
  datacard << "process ";
  for (int i = 0; i < np -1; i++) { datacard << i << " ";} 
  datacard << endl;
  datacard << "rate\t" ;
  for (int i = 0; i < np -1; i++) { datacard << setprecision(precision(h[i]->Integral(1,nbins))) << h[i]->Integral(1,nbins) << "\t";}
  datacard << endl;
  datacard << "lumi_8TeV\t\t\tlnN\t ";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.045\t"; else datacard << "-\t";}
  datacard << endl;
  
  datacard << "CMS_LepEffBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.000\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_LepResBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.000\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_METResBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.000\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_JESBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.000\t"; else datacard << "-\t";}
  datacard << endl;

  datacard << "CMS_vh3l_pu\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.010\t"; else datacard << "-\t";}
  datacard << endl;
  
  datacard << "CMS_WBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;
  datacard << "CMS_FakeRate\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "-\t"; else datacard << "1.400\t";}
  datacard << endl;

  datacard << "CMS_WZBound\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=1) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_ZZBound\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=2) datacard << "-\t"; else datacard << "1.065\t";}
  datacard << endl;

  datacard << "CMS_VVVBound\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=3) datacard << "-\t"; else datacard << "1.500\t";}
  datacard << endl;

  datacard << "UEPS\t\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i==0) datacard << "1.050\t"; else datacard << "-\t";}
  datacard << endl;
  
  datacard << "PDF\t\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i==0) datacard << "1.047\t"; else if (i==1) datacard << "1.055\t"; else if (i==2) datacard << "1.041\t"; else datacard << "-\t";}
  datacard << endl;

  datacard << "CMS_ZHStatBound8TeV\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=0) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_WZStatBound8TeV\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=1) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_ZZStatBound8TeV\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=2) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_VVVStatBound8TeV\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=3) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_WjetsStatBound8TeV\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;



  
}

