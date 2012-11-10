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
  

  //File name streams
  std::stringstream fileName;
  std::stringstream outputFileName;

  

};
