#ifndef __BUILDEVENTS_H
#define __BUILDEVENTS_H
#include <vector>
#include "TTree.h"
#include "TClonesArray.h"
#include "TArtEventInfo.hh"
#include "TArtGeCluster.hh"
#include "TArtBeam.hh"
#include "aida_treeinfo.h"
#include "Beam.h"
#include "EURICA.h"
#include "AIDA.h"


/*!
  A container to keep track of the timestamps and corresponding detectors
*/
struct detector{
  //! timestamp of the detector hit
  unsigned long long int TS;
  //! ID for the detector, 0 BigRIPS, 1 EURICA, 2 AIDA
  int ID;
};

/*!
  A class for building BigRIPS and EURICA combined events
*/
class BuildEvents {
public:
  //! Default constructor
  //! newly modified
  BuildEvents(){
      foffsetbripsaida=0;foffsetbripseurica=0;
      for (Int_t i=0;i<NumDSSD;i++){
          for (Int_t j=0;j<NumStrXY;j++){
              fdssd_thr[i][j] = 0.;
              fdssd_cal[i][j][0] = 0.;
              fdssd_cal[i][j][1] = 1.;
          }
      }
  }
  //! Initialize trees
  void Init(TTree* brtr, TTree* eutr, TTree *adtr);
  //! Set the window for event building
  void SetWindow(unsigned long long int window){fwindow = window;}

  //! Set the offset bw Brips and Eurica
  void SetOffsetBripsEurica(unsigned long long int offset){foffsetbripseurica = offset;}
  //! Set the offset bw Brips and AIDA
  void SetOffsetBripsAIDA(unsigned long long int offset){foffsetbripsaida = offset;}

  //! Set coincidence mode
  void SetCoincMode(int mode){fmode = mode;}
  //! Set the verbose level
  void SetVerbose(int verbose){fverbose = verbose;}
  //! Set the last event
  void SetLastEvent(int lastevent){flastevent = lastevent;}
  //! Read one entry from the EURICA tree
  bool ReadEURICA();
  //! Read one entry from the AIDA tree
  bool ReadAIDA();
  //! Read one entry from the BigRIPS tree
  bool ReadBigRIPS();
  //! Read one entry from each tree
  bool ReadEach();

  //! Read threshold and calibration table
  void read_aida_threshold_table(char* inf);
  void read_aida_cal_table(char* inf);


  //! Merge the data streams
  bool Merge();

  //! Set the last event
  int GetNEvents(){return fEUentries + fBRentries + fADentries;}
  int GetCurrentNEvents(){return fADentry + fBRentry + fEUentry;}
  //! Close the event and write to tree
  void CloseEvent();
  //! Get the merged tree
  TTree* GetTree(){return fmtr;}

private:
  //! BigRIPS input tree
  TTree* fBRtr;
  //! AIDA input tree
  TTree* fADtr;
  //! EURICA input tree
  TTree* fEUtr;
  //! merged output tree
  TTree* fmtr;
  //! verbose level
  int fverbose;

  //! list of detector hits
  vector<detector*> fdetectors;

  //! number of events to be read
  int flastevent;

  //! number of bytes read
  int fnbytes;
  //! current timestamp
  unsigned long long int fcurrentts;
  //! last read BigRIPS timestamp
  unsigned long long int flastBRts;
  //! last read EURICA timestamp
  unsigned long long int flastADts;
  //! last read EURICA timestamp
  unsigned long long int flastEUts;
  //! timestamp jumped in BigRIPS
  bool fBRtsjump;
  //! timestamp jumped in AIDA
  bool fADtsjump;
  //! timestamp jumped in EURICA
  bool fEUtsjump;

  //! BigRIPS timestamp
  unsigned long long int fBRts;
  //! bigrips data
  Beam* fbeam;
  //! AIDA timestamp
  unsigned long long int fADts;
  //! AIDA data
  AIDA* faida;
  //! EURICA timestamp
  unsigned long long int fEUts;
  //! eurica data
  EURICA* feurica;

  //! local copy of BigRIPS timestamp
  unsigned long long int flocalBRts;
  //! local copy of bigrips data
  Beam* flocalbeam;
  //! local copy of AIDA timestamp
  unsigned long long int flocalADts;

  unsigned long long int ffirstADts;

  //! local copy of AIDA data
  AIDAHit* flocalaida;
  //! EURICA timestamp
  unsigned long long int flocalEUts;
  //! eurica data
  EURICA* flocaleurica;

  //!newly modified
  //! number of bigrips entries
  long long int fBRentries;
  //! number of aida entries
  long long int fADentries;
  //! number of eurica entries
  long long int fEUentries;


  //! current bigrips entry
  unsigned int fBRentry;
  //! current aida entry
  unsigned int fADentry;
  //! current eurica entry
  unsigned int fEUentry;

  //! eurica event info
  TClonesArray *fEUeventinfo;
  //! eurica data
  TClonesArray *fEUcluster;

  //! newly added
  //! bigrips event info
  TClonesArray *fBReventinfo;
  //! bigrips beam data
  TClonesArray *fBRBeam;
  //! AIDA raw data containing hits only!
  aida_treeinfo* faidaorigin;

  //calibration table
  Double_t fdssd_thr[NumDSSD][NumStrXY];
  Double_t fdssd_cal[NumDSSD][NumStrXY][2];

  //! time window for eventbuilding
  unsigned long long fwindow;

  //! time offset bw brips and aida
  unsigned long long foffsetbripsaida;

  //! time offset bw brips and eurica
  unsigned long long foffsetbripseurica;

  //! modus for writing the merged data: 0 all, 1 only isomer (BR, AD and EU), 2  isomer and beta (BR, AD and EU)
  int fmode;

};

/*!
  A class for sorting the detectors in time
*/
class TSComparer {
public:
  //! comparator
  bool operator() ( detector *lhs, detector *rhs) {
    return (*rhs).TS > (*lhs).TS;
  }
};
#endif
