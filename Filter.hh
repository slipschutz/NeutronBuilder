

#include <vector>
#include <iostream>
#include "TFile.h"



class Filter {


public:

  Filter();
  

  
  void FastFilter(std::vector <UShort_t> &trace,
		  std::vector <Double_t> &thisEventsFF,Double_t FL,Double_t FG);
  
  void FastFilterFull(std::vector <UShort_t> &trace,
		      std::vector <Double_t> &thisEventsFF,
		      Double_t FL,Double_t FG,Double_t decayTime);


  
  std::vector <Double_t> CFD( std::vector <Double_t> &,Double_t,Double_t);

  
  Double_t GetZeroCrossing(std::vector <Double_t> &);

};
