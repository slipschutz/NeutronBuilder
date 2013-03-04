#ifndef __SL_EVENT_HH
#define __SL_EVENT_HH

#include "TObject.h"
#include "TFile.h"
#include <vector>
using namespace std;
class SL_Event : public TObject {
public:
  
  SL_Event();
  void Clear();
  void Finalize();

  Double_t Time_Diff;
  vector <Double_t> times;
  vector <Double_t> energies;
  vector <Double_t> channels;
  
  Double_t times2[3];
  Double_t energies2[3];
  Double_t channels2[3];
  
  Double_t shortGate;
  Double_t longGate;

  Double_t shortGate2;
  Double_t longGate2;

  Double_t shortGate3;
  Double_t longGate3;

  Double_t shortGate4;
  Double_t longGate4;


  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);

private:






  ClassDef(SL_Event, 1);
};

#endif



