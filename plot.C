



#include <string>
#include <iostream>

#include "TList.h"
#include "TFile.h"
#include "TTree.h"



void plot()
{
  
  TFile *f1 = new TFile("./run-0264-00-output.root");

  TFile *f2 = new TFile("./run-0265-00-output.root");


  TTree * tree1 = (TTree*)f1->Get("flt");
  
  TTree * tree2 = (TTree*)f2->Get("flt");

  
  /*  tree1->Draw("energy+1400","chanid==0");  
  tree1->Draw("energy+410+1400","chanid==0","same");


  tree2->SetLineColor(4);

  tree2->Draw("energy","chanid==2","same");
  
  tree2->SetLineColor(2);

  tree2->Draw("energy+3100>>h","chanid==3","same");
  
  */

  
  tree1->Draw("Integral0>>h0","Integral0>0");
  tree1->Draw("Integral1>>h1","Integral1>0");
  tree2->Draw("Integral2>>h2","Integral2>0");
  tree2->Draw("Integral3>>h3","Integral3>0");



  Int_t n=h1->GetXaxis()->GetNbins();
  
  Double_t max =0;

  for (i=20;i<n;i++){
    if (h0->GetBinContent(i) > max )
      max = h0->GetBinContent(i);
    
  }
  cout<<"max is "<<max<<endl;
  cout<<"2/3 max is "<<max *(2.0/3)<<endl;
  int no=0;
  for (i=20;i<n-2;i++){

    double num =h0->GetBinContent(i);
    double num2 =h0->GetBinContent(i);

    if ( num2 <= (2.0/3)*max && no==0){
      cout<<"I is "<<i<<endl;
      cout<<"Value is "<<h0->GetXaxis()->GetBinCenter(i)<<endl;
      no=1;
    }
  }

  cout<<"********************"<<endl;

  for (i=20;i<n;i++){
    if (h1->GetBinContent(i) > max )
      max = h1->GetBinContent(i);

  }
  cout<<"max is "<<max<<endl;
  cout<<"2/3 max is "<<max *(2.0/3)<<endl;
  int no=0;
  for (i=20;i<n-2;i++){

    double num =h1->GetBinContent(i);
    double num2 =h1->GetBinContent(i);

    if ( num2 <= (2.0/3)*max && no==0){
      cout<<"I is "<<i<<endl;
      cout<<"Value is "<<h1->GetXaxis()->GetBinCenter(i)<<endl;
      no=1;
    }
  }

  cout<<"******************"<<endl;

  for (i=20;i<n;i++){
    if (h2->GetBinContent(i) > max )
      max = h2->GetBinContent(i);

  }
  cout<<"max is "<<max<<endl;
  cout<<"2/3 max is "<<max *(2.0/3)<<endl;
  int no=0;
  for (i=20;i<n-2;i++){

    double num =h2->GetBinContent(i);
    double num2 =h2->GetBinContent(i);

    if ( num2 <= (2.0/3)*max && no==0){
      cout<<"I is "<<i<<endl;
      cout<<"Value is "<<h2->GetXaxis()->GetBinCenter(i)<<endl;
      no=1;
    }
  }

  cout<<"******************"<<endl;


  for (i=20;i<n;i++){
    if (h3->GetBinContent(i) > max )
      max = h3->GetBinContent(i);

  }
  cout<<"max is "<<max<<endl;
  cout<<"2/3 max is "<<max *(2.0/3)<<endl;
  int no=0;
  for (i=20;i<n-2;i++){

    double num =h3->GetBinContent(i);
    double num2 =h3->GetBinContent(i);

    if ( num2 <= (2.0/3)*max && no==0){
      cout<<"I is "<<i<<endl;
      cout<<"Value is "<<h3->GetXaxis()->GetBinCenter(i)<<endl;
      no=1;
    }
  }

}
