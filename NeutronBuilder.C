#include <stdio.h>
#include <stdlib.h>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include <TRandom1.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "TRandom3.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TChain.h"

//Local Headers
#include "SL_Event.hh"
#include "Filter.hh"
#include "FileManager.h"
#include "InputManager.hh"
#include "CorrectionManager.hh"

#define BAD_NUM -10008



struct Sl_Event {
  Int_t channel;
  vector <UShort_t> trace;
  Long64_t jentry;
  Double_t time;
};



/*Bool_t checkChannels(vector <Sl_Event> &in){

  vector <Bool_t> ch(20,false);  //to make this work with different cable arrangements

  for (int i=0;i<in.size();i++){
      ch[in[i].channel]=true;
  }
  // if it was a good event there should be 3 trues
  //from 3 different channels
  int count=0;
  int liq_scint_count=0; //there should be only one liq scint
  // at the moment they are pluged into 8 and 9 so only one of those 
  //should be there 


  int tot =0;
  for (int i=0;i <ch.size();i++){
    if (ch[i]==true){
      count++;
      if (i==8 || i ==9 ){
	liq_scint_count++;
      } else {
	tot=tot +i;
      }
    }
  }
  
  if (count == 3 && liq_scint_count==1 && (tot==1 || tot==5 ))
    return true;
  else 
    return false;

}


*/
Bool_t checkChannels(vector <Sl_Event> &in){

  Bool_t b=false;

  for (int i=0;i<in.size();++i){
    if (in[i].channel == 8 )
      b=true;
  }
  return b;
}


using namespace std;


int main(int argc, char **argv){

  vector <string> inputs;
  for (int i=1;i<argc;++i){
    inputs.push_back(string(argv[i]));
  }
  if (inputs.size() == 0 ){ // no argumnets display helpful message
    cout<<"Usage: ./NeutronBuilder runNumber [options:value]"<<endl;
    return 0;
  }  
  
  InputManager theInputManager;
  if ( !  theInputManager.loadInputs(inputs) ){
    return 0;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////


  //load correcctions and settings
  
  Double_t sigma=theInputManager.sigma; // the sigma used in the fitting option

  Int_t runNum=theInputManager.runNum;
  Int_t numFiles=theInputManager.numFiles;

  Long64_t maxentry=-1;

  Bool_t makeTraces=theInputManager.makeTraces;

  Bool_t extFlag=theInputManager.ext_flag;
  Bool_t ext_sigma_flag=theInputManager.ext_sigma_flag;

  //defualt Filter settings see pixie manual
  Double_t FL=theInputManager.FL;
  Double_t FG=theInputManager.FG;
  int CFD_delay=theInputManager.d; //in clock ticks
  Double_t CFD_scale_factor =theInputManager.w;
  Bool_t correctionRun =theInputManager.correction;

  CorrectionManager corMan;
  corMan.loadFile(runNum);
  Double_t SDelta_T1_Cor=corMan.get("sdt1");
  Double_t SDelta_T2_Cor=corMan.get("sdt2");
  Double_t int_corrections[4];  
  int_corrections[0]=corMan.get("int0");
  int_corrections[1]=corMan.get("int1");
  int_corrections[2]=corMan.get("int2");
  int_corrections[3]=corMan.get("int3");
  
  int degree=3;
  Double_t GOE_cor1[degree];
  Double_t GOE_cor2[degree];
  Double_t DeltaT_cor1[degree];
  Double_t DeltaT_cor2[degree];
  std::stringstream temp;
  for (int i=1;i<=degree;i++){
    temp.str("");
    temp<<"goe1_"<<i;
    GOE_cor1[i-1]=corMan.get(temp.str().c_str());
    temp.str("");
    temp<<"goe2_"<<i;
    GOE_cor2[i-1]=corMan.get(temp.str().c_str());

    temp.str("");
    temp<<"dt1_"<<i;
    DeltaT_cor1[i-1]=corMan.get(temp.str().c_str());

    temp.str("");
    temp<<"dt2_"<<i;
    DeltaT_cor1[i-1]=corMan.get(temp.str().c_str());
  }



  //prepare files and output tree
  ////////////////////////////////////////////////////////////////////////////////////
  TFile *outFile=0;
  TTree  *outT;
  FileManager * fileMan = new FileManager();
  fileMan->timingMode = theInputManager.timingMode;
  TChain * inT;

  if (!correctionRun){
    inT= new TChain("dchan");
    if (numFiles == -1 ){
      TString s = fileMan->loadFile(runNum,0);
      inT->Add(s);
    } else {
      for (Int_t i=0;i<numFiles;i++) {
	TString s = fileMan->loadFile(runNum,i);
	inT->Add(s);
      }
    }
  } else {
    inT= new TChain("flt");
    inT->Add((TString)theInputManager.specificFileName);
  }
    inT->SetMakeClass(1);
    Long64_t nentry=(Long64_t) (inT->GetEntries());

  cout <<"The number of entires is : "<< nentry << endl ;


  // Openning output Tree and output file
  if (correctionRun)
    outFile=fileMan->getOutputFile(theInputManager.specificFileName);
  else if (extFlag == false && ext_sigma_flag==false)
    outFile = fileMan->getOutputFile();
  else if (extFlag == true && ext_sigma_flag==false){
    CFD_scale_factor = CFD_scale_factor/10.0; //bash script does things in whole numbers
    outFile = fileMan->getOutputFile(FL,FG,CFD_delay,CFD_scale_factor*10);
  } else if (extFlag==false && ext_sigma_flag==true){
    sigma=sigma/10;
    outFile= fileMan->getOutputFile(sigma*10);
  }

  outT = new TTree("flt","Filtered Data Tree --- Comment Description");
  cout << "Creating filtered Tree"<<endl;
  if(!outT)
    {
      cout << "\nCould not create flt Tree in " << fileMan->outputFileName.str() << endl;
      exit(-1);
    }
  ////////////////////////////////////////////////////////////////////////////////////
  
  Int_t numOfChannels=4;  
  // set input tree branvh variables and addresses
  ////////////////////////////////////////////////////////////////////////////////////
  Int_t chanid;
  Int_t slotid;
  vector<UShort_t> trace;
  UInt_t fUniqueID;
  UInt_t energy;
  Double_t time ; 
  UInt_t timelow; // this used to be usgined long
  UInt_t timehigh; // this used to be usgined long
  UInt_t timecfd ; 
  Double_t correlatedTimes_in[numOfChannels];
  Double_t integrals_in[numOfChannels];
  if (! correctionRun ){
    //In put tree branches    
    inT->SetBranchAddress("chanid", &chanid);
    inT->SetBranchAddress("fUniqueID", &fUniqueID);
    inT->SetBranchAddress("energy", &energy);
    inT->SetBranchAddress("timelow", &timelow);
    inT->SetBranchAddress("timehigh", &timehigh);
    inT->SetBranchAddress("trace", &trace);
    inT->SetBranchAddress("timecfd", &timecfd);
    inT->SetBranchAddress("slotid", &slotid);
    inT->SetBranchAddress("time", &time);
  } else {
    inT->SetBranchAddress("Time0",&correlatedTimes_in[0]);
    inT->SetBranchAddress("Time1",&correlatedTimes_in[1]);
    inT->SetBranchAddress("Time2",&correlatedTimes_in[2]);
    inT->SetBranchAddress("Time3",&correlatedTimes_in[3]);

    inT->SetBranchAddress("Integral0",&integrals_in[0]);
    inT->SetBranchAddress("Integral1",&integrals_in[1]);
    inT->SetBranchAddress("Integral2",&integrals_in[2]);
    inT->SetBranchAddress("Integral3",&integrals_in[3]);

  }
  
  ////////////////////////////////////////////////////////////////////////////////////


  //set output tree branches and varibables 
  ////////////////////////////////////////////////////////////////////////////////////


  vector <Sl_Event> previousEvents;
  Double_t sizeOfRollingWindow=1;  //Require that a lenda bar fired in both PMTS and one liquid scint
  
  //Out put tree branches 


  //Branches for explict trace reconstruction
  TH2F *traces   = new TH2F("traces","This these are the original traces",200,0,200,10000,-1000,1000);
  TH2F *filters = new TH2F("filters","The filters",200,0,200,10000,-1000,4000);
  TH2F *CFDs  = new TH2F("CFDs","The CFDs",200,0,200,10000,-1000,1000);

  SL_Event* Event = new SL_Event();
  outT->Branch("Event","SL_Event",&Event,2000000);
  outT->BranchRef();
  //  TGraph * traces2 = new TGraph(200);


  if (makeTraces) //adding the branches to the tree slows things down   
    {             //so only do it if you really want them
      outT->Branch("Traces","TH2F",&traces,128000,0);  
      outT->Branch("Filters","TH2F",&filters,12800,0);
      outT->Branch("CFDs","TH2F",&CFDs,12800,0);
      //      outT->Branch("Traces2","TGraph",&traces2,128000,0);
    }

  ////////////////////////////////////////////////////////////////////////////////////

  if(maxentry == -1)
    maxentry=nentry;
  
  if (makeTraces)
    maxentry=50;//cap off the number of entries for explict trace making



  //non branch timing variables 
  ////////////////////////////////////////////////////////////////////////////////////

  Filter theFilter; // Filter object
  ////////////////////////////////////////////////////////////////////////////////////
  Bool_t normalSet=true;
  for (Long64_t jentry=0; jentry<10;jentry++){
    inT->GetEntry(jentry);
    if(slotid!=2){
      normalSet=false;
      break;
    }
  }
  Double_t softwareCFD;

  for (Long64_t jentry=0; jentry<maxentry;jentry++) { // Main analysis loop
    
    inT->GetEntry(jentry); // Get the event from the input tree 

    if (normalSet == false)
      chanid=slotid;
    
    
    if (makeTraces){ //reset histograms if makeTraces is on
	traces->Reset();
	filters->Reset();
	CFDs->Reset();
      }
    
    ///////////////////////////////////////////////////////////////////////////////////////////

    
    if ( previousEvents.size() >= sizeOfRollingWindow ) {
      if ( checkChannels(previousEvents) )//prelinary check to see if there are 3 distinict channels in set
	{ // there are some amount of channels
	  
	  //for cable arrangement independance
	  //sort the last size of rolling window evens by channel
	  vector <Sl_Event*> events_extra(20,(Sl_Event*)0);
	  vector <Sl_Event*> events;
	  Double_t thisEventsIntegral=0;
	  Double_t longGate,shortGate;
	  Double_t start;
	  Double_t timeDiff;

	  for (int i=0;i<previousEvents.size();++i){
	    events_extra[previousEvents[i].channel]=&(previousEvents[i]);
	  }
	  for (int i=0;i<events_extra.size();++i){
	    if (events_extra[i] != 0 ){
	      events.push_back(events_extra[i]);
	    }
	  }

	  //	  timeDiff =TMath::Abs( events[2]->time - 0.5*(events[0]->time + events[1]->time));
	  if (true){
	    ///Good event
	    //Run filters and such on these events 
	    vector <Double_t> thisEventsFF;
	    vector <Double_t> thisEventsCFD;
	    
	    for (int i=0;i<events.size();++i){
	      
	      if((theInputManager.timingMode == "softwareCFD" || theInputManager.timingMode == "fitting") ){
		theFilter.FastFilter(events[i]->trace,thisEventsFF,FL,FG);
	      }
	      thisEventsCFD = theFilter.CFD(thisEventsFF,CFD_delay,CFD_scale_factor);
	      softwareCFD = theFilter.GetZeroCrossing(thisEventsCFD);
	      start = TMath::Floor(softwareCFD) -5;
	      thisEventsIntegral = theFilter.getEnergy(events[i]->trace);
	      longGate = theFilter.getGate(events[i]->trace,start,25);
	      shortGate = theFilter.getGate(events[i]->trace,start,12);
	      
	      Event->shortGate2 =  theFilter.getGate(events[i]->trace,start,14);
	      Event->longGate2 =  theFilter.getGate(events[i]->trace,start,25);

	      Event->shortGate3 =  theFilter.getGate(events[i]->trace,start,16);
	      Event->longGate3 =  theFilter.getGate(events[i]->trace,start,25);

	      Event->shortGate4 =  theFilter.getGate(events[i]->trace,start,18);
	      Event->longGate4 =  theFilter.getGate(events[i]->trace,start,25);
	      
	      Event->Time_Diff = timeDiff;
	      Event->longGate = longGate;
	      Event->shortGate = shortGate;
	      Event->pushChannel(events[i]->channel);
	      Event->pushEnergy(thisEventsIntegral);
	      Event->pushTime(events[i]->time);
	    }
	    Event->Finalize();
	    outT->Fill();
	    Event->Clear();
    	    
	  }
	}
    }
    
    
    
    
    
    
      
    //Keep the previous event info for correlating      
    if (previousEvents.size() < sizeOfRollingWindow  ) 
      {
	Sl_Event e;
	e.channel=chanid;
	e.trace=trace;
	e.jentry=jentry;
	e.time =time;
	previousEvents.push_back(e);
      }
    else if (previousEvents.size() >= sizeOfRollingWindow )
      {
	//So starting on the fith element 
	previousEvents.erase(previousEvents.begin(),previousEvents.begin() + 1);
	Sl_Event e;
	e.channel=chanid;
	e.trace = trace;
	e.jentry =jentry;
	e.time=time;
	previousEvents.push_back(e);	  
      }
  
  //Periodic printing
  if (jentry % 10000 == 0 )
    cout<<"On event "<<jentry<<endl;
  
  //Fill the tree
  if (makeTraces)
    outT->Fill();
  
}//End for



outT->Write();
outFile->Close();

cout<<"Number of bad fits "<<theFilter.numOfBadFits<<endl;

cout<<"\n\n**Finished**\n\n";

return  0;

}

