


#include "InputManager.hh"
#include <vector>
#include <string>


//



InputManager::InputManager()
{
  //set defualts
  runNum=-1;
  numFiles=-1;
  timingMode="softwareCFD";
  makeTraces=false;

  validTimingModes.push_back("internalCFD");
  validTimingModes.push_back("softwareCFD");
  validTimingModes.push_back("traces");

}
InputManager::~InputManager()
{

}


Bool_t InputManager::loadInputs(vector <string> & inputs){
  /*
  Int_t numOfInputs = (Int_t) inputs.size();

  //look at the inputs 
  for (Int_t i =0 ;i<numOfInputs;++i){
    if ( i == 1 )
      runNum = (int) atoi(inputs[i].c_str());
    if (i==2 )
      numFiles = (Int_t) atoi(inputs[i].c_str());
    if (i>=2)
      options.push_back( inputs[i]);
  }


  // check types for runNum and numFiles

  if ( runNum == 0 ) {
    cout<<"runNum "<<endl;
    return false;
  }
  if ( numFiles ==0 && numOfInputs !=3 ){
    cout<<"numFiles"<<endl;
    return false;
  }
 


  
  if ( numOfInputs == 3 )  {
    //
    // could be ./EvtBuilder runNum numFiles
    // or
    //
    //  ./EvtBuilder runNum traces/internalCFD/otherOption
    
    if ( numFiles == 0){
      //no number of files given assume
      //there is one file
      //and it must the second option from above
      numFiles=1;
      
      for (int i=0;i<options.size();++i){
	if ( options[i] == "traces")
	  useTrace = true;
	  

      }
	
    }  
  }
  
      if ( (string) argv[2] == "traces")
	makeTraces=true;
      else if ( (string) argv[2] == "internalCFD")
	useSoftwareFilters=false;
    } else {
      // the first option from above
      numFiles= atoi(argv[2]);
    }
  } else if (argc == 4){
    // ./EvtBuilder runNum numFiles options

    runNum = (Int_t) atoi(argv[1]);
    numFiles = (Int_t) atoi(argv[2]);
    
    if (numFiles == 0 ){
      cout<<"Invalid options"<<endl;
      return -1;
    }
    //options at [3] now
    if ( (string) argv[3] == "traces")
      makeTraces=true;
    else if ( (string) argv[3] == "internalCFD")
      useSoftwareFilters=false;
    
    cout<<"Make Traces"<<endl;


    
  } else if (argc == 7){
    runNum = (Int_t) atoi(argv[1]);
    numFiles = (Int_t) atoi(argv[2]);
    FL = (Double_t) atoi(argv[3]);
    FG = (Double_t) atoi(argv[4]);
    CFD_delay = (Double_t) atoi(argv[5]);
    CFD_scale_factor = (Double_t) atof(argv[6]);

    cout<<"FL "<<FL<<endl;
    cout<<"FG "<<FG<<endl;
    cout<<"D "<<CFD_delay<<endl;
    cout<<"CFD_scale_factor "<<CFD_scale_factor<<endl;

    extFlag=true;
  }

  */
}



vector <string>  InputManager::split (const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while(getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


vector <string> InputManager::split(const string &s, char delim) {
  vector<string> elems;
  return split(s, delim, elems);
}





Bool_t InputManager::loadInputs2(vector <string> & inputs){

  vector <string> flags;
  vector <string> arguments;
  vector <string> temp(2);
  //the first input has to be the run number
  
  if ( atoi(inputs[0].c_str() ) == 0 )
    cout<<"Must supply a integer runNumber"<<endl;
  else
    runNum = atoi(inputs[0].c_str());


  if (inputs.size() == 0 ){ // no argumnets display helpful message
    cout<<"Usage: ./EvtBuilder runNumber [numFiles] [optionString]"<<endl;
    return false;
  }  
  
  for (int i =1;i<inputs.size();++i){
    
    temp = split(inputs[i],':');
    flags.push_back(temp[0]);
    arguments.push_back(temp[1]);
  }
  
  for (int i=0;i<flags.size();++i){
    
    if (flags[i] == "numFiles")
      numFiles = atoi (arguments[i].c_str() );
    else if (flags[i] == "timingMode") 
      timingMode=arguments[i];
    else if (flags[i] == "numFiles")
      numFiles =atoi( arguments[i].c_str());
    else if (flags[i] == "makeTraces"){
      if (arguments[i] == "true" ){
	makeTraces=true;
      } else if (arguments[i] =="false" ){
	makeTraces =false;
      }else {
	cout<<"makeTraces needs to be true or false"<<endl;
	return false;
      } 
    } else {
      cout<<flags[i] <<" :Unkown option"<<endl;
      return false;
    }  
  }

  return checkValues();

}


Bool_t InputManager::checkValues()
{
  Bool_t nothingWrong=true;
  Bool_t timingBool=false;
  
  if (numFiles <=0 || runNum <=0 )
    nothingWrong=false;

  if (makeTraces != true && makeTraces != false)
    nothingWrong =false;
  
  for (int i=0;i<validTimingModes.size();++i){
    if (timingMode == validTimingModes[i])
      timingBool=true;
  }
  
  if (timingBool == false ){
    nothingWrong =false;
    dumpValidModes();
  }


  return nothingWrong;
}

void InputManager::dumpValidModes(){
  cout<<"The valid timing modes are "<<endl;
  for (int i=0;i<validTimingModes.size();++i)
    cout<<validTimingModes[i]<<endl;
}
