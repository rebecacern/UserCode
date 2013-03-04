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
void datacardmaker(int mh = 125){

  const int np = 6;
  TString processName[np] =  { "ZH", "WZ","ZZ", "VVV", "Wjets", "Data"};
 
  char rootFile[300];
  sprintf(rootFile,"%d/zh3l2j_input_8TeV.root", mh);
  
  TFile *_file0 = TFile::Open(rootFile);
  
  TH1F*  h[np];
  for (int i = 0; i < np; i++){
    h[i] = (TH1F*) _file0->Get("histo_"+ processName[i]);
  }
 
  char datacardFile[300];
  sprintf(datacardFile,"%d/zh3l2j_shape_8TeV.txt", mh);

  ofstream datacard(datacardFile); 
  
  datacard << "imax 1 number of channels" << endl;
  datacard << "jmax * number of background" << endl;
  datacard << "kmax * number of nuisance parameters" << endl;
  datacard << "Observation " << h[5]->Integral(1,nbins) << endl;
  
  datacard << "shapes *   *   zh3l2j_input_8TeV.root  histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" << endl;
  datacard << "shapes data_obs * zh3l2j_input_8TeV.root  histo_Data " << endl;
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
  for (int i = 0; i < np -1; i++) { datacard << setprecision(5) << h[i]->Integral(1,nbins) << "\t";}
  datacard << endl;
  datacard << "lumi_8TeV\t\t\tlnN\t ";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.045\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_LepEffBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.000\t"; else datacard << "-\t";}
  datacard << endl;
  
  //This can be shape
  datacard << "CMS_p_scale_m\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.015\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_p_scale_e\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.020\t"; else datacard << "-\t";}
  datacard << endl;
  datacard << "CMS_res_met\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "1.020\t"; else datacard << "-\t";}
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

  datacard << "CMS_WZBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=1) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "UEPS\t\t\t\tlnN\t";
  for (int i = 0; i < np -1; i++) { if (i!=0) datacard << "1.030\t"; else datacard << "-\t";}
  datacard << endl;

  datacard << "CMS_ZHStatBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=0) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_WZStatBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=1) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_ZZStatBound\t\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=2) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_VVVStatBound\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=3) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;

  datacard << "CMS_WjetsStatBound\t\tshape\t";
  for (int i = 0; i < np -1; i++) { if (i!=4) datacard << "-\t"; else datacard << "1.000\t";}
  datacard << endl;



  
}
