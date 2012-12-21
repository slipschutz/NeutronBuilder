 

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

//Local Headers
#include "SL_Event.h"
#include "Filter.hh"
#include "FileManager.h"
#include "InputManager.hh"


#include "TChain.h"

#define BAD_NUM -10008


Bool_t checkChannels(Int_t a,Int_t b){
  if (TMath::Abs(a-b) == 1)
    return true;
  else
    return false;
}


struct Sl_Event {
  Int_t channel;
  Double_t time;
  Double_t integral;
  Double_t softwareCFD;
};

using namespace std;

//int main(Int_t runNum=0,Long64_t maxentry=-1,Bool_t makeTraces=false){
int main(int argc, char **argv){

  vector <string> inputs;
  for (int i=1;i<argc;++i){
    inputs.push_back(string(argv[i]));
  }

  InputManager theInputManager;

  if ( !  theInputManager.loadInputs2(inputs) )
    return 0;



  Int_t runNum=theInputManager.runNum;
  Int_t numFiles=theInputManager.numFiles;

  Long64_t maxentry=-1;

  Bool_t makeTraces=theInputManager.makeTraces;
  Bool_t useSoftwareFilters=true;
  Bool_t extFlag=false;//need to add meta run inputs to inputmanager


  //defualt Filter settings see pixie manual
  Double_t FL=2;
  Double_t FG=0;
  int CFD_delay=2; //in clock ticks
  Double_t CFD_scale_factor =0.25;


  

  
  TFile *inFile=0;
  TFile *outFile=0;
  TTree  *outT;

  FileManager * fileMan = new FileManager();
  TChain * inT= new TChain("dchan");
 
  if (numFiles == -1 ){
    TString s = fileMan->loadFile(runNum,0);//new TFile(fileName.str().c_str(),"read");
    inT->Add(s);
  } else {
    for (Int_t i=0;i<numFiles;i++) {
      TString s = fileMan->loadFile(runNum,i);
      inT->Add(s);
    }
  }


  //  inT = (TTree *) inFile->Get("dchan");
  
  inT->SetMakeClass(1);
  Long64_t nentry=(Long64_t) (inT->GetEntries());

  cout <<"The number of entires is : "<< nentry << endl ;

  // Openning output Tree and output file
  if (extFlag == false)
    outFile = fileMan->getOutputFile();
  else
    outFile = fileMan->getOutputFile(FL,FG,CFD_delay,CFD_scale_factor);

  outT = new TTree("flt","Filtered Data Tree --- Comment Description");
  cout << "Creating filtered Tree"<<endl;
  if(!outT)
    {
      cout << "\nCould not create flt Tree in " << fileMan->outputFileName.str() << endl;
      exit(-1);
    }



  // original
  Int_t chanid;
  vector<UShort_t> trace;
  UInt_t fUniqueID;
  UInt_t energy;
  Double_t time ; 
  UInt_t timelow; // this used to be usgined long
  UInt_t timehigh; // this used to be usgined long
  UInt_t timecfd ; 



  //In put tree branches    
  inT->SetBranchAddress("chanid", &chanid);
  inT->SetBranchAddress("fUniqueID", &fUniqueID);
  inT->SetBranchAddress("energy", &energy);
  inT->SetBranchAddress("timelow", &timelow);
  inT->SetBranchAddress("timehigh", &timehigh);
  inT->SetBranchAddress("trace", &trace);
  inT->SetBranchAddress("timecfd", &timecfd);
  inT->SetBranchAddress("time", &time);


  Int_t numOfChannels=10;///more channels then i need

  vector <Sl_Event> previousEvents;
  Double_t sizeOfRollingWindow=6;
  
  //Out put tree branches 
  Double_t timeDiff;
  outT->Branch("Time_Diff",&timeDiff,"Time_Diff/D");
 
  Double_t timeDiffRaw;
  outT->Branch("Time_Diff_Raw",&timeDiffRaw,"Time_Diff_Raw/D");

  Double_t softwareCFD;
  outT->Branch("SoftwareCFD",&softwareCFD,"SoftwareCDF/D");

  Double_t integrals[numOfChannels];
    
  outT->Branch("Integral0",&integrals[0],"Integral0/D");
  outT->Branch("Integral1",&integrals[1],"Integral1/D");
  outT->Branch("Integral2",&integrals[2],"Integral2/D");
  outT->Branch("Integral3",&integrals[3],"Integral3/D");
  outT->Branch("Integral4",&integrals[4],"Integral4/D");
  outT->Branch("Integral5",&integrals[5],"Integral5/D");

  Double_t delta_T1;
  outT->Branch("Delta_T1",&delta_T1,"Delta_T1/D");
  Double_t delta_T2;
  outT->Branch("Delta_T2",&delta_T2,"Delta_T2/D");
  
  Double_t GravityOfEnergy1;
  outT->Branch("GravityOfEnergy1",&GravityOfEnergy1,"GravityOfEnergy1/D");

  Double_t GravityOfEnergy2;
  outT->Branch("GravityOfEnergy2",&GravityOfEnergy2,"GravityOfEnergy2/D");

  Double_t GOE1;
  Double_t GOE2;

  outT->Branch("GOE1",&GOE1,"GOE1/D");
  outT->Branch("GOE2",&GOE2,"GOE2/D");


  //Branches for explict trace reconstruction
  TH2F *traces   = new TH2F("traces","This these are the original traces",200,0,200,10000,-1000,1000);
  TH2F *filters = new TH2F("filters","The filters",200,0,200,10000,-1000,4000);
  TH2F *CFDs  = new TH2F("CFDs","The CFDs",200,0,200,10000,-1000,1000);

  TGraph * traces2 = new TGraph(200);

  if (makeTraces) //adding the branches to the tree slows things down   
    {             //so only do it if you really want them
      outT->Branch("Traces","TH2F",&traces,128000,0);  
      outT->Branch("Filters","TH2F",&filters,12800,0);
      outT->Branch("CFDs","TH2F",&CFDs,12800,0);
      outT->Branch("Traces2","TGraph",&traces2,128000,0);
    }
  //////////////////////////////////////////////////////////////////////////////////////////////

  //
  
  Double_t eventNum;
  outT->Branch("Jentry",&eventNum,"Jengrty/D");

  Double_t eventTriggerNum;
  outT->Branch("EventTriggerNum",&eventTriggerNum,"EventTriggerNum/D");



  if(maxentry == -1)
    maxentry=nentry;
  
  if (makeTraces)
    maxentry=50;//cap off the number of entries for explict trace making




  Double_t Time_1 =0;
  Double_t Time_2 =0;
  Double_t prevTime =0;
  Double_t totalGoodEvents=0;
  vector <Double_t> thisEventsFF;
  vector <Double_t> thisEventsCFD;
                                   //zero crossings
  Double_t thisEventsIntegral=0;

  Filter theFilter; // Filter object


  for (Long64_t jentry=0; jentry<maxentry;jentry++) { // Main analysis loop
    
    inT->GetEntry(jentry); // Get the event from the input tree 
    eventNum=jentry;

    eventTriggerNum=0;//its 0 when there is no correlated event found on this loop
    //set to one below if there was a correlated event fround in the 
    //previous set of events (sizeOfRollingWindow)

    ///////////////////////////////////////////////////////////////////////////////////////////
    timeDiffRaw=0;     //TimeDiffRaw is just the difference between the previous event and
                       //the current one
    timeDiff = BAD_NUM;  //make it something random to distinguish uncorrleated events  
    softwareCFD = BAD_NUM;//ditto
    
    GravityOfEnergy1 = BAD_NUM;    
    GravityOfEnergy2 = BAD_NUM;    
    GOE1=BAD_NUM;
    GOE2=BAD_NUM;
    delta_T1 = BAD_NUM;
    delta_T2 = BAD_NUM;
    
    for (Int_t i=0;i<(Int_t) numOfChannels;++i)
      integrals[i]=BAD_NUM; //ditto
    
    //software genearted filters
    thisEventsCFD.clear();//Clear the CFD vector 
    thisEventsFF.clear();//Clear this events fast filter
    
    if (makeTraces) //reset histograms if makeTraces is on
      {
	traces->Reset();
	filters->Reset();
	CFDs->Reset();
      }
    
    //Time_diff raw 
    timeDiffRaw = time - prevTime;
    prevTime = time;
    ///
    if(theInputManager.timingMode == "softwareCFD" || theInputManager.timingMode == "traces"){
      if(theInputManager.timingMode == "traces" ){
	softwareCFD = theFilter.fitTrace(trace,jentry);
      } else {
	theFilter.FastFilter(trace,thisEventsFF,FL,FG);
	//theFilter.FastFilterFull(trace,thisEventsFF,FL,FG,40);
	if (makeTraces )	{
	  for (int i=0;i< (int) trace.size();i++) {
	    traces->Fill(i,trace[i]);	
	    traces2->SetPoint(i,i,trace[i]);
	    filters->Fill(i, thisEventsFF[i]);
	  }
	}
        thisEventsCFD = theFilter.CFD(thisEventsFF,CFD_delay,CFD_scale_factor);
	softwareCFD = theFilter.GetZeroCrossing(thisEventsCFD);
	if (makeTraces){
	  for (Int_t j=0;j<(Int_t) thisEventsCFD.size();++j)
	    CFDs->Fill(j,thisEventsCFD[j]);
	}
      }
    }
    
    Double_t sum=0;
    Double_t signalTotalIntegral=0;
    for ( int i=0 ;i<10;i++)
      sum = sum + trace[i]+trace[trace.size()-1-i];
    sum = sum/20; // average of first and last 10 points should be pretty good background
    for (int i=0;i< (int) trace.size();++i) {
      signalTotalIntegral = trace[i]+ signalTotalIntegral;
    }
    if (  signalTotalIntegral - sum *trace.size() > 0 ) 
      thisEventsIntegral = signalTotalIntegral - sum *trace.size();
    else{
      thisEventsIntegral = BAD_NUM;
    }
    //set the energy in the filterd tree
    integrals[chanid] = thisEventsIntegral;
    Double_t thresh=10;
    
    if ( previousEvents.size() >= sizeOfRollingWindow )
      {
	/*	cout<<endl<<"*****"<<endl;
	for (Int_t j=0;j<sizeOfRollingWindow;j++){
	  
	  cout<<"Channel "<<previousEvents[j].channel<<endl;
	  cout<<"Time "<<previousEvents[j].time<<endl;
	  cout<<"Integral "<<previousEvents[j].integral<<endl;
	  cout<<"Software CFD "<<previousEvents[j].softwareCFD<<endl;
	}
	int t;cin>>t;
	*/
	for (Int_t q=0;q<3;++q){	
	  Int_t firstSpot=-1;
	  Int_t secondSpot=-1;
	  Double_t avg1=0;
	  Double_t avg2=0;
	  for (Int_t i=1;i<(Int_t) previousEvents.size();++i) {
	    if ( (previousEvents[i].time -previousEvents[0].time < 2)
		 && checkChannels(previousEvents[i].channel,previousEvents[0].channel)) {
	      firstSpot=i;
	      break;
	    }
	  }
	  
	  for (Int_t i=firstSpot+2+q;i<(Int_t) previousEvents.size();++i) {
	    if ( (previousEvents[i].time -previousEvents[firstSpot+1+q].time < 2)
		 && checkChannels(previousEvents[i].channel,previousEvents[firstSpot+1+q].channel)) {
	      secondSpot=i;
	      break;
	    }
	  }
	  
	  if (secondSpot != -1 && firstSpot != -1 ){
	    
	    avg1= TMath::Sqrt(previousEvents[0].time * previousEvents[firstSpot].time);
	    avg2=TMath::Sqrt(previousEvents[firstSpot+1+q].time *previousEvents[secondSpot].time);
	    
	    
	    if ( (previousEvents[0].channel*previousEvents[firstSpot].channel !=
		  previousEvents[firstSpot+1+q].channel*previousEvents[secondSpot].channel) &&
		 (TMath::Abs(avg2-avg1) < 10.0)) {
	      //	      cout<<"jentry is "<<jentry<<endl;

	      //Victory
	      /*cout<<"Victory"<<endl;
	      cout<<"*************"<<endl;
	      cout<<"Time diff "<<avg2-avg1<<endl;
	      cout<<"firstSpot "<<firstSpot<<" q "<<q<< " secondSpot "<<secondSpot<<endl;
	      cout<<"Channels are "<<previousEvents[0].channel<<" "<<previousEvents[firstSpot].channel<<
		" and "<<previousEvents[firstSpot+1+q].channel << " "<<previousEvents[secondSpot].channel<<endl;
	      //	      int t;cin>>t;
	      */
	      if (q!=0 || firstSpot!=1)
		cout<<"q "<<q<<" "<<firstSpot<<endl;
	      
	      
	      /*cout<<"***********"<<endl;
	      cout<<previousEvents[0].channel-2<<endl;
	      cout<<previousEvents[firstSpot].channel-2<<endl;
	      cout<<previousEvents[firstSpot+1+q].channel-2<<endl;
	      cout<<previousEvents[secondSpot].channel-2<<endl;
	      int t; cin>>t;
	      */
	      integrals[previousEvents[0].channel] = previousEvents[0].integral;
	      integrals[previousEvents[firstSpot].channel] = previousEvents[firstSpot].integral;
	      integrals[previousEvents[firstSpot+1+q].channel] = previousEvents[firstSpot+1+q].integral;
	      integrals[previousEvents[secondSpot].channel] = previousEvents[secondSpot].integral;

	      delta_T1 =  previousEvents[firstSpot].time-previousEvents[0].time;
	      delta_T2 =  previousEvents[secondSpot].time-previousEvents[firstSpot+1+q].time;

	      GravityOfEnergy1 = (integrals[1]-integrals[0])/(integrals[0]+integrals[1]);
	      GravityOfEnergy2 = (integrals[3]-integrals[2])/(integrals[2]+integrals[3]);
	      

	      Double_t e0,e1,e2,e3; //Gain matched energies matched to chan2
	      e0 = integrals[0]+182.906;
	      e1 = integrals[1]+220.97;
	      e2 = integrals[2];
	      e3 = integrals[3]+223.688;

	      GOE1 = ( e1 - e0 ) / (e0 + e1 );
	      GOE2 = ( e3-e2 )/(e2 + e3);
	      
	      
	      if (previousEvents[firstSpot].channel == 2 || previousEvents[firstSpot].channel == 4){
		delta_T1=-1*delta_T1;//want to always have the same channel minus the the same channel
		GravityOfEnergy1 =-1*GravityOfEnergy1;
		GOE1=-1*GOE1;
	      }
	      if (previousEvents[secondSpot].channel == 2 || previousEvents[secondSpot].channel == 4){
		delta_T2=-1*delta_T2;
		GravityOfEnergy2 =-1*GravityOfEnergy2;
		GOE2=-1*GOE2;
	      }
	      
	      eventTriggerNum=1;
	      timeDiff=TMath::Abs(avg2-avg1);

	      q=1000;//kill outer loop
	    }
	  }
	}
      }
  

      //Keep the previous event info for correlating
      if (useSoftwareFilters)
	time = softwareCFD + timelow+timehigh * 4294967296.0;
      
      if (previousEvents.size() < sizeOfRollingWindow  ) 
	{
	  Sl_Event e;
	  e.channel=chanid;
	  e.time = time;
	  e.integral=thisEventsIntegral;
	  e.softwareCFD = softwareCFD;
	  previousEvents.push_back(e);
	}
      else if (previousEvents.size() >= sizeOfRollingWindow )
	{
	  //So starting on the fith element 
	  previousEvents.erase(previousEvents.begin(),previousEvents.begin() + 1);
	  Sl_Event e;
	  e.channel=chanid;
	  e.time=time;
	  e.integral=thisEventsIntegral;
	  e.softwareCFD = softwareCFD;
	  previousEvents.push_back(e);	  
	}
  

    //Periodic printing
    if (jentry % 10000 == 0 )
      cout<<"On event "<<jentry<<endl;

    //Fill the tree
    outT->Fill();
    
  }//End for
  
  
  outT->Write();
  outFile->Close();
  
  cout<<totalGoodEvents<<endl;
  cout<<"\n\n**Finished**\n\n";

  return  0;

}

