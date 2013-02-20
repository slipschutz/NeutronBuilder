

#include "TFile.h"
class SL_Event : public TObject {
  
  
public:
  SL_Event();
  Int_t channel;
  Double_t time;
  Double_t integral;
  Double_t softwareCFD;
  Double_t slowGate;
  Double_t fastGate;

private:
protected:

    ClassDef(SL_Event, 1);
};
