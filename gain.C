#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
int gain()
{

  
  gROOT->Reset();  
  TH1F * hh0;
  TH1F * hh1;
  TH1F * hh2;
  TH1F * hh3;

  TFile *output = new TFile("outFile.root", "RECREATE");
  
  TFile* f1 = new TFile("./run-0264-00-output.root");
  
  TTree * flt= (TTree*) f1->Get("flt");
  
  output->cd();

  hh0 = new TH1F("hh0","hh0",400,0,6000);
  hh1 = new TH1F("hh1","hh1",400,0,6000);

  Double_t integral0,integral1;
  Int_t numEntries = flt->GetEntries();

  flt->SetBranchAddress("Integral0",&integral0);
  flt->SetBranchAddress("Integral1",&integral1);

  
  for (Int_t i=0;i<numEntries;++i){
    flt->GetEntry(i);
    if (integral0>130 && integral0 <6000)
      hh0->Fill(integral0+182.906);
    
    if (integral1>130 && integral1<6000)
      hh1->Fill(integral1+230.97);
  }

  f1->Delete();
  TFile * f2 = new TFile("./run-0265-00-output.root");
  flt = (TTree*) f2->Get("flt");
  
  output->cd();
  
  hh2 = new TH1F("hh2","hh2",400,0,6000);
  hh3 = new TH1F("hh3","hh3",400,0,6000);

  Double_t integral2,integral3;
  numEntries = flt->GetEntries();

  flt->SetBranchAddress("Integral2",&integral2);
  flt->SetBranchAddress("Integral3",&integral3);

  for (Int_t i=0;i<numEntries;++i){
    flt->GetEntry(i);
    if (integral2>130 && integral2 <6000)
      hh2->Fill(integral2);
    
    if (integral3>130 && integral3<6000)
      hh3->Fill(integral3+223.688);
  }

  Double_t s0,s1,s2,s3;
  s0=hh0->Integral();
  s1=hh1->Integral();
  s2=hh2->Integral();
  s3=hh3->Integral();
  
  hh0->Scale(1.0/s0);
  hh1->Scale(1.0/s1);
  hh2->Scale(1.0/s2);
  hh3->Scale(1.0/s3);

  
  //  hh0->Draw();
  // hh1->SetLineColor(2);
  // hh1->Draw("same");

  hh2->Draw("");
  hh3->SetLineColor(2);
  hh3->Draw("same");

  output->Write();
  return 0;


  flt->Draw("Integral0+182.906>>h0","Integral0>-10 && Integral0<6000","");
  //flt->Draw("Integral1+220.97>>h1","Integral1>-10 && Integral1<6000","goff");

  //  h0->Draw("");
  return 1;
  TFile f2("./run-0265-00-output.root");

  flt->Draw("Integral2>>h2","Integral2>-10 && Integral2<6000","goff");
  flt->Draw("Integral3+223.688>>h3","Integral3>-10 && Integral3<6000","goff");
    

  Double_t scale0 = h0->Integral();
  Double_t scale1 = h1->Integral();

  Double_t scale2 = h2->Integral();
  Double_t scale3 = h3->Integral();


  h0->Scale(1.0/scale0);
  h1->Scale(1.0/scale1);
  

  h2->Scale(1.0/scale2);
  h3->Scale(1.0/scale3);
  


  h3->Draw("same");



    return 1;
}
