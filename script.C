

//
//
//
//  Magic numbers to take input number of kev
//  and convert it to raw number from trace amplitude
//  based on position of first coumpton edge seen in runs
//  264 and 265
//
// For bars 12 and 24
// bar 12 is channels 0,1
// bar 24 is channels 2,3
#include <sstream>
#include <iostream>
void script(Int_t num)

{

  cout<<"Channel0 "<<609*num/341<<endl;
  cout<<"Channel1 "<<539*num/341<<endl;
  cout<<"Channel2 "<<735*num/341<<endl;
  cout<<"Channel3 "<<511*num/341<<endl;




  Int_t E0=609*num/341;
  Int_t E1=539*num/341;
  Int_t E2=735*num/341;
  Int_t E3=511*num/341;


  std::stringstream s;

  
  s<<"Time_Diff> 10.8 && Time_Diff <11.8 && Integral0 >"<<E0<<" && Integral1 > "<<E1<<" &&Integral2 > "<<E2 <<" &&Integral3 >"<<E3;





  flt->Draw("Time_Diff",s.str().c_str());
  
  TFitResultPtr fitPointer = htemp->Fit("gaus","SQ");
  
  cout<<fitPointer->Value(2)*10000<<endl;
  num = num + 10;

}
