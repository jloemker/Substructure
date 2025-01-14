// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

// \author
// Alice Caluisi   -   alice.caluisi@cern.ch
// \since September 2023

//
// Task performing jet reclustering and producing primary Lund Plane histograms
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "fastjet/contrib/LundGenerator.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/AxesDefinition.hh"


#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/ASoA.h"
#include "Framework/runDataProcessing.h"
#include "Framework/HistogramRegistry.h"
#include "TDatabasePDG.h"

#include "Common/Core/TrackSelection.h"
#include "Common/Core/TrackSelectionDefaults.h"
#include "Common/DataModel/EventSelection.h"
#include "Common/DataModel/TrackSelectionTables.h"

#include "Common/Core/RecoDecay.h"

#include "PWGJE/DataModel/Jet.h"
#include "PWGJE/Core/JetFinder.h"
#include "PWGJE/DataModel/JetSubstructure.h"
#include "PWGJE/Core/FastJetUtilities.h"
#include "PWGJE/Core/JetDerivedDataUtilities.h"

#include "PWGJE/Core/JetUtilities.h"
#include "PWGJE/Core/JetSubstructureUtilities.h"

using namespace o2;
using namespace o2::track;
using namespace o2::framework;
using namespace o2::framework::expressions;
using namespace std;

struct JetLundReclustering {

  HistogramRegistry registry;

  std::vector<fastjet::PseudoJet> jetConstituents;
  std::vector<fastjet::PseudoJet> jetReclustered;
  std::vector<float> nSub;
  std::vector<float> tfJet;
  JetFinder jetReclusterer;

  Configurable<std::string> eventSelections{"eventSelections", "sel8", "choose event selection"};
  Configurable<double> Nsub{"Nsub", 2.0, "set N for (min) Nsubjettiness"};
  Configurable<double> Zcut{"Zcut", 0.1, "set z_{SD} for Nsubjettiness"};
  Configurable<double> Beta{"Beta", 0.0, "set #beta for Nsubjettiness"};
  Configurable<double> Rgcut{"Rgcut", 0.1, "set the R for upper (2*R/3) and lower (R/3) topoligical cut at which the formation time calculation transitions between soft and hard"};
  Configurable<int> Tf_limit{"Tf_limit", 3, "choose the 'general: 3', 'soft: 2' or 'hard: 1' limit for the 1st formation time at soft substrucutre values"};

  Configurable<float> jetPtMin{"jetPtMin", 5.0, "minimum jet pT cut"};
  Configurable<float> jetR{"jetR", 0.4, "jet resolution parameter"};
  Configurable<float> jet_min_eta{"jet_min_eta", -0.5, "minimum jet eta"};
  Configurable<float> jet_max_eta{"jet_max_eta", 0.5, "maximum jet eta"};
  Configurable<double> genKTp{"genKTp", 0., "select p value for generalized kT alogrithm"};//cannot be specified yet..

  Configurable<float> vertexZCut{"vertexZCut", 10.0f, "Accepted z-vertex range"};
  Configurable<int> Nsplits{"Nsplits", 3, "choose for how many splits you want to extend the histogram registry"};
  //Binning Configurable<std::vector<double>>
  ConfigurableAxis ptBinning{"pt-binning", {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4,4.4,4.8,5.2,5.6,6,6.5,7,7.5,8,9,10,11,12,13,14,15,16,17,18,19,21,23,25,27,30,34,38,42,48,56,62,70,80,90,100,110,120,130,145,160,180,200}, "pt-bins"};
  ConfigurableAxis phiBinning{"phi-binning", {90, 0., 2 * M_PI}, "phi-bins"};
  ConfigurableAxis etaBinning{"eta-binning", {90, -0.9, 0.9}, "eta-bins"};
  ConfigurableAxis lnkTBinning{"lnkt-binning", {10, -10, 10}, "lnkt-bins"};
  ConfigurableAxis deltaRBinning{"Delta-R-binning", {50, 0.0, 0.5}, "deltaR-binning"};
  ConfigurableAxis nSubRatioBinning{"NSub-Ratio-binning", {50, 0.0f, 1.2f}, "nSubRatio-binning"};
  ConfigurableAxis zgBinning{"zg-binning", {50, 0, 1.2}, "zg-bins"};
  ConfigurableAxis eradBinning{"erad-binning", {500, 0, 250}, "erad-bins"};
  ConfigurableAxis lnDeltaRBinning{"lnDeltaRBinning-binning", {50, -5, 10}, "lnDeltaR-bins"};
  ConfigurableAxis lnzThetaBinning{"lnzTheta-binning", {50, -10, 10}, "lnzTheta-bins"};
  ConfigurableAxis tfBinning{"tf-binning", {100, 0, 22}, "tf-bins"};
  ConfigurableAxis lnTfBinning{"lnTf-binning", {100, -5, 10}, "lnTf-bins"};


  AxisSpec ptAxis = {ptBinning, "#it{p}_{T} [GeV/#it{c}]"};
  AxisSpec phiAxis = {phiBinning, "#phi [rad]"};
  AxisSpec etaAxis = {etaBinning, "#eta"};
  AxisSpec lnkTAxis = {lnkTBinning, "log(k_{T})"};
  AxisSpec deltaRAxis = {deltaRBinning, "#Delta R"};
  AxisSpec nSubRatioAxis = {nSubRatioBinning, "#tau_{2}/#tau_{1}"};
  AxisSpec zgAxis = {zgBinning, "z_{g}"};
  AxisSpec eradAxis = {eradBinning, "E_{rad} [GeV]"};
  AxisSpec lnDeltaRAxis = {lnDeltaRBinning, "log(1/#Delta R)"};
  AxisSpec lnzThetaAxis = {lnzThetaBinning, "log(z_{g} #Delta R)"};
  AxisSpec tfAxis = {tfBinning, "#tau_{form}"};
  AxisSpec tfAxisSum = {tfBinning, "#Sigma #tau_{form}"};
  AxisSpec lnTfAxis = {lnTfBinning, "ln(#tau_{form})"};

  int eventSelection = -1;
  void init(InitContext const&)
  {
    eventSelection = jetderiveddatautilities::initialiseEventSelection(static_cast<std::string>(eventSelections));
    jetReclusterer.isReclustering = true;
    jetReclusterer.jetR = jetR;
    jetReclusterer.fastjetExtraParam = genKTp;// in jetfinder we use p = -1 for anti kt jetfinding, and then we do time recl. with p=0.5, kt p =1, ca p=0
    jetReclusterer.algorithm = fastjet::JetAlgorithm::genkt_algorithm;// gen kt is enum 3 in jetfiner setup

    // Jet QA
    registry.add("h3PtEtaPhi_JClus", "Correlation of clustered jet #it{p}_{T}, #eta and #phi; p_{T,jet} [GeV/#it{c}];#eta_{jet};#phi_{jet} [rad]", HistType::kTH3F, {ptAxis, etaAxis, phiAxis});
    registry.add("h3PtEtaPhi_JRecl", "Correlation of lund plane (#tau_{form} veto ) reclustered jet #it{p}_{T}, #eta and #phi; p_{T,jet} [GeV/#it{c}];#eta_{jet};#phi_{jet} [rad]", HistType::kTH3F, {ptAxis, etaAxis, phiAxis});
    registry.add("h3PtEtaPhi_JReject", "Correlation of rejected (substructure veto ) jet #it{p}_{T}, #eta and #phi; p_{T,jet} [GeV/#it{c}];#eta_{jet};#phi_{jet} [rad]", HistType::kTH3F, {ptAxis, etaAxis, phiAxis});
    registry.add("h3PtEtaPhi_JRejectTau", "Correlation of rejected (#tau_{form} veto ) jet #it{p}_{T}, #eta and #phi; p_{T,jet} [GeV/#it{c}];#eta_{jet};#phi_{jet} [rad]", HistType::kTH3F, {ptAxis, etaAxis, phiAxis});
    registry.add("h3PtEtaPhi_JReclSub2", "Correlation of (1st iteration) reclustered (groomed) jet #it{p}_{T}, #eta and #phi; p_{T,jet} [GeV/#it{c}];#eta_{jet};#phi_{jet} [rad]", HistType::kTH3F, {ptAxis, etaAxis, phiAxis});
    
    registry.add("h3Veto", " 1 hadronization cut 1st split, 2 hadroization cut x-th split, 3 jet formation time cut", HistType::kTH3F, {{4, 0, 1}, {4, 0, 1}, {4, 0, 1}});

    // Jet substructure - nSub0 = deltaR
    registry.add("h3Nsubj2Ratio", "Subjettiness Ratio correlation to p_{T} and R_{g}; #tau{2}/#tau_{1}; p_{T,jet} [GeV/#it{c}]; #Delta R",HistType::kTH3F, {nSubRatioAxis, ptAxis, deltaRAxis});
    registry.add("h3Nsubj2", "nSub2; nSub[0]; nSub[1]; nSub[2]", HistType::kTH3F, {deltaRAxis, {50, 0, 1.2}, {50, 0, 1.2}});
    registry.add("THnSubjRatioJetFormationtime", "Correlation of Nsubjettiness and formation time (sum)", HistType::kTHnSparseD, {nSubRatioAxis, ptAxis, deltaRAxis, tfAxis, tfAxisSum, tfAxisSum, tfAxisSum, tfAxisSum});

    registry.add("Th4ZgRgTF", "Correlation of N-th split between Z_{g}, R_{g}, formation times (hard, soft); Z_{g}; R_{g}; #tau_{form, hard}; #tau_{form, soft}", HistType::kTHnSparseD, {zgAxis, deltaRAxis,tfAxis,tfAxis});
    registry.add("h3ZgRgDiffTF", "Correlation of N-th split between Z_{g}, R_{g}, formation time differences (hard-soft); Z_{g}; R_{g}; #Delta #tau_{form}", HistType::kTH3F, {zgAxis, deltaRAxis,tfAxis});
    // fill 0 with ALL; 1 with 1st; 2 with 2nd ... 
    for (int nsplit = 0; nsplit < Nsplits+1; nsplit++){
      registry.add(Form("h3ZgSplit_%d",nsplit), "Momentum sharing fraction; Z_{g}", HistType::kTH3F, {zgAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3RgSplit_%d", nsplit),"Angles of the algorithm; R_{g}", HistType::kTH3F, {deltaRAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3EradSplit_%d", nsplit),"Energy of sum of the two branches; E_{rad}", HistType::kTH3F, {eradAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3LogDr12Split_%d", nsplit),"Log(1/angle) in the algorithm; Log(1/dr_{12})", HistType::kTH3F, {lnDeltaRAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3LogZthetaSplit_%d", nsplit),"Log(z*angle) in the algorithm; Log(z*dr_{12})", HistType::kTH3F, {lnzThetaAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3TfSplit_%d", nsplit),"Formation time of the split; #tau_{form}", HistType::kTH3F, {tfAxis, tfAxis, tfAxisSum});
      registry.add(Form("h3LogTfSplit_%d", nsplit),"Log(#tau_{form}); Log(#tau_{form})", HistType::kTH3F, {lnTfAxis, tfAxis, tfAxisSum});
      // Correlations
      registry.add(Form("Th4ZgRgSplit_%d", nsplit), "Correlation of N-th split between Z_{g}, R_{g}, formation time and the sum of formation times; Z_{g}; R_{g}; #tau_{form}; #Sigma #tau_{form}", HistType::kTHnSparseD, {zgAxis, deltaRAxis,tfAxis, tfAxisSum});
      // Fromation time and their vetos
      registry.add(Form("Th5TfTfcutSplit_%d", nsplit), "Correlation of N-th split between the current formation time values and formation time cut values", HistType::kTHnSparseD, {tfAxis, tfAxis, tfAxisSum, tfAxis, ptAxis});
      // Lund Plane 
      registry.add(Form("PrimaryLundPlane_kT_%d", nsplit), "Primary Lund 3D plane;ln(R/Delta);ln(k_{t}/GeV); p_{T,jet} [GeV/#it{c}]", {HistType::kTH3F, {{100, 0, 10}, {100, -10, 10}, ptAxis}});
      registry.add(Form("PrimaryLundPlane_z_%d", nsplit), "Primary Lund 3D plane;ln(R/Delta);ln(1/z); p_{T,jet} [GeV/#it{c}]", {HistType::kTH3F, {{100, 0, 10}, {100, 0, 10}, ptAxis}});
    }
  }

  Filter jetFilter = aod::jet::pt > jetPtMin&& aod::jet::r == nround(jetR.node() * 100.0f) && aod::jet::eta > jet_min_eta&& aod::jet::eta < jet_max_eta;
  Filter collisionFilter = nabs(aod::jcollision::posZ) < vertexZCut;

  // Reclustering function
  template <typename T>
  bool jetReclustering(T const& jet, double jetR, int Tf)
  {
    bool passTfcut = false;
    double hbarc = 0.19732697;
    double GeVtofm = 1./hbarc; //~5.068;
    jetReclustered.clear();
    tfJet.clear();
    fastjet::ClusterSequenceArea clusterSeq(jetReclusterer.findJets(jetConstituents, jetReclustered));
    jetReclustered = sorted_by_pt(jetReclustered);
    fastjet::PseudoJet pair = jetReclustered[0];
    fastjet::PseudoJet j1;
    fastjet::PseudoJet j2;
    int i = 0;
    double tfSum = 0.0;
    double kcut_jet = 0.0;
    while (pair.has_parents(j1, j2)) {
      if (j1.pt() < j2.pt()) {//following the softer branch for the kinematics
        std::swap(j1, j2);
      }
      double deltaR = j1.delta_R(j2);// Rg
      double erad = j1.e()+j2.e(); // = pair.e()
      double kt = j2.pt() * deltaR;
      double z = j2.pt() / (j1.pt() + j2.pt());
      // lund plane coordinates
      double jetRadius = static_cast<double>(jet.r()) / 100.0;
      double coord1 = std::log(jetRadius / deltaR);
      double coord2 = std::log(kt);
      double coord3 = std::log(1 / z);
      // formation times 
      double tf = 0.0;
      //maybe we need an additional pt cut (high energy limit required to make eq. 4 applicable !)
      double tf_general = GeVtofm / (2*pair.e()*z*(1-z)/(kt*kt/(jetR*jetR)));// new limit ! kt -> 1/GeV maybe: 2j1.e()*z*(z-1)*GeVtofm/(kt*kt)
      double tf_hard = 1./(2.*z*(1-z)*pair.perp()*GeVtofm*(1-cos(deltaR/jetR)));// tfe.push_back(1./(2.*z1*z2*CurrentJet.e()*GeVtofm*(1-fastjet::cos_theta(sj1,sj2))));
      double tf_soft =  2 / (z*pair.perp()*GeVtofm*(deltaR/jetR)*(deltaR/jetR));// tf.push_back(2./(zg*(1.-zg)*CurrentJet.perp()*GeVtofm*DeltaR*DeltaR/r0_/r0_)); // devide by r^2
      // kinematic vetos on formation times
      double kcut_em = GeVtofm*j2.perp();//*deltaR*deltaR/(jetR*jetR);//check values
      // setting return value and filling of histograms 
      if (i == 0 ){
        kcut_jet = GeVtofm*j1.perp();//*deltaR*deltaR/(jetR*jetR);// deltaR/R * R^2
        // choose the formation time limit used for the formation time of 1st split
        if (Tf == 1) tf = tf_hard;
        else if (Tf == 2) tf = tf_soft;
        else if (Tf == 3) tf = tf_general;
        if ((tf > kcut_jet) || (tf > kcut_em)){
          registry.fill(HIST("h3Veto"), 1, 0, 0);
          return false;
        } 
        else passTfcut = true;
        tfSum = tfSum + tf;
        if (tfSum <= kcut_jet){// only fill emissions in array if they can be real 
          tfJet.push_back(tfSum);
         }
        // fill 1st histogram with first split: (_1)
        // Lund Plane 
        registry.fill(HIST("PrimaryLundPlane_kT_1"), coord1, coord2, jet.pt());
        registry.fill(HIST("PrimaryLundPlane_z_1"), coord1, coord3, jet.pt());
        // Substructure
        registry.fill(HIST("h3ZgSplit_1"), z, tf, tfSum);
        registry.fill(HIST("h3RgSplit_1"), deltaR, tf, tfSum);
        registry.fill(HIST("h3EradSplit_1"), erad, tf, tfSum);
        registry.fill(HIST("h3LogDr12Split_1"), log(1/deltaR), tf, tfSum);
        registry.fill(HIST("h3LogZthetaSplit_1"),log(z*deltaR), tf, tfSum);
        registry.fill(HIST("h3TfSplit_1"), tf, tf, tfSum);
        registry.fill(HIST("h3LogTfSplit_1"), log(tf), tf, tfSum);
        // Correlations
        registry.fill(HIST("Th4ZgRgSplit_1"), z, deltaR, tf, tfSum);// add ome for crosscheck
        registry.fill(HIST("Th5TfTfcutSplit_1"), tf, kcut_em, tfSum, kcut_jet, jet.pt());
      }
      else{
        if (Tf == 1) tf = tf_hard;
        else if (Tf == 2) tf = tf_soft;
        else if (Tf == 3) tf = tf_general;

        if ((tf > kcut_jet) || (tf > kcut_em)){
          registry.fill(HIST("h3Veto"), 0, 1, 0);
          pair = j1;
          return passTfcut;
        } 
        else{
          tfSum = tfSum + tf;
          if (tfSum <= kcut_jet){// only fill emissions in array if they seem real 
            tfJet.push_back(tfSum);
           }
          else{
            registry.fill(HIST("h3Veto"), 0, 0, 1);
            return passTfcut;
          }
        }
        //else kcut_jet = kcut_jet - kcut_em;// hadronization time full jet - the time we lose to the emission
      }

      // fill 0-th histogram with every split: all (_0)
      // Lund Plane 
      registry.fill(HIST("PrimaryLundPlane_kT_0"), coord1, coord2, jet.pt());
      registry.fill(HIST("PrimaryLundPlane_z_0"), coord1, coord3, jet.pt());
      // Substructure
      registry.fill(HIST("h3ZgSplit_0"), z, tf, tfSum);
      registry.fill(HIST("h3RgSplit_0"), deltaR, tf, tfSum);
      registry.fill(HIST("h3EradSplit_0"), erad, tf, tfSum);
      registry.fill(HIST("h3LogDr12Split_0"), log(1/deltaR), tf, tfSum);
      registry.fill(HIST("h3LogZthetaSplit_0"),log(z*deltaR), tf, tfSum);
      registry.fill(HIST("h3TfSplit_0"), tf, tf, tfSum);
      registry.fill(HIST("h3LogTfSplit_0"), log(tf), tf, tfSum);
      // Correlations
      registry.fill(HIST("Th4ZgRgSplit_0"), z, deltaR, tf, tfSum);
      registry.fill(HIST("Th4ZgRgTF"), z, deltaR, tf_hard, tf_soft);
      registry.fill(HIST("Th5TfTfcutSplit_0"), tf, kcut_em, tfSum, kcut_jet, jet.pt());
      registry.fill(HIST("h3ZgRgDiffTF"), z, deltaR, fabs(tf_hard - tf_soft));
      std::cout<<"z: "<<z<<" | deltaR: "<<deltaR<<" | tf_hard: "<<tf_hard<<" | tf_soft z(1-z)E: "<<tf_soft<<" | tf_soft2 zE: " << tf_soft2 <<" | fabs(tf_hard - tf_soft): "<<fabs(tf_hard-tf_soft)<< " | fabs(tf_hard - tf_soft2): "<<fabs(tf_hard-tf_soft2)<<std::endl;
      if (i == 1){
        // fill 2nd histogram with 2nd emission: (_2)
        // Lund Plane 
        registry.fill(HIST("PrimaryLundPlane_kT_2"), coord1, coord2, jet.pt());
        registry.fill(HIST("PrimaryLundPlane_z_2"), coord1, coord3, jet.pt());
        // Substructure
        registry.fill(HIST("h3ZgSplit_2"), z, tf, tfSum);
        registry.fill(HIST("h3RgSplit_2"), deltaR, tf, tfSum);
        registry.fill(HIST("h3EradSplit_2"), erad, tf, tfSum);
        registry.fill(HIST("h3LogDr12Split_2"), log(1/deltaR), tf, tfSum);
        registry.fill(HIST("h3LogZthetaSplit_2"),log(z*deltaR), tf, tfSum);
        registry.fill(HIST("h3TfSplit_2"), tf, tf, tfSum);
        registry.fill(HIST("h3LogTfSplit_2"), log(tf), tf, tfSum);
        // Correlations
        registry.fill(HIST("Th4ZgRgSplit_2"), z, deltaR, tf, tfSum);
        registry.fill(HIST("Th5TfTfcutSplit_2"), tf, kcut_em, tfSum, kcut_jet, jet.pt());
      }
      if (i == 2){
        // fill 3rd histogram with 3rd emission: (_3)
        // Lund Plane 
        registry.fill(HIST("PrimaryLundPlane_kT_3"), coord1, coord2, jet.pt());
        registry.fill(HIST("PrimaryLundPlane_z_3"), coord1, coord3, jet.pt());
        // Substructure
        registry.fill(HIST("h3ZgSplit_3"), z, tf, tfSum);
        registry.fill(HIST("h3RgSplit_3"), deltaR, tf, tfSum);
        registry.fill(HIST("h3EradSplit_3"), erad, tf, tfSum);
        registry.fill(HIST("h3LogDr12Split_3"), log(1/deltaR), tf, tfSum);
        registry.fill(HIST("h3LogZthetaSplit_3"),log(z*deltaR), tf, tfSum);
        registry.fill(HIST("h3TfSplit_3"), tf, tf, tfSum);
        registry.fill(HIST("h3LogTfSplit_3"), log(tf), tf, tfSum);
        // Correlations
        registry.fill(HIST("Th4ZgRgSplit_3"), z, deltaR, tf, tfSum);
        registry.fill(HIST("Th5TfTfcutSplit_3"), tf, kcut_em, tfSum, kcut_jet, jet.pt());
      }
      i += 1;
      pair = j1;
    }// end of while loop
    return passTfcut;
  }

  // Process function for charged jets // make a secod function for subtractedConst,jets (PbPb)
  void processChargedJets(soa::Filtered<JetCollisions>::iterator const& collision,
                          soa::Filtered<soa::Join<aod::ChargedJets, aod::ChargedJetConstituents>> const& jets,
                          JetTracks const& tracks)
  {
    if (!jetderiveddatautilities::selectCollision(collision, eventSelection)) {
      return;
    }
    for (const auto& jet : jets) {
      registry.fill(HIST("h3PtEtaPhi_JClus"), jet.pt(), jet.eta(), jet.phi());
      jetConstituents.clear();
      for (auto& jetConstituent : jet.tracks_as<JetTracks>()) {
        fastjetutilities::fillTracks(jetConstituent, jetConstituents, jetConstituent.globalIndex());
      } //end of constituent loop 
      nSub = jetsubstructureutilities::getNSubjettiness(jet, tracks, tracks, tracks, Nsub, fastjet::contrib::CA_Axes(), true, Zcut, Beta);
      if((nSub[1] > 0) && (nSub[2]/nSub[1] >= 0) && ((nSub[2] > 0))){
        registry.fill(HIST("h3PtEtaPhi_JRecl"), jet.pt(), jet.eta(), jet.phi());
        if( (nSub[2]/nSub[1] <= 0.4) && (nSub[0] >= 2*Rgcut/3) ){//hard formation time
          if(jetReclustering(jet, jetR, 1) == true){
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
        else if( (nSub[2]/nSub[1]>= 0.6) && (nSub[0] <= Rgcut/3) ){//soft formation time (with option to switch between single and soft)
          if(jetReclustering(jet, jetR, Tf_limit) == true){
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
        else{
          if(jetReclustering(jet, jetR, Tf_limit) == true){// single as general proxy for soft emission 
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
      }
      else registry.fill(HIST("h3PtEtaPhi_JReject"), jet.pt(), jet.eta(), jet.phi());
    }// end of jet loop

  }// end of void
  PROCESS_SWITCH(JetLundReclustering, processChargedJets, "Process function for charged jets", true);

  void processEventWiseSubtractedChargedJets(soa::Filtered<JetCollisions>::iterator const& collision,
                          soa::Filtered<soa::Join<aod::ChargedEventWiseSubtractedJets, aod::ChargedEventWiseSubtractedJetConstituents>> const& jets,
                          JetTracksSub const& tracks)
  {
    if (!jetderiveddatautilities::selectCollision(collision, eventSelection)) {
      return;
    }
    for (const auto& jet : jets) {
      registry.fill(HIST("h3PtEtaPhi_JClus"), jet.pt(), jet.eta(), jet.phi());
      jetConstituents.clear();
      for (auto& jetConstituent : jet.tracks_as<JetTracksSub>()) {
        fastjetutilities::fillTracks(jetConstituent, jetConstituents, jetConstituent.globalIndex());
      } //end of constituent loop 
      nSub = jetsubstructureutilities::getNSubjettiness(jet, tracks, tracks, tracks, Nsub, fastjet::contrib::CA_Axes(), true, Zcut, Beta);
      if((nSub[1] > 0) && (nSub[2]/nSub[1] >= 0) && ((nSub[2] > 0))){
        registry.fill(HIST("h3PtEtaPhi_JRecl"), jet.pt(), jet.eta(), jet.phi());
        if( (nSub[2]/nSub[1] <= 0.4) && (nSub[0] >= 2*Rgcut/3) ){//hard formation time
          if(jetReclustering(jet, jetR, 1) == true){
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
        else if( (nSub[2]/nSub[1]>= 0.6) && (nSub[0] <= Rgcut/3) ){//soft formation time (with option to switch between single and soft)
          if(jetReclustering(jet, jetR, Tf_limit) == true){
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
        else{
          if(jetReclustering(jet, jetR, Tf_limit) == true){// single as general proxy for soft emission 
            registry.fill(HIST("h3PtEtaPhi_JReclSub2"), jet.pt(), jet.eta(), jet.phi());
            registry.fill(HIST("h3Nsubj2"), nSub[0], nSub[1], nSub[2]);
            registry.fill(HIST("h3Nsubj2Ratio"), nSub[2]/nSub[1], jet.pt(), nSub[0]);
            registry.fill(HIST("THnSubjRatioJetFormationtime"), nSub[2]/nSub[1], jet.pt(), nSub[0], tfJet[0], tfJet[1], tfJet[2], tfJet[3], tfJet[4]);
          }
          else registry.fill(HIST("h3PtEtaPhi_JRejectTau"), jet.pt(), jet.eta(), jet.phi());
        }
      }
      else registry.fill(HIST("h3PtEtaPhi_JReject"), jet.pt(), jet.eta(), jet.phi());
    }// end of jet loop

  }// end of void
  PROCESS_SWITCH(JetLundReclustering, processEventWiseSubtractedChargedJets, "Process function for eventwiese subtracted charged jets", false);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{adaptAnalysisTask<JetLundReclustering>(cfgc, TaskName{"jet-lund-reclustering"})};
}
