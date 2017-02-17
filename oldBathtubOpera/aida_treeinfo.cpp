//////////////////////////////////////////////////////////
// Coded by Vi Ho Phong - phong@ribf.riken.jp
// Last modified: Feb 11,2016
//////////////////////////////////////////////////////////

#include "aida_treeinfo.h"
aida_treeinfo::aida_treeinfo() : fChain(0)
{

}

aida_treeinfo::~aida_treeinfo()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void aida_treeinfo::GetTree(char *filename, TTree *tree){
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
       if (tree == 0) {
          TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
          if (!f || !f->IsOpen()) {
             f = new TFile(filename);
          }
          f->GetObject("tree",tree);

       }
       Init(tree);
}
void aida_treeinfo::TreeBranch(TTree *tree){
    tree->Branch("evt",&evt,"evt/L");
    tree->Branch("timestamp",&timestamp,"timestamp/L");
    tree->Branch("extTimestamp",&extTimestamp,"extTimestamp/L");

    tree->Branch("feeNo",&feeNo,"feeNo/I");
    tree->Branch("chNo",&chNo,"chNo/I");
    tree->Branch("dssdNo",&dssdNo,"dssdNo/I");
    tree->Branch("stripNo",&stripNo,"stripNo/I");

    tree->Branch("infoCode",&infoCode,"infoCode/S");
    tree->Branch("rangeType",&rangeType,"rangeType/S");
    tree->Branch("adcData",&adcData,"adcData/I");
}

Int_t aida_treeinfo::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t aida_treeinfo::GetEntries()
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntries();
}

Long64_t aida_treeinfo::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void aida_treeinfo::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("extTimestamp", &extTimestamp, &b_extTimestampInfo);
   fChain->SetBranchAddress("feeNo", &feeNo, &b_feeNo);
   fChain->SetBranchAddress("chNo", &chNo, &b_chNo);
   fChain->SetBranchAddress("dssdNo", &dssdNo, &b_dssdNo);
   fChain->SetBranchAddress("stripNo", &stripNo, &b_stripNo);
   fChain->SetBranchAddress("infoCode", &infoCode, &b_infoCode);
   fChain->SetBranchAddress("rangeType", &rangeType, &b_rangeType);
   fChain->SetBranchAddress("adcData", &adcData, &b_adcData);
   Notify();
}

Bool_t aida_treeinfo::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void aida_treeinfo::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t aida_treeinfo::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void aida_treeinfo::ResetLeaf()
{
    adcData=-9999;
    timestamp=-9999;
    extTimestamp=-9999;
    infoCode=-9999;
    feeNo=-9999;
    chNo=-9999;
    stripNo=-9999;
    dssdNo=-9999;
    rangeType=-9999;
}
/*
void aida_treeinfo::read_threshold_table()
{
    //clean up
    for (Int_t i=0;i<NumDSSD;i++){
        for (Int_t j=0;j<NumStrXY;j++){
            thr[i][j]=0;
        }
    }
    ifstream inpf("threshold_table.txt",ios::in);
    if (inpf.fail()){
        cout<<"No Theshold table is given"<<endl;
        return;
    }
    cout<<"Start reading threshold table"<<endl;
    Int_t dssd_index,strip_index,threshold;
    while (inpf.good()){
        inpf>>dssd_index>>strip_index>>threshold;
        thr[dssd_index][strip_index]=threshold;
    }
    inpf.close();
}
*/
