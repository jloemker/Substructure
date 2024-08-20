#include "plotUtils.C"

std::array<int, 2> getProjectionBins(const TAxis* axis, const double min, const double max, double epsilon = 1e-5)
{
  // Default behaviour is to include overflow and underflow bins
  int firstBin = 0, lastBin = axis->GetNbins() + 1;
  if (min > -900) { firstBin = axis->FindBin(min + epsilon); }
  if (max > -900) { lastBin = axis->FindBin(max - epsilon); }
  return std::array{firstBin, lastBin};
}

std::array<double, 2> getNorm(TH1 *VacHist, TH1 *MedHist){
  std::array<int,2> VacPtBins = getProjectionBins(VacHist->GetXaxis(), 0, 200);
  std::array<int,2> MedPtBins = getProjectionBins(MedHist->GetXaxis(), 0, 200);
  double vaclowpt = VacHist->GetXaxis()->GetBinLowEdge(VacPtBins[0]);
  double medlowpt = MedHist->GetXaxis()->GetBinLowEdge(MedPtBins[0]);
  double vachighpt = VacHist->GetXaxis()->GetBinUpEdge(VacPtBins[1]);
  double medhighpt = MedHist->GetXaxis()->GetBinUpEdge(MedPtBins[1]);

  double normPb = MedHist->Integral(MedHist->FindBin(medlowpt),MedHist->FindBin(medhighpt));
  double normPP = VacHist->Integral(VacHist->FindBin(vaclowpt),VacHist->FindBin(vachighpt));
  return std::array{normPP, normPb};
}

void plotRgs(TString strInPP, TString strInPb, TString suff){
  TFile *finPP = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());

  //Recluster info
  TH1 *h1RgSigPP = dynamic_cast<TH1*>(finPP->Get("jet-lund-reclustering/h1RgSplit_1"));// 1st split
  TH1 *h1RgSigPb = dynamic_cast<TH1*>(finPb->Get("jet-lund-reclustering/h1RgSplit_1"));

  TH3 *h3PtEtaPhiVac = dynamic_cast<TH3*>(finPP->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));// unweighted..histograms from jet-clustering
  TH3 *h3PtEtaPhiMed = dynamic_cast<TH3*>(finPb->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));//maybe better to take the weighted...

  TH1 *h1SigPtVac = h3PtEtaPhiVac->Project3D("x");
  TH1 *h1SigPtMed = h3PtEtaPhiMed->Project3D("x");

  std::array<double, 2> PtNorm = getNorm(h1SigPtVac, h1SigPtMed);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];

  h1RgSigPP->Sumw2();
  h1RgSigPP->Scale(1./normPP,"width");// norm C/A
  h1RgSigPb->Scale(1./normPb,"width");// norm C/A

  //Clone normed histograms for ratios
  TH1 *h1PgSigClone = dynamic_cast<TH1*>(h1RgSigPb->Clone("h1RgCASigClone"));
  TH1 *h1PgSigClone2 = dynamic_cast<TH1*>(h1RgSigPb->Clone("h1RgCASigClone2"));

  //Canvas for CA
  TCanvas *cRg = new TCanvas("cRgRatio"+suff,"cRgRatio"+suff,600,500);
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  pad1->cd();
  h1RgSigPP->SetStats(0);
  h1RgSigPP->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
  h1RgSigPP->GetXaxis()->SetTitle("R_{g} (SD angle)");
  h1RgSigPP->SetLineColor(kRed);
  h1RgSigPP->SetMarkerColor(kRed);
  h1RgSigPP->SetMarkerStyle(24);
  h1RgSigPP->SetTitle("");
  h1RgSigPP->Draw("E");
  h1RgSigPb->SetLineColor(kBlue);
  h1RgSigPb->SetMarkerColor(kBlue);
  h1RgSigPb->SetMarkerStyle(26);
  h1RgSigPb->Draw("Esame");

  TLegend *legRg = CreateLegend(0.25, 0.45, 0.55, 0.72);
  DrawLatex(0.23,0.75, "1st splits (LP) reclustering", 0.05);
  legRg->AddEntry(h1RgSigPP,"pp","lp");
  legRg->AddEntry(h1RgSigPb,"Pb","lp");
  legRg->Draw();

  DrawLatex(0.3, 0.45, suff , 0.05);
  
  //DrawLatex(0.5, 0.8, "JEWEL @ #sqrt{s} = ...TeV");
  //DrawLatex(0.5, 0.72, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.05);
  //DrawLatex(0.5, 0.64, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.05);
  //DrawLatex(0.5, 0.56, "SD z_{cut} = 0.1, #beta = 0", 0.05);
  //DrawLatex(0.5, 0.48, "1st split", 0.05);
  cRg->cd();

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();
  pad2->cd();
  h1PgSigClone->Divide(h1RgSigPP);
  h1PgSigClone->SetTitle("");
  h1PgSigClone->SetStats(0);
  h1PgSigClone->GetYaxis()->SetRangeUser(0, 1.2);
  h1PgSigClone->GetYaxis()->SetTitle("med/vac");
  h1PgSigClone->GetXaxis()->SetTitle("R_{g} (SD angle)"); 
  h1PgSigClone->GetXaxis()->SetLabelSize(0.09);
  h1PgSigClone->GetXaxis()->SetTitleSize(0.09);
  h1PgSigClone->GetYaxis()->SetLabelSize(0.09);
  h1PgSigClone->GetYaxis()->SetTitleSize(0.09);
  h1PgSigClone->GetYaxis()->SetTitleOffset(0.4);
  h1PgSigClone->SetLineColor(kBlack);
  h1PgSigClone->Draw("L");

  cRg->SaveAs("Rg_ratio_"+suff+".png");

}

void plotPtEtaPhi(TString strIn, TString suff){
  TFile *fin = new TFile(strIn.Data());
  std::cout<< "Plot QA info for"<<suff<<std::endl;

  TH3 *h3Clus = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));// pt, eta, phi
  TH3 *h3Recl = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));
  TH3 *h3Rej = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3PtEtaPhi_JReject"));
  TH3 *h3ReclSub = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3PtEtaPhi_JReclSub2"));

  list<TString>  proj{"xz", "yz", "xy"};  
  for (auto p : proj ){
    TH2 *h2ClusEtaPhi = h3Clus->Project3DProfile(p);
    TH2 *h2ReclEtaPhi = h3Recl->Project3DProfile(p);
    TH2 *h2RejEtaPhi = h3Rej->Project3DProfile(p);
    TH2 *h2ReclSubEtaPhi = h3ReclSub->Project3DProfile(p);
    if( !(p == "yz") ){
        h2ClusEtaPhi->GetYaxis()->SetRangeUser(0,100);
        h2ReclEtaPhi->GetYaxis()->SetRangeUser(0,100);
        h2RejEtaPhi->GetYaxis()->SetRangeUser(0,100);
        h2ReclSubEtaPhi->GetYaxis()->SetRangeUser(0,100);
    }
    TCanvas *ptCorr = new TCanvas("ptCorr_"+p,"ptCorr_"+p, 700,800);
    ptCorr->Divide(2,2);
    ptCorr->cd(1);//TH1F *rg = DrawFrame(0.,1.,0.,10.,"R_{g}","#tau_{form}");
    //gPad->SetLogz();
    h2ClusEtaPhi->SetTitle("JetClustered");
    h2ClusEtaPhi->Draw("colz same");
    DrawLatex(0.45, 0.85, suff, 0.04);
    ptCorr->cd(2);
    //gPad->SetLogz();
    h2ReclEtaPhi->SetTitle("JetReclustered (LP)");
    h2ReclEtaPhi->Draw("colz same");
    DrawLatex(0.45, 0.85, suff, 0.04);
    ptCorr->cd(3);
    //gPad->SetLogz();
    h2RejEtaPhi->SetTitle("JetRejected");
    h2RejEtaPhi->Draw("colz same");
    DrawLatex(0.45, 0.85, suff, 0.04);
    ptCorr->cd(4);
    h2ReclSubEtaPhi->SetTitle("JetReclustered (SD)");
    h2ReclSubEtaPhi->Draw("colz same");
    DrawLatex(0.45, 0.85, suff, 0.04);
    ptCorr->SaveAs("QA_Correlation_"+p+"_"+suff+"_"+".png");
  }
  //Pt
  TH1 *h1ClusPt = h3Clus->ProjectionX();
  TH1 *h1ReclPt = h3Recl->ProjectionX();
  TH1 *h1RejPt = h3Rej->ProjectionX();
  TH1 *h1ReclSubPt = h3ReclSub->ProjectionX();
  //Same legend for all qa's here 
  TLegend *legPt = CreateLegend(0.65, 0.85, 0.75, 0.9);
  legPt->SetTextSize(0.04);
  legPt->AddEntry(h1ClusPt,"Clustered Jet","l");
  legPt->AddEntry(h1ReclPt,"Reclustered Jet (LP)","l");
  legPt->AddEntry(h1RejPt,"Rejected Jet ","l");
  legPt->AddEntry(h1ReclSubPt,"Reclustered Jet (SD)","l");
  legPt->Draw();
  TCanvas *cPt = new TCanvas("cPt_"+suff,"cPt_"+suff,500,400);
  TH1F *frPt = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  gPad->SetLogy();
  h1ClusPt->SetStats(0);
  h1ClusPt->SetTitle(suff);
  h1ClusPt->GetYaxis()->SetTitle("number of entries");
  h1ClusPt->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1ClusPt->SetLineColor(kRed);
  h1ClusPt->Draw("E");
  h1ReclPt->SetLineColor(kBlue);
  h1ReclPt->Draw("Esame");
  h1RejPt->SetLineColor(kGreen);
  h1RejPt->Draw("Esame");
  h1ReclSubPt->SetLineColor(kBlack);
  h1ReclSubPt->Draw("Esame");
  legPt->Draw();
  cPt->SaveAs("QA_Jet_Pt_"+suff+".png");
  //Eta
  TH1 *h1ClusEta = h3Clus->ProjectionY();
  TH1 *h1ReclEta = h3Recl->ProjectionY();
  TH1 *h1RejEta = h3Rej->ProjectionY();
  TH1 *h1ReclSubEta = h3ReclSub->ProjectionY();
  TCanvas *cEta = new TCanvas("cEta_"+suff,"cEta_"+suff,500,400);
  TH1F *frEta = DrawFrame(0.,1.,0.,1., "#eta", "number of entries");
  //gPad->SetLogy();
  h1ClusEta->SetStats(0);
  h1ClusEta->SetTitle("");
  h1ClusEta->GetYaxis()->SetTitle("number of entries");
  h1ClusEta->GetXaxis()->SetTitle("#eta");
  h1ClusEta->SetLineColor(kRed);
  h1ClusEta->Draw("E");
  h1ReclEta->SetLineColor(kBlue);
  h1ReclEta->Draw("Esame");
  h1RejEta->SetLineColor(kGreen);
  h1RejEta->Draw("Esame");
  h1ReclSubEta->SetLineColor(kBlack);
  h1ReclSubEta->Draw("Esame");
  legPt->Draw();
  cEta->SaveAs("QA_Jet_Eta_"+suff+".png");
  //Phi
  TH1 *h1ClusPhi = h3Clus->ProjectionZ();
  TH1 *h1ReclPhi = h3Recl->ProjectionZ();
  TH1 *h1RejPhi = h3Rej->ProjectionZ();
  TH1 *h1ReclSubPhi = h3ReclSub->ProjectionZ();
  TCanvas *cPhi = new TCanvas("cPhi_"+suff,"cPhi_"+suff,500,400);
  TH1F *frPhi = DrawFrame(0.,1.,0.,1.,"#phi [rad]", "number of entries");
  //gPad->SetLogy();
  h1ClusPhi->SetStats(0);
  h1ClusPhi->SetTitle("");
  h1ClusPhi->GetYaxis()->SetTitle("number of entries");
  h1ClusPhi->GetXaxis()->SetTitle("#phi [rad]");
  h1ClusPhi->SetLineColor(kRed);
  h1ClusPhi->Draw("E");
  h1ReclPhi->SetLineColor(kBlue);
  h1ReclPhi->Draw("Esame");
  h1RejPhi->SetLineColor(kGreen);
  h1RejPhi->Draw("Esame");
  h1ReclSubPhi->SetLineColor(kBlack);
  h1ReclSubPhi->Draw("Esame");
  legPt->Draw();
  cEta->SaveAs("QA_Jet_Phi_"+suff+".png");

}

void plotTiming(TString strInH, TString strInS, TString strInT, TString suff){
  TFile *finH = new TFile(strInH.Data());
  TFile *finS = new TFile(strInS.Data());
  TFile *finT = new TFile(strInT.Data());

  std::cout<< "Plot timing info for "<<suff<<std::endl;

  TH1 *h1LogTfSigH = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogTfSplit_1"));// first splits
  TH1 *h1LogTfSigS = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogTfSplit_1"));
  TH1 *h1LogTfSigT = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogTfSplit_1"));
  TH1 *h1LogTfSigH2 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogTfSplit_2"));// second splits
  TH1 *h1LogTfSigS2 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogTfSplit_2"));
  TH1 *h1LogTfSigT2 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogTfSplit_2"));
  TH1 *h1LogTfSigH0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogTfSplit_0"));// all splits
  TH1 *h1LogTfSigS0 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogTfSplit_0"));
  TH1 *h1LogTfSigT0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogTfSplit_0"));

  TH1 *h1TfSigH = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1TfSplit_1"));
  TH1 *h1TfSigS = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1TfSplit_1"));
  TH1 *h1TfSigT = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1TfSplit_1"));
  TH1 *h1TfSigH0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1TfSplit_0"));
  TH1 *h1TfSigS0 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1TfSplit_0"));
  TH1 *h1TfSigT0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1TfSplit_0"));
  TH1 *h1TfSigH2 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1TfSplit_2"));
  TH1 *h1TfSigS2 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1TfSplit_2"));
  TH1 *h1TfSigT2 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1TfSplit_2"));
  
  TCanvas *logTf = new TCanvas("logTf"+suff,"logTf"+suff,1500,400);
  logTf->Divide(3,1);
  logTf->cd(1);
  TH1F *fr3 = DrawFrame(-10.,10.,-10.,10.,"(1st) split log(#tau_{form})","number of entries");
  gPad->SetLogy();
  h1LogTfSigH->SetStats(0);
  h1LogTfSigH->SetTitle("");
  h1LogTfSigH->SetLineColor(kBlue);
  h1LogTfSigH->GetYaxis()->SetTitle("number of entries");
  h1LogTfSigH->GetXaxis()->SetTitle("(1st) split log(#tau_{form})");
  h1LogTfSigH->Draw("E");
  h1LogTfSigS->SetLineColor(kMagenta);
  h1LogTfSigS->Draw("Esame");
  h1LogTfSigT->SetLineColor(kBlack);
  h1LogTfSigT->Draw("Esame");
  logTf->cd(2);
  TH1F *fr32 = DrawFrame(-10.,10.,-10.,10.,"(2nd) split log(#tau_{form})","number of entries");
  gPad->SetLogy();
  h1LogTfSigH2->SetStats(0);
  h1LogTfSigH2->SetTitle("");
  h1LogTfSigH2->SetLineColor(kBlue);
  h1LogTfSigH2->GetYaxis()->SetTitle("number of entries");
  h1LogTfSigH2->GetXaxis()->SetTitle("(2nd) split log(#tau_{form})");
  h1LogTfSigH2->Draw("E");
  h1LogTfSigS2->SetLineColor(kMagenta);
  h1LogTfSigS2->Draw("Esame");
  h1LogTfSigT2->SetLineColor(kBlack);
  h1LogTfSigT2->Draw("Esame");
  logTf->cd(3);
  TH1F *fr30 = DrawFrame(-10.,10.,-10.,10.,"all splits log(#tau_{form})","number of entries");
  gPad->SetLogy();
  h1LogTfSigH0->SetStats(0);
  h1LogTfSigH0->SetTitle("");
  h1LogTfSigH0->SetLineColor(kBlue);
  h1LogTfSigH0->GetYaxis()->SetTitle("number of entries");
  h1LogTfSigH0->GetXaxis()->SetTitle("all splits log(#tau_{form})");
  h1LogTfSigH0->Draw("E");
  h1LogTfSigS0->SetLineColor(kMagenta);
  h1LogTfSigS0->Draw("Esame");
  h1LogTfSigT0->SetLineColor(kBlack);
  h1LogTfSigT0->Draw("Esame");

  TLegend *leg1 = CreateLegend(0.67, 0.85, 0.78, 0.92);
  leg1->AddEntry(h1LogTfSigH,"hard","l");
  leg1->AddEntry(h1LogTfSigS,"soft","l");
  leg1->AddEntry(h1LogTfSigT,"transition","l");
  leg1->Draw();

  DrawLatex(0.3, 0.8, suff, 0.06);

  //DrawLatex(0.23, 0.9, "JEWEL @ #sqrt{s} = ...TeV", 0.04);
  //DrawLatex(0.23, 0.85, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.03);
  //DrawLatex(0.23, 0.8, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.03);
  //DrawLatex(0.23, 0.75, "SD z_{cut} = 0.1, #beta = 0", 0.03);
  logTf->SaveAs("LogTfs_"+suff+".png");

  TCanvas *tf = new TCanvas("tf"+suff,"tf"+suff,1500,400);
  tf->Divide(3,1);
  tf->cd(1);
  TH1F *frtf = DrawFrame(-10.,10.,-10.,10.,"(1st) split #tau_{form})","number of entries");
  gPad->SetLogy();
  h1TfSigH->SetStats(0);
  h1TfSigH->SetTitle("");
  h1TfSigH->SetLineColor(kBlue);
  h1TfSigH->GetYaxis()->SetTitle("number of entries");
  h1TfSigH->GetXaxis()->SetTitle("(1st) split #tau_{form}");
  h1TfSigH->Draw("E");
  h1TfSigS->SetLineColor(kMagenta);
  h1TfSigS->Draw("Esame");
  h1TfSigT->SetLineColor(kBlack);
  h1TfSigT->Draw("Esame");
  tf->cd(2);
  TH1F *frtf2 = DrawFrame(-10.,10.,-10.,10.,"(2nd) split #tau_{form})","number of entries");
  gPad->SetLogy();
  h1TfSigH2->SetStats(0);
  h1TfSigH2->SetTitle("");
  h1TfSigH2->SetLineColor(kBlue);
  h1TfSigH2->GetYaxis()->SetTitle("number of entries");
  h1TfSigH2->GetXaxis()->SetTitle("(2nd) split #tau_{form}");
  h1TfSigH2->Draw("E");
  h1TfSigS2->SetLineColor(kMagenta);
  h1TfSigS2->Draw("Esame");
  h1TfSigT2->SetLineColor(kBlack);
  h1TfSigT2->Draw("Esame");
  tf->cd(3);
  TH1F *frtf0 = DrawFrame(-10.,10.,-10.,10.,"all splits #tau_{form})","number of entries");
  gPad->SetLogy();
  h1TfSigH0->SetStats(0);
  h1TfSigH0->SetTitle("");
  h1TfSigH0->SetLineColor(kBlue);
  h1TfSigH0->GetYaxis()->SetTitle("number of entries");
  h1TfSigH0->GetXaxis()->SetTitle("all splits #tau_{form}");
  h1TfSigH0->Draw("E");
  h1TfSigS0->SetLineColor(kMagenta);
  h1TfSigS0->Draw("Esame");
  h1TfSigT0->SetLineColor(kBlack);
  h1TfSigT0->Draw("Esame");

  TLegend *leg2tf = CreateLegend(0.67, 0.85, 0.78, 0.92);
  leg2tf->AddEntry(h1TfSigH,"hard","l");
  leg2tf->AddEntry(h1TfSigS,"soft","l");
  leg2tf->AddEntry(h1TfSigT,"transition","l");
  leg2tf->Draw();

  DrawLatex(0.3, 0.8, suff, 0.06);

  tf->SaveAs("Tfs_"+suff+".png");

}

void doRaaRanges(TString strInPP, TString strInPb, TString strInPPShort, TString strInPbShort, TString strInPPLarge, TString strInPbLarge, 
                 TString strInPPIntermed, TString strInPbIntermed, TString recluster){

  TFile *finp = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  TFile *finpS = new TFile(strInPPShort.Data());
  TFile *finPbS = new TFile(strInPbShort.Data());
  TFile *finpL = new TFile(strInPPLarge.Data());
  TFile *finPbL = new TFile(strInPbLarge.Data());
  TFile *finpI = new TFile(strInPPIntermed.Data());
  TFile *finPbI = new TFile(strInPbIntermed.Data());
  // this should be done with weighted pt spectra.. but i dont have wights (yet)
  //all
  TH3 *h3PtEtaPhiVac = dynamic_cast<TH3*>(finp->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));// unweighted..histograms with tau veto LP
  TH3 *h3PtEtaPhiMed = dynamic_cast<TH3*>(finPb->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));//maybe better to take the weighted..
  TH1 *p = h3PtEtaPhiVac->Project3D("x");
  TH1 *Pb = h3PtEtaPhiMed->Project3D("x");
  std::array<double, 2> PtNorm = getNorm(p, Pb);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];
  p->Scale(1/normPP, "width");
  Pb->Scale(1/normPb, "width");
  TH1* h1SigPtWp = (TH1*)p->Clone();
  TH1* h1SigPtWPb = (TH1*)Pb->Clone();


  //large 
  TH3 *h3PtEtaPhiVacL = dynamic_cast<TH3*>(finpL->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));// unweighted..histograms with tau veto LP
  TH3 *h3PtEtaPhiMedL = dynamic_cast<TH3*>(finPbL->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));//maybe better to take the weighted..
  TH1 *Lp = h3PtEtaPhiVacL->Project3D("x");
  TH1 *LPb = h3PtEtaPhiMedL->Project3D("x");
  std::array<double, 2> LNorm = getNorm(Lp, LPb);
  Lp->Scale(1/LNorm[0], "width");
  LPb->Scale(1/LNorm[1], "width");
  TH1* h1SigPtWpL = (TH1*)Lp->Clone();
  TH1* h1SigPtWPbL = (TH1*)LPb->Clone();

  //short
  TH3 *h3PtEtaPhiVacS = dynamic_cast<TH3*>(finpS->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));// unweighted..histograms with tau veto LP
  TH3 *h3PtEtaPhiMedS = dynamic_cast<TH3*>(finPbS->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));//maybe better to take the weighted..
  TH1 *Sp = h3PtEtaPhiVacS->Project3D("x");
  TH1 *SPb = h3PtEtaPhiMedS->Project3D("x");
  std::array<double, 2> SNorm = getNorm(Sp, SPb);
  Sp->Scale(1/SNorm[0], "width");
  SPb->Scale(1/SNorm[1], "width");
  TH1* h1SigPtWpS = (TH1*)Sp->Clone();
  TH1* h1SigPtWPbS = (TH1*)SPb->Clone();

  //intermed
  TH3 *h3PtEtaPhiVacI = dynamic_cast<TH3*>(finpI->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));
  TH3 *h3PtEtaPhiMedI = dynamic_cast<TH3*>(finPbI->Get("jet-lund-reclustering/h3PtEtaPhi_JRecl"));
  TH1 *Ip = h3PtEtaPhiVacI->Project3D("x");
  TH1 *IPb = h3PtEtaPhiMedI->Project3D("x");
  std::array<double, 2> INorm = getNorm(Ip, IPb);
  Ip->Scale(1/INorm[0], "width");
  IPb->Scale(1/INorm[1], "width");
  TH1* h1SigPtWpI = (TH1*)Ip->Clone();
  TH1* h1SigPtWPbI = (TH1*)IPb->Clone();
  //short vs large formation time
  TCanvas *cPt = new TCanvas(Form("cPt_%d",0),Form("cPt_%d",0),600,500);
  cPt->cd();
  TH1F *frPt = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  //gPad->SetLogy();
  h1SigPtWPbL->Sumw2();
  h1SigPtWPbL->SetDirectory(0);
  h1SigPtWPbL->SetStats(0);
  h1SigPtWPbL->SetTitle(" ");
  h1SigPtWPbL->GetYaxis()->SetTitle("PbPb recl. #frac{1}{N_{jets, PbPb}}/pp recl. #frac{1}{N_{jets, pp}}");
  h1SigPtWPbL->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtWPbL->Divide(h1SigPtWpL);
  h1SigPtWPbL->SetLineColor(kBlack);
  h1SigPtWPbL->SetLineStyle(1);
  h1SigPtWPbL->SetLineWidth(2);  
  h1SigPtWPbL->GetYaxis()->SetRangeUser(0, 2);
  h1SigPtWPbL->GetXaxis()->SetRangeUser(0, 80);
  h1SigPtWPbL->Draw("HistL");
  h1SigPtWPbS->Divide(h1SigPtWpS);
  h1SigPtWPbS->SetLineColor(kBlue);
  h1SigPtWPbS->SetLineStyle(4);
  h1SigPtWPbS->SetLineWidth(2); 
  h1SigPtWPbS->Draw("HistLsame");
  h1SigPtWPbI->Divide(h1SigPtWpI);
  h1SigPtWPbI->SetLineColor(kGreen+2);
  h1SigPtWPbI->SetLineStyle(4);
  h1SigPtWPbI->SetLineWidth(2); 
  h1SigPtWPbI->Draw("HistLsame");
  h1SigPtWPb->Divide(h1SigPtWp);
  h1SigPtWPb->SetLineColor(kRed);
  h1SigPtWPb->SetLineStyle(4);
  h1SigPtWPb->SetLineWidth(2); 
  h1SigPtWPb->Draw("HistLsame");

  TLegend *leg1 = CreateLegend(0.25, 0.85, 0.73, 0.95);
  leg1->SetTextSize(0.03);
  leg1->SetNColumns(2);
  leg1->AddEntry(h1SigPtWPbL,"#tau > 3 (LP)","l");
  leg1->AddEntry(h1SigPtWPbS,"#tau < 1 (LP) ","l");
  leg1->AddEntry(h1SigPtWPb,"incl. #tau < 20 (LP)","l");
  leg1->AddEntry(h1SigPtWPbI,"1 < #tau < 3 (LP)","l");
  leg1->Draw();

  cPt->SaveAs("RaaRanges_"+recluster+".png");
 
  TCanvas *cPtI = new TCanvas(Form("cPtI_%d",0),Form("cPtI_%d",0),600,500);
  TH1F *frPtI = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  //gPad->SetLogy();
  p->Sumw2();
  p->SetStats(0);
  p->SetTitle("");
  p->GetYaxis()->SetTitle("normierte spectra");
  p->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
 // h1SigPtWPb->Divide(h1SigPtWp);
  p->SetLineColor(kBlack);
  p->SetLineStyle(1);
  p->SetLineWidth(2);  
  //h1SigPtWPb->GetYaxis()->SetRangeUser(0., 1.5);
  //h1SigPtWPb->GetXaxis()->SetRangeUser(110, 400);
  p->DrawCopy("HistL");
  Pb->SetLineColor(kRed);
  Pb->SetLineStyle(4);
  Pb->SetLineWidth(2); 
  Pb->DrawCopy("HistLsame");

  TLegend *leg1I = CreateLegend(0.25, 0.85, 0.7, 0.95);
  leg1I->SetTextSize(0.03);
  leg1I->SetNColumns(3);
  leg1I->AddEntry(Pb,"red PbPb","l");
  leg1I->AddEntry(p,"black pp","l");
  
  leg1I->Draw();

  cPtI->SaveAs("Raa_checkPPvsPbSpectra"+recluster+".png");
/*
  TCanvas *cPt1 = new TCanvas(Form("cPt1_%d",0),Form("cPt1_%d",0),600,500);
  TH1F *frPt1 = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  h1SigPtWPb->SetStats(0);
  h1SigPtWPb->SetTitle("");
  h1SigPtWPb->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1SigPtWPb->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtWPb->GetYaxis()->SetRangeUser(-0.2,4);
  h1SigPtWPb->Draw("L");
  cPt1->SaveAs("Raa_Sig.png");


  TCanvas *cPt2 = new TCanvas(Form("cPt2_%d",0),Form("cPt2_%d",0),600,500);

  TH1F *frPt2 = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  h1JsigTauPtWPb->SetStats(0);
  h1JsigTauPtWPb->SetTitle("");
  h1JsigTauPtWPb->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1JsigTauPtWPb->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigTauPtWPb->GetYaxis()->SetRangeUser(-0.2,4);
  h1JsigTauPtWPb->Draw("L");
  cPt2->SaveAs("Raa_Tau.png");

  TCanvas *cPt3 = new TCanvas(Form("cPt3_%d",0),Form("cPt3_%d",0),600,500);

  TH1F *frPt3 = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  h1JsigCAPtWPb->SetStats(0);
  h1JsigCAPtWPb->SetTitle("");
  h1JsigCAPtWPb->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1JsigCAPtWPb->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigCAPtWPb->GetYaxis()->SetRangeUser(-0.2,4);
  h1JsigCAPtWPb->Draw("L");
  cPt3->SaveAs("Raa_CA.png");
*/
}

void compareSplits(TString strInPT, TString strInPH, TString strInPS, TString suff){
  TFile *finT = new TFile(strInPT.Data());
  TFile *finH = new TFile(strInPH.Data());
  TFile *finS = new TFile(strInPS.Data());
  std::cout<< "Plot timing info for"<<suff<<std::endl;
   
  //check correlation of zg and rg for 1st, 2nd splits 
  TH2 *h2Rg21H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2RgTfSplit_1"));//  R_{g}; #tau_{form}
  TH2 *h2Rg21S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2RgTfSplit_1"));//  R_{g}; #tau_{form}
  TH2 *h2Rg21T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2RgTfSplit_1"));//  R_{g}; #tau_{form}

  TH2 *h2Rg2H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2RgTfSplit_2"));//  R_{g}; #tau_{form}
  TH2 *h2Rg2S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2RgTfSplit_2"));//  R_{g}; #tau_{form}
  TH2 *h2Rg2T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2RgTfSplit_2"));//  R_{g}; #tau_{form}
  TCanvas *r1 = new TCanvas("r1","r1",1000,600);
  r1->Divide(3,2);
  r1->cd(1);//TH1F *rg = DrawFrame(0.,1.,0.,10.,"R_{g}","#tau_{form}");
  gPad->SetLogz();
  h2Rg21H->SetTitle("(1st split) hard limit");
  h2Rg21H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  r1->cd(2);
  gPad->SetLogz();
  h2Rg21S->SetTitle("(1st split) soft limit");
  h2Rg21S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  r1->cd(3);
  gPad->SetLogz();
  h2Rg21T->SetTitle("(1st split) transition of limit");
  h2Rg21T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  r1->cd(4);
  gPad->SetLogz();
  h2Rg2H->SetTitle("(2nd split) hard limit");
  h2Rg2H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  r1->cd(5);
  gPad->SetLogz();
  h2Rg2S->SetTitle("(2nd split) soft limit");
  h2Rg2S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  r1->cd(6);
  gPad->SetLogz();
  h2Rg2T->SetTitle("(2nd split) transition of limit");
  h2Rg2T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);

  r1->cd();
  r1->SaveAs("Correlation1st2ndSplit_RgTf_"+suff+".png");

  TH2 *h2Zg21H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2ZgTfSplit_1"));//  R_{g}; #tau_{form}
  TH2 *h2Zg21S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2ZgTfSplit_1"));//  R_{g}; #tau_{form}
  TH2 *h2Zg21T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2ZgTfSplit_1"));//  R_{g}; #tau_{form}

  TH2 *h2Zg2H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2ZgTfSplit_2"));//  R_{g}; #tau_{form}
  TH2 *h2Zg2S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2ZgTfSplit_2"));//  R_{g}; #tau_{form}
  TH2 *h2Zg2T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2ZgTfSplit_2"));//  R_{g}; #tau_{form}
  TCanvas *z1 = new TCanvas("z1","z1",1000,600);
  z1->Divide(3,2);
  z1->cd(1);//TH1F *rg = DrawFrame(0.,1.,0.,10.,"R_{g}","#tau_{form}");
  gPad->SetLogz();
  h2Zg21H->SetTitle("(1st split) hard limit");
  h2Zg21H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->cd(2);
  gPad->SetLogz();
  h2Zg21S->SetTitle("(1st split) soft limit");
  h2Zg21S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->cd(3);
  gPad->SetLogz();
  h2Zg21T->SetTitle("(1st split) transition of limit");
  h2Zg21T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->cd(4);
  gPad->SetLogz();
  h2Zg2H->SetTitle("(2nd split) hard limit");
  h2Zg2H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->cd(5);
  gPad->SetLogz();
  h2Zg2S->SetTitle("(2nd split) soft limit");
  h2Zg2S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->cd(6);
  gPad->SetLogz();
  h2Zg2T->SetTitle("(2nd split) transition of limit");
  h2Zg2T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  z1->SaveAs("Correlation1st2ndSplit_ZgTf_"+suff+".png");

  TH2 *h2g21H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2ZgRgSplit_1"));
  TH2 *h2g21S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2ZgRgSplit_1"));
  TH2 *h2g21T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2ZgRgSplit_1"));
  TH2 *h2g2H = dynamic_cast<TH2*>(finH->Get("jet-lund-reclustering/h2ZgRgSplit_2"));
  TH2 *h2g2S = dynamic_cast<TH2*>(finS->Get("jet-lund-reclustering/h2ZgRgSplit_2"));
  TH2 *h2g2T = dynamic_cast<TH2*>(finT->Get("jet-lund-reclustering/h2ZgRgSplit_2"));
  TCanvas *g1 = new TCanvas("g1","g1",1000,600);
  g1->Divide(3,2);
  g1->cd(1);
  gPad->SetLogz();
  h2g21H->SetTitle("(1st split) hard limit");
  h2g21H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->cd(2);
  gPad->SetLogz();
  h2g21S->SetTitle("(1st split) soft limit");
  h2g21S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->cd(3);
  gPad->SetLogz();
  h2g21T->SetTitle("(1st split) transition of limit");
  h2g21T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->cd(4);
  gPad->SetLogz();
  h2g2H->SetTitle("(2nd split) hard limit");
  h2g2H->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->cd(5);
  gPad->SetLogz();
  h2g2S->SetTitle("(2nd split) soft limit");
  h2g2S->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->cd(6);
  gPad->SetLogz();
  h2g2T->SetTitle("(2nd split) transition of limit");
  h2g2T->Draw("colz same");
  DrawLatex(0.45, 0.85, suff, 0.04);
  g1->SaveAs("Correlation1st2ndSplit_ZgRg_"+suff+".png");


  TH1 *h1TfSigTau0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1TfSplit_1"));//1st split - trans
  TH1 *h1TfSigTauSoft0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1TfSplit_1"));//1st split - hard
  TH1 *h1TfSigCA0 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1TfSplit_1"));//1st split - soft
  TH1 *h1TfSigTau1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1TfSplit_2"));//2n split - trans
  TH1 *h1TfSigTauSoft1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1TfSplit_2"));//2n split -hard
  TH1 *h1TfSigCA1 = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1TfSplit_2"));//2nd split - soft
  
  TCanvas *ct = new TCanvas("tf_"+suff,"tf_"+suff,800,400);
  ct->Divide(2,1);
  ct->cd(1);
  TH1F *frctf = DrawFrame(0.,10.,0.,10., "compare 1st splits", "number of entries");
  gPad->SetLogy();
  h1TfSigTau0->SetTitle("First Splits");
  h1TfSigTau0->GetYaxis()->SetTitle("number of entries");
  h1TfSigTau0->GetXaxis()->SetTitle("#tau_{form}");
  h1TfSigTau0->SetLineColor(kBlack);
  h1TfSigTau0->Draw("E");
  h1TfSigTauSoft0->SetLineColor(kBlack);
  h1TfSigTauSoft0->SetLineStyle(2);
  h1TfSigTauSoft0->Draw("Esame");
  h1TfSigCA0->SetLineColor(kMagenta);
  h1TfSigCA0->Draw("Esame");

  TLegend *legtf = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legtf->AddEntry(h1TfSigTau0,"transition","l");
  legtf->AddEntry(h1TfSigTauSoft0,"hard","l");
  legtf->AddEntry(h1TfSigCA0,"soft","l");
  legtf->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  ct->cd(2);
  TH1F *frctf1 = DrawFrame(0.,10.,0.,10., "compare 2nd splits", "number of entries");
  gPad->SetLogy();
  h1TfSigTau1->SetTitle("Second Splits");
  h1TfSigTau1->GetYaxis()->SetTitle("number of entries");
  h1TfSigTau1->GetXaxis()->SetTitle("#tau_{form}");
  h1TfSigTau1->SetLineColor(kBlack);//Transit
  h1TfSigTau1->Draw("E");
  h1TfSigTauSoft1->SetLineColor(kBlack);//Hard
  h1TfSigTauSoft1->SetLineStyle(2);
  h1TfSigTauSoft1->Draw("Esame");
  h1TfSigCA1->SetLineColor(kMagenta);//Soft
  h1TfSigCA1->Draw("Esame");

  legtf->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  ct->SaveAs("tform_1And2ndSplits_"+suff+".png");
  
  TH1 *h1ZgSigCA0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1ZgSplit_1"));
  TH1 *h1ZgSigCA0soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1ZgSplit_1"));
  TH1 *h1ZgSigTau0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1ZgSplit_1"));
  TH1 *h1ZgSigCA1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1ZgSplit_2"));
  TH1 *h1ZgSigCA1soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1ZgSplit_2"));
  TH1 *h1ZgSigTau1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1ZgSplit_2"));

  TCanvas *cS = new TCanvas("cS_"+suff,"cS_"+suff,800,400);
  cS->Divide(2,1);
  cS->cd(1);
  TH1F *frcS = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1ZgSigCA0->SetTitle("First Splits");
  h1ZgSigCA0->GetYaxis()->SetTitle("number of entries");
  h1ZgSigCA0->GetXaxis()->SetTitle("z_{g}");
  h1ZgSigCA0->SetLineColor(kBlack);
  h1ZgSigCA0->Draw("E");
  h1ZgSigCA0soft->SetLineColor(kMagenta);
  h1ZgSigCA0soft->Draw("Esame");
  h1ZgSigTau0->SetLineColor(kGreen);
  h1ZgSigTau0->SetLineStyle(2);
  h1ZgSigTau0->Draw("Esame");

  TLegend *legS = CreateLegend(0.43, 0.78, 0.75, 0.93);
  legS->AddEntry(h1ZgSigTau0,"transition","l");
  legS->AddEntry(h1ZgSigCA0,"hard","l");
  legS->AddEntry(h1ZgSigCA0soft,"soft","l");
  legS->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cS->cd(2);
  TH1F *frcS1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1ZgSigCA1->SetTitle("Second Splits");
  h1ZgSigCA1->GetYaxis()->SetTitle("number of entries");
  h1ZgSigCA1->GetXaxis()->SetTitle("z_{g}");
  h1ZgSigCA1->SetLineColor(kBlack);
  h1ZgSigCA1->Draw("E");
  h1ZgSigCA1soft->SetLineColor(kMagenta);
  h1ZgSigCA1soft->Draw("Esame");
  h1ZgSigTau1->SetLineColor(kGreen);
  h1ZgSigTau1->SetLineStyle(2);
  h1ZgSigTau1->Draw("Esame");
  legS->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cS->SaveAs("Zg_1And2ndSplits_"+suff+".png");

  TH1 *h1RgCASig0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1RgSplit_1"));
  TH1 *h1RgCASig0soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1RgSplit_1"));
  TH1 *h1RgTauSig0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1RgSplit_1"));
  TH1 *h1RgCASig1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1RgSplit_2"));
  TH1 *h1RgCASig1soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1RgSplit_2"));
  TH1 *h1RgTauSig1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1RgSplit_2"));
  
  TCanvas *cSR = new TCanvas("cSR_"+suff,"cSR_"+suff,800,400);
  cSR->Divide(2,1);
  cSR->cd(1);
  TH1F *frcSR = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1RgCASig0->SetTitle("First Splits");
  h1RgCASig0->GetYaxis()->SetTitle("number of entries");
  h1RgCASig0->GetXaxis()->SetTitle("R_{g}");
  h1RgCASig0->SetLineColor(kBlack);
  h1RgCASig0->Draw("E");
  h1RgCASig0soft->SetLineColor(kMagenta);
  h1RgCASig0soft->Draw("Esame");
  h1RgTauSig0->SetLineColor(kGreen);
  h1RgTauSig0->SetLineStyle(2);
  h1RgTauSig0->Draw("Esame");

  TLegend *legSR = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSR->AddEntry(h1RgTauSig0,"transition","l");
  legSR->AddEntry(h1RgCASig0,"hard","l");
  legSR->AddEntry(h1RgCASig0soft,"soft","l");
  legSR->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSR->cd(2);
  TH1F *frcSR1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1RgCASig1->SetTitle("Second Splits");
  h1RgCASig1->GetYaxis()->SetTitle("number of entries");
  h1RgCASig1->GetXaxis()->SetTitle("R_{g}");
  h1RgCASig1->SetLineColor(kBlack);
  h1RgCASig1->Draw("E");
  h1RgCASig1soft->SetLineColor(kMagenta);
  h1RgCASig1soft->Draw("Esame");
  h1RgTauSig1->SetLineColor(kGreen);
  h1RgTauSig1->SetLineStyle(2);
  h1RgTauSig1->Draw("Esame");

  legSR->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSR->SaveAs("Rg_1And2ndSplits_"+suff+".png");

  //energy of sum of the two branches at each node
  TH1 *h1EradSigCA0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1EradSplit_1"));
  TH1 *h1EradSigCA0soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1EradSplit_1"));
  TH1 *h1EradSigTau0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1EradSplit_1"));
  TH1 *h1EradSigCA1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1EradSplit_2"));
  TH1 *h1EradSigCA1soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1EradSplit_2"));
  TH1 *h1EradSigTau1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1EradSplit_2"));

  TCanvas *cSE = new TCanvas("cSE_"+suff,"cSE_"+suff,800,400);
  cSE->Divide(2,1);
  cSE->cd(1);
  TH1F *frcSE = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1EradSigCA0->SetTitle("First Splits");
  h1EradSigCA0->GetYaxis()->SetTitle("number of entries");
  h1EradSigCA0->GetXaxis()->SetTitle("E_{rad} (sum of energy of two branches)");
  h1EradSigCA0->SetLineColor(kBlack);
  h1EradSigCA0->Draw("E");
  h1EradSigCA0soft->SetLineColor(kMagenta);
  h1EradSigCA0soft->Draw("Esame");
  h1EradSigTau0->SetLineColor(kGreen);
  h1EradSigTau0->SetLineStyle(2);
  h1EradSigTau0->Draw("Esame");

  TLegend *legSE = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSE->AddEntry(h1EradSigTau0,"transition","l");
  legSE->AddEntry(h1EradSigCA0,"hard","l");
  legSE->AddEntry(h1EradSigCA0soft,"soft","l");
  legSE->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSE->cd(2);
  TH1F *frcSE1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1EradSigCA1->SetTitle("Second Splits");
  h1EradSigCA1->GetYaxis()->SetTitle("number of entries");
  h1EradSigCA1->GetXaxis()->SetTitle("E_{rad} (sum of energy of two branches)");
  h1EradSigCA1->SetLineColor(kBlack);
  h1EradSigCA1->Draw("E");
  h1EradSigCA1soft->SetLineColor(kMagenta);
  h1EradSigCA1soft->Draw("Esame");
  h1EradSigTau1->SetLineColor(kGreen);
  h1EradSigTau1->SetLineStyle(2);
  h1EradSigTau1->Draw("Esame");

  legSE->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSE->SaveAs("EnergySum_1And2ndSplits_"+suff+".png");

  //Log(1/angle) in the algorithm
  TH1 *h1LogDr12SigCA0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogDr12Split_1"));
  TH1 *h1LogDr12SigCA0soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogDr12Split_1"));
  TH1 *h1LogDr12SigTau0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogDr12Split_1"));
  TH1 *h1LogDr12SigCA1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogDr12Split_2"));
  TH1 *h1LogDr12SigCA1soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogDr12Split_2"));
  TH1 *h1LogDr12SigTau1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogDr12Split_2"));

  TCanvas *cSL = new TCanvas("cSL_"+suff,"cSL_"+suff,800,400);
  cSL->Divide(2,1);
  cSL->cd(1);
  TH1F *frcSEL0 = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1LogDr12SigCA0->SetTitle("First Splits");
  h1LogDr12SigCA0->GetYaxis()->SetTitle("number of entries");
  h1LogDr12SigCA0->GetXaxis()->SetTitle("Log(1/angle)");
  h1LogDr12SigCA0->SetLineColor(kBlack);
  h1LogDr12SigCA0->Draw("E");
  h1LogDr12SigCA0soft->SetLineColor(kMagenta);
  h1LogDr12SigCA0soft->Draw("Esame");
  h1LogDr12SigTau0->SetLineColor(kGreen);
  h1LogDr12SigTau0->SetLineStyle(2);
  h1LogDr12SigTau0->Draw("Esame");

  TLegend *legSL = CreateLegend(0.23, 0.48, 0.2, 0.4);
  legSL->SetTextSize(0.03);
  legSL->AddEntry(h1LogDr12SigTau0,"transition","l");
  legSL->AddEntry(h1LogDr12SigCA0,"hard","l");
  legSL->AddEntry(h1LogDr12SigCA0soft,"soft","l");
  legSL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSL->cd(2);
  TH1F *frcSE1L = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1LogDr12SigCA1->SetTitle("Second Splits");
  h1LogDr12SigCA1->GetYaxis()->SetTitle("number of entries");
  h1LogDr12SigCA1->GetXaxis()->SetTitle("Log(1/angle)");
  h1LogDr12SigCA1->SetLineColor(kBlack);
  h1LogDr12SigCA1->Draw("E");
  h1LogDr12SigCA1soft->SetLineColor(kMagenta);
  h1LogDr12SigCA1soft->Draw("Esame");
  h1LogDr12SigTau1->SetLineColor(kGreen);
  h1LogDr12SigTau1->SetLineStyle(2);
  h1LogDr12SigTau1->Draw("Esame");
  legSL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSL->SaveAs("Log1overAngle_1And2ndSplits_"+suff+".png");

  // Log(z*angle) in the algorithm
  TH1 *h1LogZthetaSigCA0 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogZthetaSplit_1"));
  TH1 *h1LogZthetaSigCA0soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogZthetaSplit_1"));
  TH1 *h1LogZthetaSigTau0 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogZthetaSplit_1"));
  TH1 *h1LogZthetaSigCA1 = dynamic_cast<TH1*>(finH->Get("jet-lund-reclustering/h1LogZthetaSplit_2"));
  TH1 *h1LogZthetaSigCA1soft = dynamic_cast<TH1*>(finS->Get("jet-lund-reclustering/h1LogZthetaSplit_2"));
  TH1 *h1LogZthetaSigTau1 = dynamic_cast<TH1*>(finT->Get("jet-lund-reclustering/h1LogZthetaSplit_2"));

  TCanvas *cSEL = new TCanvas("cSEL_"+suff,"cSEL_"+suff,800,400);
  cSEL->Divide(2,1);
  cSEL->cd(1);
  TH1F *frcSEL = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1LogZthetaSigCA0->SetTitle("First Splits");
  h1LogZthetaSigCA0->GetYaxis()->SetTitle("number of entries");
  h1LogZthetaSigCA0->GetXaxis()->SetTitle("Log(z*angle)");
  h1LogZthetaSigCA0->SetLineColor(kBlack);
  h1LogZthetaSigCA0->Draw("E");
  h1LogZthetaSigCA0soft->SetLineColor(kMagenta);
  h1LogZthetaSigCA0soft->Draw("Esame");
  h1LogZthetaSigTau0->SetLineColor(kGreen);
  h1LogZthetaSigTau0->SetLineStyle(2);
  h1LogZthetaSigTau0->Draw("Esame");

  TLegend *legSEL = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSEL->AddEntry(h1LogZthetaSigTau0,"transition","l");
  legSEL->AddEntry(h1LogZthetaSigCA0,"hard","l");
  legSEL->AddEntry(h1LogZthetaSigCA0soft,"soft","l");
  legSEL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSEL->cd(2);
  TH1F *frcSEL1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1LogZthetaSigCA1->SetTitle("Second Splits");
  h1LogZthetaSigCA1->GetYaxis()->SetTitle("number of entries");
  h1LogZthetaSigCA1->GetXaxis()->SetTitle("Log(z*angle)");
  h1LogZthetaSigCA1->SetLineColor(kBlack);
  h1LogZthetaSigCA1->Draw("E");
  h1LogZthetaSigCA1soft->SetLineColor(kMagenta);
  h1LogZthetaSigCA1soft->Draw("Esame");
  h1LogZthetaSigTau1->SetLineColor(kGreen);
  h1LogZthetaSigTau1->SetLineStyle(2);
  h1LogZthetaSigTau1->Draw("Esame");

  legSEL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSEL->SaveAs("LogTimesZfrac_1And2ndSplits_"+suff+".png");
  
  
}

void plotNsubj( TString strIn, TString suff){ // add these sigJetRecurZcut_tau21, sigJetRecurZcut_tau32, sigJetRecurTauZcut_tau21, sigJetRecurTauZcut_tau32
  TFile *fin = new TFile(strIn.Data());
  /*
    // Jet substructure - nSub0 = deltaR
    registry.add("h3Nsubj2Ratio", "Subjettiness Ratio correlation to p_{T} and R_{g}; #tau{2}/#tau_{1}; p_{T,jet} [GeV/#it{c}]; #Delta R", {HistType::kTH3F, {{50, 0, 1}, {200, 0, 200}, {25, 0, 0.5}}});
    registry.add("h3Nsubj2", "nSub2; nSub[0]; nSub[1]; nSub[2]", {HistType::kTH3F, {{50, 0, 1}, {50, 0, 1}, {50, 0, 1}}});

*/
  std::cout<< "Plot subjettiness info for "<<suff<<std::endl;
  TH3 *h3Nsubj = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3Nsubj2"));// deltaR, tau1, tau2
  TH3 *h3NsubjRatio = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3Nsubj2Ratio"));// deltaR, tau2/tau1, pT
  h3Nsubj->Sumw2();
  TH2 *h2NsubjRatio21XYMan0 = new TH2F("h2NsubjRatio21XYMan", "h2NsubjRatio21XYMan",50, 0.,1, 50, 0, 1);
  for (int i = 1; i<h3Nsubj->GetNbinsX()+1; i ++){
    h3Nsubj->GetXaxis()->SetRange(i,i);
    for (int j = 1; j<h3Nsubj->GetNbinsY()+1; j++){
      for (int k = 1; k<h3Nsubj->GetNbinsY()+1; k++){
      double Rg = h3Nsubj->GetXaxis()->GetBinCenter(i);
      TH1 *projY = h3Nsubj->Project3D("y");
      TH1 *projZ = h3Nsubj->Project3D("z");
      TH2 *proj = h3Nsubj->Project3DProfile("yz");
      if( (projZ->GetBinContent(j) >= 0) && (projY->GetBinContent(j)) >= 0){
        if( (projZ->GetBinCenter(j)/projY->GetBinCenter(k)) <= 1){//else values up to 4
          double tauRatioWeight = proj->GetBinContent(j,k);///projY->GetBinContent(k);
          double tauRatioVal = projZ->GetBinCenter(j)/projY->GetBinCenter(k);
          if ((tauRatioWeight < 1000 ) && (tauRatioWeight >= 0)) h2NsubjRatio21XYMan0->Fill(Rg, tauRatioVal, tauRatioWeight);
          else std::cout<<"bin: "<<i<<" Rg: "<<Rg<<" tauRatioWeight: "<<tauRatioWeight<<" projZ->GetBinCenter(j) "<<projZ->GetBinCenter(j)<<" projY->GetBinCenter(k) "<<projY->GetBinCenter(k)<<std::endl;
        }
      }

      }
    }
    //For historical reasons, SetRange(0,0) resets the range even though bin 0 is technically reserved for the underflow; 
    //in order to set the range of the axis so that it only includes the underflow, use SetRange(-1,0). 
    h3Nsubj->GetXaxis()->SetRange(0,0);
  }
  
  TH2 *h2Nsubj21YZ = h3Nsubj->Project3DProfile("yz");
  TH2 *h2Nsubj21XZ = h3Nsubj->Project3DProfile("xz");
  TH2 *h2Nsubj21XY = h3Nsubj->Project3DProfile("xy");
  TH2 *h2NsubjRatio21XYMan = h3Nsubj->Project3DProfile("xy");// need to loop over TH3 to fill histo with delta r and tau2/tau1 !
  TH2 *h2NsubjRatio21XYMan1 = h3Nsubj->Project3DProfile("xz");

  TH2 *h2NsubjRatio21XY = h3NsubjRatio->Project3DProfile("xz");

  TCanvas *c2Nsubj21 = new TCanvas("c2Nsubj21_"+suff, "c2Nsubj21_"+suff,1500,800);
  c2Nsubj21->Divide(3,2);
  c2Nsubj21->cd(1);
  TH1F *frYZ = DrawFrame(0.,1.,0.,1.,"#tau_{1}","#tau_{2}");
  //gPad->SetLogz();
  h2Nsubj21YZ->Draw("colz same");
  c2Nsubj21->cd(2);
  TH1F *frXZ = DrawFrame(0.,1.,0.,1.,"#Delta R", "#tau_{2}");
  //gPad->SetLogz();
  h2Nsubj21XZ->Draw("colz same");
  c2Nsubj21->cd(3);
  TH1F *frXY = DrawFrame(0.,1.,0.,1.,"#Delta R","#tau_{1}");
  //gPad->SetLogz();
  h2Nsubj21XY->Draw("colz same");
  c2Nsubj21->cd(4);
  TH1F *frXY2 = DrawFrame(0.,1.,0.,1.,"#Delta R","(o2Ratio) #tau_{2}/#tau_{1}");
  //gPad->SetLogz();
  h2NsubjRatio21XY->Draw("colz same");
  c2Nsubj21->cd(5);
  TH1F *frXY2Man = DrawFrame(0.,1.,0.,1.,"#Delta R","(plotting) #tau_{2}/#tau_{1}");
  //gPad->SetLogz();
  //h2NsubjRatio21XYMan->Divide(h2NsubjRatio21XYMan1);
  h2NsubjRatio21XYMan0->Draw("colz same");

  TH1 *projO2 = h2NsubjRatio21XYMan0->ProjectionY();
  TH1 *projPlot = h2NsubjRatio21XY->ProjectionY();
  c2Nsubj21->cd(6);
  TH1F *frProj = DrawFrame(0., 1.1, 0., 90.,"#tau_{2}/#tau_{1}", "number of entries");
  projPlot->SetStats(0);
  projPlot->SetTitle(" ");
  projPlot->SetLineColor(kRed);
  projPlot->Draw("E");
  projO2->SetLineColor(kBlue);
  projO2->Draw("Esame");

  TLegend *legPr = CreateLegend(0.52, 0.89, 0.67, 0.92);
  legPr->AddEntry(projPlot, "o2physics","lp");
  legPr->AddEntry(projO2, "plotting","lp");
  legPr->Draw();
  c2Nsubj21->SaveAs("NsubjRTau1Tau2_"+suff+".png");

}

void plotNsubjInRanges(TString strInPP, TString strInPb, TString strInPPS, TString strInPbS, TString strInPPI, TString strInPbI, TString strInPPL, TString strInPbL, TString dy){
  TFile *finPP = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  TH3 *h3PtEtaPhiVac = dynamic_cast<TH3*>(finPP->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));// unweighted..histograms from jet-clustering
  TH3 *h3PtEtaPhiMed = dynamic_cast<TH3*>(finPb->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));//maybe better to take the weighted...

  TH1 *h1SigPtVac = h3PtEtaPhiVac->Project3D("x");
  TH1 *h1SigPtMed = h3PtEtaPhiMed->Project3D("x");

  std::array<double, 2> PtNorm = getNorm(h1SigPtVac, h1SigPtMed);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];

  list<TString>  ifiles{strInPP, strInPPS, strInPPI, strInPPL, strInPb, strInPbS, strInPbI, strInPbL};  
  TString ilegend[8] = {"vac. incl.", "vac. #tau < 1", "vac. 1 < #tau < 3", "vac. #tau > 3", "med. incl.", "med. #tau < 1", "med. 1 < #tau < 3", "med. #tau > 3"};  
  
  TCanvas *projNsubj = new TCanvas("projNsubj","projNsubj",900,400);
  projNsubj->Divide(2);
  TPad *padTau1 = new TPad("padTau1", "padTau1", 0, 0, 1, 1);
  padTau1->SetLeftMargin(0.18);
  TPad *padTau2 = new TPad("padTau2", "padTau2", 0, 0, 1, 1);
  padTau2->SetLeftMargin(0.18);

  int idx = 0;
  TLegend *leg = CreateLegend(0.52, 0.89, 0.67, 0.92);
  TLegend *leg2 = CreateLegend(0.52, 0.89, 0.67, 0.92);
  leg->SetTextSize(0.035);
  leg2->SetTextSize(0.035);
  leg->SetNColumns(2);
  leg2->SetNColumns(2);

/*
    // Jet substructure - nSub0 = deltaR
    registry.add("h3Nsubj2Ratio", "Subjettiness Ratio correlation to p_{T} and R_{g}; #tau{2}/#tau_{1}; p_{T,jet} [GeV/#it{c}]; #Delta R", {HistType::kTH3F, {{50, 0, 1}, {200, 0, 200}, {25, 0, 0.5}}});
    registry.add("h3Nsubj2", "nSub2; nSub[0]; nSub[1]; nSub[2]", {HistType::kTH3F, {{50, 0, 1}, {50, 0, 1}, {50, 0, 1}}});

*/

  for(auto i : ifiles){
    std::cout<< i <<std::endl;
    TFile *fin = new TFile(i.Data());
    TH3 *h2Nsubj21 = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3Nsubj2Ratio"));// x =  #tau{2}/#tau_{1}; y = p_{T,jet} [GeV/#it{c}]; z = #Delta R
    TH1 *tau21 = h2Nsubj21->Project3D("x");
    tau21->SetLineWidth(2);
    projNsubj->cd(1);
    if (idx < 1){
      tau21->Scale(1./normPP,"width");
      padTau1->Draw();
      padTau1->cd();
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#tau_{2}/#tau_{1})}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#tau_{2}/#tau_{1}");
      tau21->DrawCopy("g");
      DrawLatex(0.6,0.95, "vac "+dy, 0.06);
      leg->AddEntry(tau21, ilegend[idx],"lp");

    }
    else if (idx == 4){//PbPb
      tau21->Scale(1./normPb,"width");
      projNsubj->cd(2);
      padTau2->Draw();
      padTau2->cd();
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx-3);
      tau21->GetYaxis()->SetTitle("number of entries");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#tau_{2}/#tau_{1}");
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#tau_{2}/#tau_{1})}");
      tau21->DrawCopy("g");
      DrawLatex(0.6,0.95, "med "+dy, 0.06);
      leg2->AddEntry(tau21, ilegend[idx],"lp");
    }
    else if (idx > 4){
      tau21->Scale(1./normPb,"width");
      projNsubj->cd(2);
      padTau2->cd();
      tau21->SetLineColor(idx-3);
      tau21->DrawCopy("hsame");
      leg2->AddEntry(tau21, ilegend[idx],"lp");
    }
    else{
      padTau1->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetLineColor(idx+1);
      tau21->DrawCopy("hsame");
      leg->AddEntry(tau21, ilegend[idx],"lp");
    }
    idx = idx + 1;
  }
  padTau1->cd();
  leg->Draw();
  padTau2->cd();
  leg2->Draw();
  projNsubj->SaveAs("projFullNsubj21_"+dy+".png");
  
  // subjettiness deltaR
  TCanvas *projDeltaR = new TCanvas("projDeltaR","projDeltaR",900,400);
  projDeltaR->Divide(2);
  TPad *padTau132 = new TPad("padTau132", "padTau132", 0, 0, 1, 1);
  padTau132->SetLeftMargin(0.18);
  TPad *padTau232 = new TPad("padTau232", "padTau232", 0, 0, 1, 1);
  padTau232->SetLeftMargin(0.18);

  int idx32 = 0;
  TLegend *leg32 = CreateLegend(0.25, 0.5, 0.4, 0.78);
  TLegend *leg232 = CreateLegend(0.25, 0.5, 0.4, 0.78);
  leg32->SetTextSize(0.035);
  leg232->SetTextSize(0.035);

  for(auto i : ifiles){
    TFile *fin = new TFile(i.Data());
    TH3 *h2Nsubj21 = dynamic_cast<TH3*>(fin->Get("jet-lund-reclustering/h3Nsubj2Ratio"));// x =  #tau{2}/#tau_{1}; y = p_{T,jet} [GeV/#it{c}]; z = #Delta R
    TH1 *tau21 = h2Nsubj21->Project3D("z");
    tau21->SetLineWidth(2);
    projDeltaR->cd(1);
    if (idx32 < 1){
      padTau132->Draw();
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#Delta R)}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#Delta R");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "vac "+dy, 0.06);
      leg32->AddEntry(tau21, ilegend[idx32],"lp");

    }
    else if (idx32 == 4){//PbPb
      projDeltaR->cd(2);
      padTau232->Draw();
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32-3);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#Delta R)}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#Delta R");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "med "+dy, 0.06);
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else if (idx32 > 4){
      projDeltaR->cd(2);
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetLineColor(idx32-3);
      tau21->DrawCopy("hsame");
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else{
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetLineColor(idx32+1);
      tau21->DrawCopy("hsame");
      leg32->AddEntry(tau21, ilegend[idx32],"lp");
    }
    idx32 = idx32 + 1;
  }
  padTau132->cd();
  leg32->Draw();
  padTau232->cd();
  leg232->Draw();
  projDeltaR->SaveAs("projFullNsubjDeltaR_"+dy+".png");
}

void plotRgZgInRanges(TString strInPP, TString strInPb, TString strInPPS, TString strInPbS, TString strInPPI, TString strInPbI, TString strInPPL, TString strInPbL, TString dy){
  TFile *finPP = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  TH3 *h3PtEtaPhiVac = dynamic_cast<TH3*>(finPP->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));// unweighted..histograms from jet-clustering
  TH3 *h3PtEtaPhiMed = dynamic_cast<TH3*>(finPb->Get("jet-lund-reclustering/h3PtEtaPhi_JClus"));//maybe better to take the weighted...
  TH1 *h1SigPtVac = h3PtEtaPhiVac->Project3D("x");
  TH1 *h1SigPtMed = h3PtEtaPhiMed->Project3D("x");
  std::array<double, 2> PtNorm = getNorm(h1SigPtVac, h1SigPtMed);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];

  list<TString>  ifiles{strInPP, strInPPS, strInPPI, strInPPL, strInPb, strInPbS, strInPbI, strInPbL};  
  TString ilegend[8] = {"vac. incl.", "vac. #tau < 1", "vac. 1 < #tau < 3", "vac. #tau > 3", "med. incl.", "med. #tau < 1", "med. 1 < #tau < 3", "med. #tau > 3"};  
  
  //Rg
  TCanvas *projRg = new TCanvas("projRg","projRg",900,400);
  projRg->Divide(2);
  TPad *padTau1 = new TPad("padTau1", "padTau1", 0, 0, 1, 1);
  padTau1->SetLeftMargin(0.18);
  TPad *padTau2 = new TPad("padTau2", "padTau2", 0, 0, 1, 1);
  padTau2->SetLeftMargin(0.18);

  int idx = 0;
  TLegend *leg = CreateLegend(0.52, 0.89, 0.67, 0.92);
  TLegend *leg2 = CreateLegend(0.52, 0.89, 0.67, 0.92);
  leg->SetTextSize(0.035);
  leg2->SetTextSize(0.035);
  leg->SetNColumns(2);
  leg2->SetNColumns(2);

  for(auto i : ifiles){
    std::cout<<i<<std::endl;
    TFile *fin = new TFile(i.Data());
    i.ReplaceAll(".root","");
    i.ReplaceAll("/","_");
    std::cout<<" new i: "<<i<<std::endl;
    TH2 *h2Rg21 = dynamic_cast<TH2*>(fin->Get("jet-lund-reclustering/h2RgTfSplit_0"));//  R_{g}; #tau_{form}
    
    TH1 *tau21 = h2Rg21->ProjectionX();
    tau21->SetLineWidth(2);
    projRg->cd(1);
    if (idx < 1){
      tau21->Scale(1./normPP,"width");
      padTau1->Draw();
      padTau1->cd();
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->DrawCopy("g");
      DrawLatex(0.6,0.95, "pp "+dy, 0.06);
      leg->AddEntry(tau21, ilegend[idx],"lp");

    }
    else if (idx == 4){//PbPb
      tau21->Scale(1./normPb,"width");
      projRg->cd(2);
      padTau2->Draw();
      padTau2->cd();
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx-3);
      tau21->GetYaxis()->SetTitle("number of entries");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
      tau21->DrawCopy("g");
      DrawLatex(0.6,0.95, "PbPb "+dy, 0.06);
      leg2->AddEntry(tau21, ilegend[idx],"lp");
    }
    else if (idx > 4){
      tau21->Scale(1./normPb,"width");
      projRg->cd(2);
      padTau2->cd();
      tau21->SetLineColor(idx-3);
      tau21->DrawCopy("hsame");
      leg2->AddEntry(tau21, ilegend[idx],"lp");
    }
    else{
      padTau1->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetLineColor(idx+1);
      tau21->DrawCopy("hsame");
      leg->AddEntry(tau21, ilegend[idx],"lp");
    }
    idx = idx + 1;
  }
  padTau1->cd();
  leg->Draw();
  padTau2->cd();
  leg2->Draw();
  projRg->SaveAs("projFullRg_"+dy+".png");
  
  // Zg
  TCanvas *projZg = new TCanvas("projZg","projZg",900,400);
  projZg->Divide(2);
  TPad *padTau132 = new TPad("padTau132", "padTau132", 0, 0, 1, 1);
  padTau132->SetLeftMargin(0.18);
  TPad *padTau232 = new TPad("padTau232", "padTau232", 0, 0, 1, 1);
  padTau232->SetLeftMargin(0.18);

  int idx32 = 0;
  TLegend *leg32 = CreateLegend(0.25, 0.5, 0.4, 0.78);
  TLegend *leg232 = CreateLegend(0.25, 0.5, 0.4, 0.78);
  leg32->SetTextSize(0.035);
  leg232->SetTextSize(0.035);

  for(auto i : ifiles){
    TFile *fin = new TFile(i.Data());
    i.ReplaceAll(".root","");
    i.ReplaceAll("/","_");
    if (i.Contains("PP")){
        if(i.Contains("short")){
          i = "pp_short";
        }else if (i.Contains("large"))
        {
          i = "pp_large";
        }else if (i.Contains("intermed"))
        {
          i = "pp_intermed";
        }
        else i = "pp_incl";
    }else if (i.Contains("Pb"))
    {
        if(i.Contains("short")){
          i = "PbPb_short";
        }else if (i.Contains("large"))
        {
          i = "PbPb_large";
        }else if (i.Contains("intermed"))
        {
          i = "PbPb_intermed";
        }
        else i = "PbPb_incl";
    }
    
    TH2 *h2ZgTf = dynamic_cast<TH2*>(fin->Get("jet-lund-reclustering/h2ZgTfSplit_0"));//  Z_{g}; #tau_{form}
    TH2 *h2ZgRg = dynamic_cast<TH2*>(fin->Get("jet-lund-reclustering/h2ZgRgSplit_0"));//  Z_{g}; R_{g}
    TH2 *h2RgTf = dynamic_cast<TH2*>(fin->Get("jet-lund-reclustering/h2RgTfSplit_0"));//  R_{g}; #tau_{form}
    
    TCanvas *corrZgRgTF = new TCanvas("corrZgRgTF_"+i,"corrZgRgTF_"+i,1500,400);
    corrZgRgTF->Divide(3,1);
    corrZgRgTF->cd(1);
    TH1F *zg = DrawFrame(0.,1.,0.,10.,"z_{g}","#tau_{form}");
    gPad->SetLogz();
    h2ZgTf->Draw("colz same");
    corrZgRgTF->cd(2);
    TH1F *zg2 = DrawFrame(0.,1.,0.,1.,"z_{g}","R_{g}");
    gPad->SetLogz();
    h2ZgRg->Draw("colz same");
    corrZgRgTF->cd(3);
    TH1F *rg = DrawFrame(0.,1.,0.,10.,"R_{g}","#tau_{form}");
    gPad->SetLogz();
    h2RgTf->Draw("colz same");
    DrawLatex(0.55, 0.55, i+"  "+dy, 0.06);
    corrZgRgTF->SaveAs("corrZgRgTF"+i+"_"+dy+".png");

    TH1 *tau21 = h2ZgTf->ProjectionX();
    tau21->SetLineWidth(2);
    projZg->cd(1);
    if (idx32 < 1){
      padTau132->Draw();
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dz_{g}}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "pp "+dy, 0.06);
      leg32->AddEntry(tau21, ilegend[idx32],"lp");

    }
    else if (idx32 == 4){//PbPb
      projZg->cd(2);
      padTau232->Draw();
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32-3);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dz_{g}}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "PbPb "+dy, 0.06);
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else if (idx32 > 4){
      projZg->cd(2);
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetLineColor(idx32-3);
      tau21->DrawCopy("hsame");
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else{
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetLineColor(idx32+1);
      tau21->DrawCopy("hsame");
      leg32->AddEntry(tau21, ilegend[idx32],"lp");
    }
    idx32 = idx32 + 1;
  }
  padTau132->cd();
  leg32->Draw();
  padTau232->cd();
  leg232->Draw();
  projZg->SaveAs("projFullZg"+dy+".png");

}


//The main function
void o2plots() {

  TString strInPb = "PbPb/AnalysisResults_Pb_trans.root";
  TString strInPbS = "PbPb/AnalysisResults_Pb_trans_short.root";
  TString strInPbI = "PbPb/AnalysisResults_Pb_trans_intermed.root";
  TString strInPbL = "PbPb/AnalysisResults_Pb_trans_large.root";
  TString strInPP = "PP/AnalysisResults_PP_trans.root";
  TString strInPPS = "PP/AnalysisResults_PP_trans_short.root";
  TString strInPPI = "PP/AnalysisResults_PP_trans_intermed.root";
  TString strInPPL = "PP/AnalysisResults_PP_trans_large.root";
  /*
  TString strInPb = "PbPb/AnalysisResults_Pb_1hard.root";
  TString strInPbS = "PbPb/AnalysisResults_Pb_1hard_short.root";
  TString strInPbI = "PbPb/AnalysisResults_Pb_1hard_intermed.root";
  TString strInPbL = "PbPb/AnalysisResults_Pb_1hard_large.root";
  TString strInPP = "PP/AnalysisResults_PP_1hard.root";
  TString strInPPS = "PP/AnalysisResults_PP_1hard_short.root";
  TString strInPPI = "PP/AnalysisResults_PP_1hard_intermed.root";
  TString strInPPL = "PP/AnalysisResults_PP_1hard_large.root";
  
  TString strInPb = "PbPb/AnalysisResults_Pb_2soft.root";
  TString strInPbS = "PbPb/AnalysisResults_Pb_2soft_short.root";
  TString strInPbI = "PbPb/AnalysisResults_Pb_2soft_intermed.root";
  TString strInPbL = "PbPb/AnalysisResults_Pb_2soft_large.root";
  TString strInPP = "PP/AnalysisResults_PP_2soft.root";
  TString strInPPS = "PP/AnalysisResults_PP_2soft_short.root";
  TString strInPPI = "PP/AnalysisResults_PP_2soft_intermed.root";
  TString strInPPL = "PP/AnalysisResults_PP_2soft_large.root";
  */


  
  return;
  //doRaaRanges(strInPP, strInPb, strInPPS, strInPbS, strInPPL, strInPbL, strInPPI, strInPbI, "transition");// norm and event weight !!! looks bad for now
  //plotNsubjInRanges(strInPP, strInPb, strInPPS, strInPbS, strInPPI, strInPbI, strInPPL, strInPbL, "trans"); //cross check nsub above for underflow problem !
  //plotNsubj(strInPb, "PbPb_transition");// not optima, but for sure an underflow issue ! - add statement to o2 physics code !
  // These are "okey"
  //compareSplits( "PbPb/AnalysisResults_Pb_trans.root",  "PbPb/AnalysisResults_Pb_1hard.root",  "PbPb/AnalysisResults_Pb_2soft.root", "PbPb");
  //plotRgZgInRanges(strInPP, strInPb, strInPPS, strInPbS, strInPPI, strInPbI, strInPPL, strInPbL, "trans");
  //plotPtEtaPhi(strInPP, "pp transition");
  //plotTiming("PP/AnalysisResults_PP_1hard.root", "PP/AnalysisResults_PP_2soft.root", "PP/AnalysisResults_PP_trans.root", "pp");
  //plotRgs("PP/AnalysisResults_PP_trans.root", "PbPb/AnalysisResults_Pb_trans.root", "transition");
  
  

}