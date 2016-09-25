#include "BuildEvents.h"

using namespace std;

/*!
  Initialyze the event building
  \param brtr tree with input bigrips data
  \param eutr tree with input eurica data
*/
void BuildEvents::Init(TTree* brtr, TTree* eutr, TTree* adtr){
  flastevent = -1;

  fBRtr = brtr;
  fEUtr = eutr;
  fADtr = adtr;

  fBRentry = 0;
  fEUentry = 0;
  fADentry = 0;

  fnbytes = 0;

  fBRts = 0;
  fbeam = new Beam;
  fADts = 0;
  faida = new AIDA;
  fEUts = 0;
  feurica = new EURICA;

  //! newly added
  fBReventinfo=NULL;
  fBRBeam=NULL;

  fEUeventinfo = NULL;
  fEUcluster = NULL;

  //local copy for intermediate storing
  flocalBRts = 0;
  flocalbeam = new Beam;
  flocalADts = 0;
  flocalaida = new AIDAHit;
  flocalEUts = 0;
  flocaleurica = new EURICA;

  //fBRtr->SetBranchAddress("timestamp",&flocalBRts);
  //fBRtr->SetBranchAddress("beam",&flocalbeam);
  //! newly added
  fBRtr->SetBranchAddress("EventInfo",&fBReventinfo);
  fBRtr->SetBranchAddress("BigRIPSBeam",&fBRBeam);
  faidaorigin = new aida_treeinfo;
  faidaorigin->Init(fADtr);

  fEUtr->SetBranchAddress("EventInfo",&fEUeventinfo);
  fEUtr->SetBranchAddress("GeCluster",&fEUcluster);

  fmtr = new TTree("mtr","merged tree");
  fmtr->Branch("beam",&fbeam,320000);
  fmtr->Branch("brTS",&fBRts,320000);
  fmtr->Branch("brentry",&fBRentry,320000);
  fmtr->Branch("aida",&faida,320000);
  fmtr->Branch("adTS",&fADts,320000);
  fmtr->Branch("adentry",&fADentry,320000);
  fmtr->Branch("eurica",&feurica,320000);
  fmtr->Branch("euTS",&fEUts,320000);
  fmtr->Branch("euentry",&fEUentry,320000);
  fmtr->BranchRef();

  fBRentries = fBRtr->GetEntries();
  cout << fBRentries << " entries in BigRIPS tree" << endl;
  fADentries = faidaorigin->GetEntries();
  cout << fADentries << " entries in AIDA tree" << endl;
  fEUentries = fEUtr->GetEntries();
  cout << fEUentries << " entries in EURICA tree" << endl;

  //! Check first and last events for data integrity
  if(fverbose>0){
    Int_t status = fBRtr->GetEvent(0);
    if(status<0)
    cout << "first BigRIPS entry faulty!" << endl;
    //!newly added
    flocalBRts=((TArtEventInfo*) fBReventinfo->At(0))->GetTimeStamp();
    cout << "first BigRIPS timestamp: " << flocalBRts << endl;;
    status = fEUtr->GetEvent(0);
    if(status<0)
      cout << "first EURICA entry faulty!" << endl;
    //!newly modified
    flocalEUts =((TArtEventInfo*) fEUeventinfo->At(0))->GetTimeStamp() + foffsetbripseurica;
    cout << "first EURICA timestamp: " << flocalEUts << endl;
    //!newly added
    status = faidaorigin->GetEntry(0);
    flocalADts = faidaorigin->extTimestamp + foffsetbripsaida;
    if (status<0)
    cout << "first AIDA entry faulty!" <<endl;
    cout << "first AIDA timestamp " << flocalADts <<endl;


    status = fBRtr->GetEvent(fBRentries-1);
    if(status<0)
      cout << "last BigRIPS entry faulty!" << endl;
    flocalBRts=((TArtEventInfo*) fBReventinfo->At(0))->GetTimeStamp();
    cout << "last BigRIPS timestamp: " << flocalBRts << endl;;
    status = fEUtr->GetEvent(fEUentries-1);
    if(status<0)
      cout << "last EURICA entry faulty!" << endl;
    flocalEUts =((TArtEventInfo*) fEUeventinfo->At(0))->GetTimeStamp() + foffsetbripseurica;
    cout << "last EURICA timestamp: " << flocalEUts << endl;
    //!newly added
    status = faidaorigin->GetEntry(fADentries-1);
    flocalADts = faidaorigin->extTimestamp + foffsetbripsaida;
    if (status<0)
    cout << "last AIDA entry faulty!" <<endl;
    cout << "last AIDA timestamp " << flocalADts <<endl;
  }
  //! newly modified/add
  flocalBRts = 0;
  flocalADts = 0;
  flocalEUts = 0;
  flocalbeam->Clear();
  faidaorigin->ResetLeaf();

  fEUeventinfo->Clear();
  fEUcluster->Clear();

  //! newly added
  fBReventinfo->Clear();
  fBRBeam->Clear();

  flastBRts = 0;
  flastADts = 0;
  flastEUts = 0;
  fBRtsjump = false;
  fADtsjump = false;
  fEUtsjump = false;
}
bool BuildEvents::ReadBigRIPS(){
  if(fverbose>1)
    cout << __PRETTY_FUNCTION__ << endl;

  flocalbeam->Clear();
  flocalBRts = 0;
  if(fBRentry==fBRentries){
    return false;
  }
  Int_t status = fBRtr->GetEvent(fBRentry);
  if(fverbose>2)
    cout << "status " << status << endl;
  if(status == -1){
    cerr<<"Error occured, couldn't read entry "<<fBRentry<<" from tree "<<fBRtr->GetName()<<endl;
    return false;
  }
  else if(status == 0){
    cerr<<"Error occured, entry "<<fBRentry<<" in tree "<<fBRtr->GetName()<<" in file doesn't exist"<<endl;
    return false;
  }


  //! newly added
  flocalBRts = ((TArtEventInfo*) fBReventinfo->At(0))->GetTimeStamp();

  if(flocalBRts<flastBRts){
    cout << endl << "BigRIPS timestamp jump detected. this = " << flocalBRts << ", last = " << flastBRts << endl;
    fBRtsjump = true;
    return false;
  }

  //! newly added
  for(int i=0;i<fBRBeam->GetEntries();i++){
      TArtBeam *hit=(TArtBeam*) fBRBeam->At(i);
      if (hit->GetAoQ()>0&&hit->GetZet()>0){
          if(fverbose>2)
          cout << fBRentry << "\t" << i <<"\t" << hit->GetAoQ() << "\t" << hit->GetZet() << endl;

          flocalbeam->SetAQ(i,hit->GetAoQ());
          flocalbeam->SetZ(i,hit->GetZet());
      }
  }

  if(fverbose>0)
    cout << "read new bigrips with TS = " << flocalBRts << " zet = "<< flocalbeam->GetZ(0)<< endl;
  fnbytes += status;
  fBRentry++;

  detector* det = new detector;
  det->TS = flocalBRts;
  det->ID = 0;
  fdetectors.push_back(det);

  flastBRts = flocalBRts;
  return true;
}

//! newly added
bool BuildEvents::ReadAIDA(){

    if(fverbose>1)
      cout << __PRETTY_FUNCTION__ << endl;
    if(fADentry==fADentries){
      return false;
    }

    Int_t status = fADtr->GetEntry(fADentry);
    if (fverbose>2)
        cout << "status " << status << endl;
    if(status == -1){
        cerr<<"Error occured, couldn't read AIDA entry "<<fADentry<<" from tree "<<fADtr->GetName()<<endl;
        return false;
    }
    else if(status == 0){
        cerr<<"Error occured, AIDA entry "<<fADentry<<" in tree "<<fADtr->GetName()<<" in file doesn't exist"<<endl;
        return false;
    }


    if (fmode==1){ //isomer
        //! omit all fast discriminator data and high gain data
        while ((faidaorigin->infoCode != 0) || (faidaorigin->infoCode == 0 && faidaorigin->rangeType == 0)){
            fADentry++;
            if(fADentry==fADentries){
              return false;
            }
            status = fADtr->GetEntry(fADentry);
            if (fverbose > 2)
                cout << "status " << status << endl;
            if(status == -1){
                cerr<<"Error occured, couldn't read AIDA entry "<<fADentry<<" from tree "<<fADtr->GetName()<<endl;
                exit(0);
                return false;
            }
        }
    }
    else if (fmode==2){//beta
        //! omit all fast discriminator, low gain data and hits which bellow the threshold
        while ((faidaorigin->infoCode != 0) || (faidaorigin->infoCode == 0 && faidaorigin->rangeType == 1) ||
               (faidaorigin->infoCode == 0 && faidaorigin->rangeType == 0 && faidaorigin->adcData <= fdssd_thr[faidaorigin->dssdNo][faidaorigin->stripNo])){
            fADentry++;
            if(fADentry==fADentries){
              return false;
            }
            status = fADtr->GetEntry(fADentry);
            if (fverbose > 2)
                cout << "status " << status << endl;
            if(status == -1){
                cerr<<"Error occured, couldn't read AIDA entry "<<fADentry<<" from tree "<<fADtr->GetName()<<endl;
                exit(0);
                return false;
            }
        }
    }

    else if (fmode==0){//both
        //! omit all fast discriminator data and high gain data which bellow the threshold
        while ((faidaorigin->infoCode != 0)||(faidaorigin->infoCode == 0 && faidaorigin->rangeType == 0 && faidaorigin->adcData <= fdssd_thr[faidaorigin->dssdNo][faidaorigin->stripNo])){
            fADentry++;
            if(fADentry==fADentries){
              return false;
            }
            status = fADtr->GetEntry(fADentry);
            if (fverbose > 2)
                cout << "status " << status << endl;
            if(status == -1){
                cerr<<"Error occured, couldn't read AIDA entry "<<fADentry<<" from tree "<<fADtr->GetName()<<endl;\
                exit(0);
                return false;
            }
        }
    }

    fnbytes += status;
    fADentry++;
    flocalADts = faidaorigin->extTimestamp*4 + foffsetbripsaida;

    if (flocalADts<flastADts){
        cout <<" AIDA timestamp jump detected. this = " << flocalADts << ", last= " << flastADts << endl;
        fADtsjump = true;
        return false;
    }

    flocalaida->SetRange(faidaorigin->rangeType);
    flocalaida->SetZ((short)faidaorigin->dssdNo);
    flocalaida->SetXY((short)faidaorigin->stripNo);
    flocalaida->SetID((short)faidaorigin->dssdNo * faidaorigin->stripNo);
    flocalaida->SetEnergy(faidaorigin->adcData * fdssd_cal[faidaorigin->dssdNo][faidaorigin->stripNo][1] + fdssd_cal[faidaorigin->dssdNo][faidaorigin->stripNo][0]);
    flocalaida->SetADC(faidaorigin->adcData);
    flocalaida->SetTimestamp(faidaorigin->extTimestamp + foffsetbripsaida);

    detector* det = new detector;
    det->TS = flocalADts;
    det->ID = 2;
    fdetectors.push_back(det);
    flastADts = flocalADts;
    return true;
}

bool BuildEvents::ReadEURICA(){
  if(fverbose>1)
    cout << __PRETTY_FUNCTION__ << endl;
  if(fEUentry==fEUentries){
    return false;
  }
  Int_t status = fEUtr->GetEvent(fEUentry);
  if(fverbose>2)
    cout << "status " << status << endl;
  if(status == -1){
    cerr<<"Error occured, couldn't read entry "<<fEUentry<<" from tree "<<fEUtr->GetName()<<endl;
    return false;
  }
  else if(status == 0){
    cerr<<"Error occured, entry "<<fEUentry<<" in tree "<<fEUtr->GetName()<<" in file doesn't exist"<<endl;
    return false;
  }
  fnbytes += status;

  //! newly modified
  flocalEUts = ((TArtEventInfo*) fEUeventinfo->At(0))->GetTimeStamp() + foffsetbripseurica;

  if(flocalEUts<flastEUts){
    cout <<"EURICA timestamp jump detected. this = " << flocalEUts << ", last = " << flastEUts << endl;
    fEUtsjump = true;
    return false;
  }


  for(int i=0;i<fEUcluster->GetEntries();i++){

    TArtGeCluster *hit =(TArtGeCluster*) fEUcluster->At(i);
    if(hit->GetEnergy()>1){
      EURICAHit *euhit = new EURICAHit(hit->GetChannel(),hit->GetEnergy(),hit->GetTiming(),1,flocalEUts);
      flocaleurica->AddHit(euhit);
    }
  }
  fEUentry++;

  detector* det = new detector;
  det->TS = flocalEUts;
  det->ID = 1;
  fdetectors.push_back(det);

  flastEUts = flocalEUts;
  return true;
}
bool BuildEvents::ReadEach(){
  flastBRts = 0;
  flastEUts = 0;
  flastADts = 0;
  bool success = ReadEURICA();
  success += ReadBigRIPS();
  success +=ReadAIDA();
  if(success)
    return true;
  else{
    cout << "failed to read from both EURICA, BigRIPS and AIDA" << endl;
    return false;
  }
}
void BuildEvents::CloseEvent(){
  if(fverbose>0)
    cout << __PRETTY_FUNCTION__ << endl;
  // // bool printme = false;
  // if(fBRts>0 && fEUts >0){
  //   //printme = true;
  //   cout << "fBRentry = " << fBRentry << "\tfBRts = " << fBRts << ", fEUentry = " << fEUentry << "\tfEUts = " << fEUts << "\tmult = " << feurica->GetMult()<< endl;
  //   cout << "BR AoQ = " << fbeam->GetAQ(1) << " Z = " << fbeam->GetZ(1) << endl;
  //   feurica->Print();
  // }
  if(fverbose>0 && fBRts>0){
    cout << "closing event with local TS = " << flocalBRts << " zet = "<< flocalbeam->GetZ(0)<< endl;
    cout << "closing event with set TS = " << fBRts << " zet = "<< fbeam->GetZ(0)<< endl;
  }
  switch(fmode){
  default:
  case 0: //write all events
    fmtr->Fill();
    break;
  case 1://isomer data BR and EU coincidence
    if(fBRts>0 && fEUts >0)
      fmtr->Fill();
    break;
  case 2://write only beta
    if (fADts>0&&fBRts==0&&faida->GetMult()<64)
        fmtr->Fill();
      break;
  }

  fBRts = 0;
  fbeam->Clear();
  fADts = 0;
  faida->Clear();
  fEUts = 0;
  feurica->Clear();
  // if(printme){
  //   cout << "after clearing " << endl;
  //   feurica->Print();
  // }
  if(fverbose>0)
    cout << "end "<< __PRETTY_FUNCTION__ << endl;
}
bool BuildEvents::Merge(){
  if(fverbose>1)
    cout << __PRETTY_FUNCTION__ << endl;

  if(fverbose>1){
    cout << "before sorting" << endl;
    for(vector<detector*>::iterator det=fdetectors.begin(); det!=fdetectors.end(); det++){
      cout << "ID = " << (*det)->ID << ", TS = " << (*det)->TS << endl;
    }
  }
  sort(fdetectors.begin(), fdetectors.end(),TSComparer());
  if(fverbose>1){
    cout << "after sorting" << endl;
    for(vector<detector*>::iterator det=fdetectors.begin(); det!=fdetectors.end(); det++){
      cout << "ID = " << (*det)->ID << ", TS = " << (*det)->TS << endl;
    }
  }

  switch(fdetectors.at(0)->ID){
  case 0: //BigRIPS
    if(fBRts>0&&fmode<2){
      if(fverbose>1)
        cout << "has already BigRIPS" << endl;
      CloseEvent();
      fcurrentts = flocalBRts;
    }
    else if(flocalBRts - fcurrentts > fwindow){
      if(fverbose>0)
    cout << "BR larger than window" << endl;
      CloseEvent();
      fcurrentts = flocalBRts;
    }
    fBRts = flocalBRts;
    fbeam = (Beam*)flocalbeam->Clone();

    //fcurrentts = fBRts;
    fdetectors.erase(fdetectors.begin());
    if(!ReadBigRIPS()&&fBRtsjump==false)
      cout << "failed to read BigRIPS, end of file" << endl;
    break;
  case 1: //EURICA
    if(flocalEUts - fcurrentts > fwindow){
      if(fverbose>0)
    cout << "EU larger than window" << endl;
      CloseEvent();
      fcurrentts = flocalEUts;
    }
    fEUts = flocalEUts;
    feurica->AddHits(flocaleurica->GetHits());
    flocaleurica->Clear();
    //fcurrentts = fEUts;
    fdetectors.erase(fdetectors.begin());
    if(!ReadEURICA()&&fEUtsjump==false)
      cout << "failed to read EURICA, end of file" << endl;
    break;
  case 2: //AIDA
    if(flocalADts - fcurrentts > fwindow){
      if(fverbose>0)
    cout << "AIDA larger than window" << endl;
      //! is it ok?
      //! do hit position determination here?
      fADts=ffirstADts;
      CloseEvent();
      fcurrentts = flocalADts;
      //fADts = flocalADts;
    }
    if (faida->GetMult()==0) ffirstADts=flocalADts;
    fADts = flocalADts;
    faida->AddHit((AIDAHit*)flocalaida->Clone());
    flocalaida->Clear();
    //fcurrentts = fADts;
    fdetectors.erase(fdetectors.begin());
    if(!ReadAIDA()&&fADtsjump==false)
      cout << "failed to read AIDA, end of file" << endl;
    break;

  default:
    break;
  }
  if(flastevent>0 && flastevent == (int)(fBRentry + fEUentry + fADentry)){
    cout << "last event reached " << endl;
    return false;
  }

  //! time jump handler!
  if(fdetectors.size()==0){
    if(fEUtsjump==true && fBRtsjump==true && fADtsjump==true){
      cout << "all timestamps jumped" << endl;
      fEUtsjump = false;
      fBRtsjump = false;
      fADtsjump = false;
      return ReadEach();
    }
    cout << "all files finished " << endl;
    return false;
  }
  return true;
}

void BuildEvents::read_aida_threshold_table(char* inf)
{
    //clean up
    for (Int_t i=0;i<NumDSSD;i++){
        for (Int_t j=0;j<NumStrXY;j++){
            fdssd_thr[i][j]=0.;
        }
    }
    cout<<inf<<endl;
    ifstream inpf(inf);
    if (inpf.fail()){
        cout<<"No Theshold table is given"<<endl;
        return;
    }
    cout<<"Start reading threshold table"<<endl;
    Int_t dssd_index,strip_index;
    Double_t threshold;
    Int_t mm=0;
    while (inpf.good()){
    //for (Int_t i=0;i<100;i++){
        inpf>>dssd_index>>strip_index>>threshold;
        fdssd_thr[dssd_index][strip_index]=threshold;
        //cout<<fdssd_thr[dssd_index][strip_index]<<endl;
        mm++;
    }
    cout<<"Read "<<mm<<" line"<<endl;
    inpf.close();
    faida->SetThreshold(fdssd_thr);
}

void BuildEvents::read_aida_cal_table(char* inf){
    //clean up
    for (Int_t i=0;i<NumDSSD;i++){
        for (Int_t j=0;j<NumStrXY;j++){
            fdssd_cal[i][j][0]=0.;
            fdssd_cal[i][j][1]=1.;
        }
    }
    cout<<inf<<endl;
    ifstream inpf(inf);
    if (inpf.fail()){
        cout<<"No Calibration table is given"<<endl;
        return;
    }
    cout<<"Start reading calibration table"<<endl;
    Int_t dssd_index,strip_index;
    Double_t cal1,cal2;
    Int_t mm=0;
    while (inpf.good()){
    //for (Int_t i=0;i<100;i++){
        inpf>>dssd_index>>strip_index>>cal1>>cal2;
        fdssd_cal[dssd_index][strip_index][0]=cal1;
        fdssd_cal[dssd_index][strip_index][1]=cal2;
        //cout<<fdssd_thr[dssd_index][strip_index]<<endl;
        mm++;
    }
    cout<<"Read "<<mm<<" line"<<endl;
    inpf.close();
    faida->SetCalib(fdssd_cal);
}


