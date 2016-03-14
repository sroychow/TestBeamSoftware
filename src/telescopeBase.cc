#define telescopeBase_cxx
#include "telescopeBase.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

bool telescopeBase::hasTrk(Long64_t dutEvt)
{
   //if (fChain_tel == 0) return;

   Long64_t nentries = fChain_tel->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   bool hasTk = false; 
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain_tel->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(euEvt != dutEvt) continue;
      if(nTrackParams == 1) hasTk = true;
      break;
   }
   return hasTk;
}
bool telescopeBase::isValidChain(){

   if (fChain_tel == 0) return false;
   return true;
}      
