//
//  File managerment class
//

#include "TFile.h"
#include <sstream>

class FileManager {

public:
  FileManager();
  TFile * loadFile(Int_t runNum );

  TFile * getOutputFile();
  TFile * getOutputFile(Double_t FL, Double_t FG, Double_t d, Double_t w);

  //File name streams
  std::stringstream fileName;
  std::stringstream outputFileName;

  

};
