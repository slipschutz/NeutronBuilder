//
// script to find the bin value coresponding to the 2/3 of the maximum 
// of first compton edge.  
// 
// Adjust the start and end variables before use
//



#include <string>
#include <iostream>

#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"




int FindSpot (TH1F * h)
{
 int  start =490;

  int endd=900;
  
  int  binStart = h->FindBin(start);
  int binEnd = h->FindBin(endd);


  cout<<"Bin start "<<binStart<<endl;

  cout<<"Bin end "<<binEnd<<endl;

  Double_t max =0;

  //find the max
  for(int i=binStart ; i<binEnd ;i++){
    if ( h->GetBinContent(i) > max)
      {
	max = h->GetBinContent(i);
      }
  }


  cout<<"The Maximum is "<< max <<endl;


  Double_t twoThirdmax = (2.0/3.0)*max;

  cout<<"twoThirdMax is "<<twoThirdmax<<endl;

  for (int i=binStart;i<binEnd;i++){


    if ( h->GetBinContent(i) <= twoThirdmax ){
      cout<<"i is "<<i <<" Bin center is "<<h->GetBinCenter(i)<<endl;
      break;
    }

  }
  return 0;
}
