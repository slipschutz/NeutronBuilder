

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "TTree.h"

using namespace std;
class Entry{
public:
  double value;
  string tag;
};

class CorrectionManager {

public:
  
  CorrectionManager();
  
  void addEntry(string,double);
  int findEntry(string);
  
  Double_t get(string);
  
  void loadFile(int);
  void dump();

private:
  
  vector <Entry> theCorrections; 



};
