
#include "FileManager.h"

#include <iostream>
#include <sstream>

#include "TFile.h"
#include <stdlib.h>

using namespace std;
FileManager::FileManager()
{
  //Constructor for file manager

  outputFileName.str("Defualt");
  fileName.str("Defualt");


}

TFile *FileManager::loadFile(Int_t runNum) {
  
  
  if (runNum <10){
    fileName<<"./run-000"<< runNum <<"-00";
  } else if (runNum < 100){
    fileName<<"./run-00"<< runNum <<"-00";
  } else if (runNum <1000) {
    fileName<<"./run-0"<< runNum <<"-00";
  } else {
    cout<<"Update run number parsing"<<endl; return 0;
  }
  
  outputFileName << fileName.str()<<"-output.root"; //For later                                                                                              
  fileName << ".root";
  
  cout<<"Loading file "<<fileName.str()<<"..."<<endl;
  
  
  
  return new TFile(fileName.str().c_str(),"read");


}

TFile * FileManager::getOutputFile(){
 
  TFile * temp = new TFile(outputFileName.str().c_str(),"recreate");
   
  if(!temp)
    {
      cout << "\nCould not open " << outputFileName.str() <<endl;
      exit(-1);
    } else
      cout << "Opened output file " <<outputFileName.str()<< endl;

  return temp;
}
