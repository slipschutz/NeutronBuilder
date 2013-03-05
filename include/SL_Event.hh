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

  vector < vector <UShort_t> > traces;
  

  vector <Double_t> shiftCorrectedTimes;


  Double_t times2[3];
  Double_t energies2[3];
  Double_t channels2[3];
  
  Double_t shortGate;
  Double_t longGate;




  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);

  void pushShiftCorrections(Double_t,Double_t);

private:






  ClassDef(SL_Event, 1);
};

#endif



