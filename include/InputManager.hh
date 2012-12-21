

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include "TTree.h"

using namespace std;

class InputManager {

public:
  
  InputManager();
  ~InputManager();
  
  Bool_t loadInputs(vector <string> &);

  Bool_t loadInputs2(vector <string> &);

  Int_t numFiles,runNum;
  vector <string> options;

  Bool_t makeTraces;
  
  string timingMode;
  
private:
  vector <string> split (const string &s, char delim, vector<string> &elems);
  vector <string> split (const string &s, char delim);

  Bool_t checkValues();

  vector<string> validTimingModes;
  void dumpValidModes();

};
