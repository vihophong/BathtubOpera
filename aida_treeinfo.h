//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 11 16:48:26 2016 by ROOT version 5.34/34
// Coded by Vi Ho Phong - phong@ribf.riken.jp
// Last modified: May 11,2016
//////////////////////////////////////////////////////////

#ifndef aida_treeinfo_h
#define aida_treeinfo_h

#include "Riostream.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObject.h>
#include <TMath.h>
#include <TString.h>

/*
#define NumFee 33
#define n_side 2
#define n_asic 16
#define n_dssd 7
#define n_strip_X 128
#define n_strip_Y 128
#define n_strip 256
*/

#define NumDSSD 6
#define NumStrX 128
#define NumStrY 128
#define NumStrXY 256
#define NumFee 33

#define NumChFee 64


#define tm_stp_scaler_ratio 4 //AIDA clock 100 MHz,Correlation scaler 25 MHz

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class aida_treeinfo {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Long64_t        evt;
   Long64_t        timestamp;
   Long64_t        extTimestamp;
   Int_t           feeNo;
   Int_t           chNo;
   Int_t           dssdNo;
   Int_t           stripNo;
   Short_t         infoCode;
   Short_t         rangeType;
   Int_t           adcData;

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_extTimestampInfo;   //!
   TBranch        *b_feeNo;   //!
   TBranch        *b_chNo;   //!
   TBranch        *b_dssdNo;   //!
   TBranch        *b_stripNo;   //!
   TBranch        *b_infoCode;   //!
   TBranch        *b_rangeType;   //!
   TBranch        *b_adcData;   //!

   aida_treeinfo();
   virtual ~aida_treeinfo();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual Long64_t GetEntries();

   virtual void     GetTree(char* filename,TTree *tree=0);
   virtual void     TreeBranch(TTree *tree);

   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void     ResetLeaf();

   //Int_t thr[NumDSSD][NumStrXY];
   //virtual void     read_threshold_table();
};

#endif


