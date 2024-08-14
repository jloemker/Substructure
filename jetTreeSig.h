//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar  2 14:47:15 2021 by ROOT version 6.12/06
// from TTree jetTreeSig/JetToyHI tree
// found on file: JetToyHIResultTimeClusBkg_Merged.root
//////////////////////////////////////////////////////////

#ifndef jetTreeSig_h
#define jetTreeSig_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class jetTreeSig {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   // Parton info only in pythia.
   vector<double>  *eventWeight;
   vector<double>  *partonsPt;
   vector<double>  *partonsEta;
   vector<double>  *partonsPhi;
   vector<double>  *partonsM;
   vector<int>     *partonsPDG;
   vector<double>  *partonsFirstSplitPt;
   vector<double>  *partonsFirstSplitEta;
   vector<double>  *partonsFirstSplitPhi;
   vector<double>  *partonsFirstSplitM;
   vector<int>     *partonsFirstSplitPDG;

   vector<double>  *drSplit;
   vector<double>  *tfSplit;
   vector<double>  *sigJetPt;
   vector<double>  *sigJetEta;
   vector<double>  *sigJetPhi;
   vector<double>  *sigJetM;
   vector<double>  *sigJetArea;

   vector<vector<double>> *jetCollectionSigDYEPt;
   vector<double>  *jetCollectionSigDYEEta;

   vector<double>  *sigJetRecurZcut_nSD;
   vector<double>  *sigJetRecurZcut_zSD;
   vector<double>  *sigJetRecur_nSD;
   vector<double>  *sigJetRecur_partonMatchDr;
   vector<double>  *sigJetRecur_zSD;
   //add vectors for dynGrooming
   vector<vector<double> > *sigJetDYE_z;         //zg of groomed jets
   vector<vector<double> > *sigJetDYE_dr12;       //distance between the two subjet
   vector<vector<double> > *sigJetDYE_kt;         //kt of groomed jets
   vector<vector<double> > *sigJetDYE_tau21;      //n-subjettiness ratio 21
   vector<vector<double> > *sigJetDYE_tau32;      //n-subjettiness ratio 32
   vector<vector<double> > *sigJetDYE_tf;     	// formation time tf = 2 / (z*(1-z)*pt*theta^2) (soft limit)
   vector<vector<double> > *sigJetDYE_tfe;     	// formation time tf = 1 / 2*(z*(1-z)*E*(1-cos(theta))) (hard limit)
   vector<vector<double> > *sigJetkappaDYE;      //kappa of groomed jets
   vector<vector<double> > *sigJetDYE_drBranches;      //number of dropped branches
   vector<vector<double> > *sigJetTauDYE_z;
   vector<vector<double> > *sigJetTauDYE_dr12;
   vector<vector<double> > *sigJetTauDYE_kt;
   vector<vector<double> > *sigJetTauDYE_tau21;
   vector<vector<double> > *sigJetTauDYE_tau32;
   vector<vector<double> > *sigJetTauDYE_tf;
   vector<vector<double> > *sigJetTauDYE_tfe;
   vector<vector<double> > *sigJetTaukappaDYE;
   vector<vector<double> > *sigJetTauDYE_drBranches;

   vector<vector<double> > *sigJetRecurZcut_dr12;
   vector<vector<double> > *sigJetRecurZcut_erad;
   vector<vector<double> > *sigJetRecurZcut_jetpt;
   vector<vector<double> > *sigJetRecurZcut_logdr12;
   vector<vector<double> > *sigJetRecurZcut_logztheta;
   vector<vector<double> > *sigJetRecurZcut_tf;
   vector<vector<double> > *sigJetRecurZcut_tfe;
   vector<vector<double> > *sigJetRecurZcut_z;
   vector<vector<double> > *sigJetRecurZcut_tau21;
   vector<vector<double> > *sigJetRecurZcut_tau32;
   vector<vector<double> > *sigJetRecur_dr12;
   vector<vector<double> > *sigJetRecur_erad;
   vector<vector<double> > *sigJetRecur_jetpt;
   vector<vector<double> > *sigJetRecur_logdr12;
   vector<vector<double> > *sigJetRecur_logztheta;
   vector<vector<double> > *sigJetRecur_tf;
   vector<vector<double> > *sigJetRecur_tfe;
   vector<vector<double> > *sigJetRecur_z;
   vector<int>     *sigJetRecur_partonMatchID;

   vector<double>  *sigJetRecurTauZcut_nSD;
   vector<double>  *sigJetRecurTauZcut_zSD;
   vector<double>  *sigJetRecurTau_nSD;
   vector<double>  *sigJetRecurTau_partonMatchDr;
   vector<double>  *sigJetRecurTau_zSD;
   vector<vector<double> > *sigJetRecurTauZcut_dr12;
   vector<vector<double> > *sigJetRecurTauZcut_erad;
   vector<vector<double> > *sigJetRecurTauZcut_jetpt;
   vector<vector<double> > *sigJetRecurTauZcut_logdr12;
   vector<vector<double> > *sigJetRecurTauZcut_logztheta;
   vector<vector<double> > *sigJetRecurTauZcut_tf;
   vector<vector<double> > *sigJetRecurTauZcut_tfe;
   vector<vector<double> > *sigJetRecurTauZcut_z;
   vector<vector<double> > *sigJetRecurTauZcut_tau21;
   vector<vector<double> > *sigJetRecurTauZcut_tau32;
   vector<vector<double> > *sigJetRecurTau_dr12;
   vector<vector<double> > *sigJetRecurTau_erad;
   vector<vector<double> > *sigJetRecurTau_jetpt;
   vector<vector<double> > *sigJetRecurTau_logdr12;
   vector<vector<double> > *sigJetRecurTau_logztheta;
   vector<vector<double> > *sigJetRecurTau_tf;
   vector<vector<double> > *sigJetRecurTau_tfe;
   vector<vector<double> > *sigJetRecurTau_z;
   vector<int>     *sigJetRecurTau_partonMatchID;

   // List of branches
   TBranch        *b_eventWeight;   //!
   TBranch        *b_partonsPt;   //!
   TBranch        *b_partonsEta;   //!
   TBranch        *b_partonsPhi;   //!
   TBranch        *b_partonsM;   //!
   TBranch        *b_partonsPDG;   //!
   TBranch        *b_partonsFirstSplitPt;   //!
   TBranch        *b_partonsFirstSplitEta;   //!
   TBranch        *b_partonsFirstSplitPhi;   //!
   TBranch        *b_partonsFirstSplitM;   //!
   TBranch        *b_partonsFirstSplitPDG;   //!
   TBranch        *b_drSplit;   //!
   TBranch        *b_tfSplit;   //!
   TBranch        *b_sigJetPt;   //!
   TBranch        *b_sigJetEta;   //!
   TBranch        *b_sigJetPhi;   //!
   TBranch        *b_sigJetM;   //!
   TBranch        *b_sigJetArea;   //!

   TBranch        *b_sigJetRecurZcut_nSD;   //!
   TBranch        *b_sigJetRecurZcut_zSD;   //!
   TBranch        *b_sigJetRecur_nSD;   //!
   TBranch        *b_sigJetRecur_partonMatchDr;   //!
   TBranch        *b_sigJetRecur_zSD;   //!
   TBranch        *b_sigJetRecurZcut_dr12;   //!
   TBranch        *b_sigJetRecurZcut_erad;   //!
   TBranch        *b_sigJetRecurZcut_jetpt;   //!
   TBranch        *b_sigJetRecurZcut_logdr12;   //!
   TBranch        *b_sigJetRecurZcut_logztheta;   //!
   TBranch        *b_sigJetRecurZcut_tf;   //!
   TBranch        *b_sigJetRecurZcut_tfe;   //!
   TBranch        *b_sigJetRecurZcut_z;   //!
   TBranch        *b_sigJetRecurZcut_tau21;
   TBranch        *b_sigJetRecurZcut_tau32;
   TBranch        *b_sigJetRecur_dr12;   //!
   TBranch        *b_sigJetRecur_erad;   //!
   TBranch        *b_sigJetRecur_jetpt;   //!
   TBranch        *b_sigJetRecur_logdr12;   //!
   TBranch        *b_sigJetRecur_logztheta;   //!
   TBranch        *b_sigJetRecur_tf;   //!
   TBranch        *b_sigJetRecur_tfe;   //!
   TBranch        *b_sigJetRecur_z;   //!
   TBranch        *b_sigJetRecur_partonMatchID;   //!

   TBranch        *b_sigJetRecurTauZcut_nSD;   //!
   TBranch        *b_sigJetRecurTauZcut_zSD;   //!
   TBranch        *b_sigJetRecurTau_nSD;   //!
   TBranch        *b_sigJetRecurTau_partonMatchDr;   //!
   TBranch        *b_sigJetRecurTau_zSD;   //!
   TBranch        *b_sigJetRecurTauZcut_dr12;   //!
   TBranch        *b_sigJetRecurTauZcut_erad;   //!
   TBranch        *b_sigJetRecurTauZcut_jetpt;   //!
   TBranch        *b_sigJetRecurTauZcut_logdr12;   //!
   TBranch        *b_sigJetRecurTauZcut_logztheta;   //!
   TBranch        *b_sigJetRecurTauZcut_tf;   //!
   TBranch        *b_sigJetRecurTauZcut_tfe;   //!
   TBranch        *b_sigJetRecurTauZcut_z;   //!
   TBranch        *b_sigJetRecurTauZcut_tau21;
   TBranch        *b_sigJetRecurTauZcut_tau32;
   TBranch        *b_sigJetRecurTau_dr12;   //!
   TBranch        *b_sigJetRecurTau_erad;   //!
   TBranch        *b_sigJetRecurTau_jetpt;   //!
   TBranch        *b_sigJetRecurTau_logdr12;   //!
   TBranch        *b_sigJetRecurTau_logztheta;   //!
   TBranch        *b_sigJetRecurTau_tf;   //!
   TBranch        *b_sigJetRecurTau_tfe;   //!
   TBranch        *b_sigJetRecurTau_z;   //!
   TBranch        *b_sigJetRecurTau_partonMatchID;   //!

   TBranch         *b_sigJetDYE_z; 
   TBranch         *b_sigJetDYE_dr12; 
   TBranch         *b_sigJetDYE_kt; 
   TBranch         *b_sigJetDYE_tau21; 
   TBranch         *b_sigJetDYE_tau32; 
   TBranch         *b_sigJetDYE_tf; 
   TBranch         *b_sigJetDYE_tfe; 
   TBranch         *b_sigJetkappaDYE; 
   TBranch         *b_sigJetDYE_drBranches;
   TBranch         *b_sigJetTauDYE_z; 
   TBranch         *b_sigJetTauDYE_dr12; 
   TBranch         *b_sigJetTauDYE_kt; 
   TBranch         *b_sigJetTauDYE_tau21; 
   TBranch         *b_sigJetTauDYE_tau32; 
   TBranch         *b_sigJetTauDYE_tf; 
   TBranch         *b_sigJetTauDYE_tfe; 
   TBranch         *b_sigJetTaukappaDYE; 
   TBranch         *b_sigJetTauDYE_drBranches;

   jetTreeSig(TTree *tree=0);
   virtual ~jetTreeSig();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
  // virtual TTree*   CopyTree(const char* selection);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Bool_t reduced, TString generator, TString duration);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef jetTreeSig_cxx
jetTreeSig::jetTreeSig(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) { //new TFile("Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_"+generator+"_"+duration+".root","RECREATE");
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("JetToyHIResultTimeClusJewel_simple.root");// JetToyHIResultTimeClusJewel_vac.root
      //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("JetToyHIResultTimeClusJewel_simple.root");// JetToyHIResultTimeClusJewel_vac.root
      if (!f || !f->IsOpen()) {
         f = new TFile("JetToyHIResultTimeClusJewel_simple.root");// JetToyHIResultTimeClusJewel_vac.root
         //f = new TFile("JetToyHIResultTimeClusJewel_simple.root");// JetToyHIResultTimeClusJewel_vac.root
      }
      f->GetObject("jetTreeSig",tree);

   }
   Init(tree);
}

jetTreeSig::~jetTreeSig()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t jetTreeSig::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t jetTreeSig::LoadTree(Long64_t entry)
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

void jetTreeSig::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   eventWeight = 0;
   partonsPt = 0;
   partonsEta = 0;
   partonsPhi = 0;
   partonsM = 0;
   partonsPDG = 0;
   partonsFirstSplitPt = 0;
   partonsFirstSplitEta = 0;
   partonsFirstSplitPhi = 0;
   partonsFirstSplitM = 0;
   partonsFirstSplitPDG = 0;
   drSplit = 0;
   tfSplit = 0;
   sigJetPt = 0;
   //jetCollectionSigDYEPt = 0;
   sigJetEta = 0;
   sigJetPhi = 0;
   sigJetM = 0;
   sigJetArea = 0;
   // Reclustering effects
   sigJetRecurZcut_nSD = 0;
   sigJetRecurZcut_zSD = 0;
   sigJetRecur_nSD = 0;
   sigJetRecur_partonMatchDr = 0;
   sigJetRecur_zSD = 0;
   sigJetRecurZcut_dr12 = 0;
   sigJetRecurZcut_erad = 0;
   sigJetRecurZcut_jetpt = 0;
   sigJetRecurZcut_logdr12 = 0;
   sigJetRecurZcut_logztheta = 0;
   sigJetRecurZcut_tf = 0;
   sigJetRecurZcut_tfe = 0;
   sigJetRecurZcut_z = 0;
   sigJetRecurZcut_tau21 = 0;
   sigJetRecurZcut_tau32 = 0;
   sigJetRecur_dr12 = 0;
   sigJetRecur_erad = 0;
   sigJetRecur_jetpt = 0;
   sigJetRecur_logdr12 = 0;
   sigJetRecur_logztheta = 0;
   sigJetRecur_tf = 0;
   sigJetRecur_tfe = 0;
   sigJetRecur_z = 0;
   sigJetRecur_partonMatchID = 0;

   sigJetRecurTauZcut_nSD = 0;
   sigJetRecurTauZcut_zSD = 0;
   sigJetRecurTau_nSD = 0;
   sigJetRecurTau_partonMatchDr = 0;
   sigJetRecurTau_zSD = 0;
   sigJetRecurTauZcut_dr12 = 0;
   sigJetRecurTauZcut_erad = 0;
   sigJetRecurTauZcut_jetpt = 0;
   sigJetRecurTauZcut_logdr12 = 0;
   sigJetRecurTauZcut_logztheta = 0;
   sigJetRecurTauZcut_tf = 0;
   sigJetRecurTauZcut_tfe = 0;
   sigJetRecurTauZcut_z = 0;
   sigJetRecurTauZcut_tau21 = 0;
   sigJetRecurTauZcut_tau32 = 0;
   sigJetRecurTau_dr12 = 0;
   sigJetRecurTau_erad = 0;
   sigJetRecurTau_jetpt = 0;
   sigJetRecurTau_logdr12 = 0;
   sigJetRecurTau_logztheta = 0;
   sigJetRecurTau_tf = 0;
   sigJetRecurTau_tfe = 0;
   sigJetRecurTau_z = 0;

   sigJetDYE_z = 0;         //zg of groomed jets
   sigJetDYE_dr12 = 0;       //distance between the two subjet
   sigJetDYE_kt = 0;         //kt of groomed jets
   sigJetDYE_tau21 = 0;      //n-subjettiness ratio 21
   sigJetDYE_tau32 = 0;      //n-subjettiness ratio 32
   sigJetDYE_tf = 0;     	// formation time tf = 2 / (z*(1-z)*pt*theta^2) (soft limit)
   sigJetDYE_tfe = 0;     	// formation time tf = 1 / 2*(z*(1-z)*E*(1-cos(theta))) (hard limit)
   sigJetkappaDYE = 0;      //kappa of groomed jets
   sigJetDYE_drBranches = 0;
   sigJetTauDYE_z = 0;
   sigJetTauDYE_dr12 = 0;
   sigJetTauDYE_kt = 0;
   sigJetTauDYE_tau21 = 0;
   sigJetTauDYE_tau32 = 0;
   sigJetTauDYE_tf = 0;
   sigJetTauDYE_tfe = 0;
   sigJetTaukappaDYE = 0;
   sigJetTauDYE_drBranches = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventWeight", &eventWeight, &b_eventWeight);
   // True parton info
   fChain->SetBranchAddress("partonsPt", &partonsPt, &b_partonsPt);
   fChain->SetBranchAddress("partonsEta", &partonsEta, &b_partonsEta);
   fChain->SetBranchAddress("partonsPhi", &partonsPhi, &b_partonsPhi);
   fChain->SetBranchAddress("partonsM", &partonsM, &b_partonsM);
   fChain->SetBranchAddress("partonsPDG", &partonsPDG, &b_partonsPDG);
   fChain->SetBranchAddress("partonsFirstSplitPt", &partonsFirstSplitPt, &b_partonsFirstSplitPt);
   fChain->SetBranchAddress("partonsFirstSplitEta", &partonsFirstSplitEta, &b_partonsFirstSplitEta);
   fChain->SetBranchAddress("partonsFirstSplitPhi", &partonsFirstSplitPhi, &b_partonsFirstSplitPhi);
   fChain->SetBranchAddress("partonsFirstSplitM", &partonsFirstSplitM, &b_partonsFirstSplitM);
   fChain->SetBranchAddress("partonsFirstSplitPDG", &partonsFirstSplitPDG, &b_partonsFirstSplitPDG);
   fChain->SetBranchAddress("drSplit", &drSplit, &b_drSplit);
   fChain->SetBranchAddress("tfSplit", &tfSplit, &b_tfSplit);
   // True jet info (anti-kt clustering)
   fChain->SetBranchAddress("sigJetPt", &sigJetPt, &b_sigJetPt);
   fChain->SetBranchAddress("sigJetEta", &sigJetEta, &b_sigJetEta);
   fChain->SetBranchAddress("sigJetPhi", &sigJetPhi, &b_sigJetPhi);
   fChain->SetBranchAddress("sigJetM", &sigJetM, &b_sigJetM);
   fChain->SetBranchAddress("sigJetArea", &sigJetArea, &b_sigJetArea);
   //pTDSig and widthSig (from lambda angularity)
   // Declustering info C/A with grooming
   fChain->SetBranchAddress("sigJetRecurZcut_nSD", &sigJetRecurZcut_nSD, &b_sigJetRecurZcut_nSD);
   fChain->SetBranchAddress("sigJetRecurZcut_zSD", &sigJetRecurZcut_zSD, &b_sigJetRecurZcut_zSD);
   fChain->SetBranchAddress("sigJetRecur_partonMatchDr", &sigJetRecur_partonMatchDr, &b_sigJetRecur_partonMatchDr);
   fChain->SetBranchAddress("sigJetRecurZcut_dr12", &sigJetRecurZcut_dr12, &b_sigJetRecurZcut_dr12);
   fChain->SetBranchAddress("sigJetRecurZcut_erad", &sigJetRecurZcut_erad, &b_sigJetRecurZcut_erad);
   fChain->SetBranchAddress("sigJetRecurZcut_jetpt", &sigJetRecurZcut_jetpt, &b_sigJetRecurZcut_jetpt);
   fChain->SetBranchAddress("sigJetRecurZcut_logdr12", &sigJetRecurZcut_logdr12, &b_sigJetRecurZcut_logdr12);
   fChain->SetBranchAddress("sigJetRecurZcut_logztheta", &sigJetRecurZcut_logztheta, &b_sigJetRecurZcut_logztheta);
   fChain->SetBranchAddress("sigJetRecurZcut_tf", &sigJetRecurZcut_tf, &b_sigJetRecurZcut_tf);
   fChain->SetBranchAddress("sigJetRecurZcut_tfe", &sigJetRecurZcut_tfe, &b_sigJetRecurZcut_tfe);
   fChain->SetBranchAddress("sigJetRecurZcut_z", &sigJetRecurZcut_z, &b_sigJetRecurZcut_z);
   fChain->SetBranchAddress("sigJetRecurZcut_tau21", &sigJetRecurZcut_tau21, &b_sigJetRecurZcut_tau21);
   fChain->SetBranchAddress("sigJetRecurZcut_tau32", &sigJetRecurZcut_tau32, &b_sigJetRecurTauZcut_tau32);
   // Declustering info C/A no grooming
   fChain->SetBranchAddress("sigJetRecur_erad", &sigJetRecur_erad, &b_sigJetRecur_erad);
   fChain->SetBranchAddress("sigJetRecur_jetpt", &sigJetRecur_jetpt, &b_sigJetRecur_jetpt);
   fChain->SetBranchAddress("sigJetRecur_logdr12", &sigJetRecur_logdr12, &b_sigJetRecur_logdr12);
   fChain->SetBranchAddress("sigJetRecur_dr12", &sigJetRecur_dr12, &b_sigJetRecur_dr12);
   fChain->SetBranchAddress("sigJetRecur_logztheta", &sigJetRecur_logztheta, &b_sigJetRecur_logztheta);
   fChain->SetBranchAddress("sigJetRecur_tf", &sigJetRecur_tf, &b_sigJetRecur_tf);
   fChain->SetBranchAddress("sigJetRecur_tfe", &sigJetRecur_tfe, &b_sigJetRecur_tfe);
   fChain->SetBranchAddress("sigJetRecur_nSD", &sigJetRecur_nSD, &b_sigJetRecur_nSD);
   fChain->SetBranchAddress("sigJetRecur_zSD", &sigJetRecur_zSD, &b_sigJetRecur_zSD);
   fChain->SetBranchAddress("sigJetRecur_z", &sigJetRecur_z, &b_sigJetRecur_z);
   fChain->SetBranchAddress("sigJetRecur_partonMatchID", &sigJetRecur_partonMatchID, &b_sigJetRecur_partonMatchID);
   // Declustering info tau with grooming
   fChain->SetBranchAddress("sigJetRecurTauZcut_nSD", &sigJetRecurTauZcut_nSD, &b_sigJetRecurTauZcut_nSD);
   fChain->SetBranchAddress("sigJetRecurTauZcut_zSD", &sigJetRecurTauZcut_zSD, &b_sigJetRecurTauZcut_zSD);
   //fChain->SetBranchAddress("sigJetRecurTauZcut_partonMatchDr", &sigJetRecurTau_partonMatchDr, &b_sigJetRecurTau_partonMatchDr);
   fChain->SetBranchAddress("sigJetRecurTauZcut_dr12", &sigJetRecurTauZcut_dr12, &b_sigJetRecurTauZcut_dr12);
   fChain->SetBranchAddress("sigJetRecurTauZcut_erad", &sigJetRecurTauZcut_erad, &b_sigJetRecurTauZcut_erad);
   fChain->SetBranchAddress("sigJetRecurTauZcut_jetpt", &sigJetRecurTauZcut_jetpt, &b_sigJetRecurTauZcut_jetpt);
   fChain->SetBranchAddress("sigJetRecurTauZcut_logdr12", &sigJetRecurTauZcut_logdr12, &b_sigJetRecurTauZcut_logdr12);
   fChain->SetBranchAddress("sigJetRecurTauZcut_logztheta", &sigJetRecurTauZcut_logztheta, &b_sigJetRecurTauZcut_logztheta);
   fChain->SetBranchAddress("sigJetRecurTauZcut_tf", &sigJetRecurTauZcut_tf, &b_sigJetRecurTauZcut_tf);
   fChain->SetBranchAddress("sigJetRecurTauZcut_tfe", &sigJetRecurTauZcut_tfe, &b_sigJetRecurTauZcut_tfe);
   fChain->SetBranchAddress("sigJetRecurTauZcut_z", &sigJetRecurTauZcut_z, &b_sigJetRecurTauZcut_z);
   fChain->SetBranchAddress("sigJetRecurTauZcut_tau21", &sigJetRecurTauZcut_tau21, &b_sigJetRecurTauZcut_tau21);
   fChain->SetBranchAddress("sigJetRecurTauZcut_tau32", &sigJetRecurTauZcut_tau32, &b_sigJetRecurTauZcut_tau32);

   // Declustering info tau no grooming
   fChain->SetBranchAddress("sigJetRecurTau_erad", &sigJetRecurTau_erad, &b_sigJetRecurTau_erad);
   fChain->SetBranchAddress("sigJetRecurTau_jetpt", &sigJetRecurTau_jetpt, &b_sigJetRecurTau_jetpt);
   fChain->SetBranchAddress("sigJetRecurTau_logdr12", &sigJetRecurTau_logdr12, &b_sigJetRecurTau_logdr12);
   fChain->SetBranchAddress("sigJetRecurTau_dr12", &sigJetRecur_dr12, &b_sigJetRecurTau_dr12);
   fChain->SetBranchAddress("sigJetRecurTau_logztheta", &sigJetRecurTau_logztheta, &b_sigJetRecurTau_logztheta);
   fChain->SetBranchAddress("sigJetRecurTau_tf", &sigJetRecurTau_tf, &b_sigJetRecurTau_tf);
   fChain->SetBranchAddress("sigJetRecurTau_tfe", &sigJetRecurTau_tfe, &b_sigJetRecurTau_tfe);
   fChain->SetBranchAddress("sigJetRecurTau_z", &sigJetRecurTau_z, &b_sigJetRecurTau_z);
   fChain->SetBranchAddress("sigJetRecurTau_nSD", &sigJetRecurTau_nSD, &b_sigJetRecurTau_nSD);
   fChain->SetBranchAddress("sigJetRecurTau_zSD", &sigJetRecurTau_zSD, &b_sigJetRecurTau_zSD);
   //fChain->SetBranchAddress("sigJetRecurTau_partonMatchID", &sigJetRecurTau_partonMatchID, &b_sigJetRecurTau_partonMatchID); 
   // Effect of bkg subtraction removed here.
   fChain->SetBranchAddress("sigJetDYE_z", &sigJetDYE_z, &b_sigJetDYE_z);
   fChain->SetBranchAddress("sigJetDYE_dr12", &sigJetDYE_dr12, &b_sigJetDYE_dr12);
   fChain->SetBranchAddress("sigJetDYE_kt", &sigJetDYE_kt, &b_sigJetDYE_kt);
   fChain->SetBranchAddress("sigJetDYE_tau21", &sigJetDYE_tau21, &b_sigJetDYE_tau21);
   fChain->SetBranchAddress("sigJetDYE_tau32", &sigJetDYE_tau32, &b_sigJetDYE_tau32);
   fChain->SetBranchAddress("sigJetDYE_tf", &sigJetDYE_tf, &b_sigJetDYE_tf);
   fChain->SetBranchAddress("sigJetDYE_tfe", &sigJetDYE_tfe, &b_sigJetDYE_tfe);
   fChain->SetBranchAddress("sigJetkappaDYE", &sigJetkappaDYE, &b_sigJetkappaDYE);
   fChain->SetBranchAddress("sigJetDYE_drBranches", &sigJetDYE_drBranches, &b_sigJetDYE_drBranches);
   fChain->SetBranchAddress("sigJetTauDYE_z", &sigJetTauDYE_z, &b_sigJetTauDYE_z);
   fChain->SetBranchAddress("sigJetTauDYE_dr12", &sigJetTauDYE_dr12, &b_sigJetTauDYE_dr12);   
   fChain->SetBranchAddress("sigJetTauDYE_kt", &sigJetTauDYE_kt, &b_sigJetTauDYE_kt);
   fChain->SetBranchAddress("sigJetTauDYE_tau21", &sigJetTauDYE_tau21, &b_sigJetTauDYE_tau21);
   fChain->SetBranchAddress("sigJetTauDYE_tau32", &sigJetTauDYE_tau32, &b_sigJetTauDYE_tau32);
   fChain->SetBranchAddress("sigJetTauDYE_tf", &sigJetTauDYE_tf, &b_sigJetTauDYE_tf);
   fChain->SetBranchAddress("sigJetTauDYE_tfe", &sigJetTauDYE_tfe, &b_sigJetTauDYE_tfe);
   fChain->SetBranchAddress("sigJetTaukappaDYE", &sigJetTaukappaDYE, &b_sigJetTaukappaDYE);
   fChain->SetBranchAddress("sigJetTauDYE_drBranches", &sigJetTauDYE_drBranches, &b_sigJetTauDYE_drBranches);
   

   Notify();
}

Bool_t jetTreeSig::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void jetTreeSig::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t jetTreeSig::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef jetTreeSig_cxx
