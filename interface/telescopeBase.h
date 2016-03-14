//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  2 15:59:54 2016 by ROOT version 6.06/00
// from TTree tracks/tracks
// found on file: NtupleTelescope_918.root
//////////////////////////////////////////////////////////

#ifndef telescopeBase_h
#define telescopeBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <iostream>
#include "vector"
using std::vector;
using std::cout;
using std::cerr;
class telescopeBase {
public :
   TTree          *fChain_tel;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent_tel; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           nTrackParams;
   Int_t           euEvt;
   vector<double>  *xPos;
   vector<double>  *yPos;
   vector<double>  *dxdz;
   vector<double>  *dydz;
   vector<int>     *trackNum;
   vector<int>     *iden;
   vector<double>  *chi2;
   vector<double>  *ndof;

   // List of branches
   TBranch        *b_nTrackParams;   //!
   TBranch        *b_euEvt;   //!
   TBranch        *b_xPos;   //!
   TBranch        *b_yPos;   //!
   TBranch        *b_dxdz;   //!
   TBranch        *b_dydz;   //!
   TBranch        *b_trackNum;   //!
   TBranch        *b_iden;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_ndof;   //!

   telescopeBase(const TString& inFile);
   virtual ~telescopeBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   // virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   bool     hasTrk(Long64_t dutEvt);
   bool     isValidChain();
};

#endif

#ifdef telescopeBase_cxx
telescopeBase::telescopeBase(const TString& inFile)  
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  TFile* f = TFile::Open(inFile);
  if (!f || !f->IsOpen()) {
    cerr << " Telescope File not found" << std::endl;
    exit(1);
  }
  fChain_tel = dynamic_cast<TTree*>(f->Get("tracks"));
  if(!fChain_tel) {
    cerr << "Tree does not exist in  Telescope file" << std::endl;
    exit(1);
  }
  
  Init(fChain_tel);
}

telescopeBase::~telescopeBase()
{
   if (!fChain_tel) return;
   delete fChain_tel->GetCurrentFile();
}

Int_t telescopeBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain_tel) return 0;
   return fChain_tel->GetEntry(entry);
}
Long64_t telescopeBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain_tel) return -5;
   Long64_t centry = fChain_tel->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain_tel->GetTreeNumber() != fCurrent_tel) {
      fCurrent_tel = fChain_tel->GetTreeNumber();
      Notify();
   }
   return centry;
}

void telescopeBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   xPos = 0;
   yPos = 0;
   dxdz = 0;
   dydz = 0;
   trackNum = 0;
   iden = 0;
   chi2 = 0;
   ndof = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain_tel = tree;
   fCurrent_tel = -1;
   fChain_tel->SetMakeClass(1);

   fChain_tel->SetBranchAddress("nTrackParams", &nTrackParams, &b_nTrackParams);
   fChain_tel->SetBranchAddress("euEvt", &euEvt, &b_euEvt);
   fChain_tel->SetBranchAddress("xPos", &xPos, &b_xPos);
   fChain_tel->SetBranchAddress("yPos", &yPos, &b_yPos);
   fChain_tel->SetBranchAddress("dxdz", &dxdz, &b_dxdz);
   fChain_tel->SetBranchAddress("dydz", &dydz, &b_dydz);
   fChain_tel->SetBranchAddress("trackNum", &trackNum, &b_trackNum);
   fChain_tel->SetBranchAddress("iden", &iden, &b_iden);
   fChain_tel->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain_tel->SetBranchAddress("ndof", &ndof, &b_ndof);
   Notify();
}

Bool_t telescopeBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void telescopeBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain_tel) return;
   fChain_tel->Show(entry);
}
Int_t telescopeBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef telescopeBase_cxx
