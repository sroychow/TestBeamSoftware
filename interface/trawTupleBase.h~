//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Nov  1 12:18:35 2015 by ROOT version 5.34/23
// from TTree sensorHitTree/sensorHitTree from RAW files
// found on file: Rawtuple_run444_dqm.root
//////////////////////////////////////////////////////////

#ifndef trawTupleBase_h
#define trawTupleBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::cerr;

// Fixed size dimensions of array or collections stored in the TTree if any.

class trawTupleBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<int>     *dutEven_channel;
   vector<int>     *dutOdd_channel;

   // List of branches
   TBranch        *b_dutEven_channel;   //!
   TBranch        *b_dutOdd_channel;   //!

   //trawTupleBase(TTree *tree=0);
   trawTupleBase(const TString& inFile);
   virtual ~trawTupleBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef trawTupleBase_cxx
trawTupleBase::trawTupleBase(const TString& inFile) {
  TFile* f = TFile::Open(inFile);
  if (!f || !f->IsOpen()) {
    cerr << "File not found" << std::endl;
    exit(1);
  }
  fChain = dynamic_cast<TTree*>(f->Get("sensorHitTree"));
  if(!fChain) {
    cerr << "Tree does not exist in file" << std::endl;
    exit(1);
  }
  
  Init(fChain);
}
/*
trawTupleBase::trawTupleBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Rawtuple_run444_dqm.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Rawtuple_run444_dqm.root");
      }
      f->GetObject("sensorHitTree",tree);

   }
   Init(tree);
}
*/
trawTupleBase::~trawTupleBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t trawTupleBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t trawTupleBase::LoadTree(Long64_t entry)
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

void trawTupleBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   dutEven_channel = 0;
   dutOdd_channel = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("dutEven_channel", &dutEven_channel, &b_dutEven_channel);
   fChain->SetBranchAddress("dutOdd_channel", &dutOdd_channel, &b_dutOdd_channel);
   Notify();
}

Bool_t trawTupleBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void trawTupleBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t trawTupleBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef trawTupleBase_cxx
