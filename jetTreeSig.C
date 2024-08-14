#define jetTreeSig_cxx
#include "jetTreeSig.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>

void jetTreeSig::Loop(Bool_t reduced, TString generator, TString duration)
{
//   In a ROOT session, you can do:
//      root> .L jetTreeSig.C
//      root> jetTreeSig t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//    Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   //Long64_t nentries = fChain->GetEntries();
   int nentries = fChain->GetEntriesFast();
   std::cout << "nentries: " << nentries << std::endl;
   
   //Jet Energy Scale 
   TH1F *h1JESCS = new TH1F("h1JESCS","hJESCS",100,0.,2.);
   TH2F *h2PtJESCS = new TH2F("h2PtJESCS","h2PtJESCS",200.,0.,400.,100,0.,2.);
   //Crosscheck - add weight ("eventWeight",   eventWeight);
   TH1F *h1JPtW = new TH1F("h1JPtW","h1JPtW",40,0.,400.);
   TH1F *h1JPt = new TH1F("h1JPt","h1JPt",40,0.,400.);

   TH1F *h1JsigTauPtW = new TH1F("h1JsigTauPtW","h1JsigTauPtW",40,0.,400.);
   TH1F *h1JsigTauPt = new TH1F("h1JsigTauPt","h1JsigTauPt",40,0.,400.);

   TH1F *h1JsigCAPtW = new TH1F("h1JsigCAPtW","h1JsigCAPtW",40,0.,400.);
   TH1F *h1JsigCAPt = new TH1F("h1JsigCAPt","h1JsigCAPt",40,0.,400.);

   TH1F *h1JsigDyPtW = new TH1F("h1JsigDyPtW","h1JsigDyPtW",40,0.,400.);
   TH1F *h1JsigDyPt = new TH1F("h1JsigDyPt","h1JsigDyPt",40,0.,400.);
   //Formation time
   TH1F *h1LogTfSig = new TH1F("h1LogTfSig","h1LogTfSig",50,-10.,10.);
   TH1F *h1LogTfSigTau = new TH1F("h1LogTfSigTau","h1LogTfSigTau",50,-10.,10.);
   TH1F *h1LogTfSigZ = new TH1F("h1LogTfSigZ","h1LogTfSigZ",50,-10.,10.);
   TH1F *h1LogTfSigZTau = new TH1F("h1LogTfSigZTau","h1LogTfSigZTau",50,-10.,10.);

   TH2F *h2SigTfTauTf = new TH2F("h2SigTfTauTf","h2SigTfTauTf",100,0.,10.,100,0.,10.);
   TH2F *h2SigTfTauTfZcut = new TH2F("h2SigTfTauTfZcut","h2SigTfTauTfZcut",100,0.,10.,100,0.,10.);
   TH2F *h2SigTfTauTfZcutSoft = new TH2F("h2SigTfTauTfZcutSoft","h2SigTfTauTfZcutSoft",100,0.,10.,100,0.,10.);

   TH2F *h2SigTfDgTf = new TH2F("h2SigTfDgTf","h2SigTfDgTf",100,0.,10.,100,0.,10.);
   TH2F *h2SigTfDgTfSoft = new TH2F("h2SigTfDgTfSoft","h2SigTfDgTfSoft",100,0.,10.,100,0.,10.);
   
   TH2F *h2SigRgTauRgZcut = new TH2F("h2SigRgTauRgZcut","h2SigRgTauRgZcut",100,0.,1.,100,0.,1.);// Rg corr
   //Number of calculatest formation times in jet
   TH1F *h1SigNCaTf = new TH1F("h1SigNCaTf","h1SigNCaTf",20,0.,20.);
   TH1F *h1SigNCaTfZcut = new TH1F("h1SigNCaTfZcut","h1SigNCafZcut",20,0.,20.);
   TH1F *h1SigNTauTf = new TH1F("h1SigNTauTf","h1SigNTauTf",20,0.,20.);
   TH1F *h1SigNTauTfZcut = new TH1F("h1SigNTauTfZcut","h1SigNTauTfZcut",20,0.,20.);
   TH1F *h1SigNTauTfZcutSoft = new TH1F("h1SigNTauTfZcutSoft","h1SigNTauTfZcutSoft",20,0.,20);
   TH1F *h1SigNTauTfDg = new TH1F("h1SigNTauTfDg","h1SigNTauTfDg",20,0.,20);
   TH1F *h1SigNTfDg = new TH1F("h1SigNTfDg","h1SigNTfDg",20,0.,20);
   //Dynamical grooming 
   TH2F *h2Kappa = new TH2F("h2Kappa","h2Kappa",100,0.,10.,100,0.,10.);
   //Dropped branches
   TH1F *h1DroppedBrTau = new TH1F("h1DroppedBrTau","h1DroppedBrTau",20,0.,20);
   TH1F *h1DroppedBr = new TH1F("h1DroppedBr","h1DroppedBr",20,0.,20);
   //Nsubjettiness (dy)
   TH2F *h2Nsubj21 = new TH2F("h2Nsubj21","h2Nsubj21",20,0.,1.,20,0.,1.);
   TH2F *h2Nsubj32 = new TH2F("h2Nsubj32","h2Nsubj32",20,0.,1.,20,0.,1.);
   TH2F *h2TauNsubj21 = new TH2F("h2TauNsubj21","h2TauNsubj21",20,0.,10.,20,0.,1.);
   TH2F *h2TauNsubj21Soft = new TH2F("h2TauNsubj21Soft","h2TauNsubj21Soft",20,0.,10.,20,0.,1.);
   //Nsubjettiness (ca/tau)
   TH2F *h2CaTauNsubj21 = new TH2F("h2CaTauNsubj21","h2CaTauNsubj21",20,0.,1.,20,0.,1.);
   TH2F *h2CaTauNsubj32 = new TH2F("h2CaTauNsubj32","h2CaTauNsubj32",20,0.,1.,20,0.,1.);

   /* pt's of each node -- to be added !!!!!
   TH1F *h1PtsSigCA = new TH1F("h1PtsSigCA","h1PtsSigCA",50,0.,0.5);
   TH1F *h1PtsSigTau = new TH1F("h1PtsSigTau","h1PtsSigTau",50,0.,0.5);
   // all kt's in the algorithm
   TH1F *h1LKtSigCA = new TH1F("h1LKtSigCA","h1LKtSigCA",50,0.,0.5);
   TH1F *h1KtSigTau = new TH1F("h1KtSigTau","h1KtSigTau",50,0.,0.5);
   // to be added from dyn. grooming         
   vector<double> *sigJetDYE_kt;         //kt of groomed jets
   vector<double> *sigJetTauDYE_kt;

   */

   TList *fSplitList = new TList();
   fSplitList->SetName("fSplitList");
   fSplitList->SetOwner(kTRUE);
   Int_t Nsplit = 2;

   TH1F *h1TfSigTau[Nsplit]; 
   TH1F *h1TfSigTauSoft[Nsplit]; 
   TH1F *h1TfSigCA[Nsplit]; 
   TH1F *h1TfSigDy[Nsplit]; 
   TH1F *h1ZgSigCA[Nsplit]; 
   TH1F *h1ZgSigTau[Nsplit];
   TH1F *h1ZgSigDg[Nsplit];
   TH1F *h1ZgSigDgTau[Nsplit];
   TH1F *h1RgCASig[Nsplit];
   TH1F *h1RgTauSig[Nsplit];
   TH1F *h1RgDgSig[Nsplit];
   TH1F *h1RgDgTauSig[Nsplit];
   TH1F *h1EradSigCA[Nsplit];
   TH1F *h1EradSigTau[Nsplit];
   TH1F *h1LogDr12SigCA[Nsplit];
   TH1F *h1LogDr12SigTau[Nsplit];
   TH1F *h1LogZthetaSigCA[Nsplit];
   TH1F *h1LogZthetaSigTau[Nsplit];

   for (int nsplit = 0; nsplit < 2; nsplit++){
    h1TfSigTau[Nsplit] = NULL; 
    h1TfSigTauSoft[Nsplit] = NULL; 
    h1TfSigCA[Nsplit] = NULL; 
    h1TfSigDy[Nsplit] = NULL; 
    h1ZgSigCA[nsplit] = NULL;
    h1ZgSigTau[nsplit] = NULL;
    h1ZgSigDg[nsplit] = NULL;
    h1ZgSigDgTau[nsplit] = NULL;
    h1RgCASig[nsplit] = NULL;
    h1RgTauSig[nsplit] = NULL;
    h1RgDgSig[nsplit] = NULL;
    h1RgDgTauSig[nsplit] = NULL;
    h1EradSigCA[nsplit] = NULL;
    h1EradSigTau[nsplit] = NULL;
    h1LogDr12SigCA[nsplit] = NULL;
    h1LogDr12SigTau[nsplit] = NULL;
    h1LogZthetaSigCA[nsplit] = NULL;
    h1LogZthetaSigTau[nsplit] = NULL;
   }

   //Jet substructure -- loop for 0th and first
   for (int nsplit = 0; nsplit < 2; nsplit++){
    //fill 0 with 1st split, 1 with 2nd and 3 with all.
    h1ZgSigCA[nsplit] = new TH1F(Form("h1ZgSigCA[%d]",nsplit), Form("h1ZgSigCA[%d]",nsplit),50,0.,1);
    fSplitList->Add(h1ZgSigCA[nsplit]);
    h1ZgSigTau[nsplit] = new TH1F(Form("h1ZgSigTau[%d]", nsplit), Form("h1ZgSigTau[%d]",nsplit),50,0.,1);
    fSplitList->Add(h1ZgSigTau[nsplit]);
    h1ZgSigDg[nsplit] = new TH1F(Form("h1ZgSigDg[%d]", nsplit), Form("h1ZgSigDg[%d]",nsplit),50,0.,1);
    fSplitList->Add(h1ZgSigDg[nsplit]);
    h1ZgSigDgTau[nsplit] = new TH1F(Form("h1ZgSigDgTau[%d]", nsplit), Form("h1ZgSigDgTau[%d]",nsplit),50,0.,1);
    fSplitList->Add(h1ZgSigDgTau[nsplit]);
    //angles of the algorithm
    h1RgCASig[nsplit] = new TH1F(Form("h1RgCASig[%d]", nsplit),Form("h1RgCASig[%d]", nsplit),50,0.,0.5);
    fSplitList->Add(h1RgCASig[nsplit]);
    h1RgTauSig[nsplit] = new TH1F(Form("h1RgTauSig[%d]", nsplit),Form("h1RgTauSig[%d]", nsplit),50,0.,0.5);
    fSplitList->Add(h1RgTauSig[nsplit]);
    h1RgDgSig[nsplit] = new TH1F(Form("h1RgDgSig[%d]", nsplit),Form("h1RgDgSig[%d]", nsplit),50,0.,0.5);
    fSplitList->Add(h1RgDgSig[nsplit]);
    h1RgDgTauSig[nsplit] = new TH1F(Form("h1RgDgTauSig[%d]", nsplit),Form("h1RgDgTauSig[%d]", nsplit),50,0.,0.5);
    fSplitList->Add(h1RgDgTauSig[nsplit]);
    //energy of sum of the two branches at each node
    h1EradSigCA[nsplit] = new TH1F(Form("h1EradSigCA[%d]", nsplit), Form("h1EradSigCA[%d]", nsplit),100,0,1000);
    fSplitList->Add(h1EradSigCA[nsplit]);
    h1EradSigTau[nsplit] = new TH1F(Form("h1EradSigTau[%d]", nsplit),Form("h1EradSigTau[%d]", nsplit),100,0,1000);
    fSplitList->Add(h1EradSigTau[nsplit]);
    //Log(1/angle) in the algorithm
    h1LogDr12SigCA[nsplit] = new TH1F(Form("h1LogDr12SigCA[%d]", nsplit),Form("h1LogDr12SigCA[%d]", nsplit),50,-5,10);
    fSplitList->Add(h1LogDr12SigCA[nsplit]);
    h1LogDr12SigTau[nsplit] = new TH1F(Form("h1LogDr12SigTau[%d]", nsplit), Form("h1LogDr12SigTau[%d]", nsplit),50,-5,10);
    fSplitList->Add(h1LogDr12SigTau[nsplit]);
    // Log(z*angle) in the algorithm
    h1LogZthetaSigCA[nsplit] = new TH1F(Form("h1LogZthetaSigCA[%d]", nsplit),Form("h1LogZthetaSigCA[%d]", nsplit),50,-10,10);
    fSplitList->Add(h1LogZthetaSigCA[nsplit]);
    h1LogZthetaSigTau[nsplit] = new TH1F(Form("h1LogZthetaSigTau[%d]", nsplit), Form("h1LogZthetaSigTau[%d]", nsplit),50,-10,10);
    fSplitList->Add(h1LogZthetaSigTau[nsplit]);
    // formation time
    h1TfSigTau[nsplit] = new TH1F(Form("h1TfSigTau[%d]", nsplit), Form("h1TfSigTau[%d]", nsplit),100,0,10);
    fSplitList->Add(h1TfSigTau[nsplit]);
    h1TfSigTauSoft[nsplit] = new TH1F(Form("h1TfSigTauSoft[%d]", nsplit), Form("h1TfSigTauSoft[%d]", nsplit),100,0,10);
    fSplitList->Add(h1TfSigTauSoft[nsplit]);
    h1TfSigCA[nsplit] = new TH1F(Form("h1TfSigCA[%d]", nsplit), Form("h1TfSigCA[%d]", nsplit),100,0,10);
    fSplitList->Add(h1TfSigCA[nsplit]);
    h1TfSigDy[nsplit] = new TH1F(Form("h1TfSigDy[%d]", nsplit), Form("h1TfSigDy[%d]", nsplit),100,0,10);
    fSplitList->Add(h1TfSigDy[nsplit]);
   }

  /*
   TH2F *h2PartonSigTf = new TH2F("h2PartonSigTf","h2PartonSigTf",100,0.,1.,100,0.,1.);
   TH2F *h2PartonSigTfTau = new TH2F("h2PartonSigTfTau","h2PartonSigTfTau",100,0.,1.,100,0.,1.);
   TH2F *h2PartonSigTfZcut = new TH2F("h2PartonSigTfZcut","h2PartonSigTfZcut",100,0.,1.,100,0.,1.);
   TH2F *h2PartonSigTfZcutTau = new TH2F("h2PartonSigTfZcutTau","h2PartonSigTfZcutTau",100,0.,1.,100,0.,1.);
  */
   //TH2F *h2PartonSigChTf = new TH2F("h2PartonSigChTf","h2PartonSigChTf",100,0.,1.,100,0.,1.);
   //TH2F *h2PartonCSTf = new TH2F("h2PartonCSTf","h2PartonCSTf",100,0.,1.,100,0.,1.);
   //TH2F *h2PartonCSFullTf = new TH2F("h2PartonCSFullTf","h2PartonCSFullTf",100,0.,1.,100,0.,1.);


   
   Long64_t nbytes = 0, nb = 0;
  if(reduced==true){
    std::cout<< " cutting tree "<<std::endl;
  }
   std::cout << "before first loop " << std::endl;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      //if (Cut(ientry) < 0) continue;
      double evWeight = eventWeight->at(0);
      //std::cout << "jentry: " << jentry << std::endl;
      //std::cout << "ientry: " << ientry << std::endl;
      //std::cout << "event weight in first loop: " << evWeight << std::endl;

      for(int i = 0; i<sigJetPt->size(); ++i) {
        double sigPt = sigJetPt->at(i);
        
        if(sigPt<120. || abs(sigJetEta->at(i))>2.) continue;//this eta was set to >2 , now i removed the full cut

        std::vector<double> sigTfsZ = sigJetRecurZcut_tfe->at(i);//sig after c/a in hard limit
        std::vector<double> sigTfsZTau = sigJetRecurTauZcut_tfe->at(i);//sig after tau in hard limit
        std::vector<double> sigTfsZTauSoft = sigJetRecurTauZcut_tf->at(i);//sig after tau in soft limit 
        std::vector<double> sigTfsZSoft = sigJetRecurZcut_tf->at(i);//sig after tau in soft limit 
        //formation time wih dyn. grooming (kapa) veto
        std::vector<double>	sigTfsDg = sigJetDYE_tfe->at(i);     	// dyn. groom with C/A history; tf = 1 / 2*(z*(1-z)*E*(1-cos(theta))) (hard limit)
        std::vector<double> sigTfsDgSoft = sigJetDYE_tf->at(i);     	// dyn. groom with C/A history; tf = 2 / (z*(1-z)*pt*theta^2) (soft limit)
        std::vector<double>	sigTfsDgTau = sigJetTauDYE_tfe->at(i);     	// dyn. groom with tau history; tf = 1 / 2*(z*(1-z)*E*(1-cos(theta))) (hard limit)
        std::vector<double> sigTfsDgTauSoft = sigJetTauDYE_tf->at(i);     	// dyn. groom with tau history; tf = 2 / (z*(1-z)*pt*theta^2) (soft limit)
        std::vector<double> kappaCA = sigJetkappaDYE->at(i);      //kappa of groomed jets (double kappa = 1/(z * (1-z) * pt * pow(theta,a_));)
        std::vector<double> kappaTau = sigJetTaukappaDYE->at(i);
  
        if(reduced==true && generator=="CA"){
          if(!(sigTfsZ.size()>0)) continue;
          if(duration == "short" && (sigTfsZ[0] >= 1)) continue;
          if(duration == "supershort" && (sigTfsZ[0] >= 0.5)) continue;
          if(duration == "large" && (sigTfsZ[0] <= 3)) continue;
          if(duration == "intermed" && ((sigTfsZ[0] < 1) || (sigTfsZ[0] > 3))) continue;
        }
        if(reduced==true && generator=="Tau"){
          if(!(sigTfsZTau.size()>0)) continue;
          if(duration == "short" && (sigTfsZTau[0] >= 1)) continue;
          if(duration == "supershort" && (sigTfsZTau[0] >= 0.5)) continue;
          if(duration == "large" && (sigTfsZTau[0] <= 3)) continue;
          if(duration == "intermed" && ((sigTfsZTau[0] < 1) || (sigTfsZTau[0] > 3))) continue;
        }
        if(reduced==true && generator=="Soft"){
          if(!(sigTfsZTau.size()>0)) continue;
          if(duration == "short" && (sigTfsZTauSoft[0] >= 1)) continue;
          if(duration == "supershort" && (sigTfsZTauSoft[0] >= 0.5)) continue;
          if(duration == "large" && (sigTfsZTauSoft[0] <= 3)) continue;
          if(duration == "intermed" && ((sigTfsZTauSoft[0] < 1) || (sigTfsZTauSoft[0] > 3))) continue;
        }
        if(reduced==true && generator=="Dy"){
          if(!(sigTfsZTau.size()>0)) continue;
          if(duration == "short" && (sigTfsDg[0] >= 0.5)) continue;
          if(duration == "supershort" && (sigTfsDg[0] >= 0.5)) continue;
          if(duration == "large" && (sigTfsDg[0] <= 3)) continue;
          if(duration == "intermed" && ((sigTfsDg[0] < 1) || (sigTfsDg[0] > 3))) continue;
        }
        h1JPtW->Fill(sigPt, evWeight);// add jet norm im plotting - from pt spectrun
        h1JPt->Fill(sigPt);

        std::vector<double> sigCAPt = sigJetRecurZcut_jetpt->at(i);//sig after c/a 
        if(sigCAPt.size()>0){
          h1JsigCAPtW->Fill(sigCAPt[0], evWeight);
          h1JsigCAPt->Fill(sigCAPt[0]);
        } 

        std::vector<double> sigTauPt = sigJetRecurTauZcut_jetpt->at(i);//sig after tau
        if(sigTauPt.size()>0){
          h1JsigTauPtW->Fill(sigTauPt[0], evWeight);
          h1JsigTauPt->Fill(sigTauPt[0]);
        } 

       // std::vector<double> sigDyPt = jetCollectionSigDYEPt->at(i);//sigDy no history
       // if(sigDyPt.size()>0){
       //   h1JsigDyPtW->Fill(sigDyPt[0], evWeight);
        //  h1JsigDyPt->Fill(sigDyPt[0]);
       // } 

        //Dynamical grooming 
        if( (kappaCA.size()>0) && (kappaTau.size()>0)) h2Kappa->Fill(kappaCA[0],kappaTau[0]);
        //Number of dropped branches
        //std::cout<<"before Ndropp"<<std::endl;
        std::vector<double> drTauDYEBr = sigJetTauDYE_drBranches->at(0);      //dropped branches tau history
        std::vector<double> drDYEBr = sigJetDYE_drBranches->at(i);      //dropped branches C/A history
        if (drTauDYEBr.size()>0) h1DroppedBrTau->Fill(drTauDYEBr[0]);
        if (drDYEBr.size()>0) h1DroppedBr->Fill(drDYEBr[0]);
        //std::cout<<"after Ndropp"<<std::endl;
        //Nsubjettiness (Dy per jet)
        std::vector<double> tau21 = sigJetDYE_tau21->at(i);      //n-subjettiness ratio 21
        std::vector<double> tau21Tau = sigJetTauDYE_tau21->at(i);
        if( (tau21.size()>0) && (tau21Tau.size()>0)) h2Nsubj21->Fill(tau21[0],tau21Tau[0]);
        std::vector<double> tau32 = sigJetDYE_tau32->at(i);      //n-subjettiness ratio 32
        std::vector<double> tau32Tau = sigJetTauDYE_tau32->at(i);
        if( (tau32.size()>0) && (tau32Tau.size()>0)) h2Nsubj32->Fill(tau32[0],tau32Tau[0]);
        // correlate hard and soft formatine time from fyn grooming with jet nsubjetiness
        if( (sigTfsDgTau.size()>0) && (tau21Tau.size()>0)) h2TauNsubj21->Fill(sigTfsDgTau[0],tau21Tau[0]);
        if( (sigTfsDgTauSoft.size()>0) && (tau21Tau.size()>0)) h2TauNsubj21Soft->Fill(sigTfsDgTauSoft[0],tau21Tau[0]);
        //Nsubjettiness (ca/tau)
        std::vector<double> tauCa21 = sigJetRecurZcut_tau21->at(0);
        std::vector<double> tauTau21 = sigJetRecurTauZcut_tau21->at(0);
        std::vector<double> tauCa32 = sigJetRecurZcut_tau32->at(0);
        std::vector<double> tauTau32 = sigJetRecurTauZcut_tau32->at(0);
        if( tauCa21.size()>0 && tauTau21.size()>0) h2CaTauNsubj21->Fill(tauCa21[0], tauTau21[0]);
        if( tauCa32.size()>0 && tauTau32.size()>0) h2CaTauNsubj32->Fill(tauCa32[0], tauTau32[0]);

        std::vector<double> sigTfs = sigJetRecur_tfe->at(i);//sig after c/a no grooming
        if(sigTfs.size()>0) h1LogTfSig->Fill(log(sigTfs[0]));
        if(sigTfsZ.size()>0) h1LogTfSigZ->Fill(log(sigTfsZ[0]));// sd grooming

        std::vector<double> sigTfsTau = sigJetRecurTau_tfe->at(i);//sig after tau no grooming
        if(sigTfsTau.size()>0) h1LogTfSigTau->Fill(log(sigTfsTau[0]));
        if(sigTfsZTau.size()>0) h1LogTfSigZTau->Fill(log(sigTfsZTau[0]));// sd grooming
        

        if(sigTfs.size()>0 && sigTfsTau.size()>0) h2SigTfTauTf->Fill(sigTfs[0], sigTfsTau[0]);
        if(sigTfsZ.size()>0 && sigTfsZTau.size()>0) h2SigTfTauTfZcut->Fill(sigTfsZ[0], sigTfsZTau[0]);
        //checking time for different calculation in hard instead of soft limit
        if(sigTfsZSoft.size()>0 && sigTfsZTauSoft.size()>0) h2SigTfTauTfZcutSoft->Fill(sigTfsZSoft[0], sigTfsZTauSoft[0]);
        //checking time from dynamical grooming
        if(sigTfsDg.size()>0 && sigTfsDgTau.size()>0) h2SigTfDgTf->Fill(sigTfsDg[0], sigTfsDgTau[0]);
        if(sigTfsDgSoft.size()>0 && sigTfsDgTauSoft.size()>0) h2SigTfDgTfSoft->Fill(sigTfsDgSoft[0], sigTfsDgTauSoft[0]);

        //Number of split times calculated via CA algorithm 
        if(sigTfs.size()>0) h1SigNCaTf->Fill(sigTfs.size());
        if(sigTfsZ.size()>0) h1SigNCaTfZcut->Fill(sigTfsZ.size());
        if(sigTfsZ.size()>1){
          h1TfSigCA[0]->Fill(sigTfsZ[0]);
          h1TfSigCA[1]->Fill(sigTfsZ[1]);
        } 
        //Number of split times calculated via tau algorithm
        if(sigTfsTau.size()>0) h1SigNTauTf->Fill(sigTfsTau.size());
        if(sigTfsZTau.size()>0) h1SigNTauTfZcut->Fill(sigTfsZTau.size());
        if(sigTfsZTau.size()>1){
          h1TfSigTau[0]->Fill(sigTfsZTau[0]);
          h1TfSigTau[1]->Fill(sigTfsZTau[1]);
        } 
        //checking time for different calculation in soft instead of soft limit
        if(sigTfsZSoft.size()>0 && sigTfsZTauSoft.size()>0) h1SigNTauTfZcutSoft->Fill(sigTfsZTauSoft.size());
        if(sigTfsZTauSoft.size()>0){
          h1TfSigTauSoft[0]->Fill(sigTfsZTauSoft[0]);
          h1TfSigTauSoft[1]->Fill(sigTfsZTauSoft[1]);
        }
        //check number if formation times from dynamical grooming
        if(sigTfsDgTau.size()>0) h1SigNTauTfDg->Fill(sigTfsDgTau.size());
        if(sigTfsDgTau.size()>0){
          h1TfSigDy[0]->Fill(sigTfsDgTau[0]);
          h1TfSigDy[1]->Fill(sigTfsDgTau[1]);
        } 
        if(sigTfsDg.size()>0) h1SigNTfDg->Fill(sigTfsDg.size());
        
        // Rg
        std::vector<double> sigRgs = sigJetRecurZcut_dr12->at(i);//sig after c/a 0.1 grooming
        if(sigRgs.size()>1){
          h1RgCASig[0]->Fill(sigRgs[0]);
          h1RgCASig[1]->Fill(sigRgs[1]);
        } 
        std::vector<double> sigRgsTau = sigJetRecurTauZcut_dr12->at(i);//sig tau c/a 0.1 grooming
        if(sigRgsTau.size()>0){
          h1RgTauSig[0]->Fill(sigRgsTau[0]);
          h1RgTauSig[1]->Fill(sigRgsTau[1]);
        }
        std::vector<double> sigRgsD = sigJetDYE_dr12->at(i);
        if(sigRgsD.size()>0){// has only one split per jet -> only first
          h1RgDgSig[0]->Fill(sigRgsD[0]);
          //h1RgDgSig[1]->Fill(sigRgsD[1]);
        }
        std::vector<double> sigRgsDTau = sigJetTauDYE_dr12->at(i);
        if(sigRgsDTau.size()>0){
          h1RgDgTauSig[0]->Fill(sigRgsDTau[0]);
          //h1RgDgTauSig[1]->Fill(sigRgsDTau[1]);
        }
        //2D corr
        if(sigRgsTau.size()>0 && sigRgs.size()>0){
          h2SigRgTauRgZcut->Fill(sigRgs[0], sigRgsTau[0]);
        }
        // Zg
        std::vector<double> sigZgs = sigJetRecurZcut_z->at(i);
        if(sigZgs.size()>0){
          h1ZgSigCA[0]->Fill(sigZgs[0]);
          h1ZgSigCA[1]->Fill(sigZgs[1]);          
        }
        std::vector<double> sigZgsTau = sigJetRecurTauZcut_z->at(i);
        if(sigZgsTau.size()>0){
          h1ZgSigTau[0]->Fill(sigZgsTau[0]);
          h1ZgSigTau[1]->Fill(sigZgsTau[1]);                
        }
        std::vector<double> sigZgsD = sigJetDYE_z->at(i);
        if(sigZgsD.size()>0){
          h1ZgSigDg[0]->Fill(sigZgsD[0]);
          //h1ZgSigDg[1]->Fill(sigZgsD[1]);                
        }
        std::vector<double> sigZgsDTau = sigJetTauDYE_z->at(i);
        if(sigZgsDTau.size()>0){
          h1ZgSigDgTau[0]->Fill(sigZgsDTau[0]);
         // h1ZgSigDgTau[1]->Fill(sigZgsDTau[1]);                
        }
        // erad 
        std::vector<double> sigErads = sigJetRecurZcut_erad->at(i);
        if(sigErads.size()>0){
          h1EradSigCA[0]->Fill(sigErads[0]);
          h1EradSigCA[1]->Fill(sigErads[1]);
        }
        std::vector<double> sigEradsTau = sigJetRecurTauZcut_erad->at(i);
        if(sigEradsTau.size()>0){
          h1EradSigTau[0]->Fill(sigEradsTau[0]);
          h1EradSigTau[1]->Fill(sigEradsTau[1]);
        }
        // log(1/angle)
        std::vector<double> sigLogdr12 = sigJetRecurZcut_logdr12->at(i);
        if(sigLogdr12.size()>0){
          h1LogDr12SigCA[0]->Fill(sigLogdr12[0]);
          h1LogDr12SigCA[1]->Fill(sigLogdr12[1]);
        }
        std::vector<double> sigLogdr12Tau = sigJetRecurTauZcut_logdr12->at(i);
        if(sigLogdr12Tau.size()>0){
          h1LogDr12SigTau[0]->Fill(sigLogdr12Tau[0]);
          h1LogDr12SigTau[1]->Fill(sigLogdr12Tau[1]);
        }
        // log(z*angle)
        std::vector<double> sigLogZtheta = sigJetRecurZcut_logztheta->at(i);
        if(sigLogZtheta.size()>0){          
          h1LogZthetaSigCA[0]->Fill(sigLogZtheta[0]);
          h1LogZthetaSigCA[1]->Fill(sigLogZtheta[1]);
        } 
        std::vector<double> sigLogZthetaTau = sigJetRecurTauZcut_logztheta->at(i);
        if(sigLogZthetaTau.size()>0){
          h1LogZthetaSigTau[0]->Fill(sigLogZthetaTau[0]);
          h1LogZthetaSigTau[1]->Fill(sigLogZthetaTau[1]);
        } 


        /*find corresponding parton splitting formation time
        int iparton = sigJetRecur_partonMatchID->at(i);
        double drparton = sigJetRecur_partonMatchDr->at(i);
        if(drparton<0.4) { //only consider partons close to jet
          double drsplit = drSplit->at(iparton);
          double tfsplit = tfSplit->at(iparton);

          if(sigTfs.size()>0) {
            std::cout << "Fill hitstos of interest " << std::endl;
            h2PartonSigTf->Fill(tfsplit,sigTfs[0]);
            h1TfScaleSigp->Fill(sigTfs[0]/tfsplit);
          }
          if(sigTfsTau.size()>0) {
            std::cout << "Fill hitstos of interest " << std::endl;
            h2PartonSigTfTau->Fill(tfsplit, sigTfsTau[0]);
          }
          if(sigTfsZ.size()>0) {
            std::cout << "Fill hitstos of interest " << std::endl;
            h2PartonSigTfZcut->Fill(tfsplit, sigTfsZ[0]);
          }
          if(sigTfsZTau.size()>0) {
            std::cout << "Fill hitstos of interest " << std::endl;
            h2PartonSigTfZcutTau->Fill(tfsplit, sigTfsZTau[0]);
          }
        }
        */


        
        // for(int j = 0; j<sigZgs.size(); ++j) {
        //   std::cout << "sigPt: " << sigPt << " j: " << j << "  zg: " << sigZgs[j] << j << "  Rg: " << sigRgs[j] << " tf: " << sigTfs[j] << " nSD: " << sigJetRecurZcut_nSD->at(i) << std::endl;
        // }
        
      }

   }

   TFile *fout;
   if(reduced == true ){
    fout = new TFile("Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_"+generator+"_"+duration+".root","RECREATE");
    //fout = new TFile("Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_"+generator+"_"+duration+".root","RECREATE");
   }else{
    fout = new TFile("Johanna/Jewel/JetToyHITimeClusHistosJewel_simple.root","RECREATE");
    //fout = new TFile("Johanna/Jewel/JetToyHITimeClusHistosJewel_simple.root","RECREATE");
  }

  //add new histos to write !
   std::cout << "write content" << std::endl;
   h1LogTfSig->Write();
   h1LogTfSigTau->Write();
   h1LogTfSigZ->Write();
   h1LogTfSigZTau->Write();
   
   h2SigTfTauTf->Write();
   h2SigTfTauTfZcut->Write();
   h2SigTfTauTfZcutSoft->Write(); 
   h2SigTfDgTf->Write();
   h2SigTfDgTfSoft->Write();
   h1SigNCaTf->Write();
   h1SigNCaTfZcut->Write();
   h1SigNTauTf->Write();
   h1SigNTauTfZcut->Write();
   h1SigNTauTfZcutSoft->Write();
   h1SigNTfDg->Write();
   h1SigNTauTfDg->Write();

   h2SigRgTauRgZcut->Write();

   h1JPtW->Write();
   h1JPt->Write();
   h1JsigTauPtW->Write();
   h1JsigTauPt->Write();
   h1JsigCAPtW->Write();
   h1JsigCAPt->Write();
   h1JsigDyPtW->Write();
   h1JsigDyPt->Write();

   h2Kappa->Write();
   h1DroppedBr->Write();
   h1DroppedBrTau->Write();
   h2Nsubj21->Write();
   h2Nsubj32->Write();
   h2TauNsubj21->Write();
   h2TauNsubj21Soft->Write();

   h2CaTauNsubj21->Write();
   h2CaTauNsubj32->Write();
   
   //Jet substructure // needs plottting tools
   for (int nsplit = 0; nsplit < Nsplit; nsplit++){
    //fill 0 with 1st split, 1 with 2nd and 3 with all.
    h1TfSigTau[nsplit]->Write();
    h1TfSigTauSoft[nsplit]->Write();
    h1TfSigCA[nsplit]->Write();
    h1TfSigDy[nsplit]->Write();
    h1ZgSigCA[nsplit]->Write();
    h1ZgSigTau[nsplit]->Write();
    h1ZgSigDg[nsplit]->Write();
    h1ZgSigDgTau[nsplit]->Write();
    h1RgCASig[nsplit]->Write();
    h1RgTauSig[nsplit]->Write();
    h1RgDgSig[nsplit]->Write();
    h1RgDgTauSig[nsplit]->Write();
    h1EradSigCA[nsplit]->Write();
    h1EradSigTau[nsplit]->Write();
    h1LogDr12SigCA[nsplit]->Write();
    h1LogDr12SigTau[nsplit]->Write();
    h1LogZthetaSigCA[nsplit]->Write();
    h1LogZthetaSigTau[nsplit]->Write();
    
   }
   //fSplitList->Write();
   fout->Write();
   fout->Close();

}
