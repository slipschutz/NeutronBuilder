//
//  File managerment class
//

#include "TFile.h"
#include <sstream>

class FileManager {

public:
  FileManager();
  TString loadFile(Int_t runNum,Int_t );

  TFile * getOutputFile();
  TFile * getOutputFile(Double_t FL, Double_t FG, Double_t d, Double_t w);

  //File name streams
  std::stringstream fileName;
  std::stringstream outputFileName;


  
  

};
