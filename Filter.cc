
//
//  Filter object
//


#include "Filter.hh"

#include "TFile.h"
#include "TMath.h"


Filter::Filter()
{


}


void Filter::FastFilter(std::vector <UShort_t> &trace,std::vector <Double_t> &thisEventsFF,Double_t FL,Double_t FG){
  Double_t sumNum1=0;
  Double_t sumNum2=0;
  
  
  for (int i=0;i< (int) trace.size();i++)
    {
      for (int j= i-(FL-1) ;j<i;j++)
	{
	  if (j>=0)
	    sumNum1 = sumNum1+ trace[j];
	}

      for (int j=i-(2*FL+FG-1);j<i-(FL+FG);j++)
	{
	  if (j>=0)
	    sumNum2 = sumNum2+ trace[j];
	}
      thisEventsFF.push_back(sumNum1-sumNum2);
      sumNum1=0;
      sumNum2=0;
    }//End for    
}


void Filter:: FastFilterFull(std::vector <UShort_t> &trace,
			     std::vector <Double_t> &thisEventsFF,
			     Double_t FL,Double_t FG,Double_t decayTime)
{

  Double_t S0, Sg, S1; // Varibale names from Tan paper
  
  Double_t deltaT = 1; //1 clock tick

  Double_t b1 = TMath::Exp(-deltaT/decayTime);

  Double_t r1 = 1.0 / ( 1-b1);

  Double_t A0,A1,J;
  
  Double_t baseLine=0;

  for (int k=0;k<20;k++)
    baseLine+=trace[k];
  
  baseLine = baseLine/20.0;
  
  thisEventsFF.resize(trace.size(),0);

  for (int i=FL*2+FG;i< (int) trace.size(); ++i){
   
    S0=0;
    Sg=0;
    S1=0;
    
    //S0 loop
    for (int j=i-FL*2-FG;j<i-FL-FG;++j){
      if (j>=0 && j <(int) trace.size()){
	S0+=(trace[j]-baseLine);
	if (S0 <0)
	  S0=0;
      }
    }

    //Sg loop
    for (int j=i-FL-FG;j<i-FL;++j){
      if (j>=0 &&j <(int) trace.size()){
	Sg+=(trace[j]-baseLine);
	if (Sg<0)
	  Sg=0;
      }
    }

    //S1 loop

    for (int j=i-FL;j<i;++j){
      if (j>=0 &&j < (int) trace.size()){
	S1+=(trace[j]-baseLine);
	if (S1 <0)
	  S1=0;
      }
    }
    
     
    /*    A0= S0 / (r1*(1-Power(b1,FL)));
    
    J=(Power(b1,FL)*(1-Power(b1,FG))*S0 )/ (Power(b1,FL)-1) +Sg;
    
    A1=(1/r1)*( (Power(b1,FL+FG)*S0)/(Power(b1,FL)-1) - S1/(Power(b1,FL)-1));
    */
    
    Double_t a0,ag,a1;
    ag=1;
    a0=TMath::Power(b1,FL)/(TMath::Power(b1,FL)-1);
    a1=(-1.0/((1-TMath::Power(b1,FL))) );

    
    if (i+2*FL+FG < thisEventsFF.size()){
      thisEventsFF[i]=ag*Sg+a0*S0+a1*S1;
    }
  }


}


std::vector <Double_t> Filter::CFD(std::vector <Double_t> &thisEventsFF,
				   Double_t CFD_delay,
				   Double_t CFD_scale_factor){

  std::vector <Double_t> thisEventsCFD;
  thisEventsCFD.resize(thisEventsFF.size(),0);
  
  for (int j=0;j<(int) thisEventsFF.size() - CFD_delay;j++) {
    thisEventsCFD[j+CFD_delay] = thisEventsFF[j+CFD_delay] - 
      thisEventsFF[j]/ ( TMath::Power(2,CFD_scale_factor+1) );
  }

  return thisEventsCFD;

}


Double_t Filter::GetZeroCrossing(std::vector <Double_t> & CFD){


  Double_t softwareCFD;
  std::vector <Double_t> thisEventsZeroCrossings(0);

  for (int j=0;j< (int) CFD.size()-1;j++) { 
    if (CFD[j] >= 0 && CFD[j+1] < 0 && 
	TMath::Abs(CFD[j] - CFD[j+1]) > 15 && j>20)
      {//zero crossing point
	
	softwareCFD =j+ CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
	thisEventsZeroCrossings.push_back(softwareCFD);
	
      }
  }
  
  return thisEventsZeroCrossings[0]; // take the first one
}
