

#include "SL_Event.hh"
#include <iostream>
using namespace std;



SL_Event::SL_Event()
{

  Clear();

}

#define BAD_NUM -1008
void SL_Event::Clear(){

  Time_Diff=BAD_NUM;
  times.clear();
  energies.clear();
  channels.clear();
  shortGate=BAD_NUM;
  longGate=BAD_NUM;

}

void SL_Event::pushTime(Double_t t){
  times.push_back(t);

}
void SL_Event::pushEnergy(Double_t e){
  energies.push_back(e);

}
void SL_Event::pushChannel(Double_t c){
  channels.push_back(c);
}

void SL_Event::Finalize(){
  for (int i=0;i<3;i++){
    times2[i]=times[i];
    channels2[i]=channels[i];
    energies2[i]=energies[i];
  }

}
