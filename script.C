

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

void script(Int_t num)

{

  cout<<"Channel0 "<<601*num/341<<endl;
  cout<<"Channel1 "<<563*num/341<<endl;
  cout<<"Channel2 "<<784*num/341<<endl;
  cout<<"Channel3 "<<560*num/341<<endl;



  Int_t E1=601*num/341;

  Int_t E2=563*num/341;

  Int_t E3=784*num/341;
  
  Int_t E4=560*num/341;


  //  flt->Draw("Time_Diff","Time_Diff > -1 && Time_Diff <10 && Integral0 > E1 &&Integral1>E2&&Integral2>E3&&Integral3>E4");
  

}
