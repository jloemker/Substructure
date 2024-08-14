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
  std::array<int,2> VacPtBins = getProjectionBins(VacHist->GetXaxis(), 0, 420);
  std::array<int,2> MedPtBins = getProjectionBins(MedHist->GetXaxis(), 0, 420);
  double vaclowpt = VacHist->GetXaxis()->GetBinLowEdge(VacPtBins[0]);
  double medlowpt = MedHist->GetXaxis()->GetBinLowEdge(MedPtBins[0]);
  double vachighpt = VacHist->GetXaxis()->GetBinUpEdge(VacPtBins[1]);
  double medhighpt = MedHist->GetXaxis()->GetBinUpEdge(MedPtBins[1]);

  double normPb = MedHist->Integral(MedHist->FindBin(medlowpt),MedHist->FindBin(medhighpt));
  double normPP = VacHist->Integral(VacHist->FindBin(vaclowpt),VacHist->FindBin(vachighpt));
  return std::array{normPP, normPb};
}

void plotPartonInfo(TString strIn) {
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  //Recluster info
  TH2 *h2PartonSigTf = dynamic_cast<TH2*>(fin->Get("h2PartonSigTf"));
  TH2 *h2PartonSigTfTau = dynamic_cast<TH2*>(fin->Get("h2PartonSigTfTau"));

  TH2 *h2PartonSigTfZcut = dynamic_cast<TH2*>(fin->Get("h2PartonSigTfZcut"));
  TH2 *h2PartonSigTfZcutTau = dynamic_cast<TH2*>(fin->Get("h2PartonSigTfZcutTau"));

  //correlation to parton split - no parton split info in Jewel
  TCanvas *c3 = new TCanvas(Form("c3_%d",0),Form("c3_%d",0),500,400);
  TH1F *fr3 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster C/A} (fm/c)");
  gPad->SetLogz();
  h2PartonSigTf->Draw("colz same");
  c3->SaveAs("TfPartonSigCa.png");

  TCanvas *c4 = new TCanvas(Form("c4_%d",0),Form("c4_%d",0),500,400);
  TH1F *fr4 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster #tau}  (fm/c)");
  gPad->SetLogz();
  h2PartonSigTfTau->Draw("colz same");
  c4->SaveAs("TfPartonSigTau.png");

  TCanvas *c5 = new TCanvas(Form("c5_%d",0),Form("c5_%d",0),500,400);
  TH1F *fr5 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster C/A} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2PartonSigTfZcut->Draw("colz same");
  c5->SaveAs("TfPartonSigCaZ.png");

  TCanvas *c6 = new TCanvas(Form("c6_%d",0),Form("c6_%d",0),500,400);
  TH1F *fr6 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster #tau} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2PartonSigTfZcutTau->Draw("colz same");
  c6->SaveAs("TfPartonSigTauZ.png");
}

void plotRgs(TString strInPP, TString strInPb, TString reclusterCut){
  TFile *finPP = new TFile(strInPP.Data());
  finPP->ls();
  TFile *finPb = new TFile(strInPb.Data());
  finPb->ls();

  //Recluster info
  TH1 *h1RgSigPP = dynamic_cast<TH1*>(finPP->Get("h1RgCASig[0]"));
  TH1 *h1RgTauSigPP = dynamic_cast<TH1*>(finPP->Get("h1RgTauSig[0]"));

  TH1 *h1RgSigPb = dynamic_cast<TH1*>(finPb->Get("h1RgCASig[0]"));
  TH1 *h1RgTauSigPb = dynamic_cast<TH1*>(finPb->Get("h1RgTauSig[0]"));

  TH1 *h1RgDgSigPP = dynamic_cast<TH1*>(finPP->Get("h1RgDgSig[0]"));
  TH1 *h1RgDgTauSigPP = dynamic_cast<TH1*>(finPP->Get("h1RgDgTauSig[0]"));

  TH1 *h1RgDgSigPb = dynamic_cast<TH1*>(finPb->Get("h1RgDgSig[0]"));
  TH1 *h1RgDgTauSigPb = dynamic_cast<TH1*>(finPb->Get("h1RgDgTauSig[0]"));

  //Normalize by the number of signal jets - ungroomed
  TH1 *h1SigPtVac = dynamic_cast<TH1*>(finPP->Get("h1JPt"));// unweighted..histograms from signalJetCollection
  TH1 *h1SigPtMed = dynamic_cast<TH1*>(finPb->Get("h1JPt"));//maybe better to take the weighted...

  std::array<double, 2> PtNorm = getNorm(h1SigPtVac, h1SigPtMed);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];
  //std::cout<<"Integral Vacuum: "<< h1SigPtVac->Integral(0,40)<<" vs new: "<<h1SigPtMed->Integral(h1SigPtMed->FindBin(120),h1SigPtMed->FindBin(390))<<" Integral medium: "<<h1SigPtMed->Integral(0,40)<<std::endl;
  h1RgSigPP->Sumw2();
  h1RgSigPP->Scale(1./normPP,"width");// norm C/A
  h1RgTauSigPP->Scale(1./normPP,"width");// norm tau
  h1RgSigPb->Scale(1./normPb,"width");// norm C/A
  h1RgTauSigPb->Scale(1./normPb,"width");// norm tau

  h1RgDgSigPP->Scale(1./normPP,"width");// norm C/A dyn.
  h1RgDgTauSigPP->Scale(1./normPP,"width");// norm tau dyn.
  h1RgDgSigPb->Scale(1./normPb,"width");// norm C/A dyn.
  h1RgDgTauSigPb->Scale(1./normPb,"width");// norm tau dyn.

  //Clone normed histograms for ratios
  TH1 *h1PgSigClone = dynamic_cast<TH1*>(h1RgSigPb->Clone("h1RgCASigClone"));
  TH1 *h1PgTauSigClone = dynamic_cast<TH1*>(h1RgTauSigPb->Clone("h1RgTauSigClone"));
  TH1 *h1PgSigClone2 = dynamic_cast<TH1*>(h1RgSigPb->Clone("h1RgCASigClone2"));
  TH1 *h1PgTauSigClone2 = dynamic_cast<TH1*>(h1RgTauSigPb->Clone("h1RgTauSigClone2"));

  TH1 *h1RgDgSigPbClone = dynamic_cast<TH1*>(h1RgDgSigPb->Clone("h1RgDgSigPbClone"));
  TH1 *h1RgDgTauSigPbClone = dynamic_cast<TH1*>(h1RgDgTauSigPb->Clone("h1RgDgTauSigPbClone"));
  TH1 *h1RgDgSigPbClone2 = dynamic_cast<TH1*>(h1RgDgSigPb->Clone("h1RgDgSigPbClone2"));
  TH1 *h1RgDgTauSigPbClone2 = dynamic_cast<TH1*>(h1RgDgTauSigPb->Clone("h1RgDgTauSigPbClone2"));
  //Canvas for CA
  TCanvas *cRg = new TCanvas("cRg_CA","cRg_CA",600,500);
	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
  pad1->SetBottomMargin(0);
	pad1->Draw();
  pad1->cd();
  h1RgSigPb->SetStats(0);
  h1RgSigPb->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
  h1RgSigPb->GetXaxis()->SetTitle("R_{g} (SD angle)");
  h1RgSigPb->SetLineColor(kRed);
  h1RgSigPb->SetMarkerColor(kRed);
  h1RgSigPb->SetMarkerStyle(24);
  h1RgSigPb->SetTitle("");
  h1RgSigPb->Draw("E");
  h1RgSigPP->SetLineColor(kBlue);
  h1RgSigPP->SetMarkerColor(kBlue);
  h1RgSigPP->SetMarkerStyle(26);
  h1RgSigPP->Draw("Esame");
  h1RgDgSigPP->SetLineColor(kGreen);
  h1RgDgSigPP->SetMarkerColor(kGreen);
  h1RgDgSigPP->SetMarkerStyle(24);
  h1RgDgSigPP->Draw("Esame");
  h1RgDgSigPb->SetLineColor(kGreen+2);
  h1RgDgSigPb->SetMarkerColor(kGreen+2);
  h1RgDgSigPb->SetMarkerStyle(26);
  h1RgDgSigPb->Draw("Esame");

  TLegend *legRg = CreateLegend(0.25, 0.45, 0.55, 0.72);
  DrawLatex(0.23,0.75, "C/A reclustering", 0.05);
  legRg->AddEntry(h1RgSigPP,"vacuum","lp");
  legRg->AddEntry(h1RgSigPb,"medium","lp");
  legRg->AddEntry(h1RgDgSigPP,"dyn. vacuum","lp");
  legRg->AddEntry(h1RgDgSigPb,"dyn. medium","lp");
  legRg->Draw();
  
  DrawLatex(0.5, 0.8, "JEWEL @ #sqrt{s} = ...TeV");
  DrawLatex(0.5, 0.72, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.05);
  DrawLatex(0.5, 0.64, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.05);
  DrawLatex(0.5, 0.56, "SD z_{cut} = 0.1, #beta = 0", 0.05);
  DrawLatex(0.5, 0.48, "1st split", 0.05);
  cRg->cd();

	TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
	pad2->Draw();
  pad2->cd();
  h1PgSigClone->Divide(h1RgSigPP);
  h1PgSigClone->SetTitle("");
  h1PgSigClone->SetStats(0);
  h1PgSigClone->GetYaxis()->SetTitle("med/vac");
  h1PgSigClone->GetXaxis()->SetTitle("R_{g} (SD angle)"); 
  h1PgSigClone->GetXaxis()->SetLabelSize(0.09);
  h1PgSigClone->GetXaxis()->SetTitleSize(0.09);
  h1PgSigClone->GetYaxis()->SetLabelSize(0.09);
  h1PgSigClone->GetYaxis()->SetTitleSize(0.09);
  h1PgSigClone->GetYaxis()->SetTitleOffset(0.4);
  h1PgSigClone->SetLineColor(kBlack);
  h1PgSigClone->Draw("L");
  h1RgDgSigPbClone->Divide(h1RgDgSigPP);
  h1RgDgSigPbClone->SetLineColor(kGreen);
  h1RgDgSigPbClone->Draw("Lsame");

  cRg->SaveAs("Rg_CA"+reclusterCut+".png");

  //Canvas for Tau
  TCanvas *cTRg = new TCanvas("cRg_Tau","cRg_Tau",600,500);
	TPad *padTau1 = new TPad("padTau1", "padTau1", 0, 0.3, 1, 1);
  padTau1->SetBottomMargin(0);
	padTau1->Draw();
  padTau1->cd();
  h1RgTauSigPb->SetStats(0);
  h1RgTauSigPb->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
  h1RgTauSigPb->GetXaxis()->SetTitle("R_{g} (SD angle)");
  h1RgTauSigPb->SetLineColor(kRed);
  h1RgTauSigPb->SetMarkerColor(kRed);
  h1RgTauSigPb->SetMarkerStyle(24);
  h1RgTauSigPb->SetTitle("");
  h1RgTauSigPb->Draw("E");
  h1RgTauSigPP->SetLineColor(kBlue);
  h1RgTauSigPP->SetMarkerColor(kBlue);
  h1RgTauSigPP->SetMarkerStyle(26);
  h1RgTauSigPP->Draw("Esame");
  h1RgDgTauSigPP->SetLineColor(kGreen);
  h1RgDgTauSigPP->SetMarkerColor(kGreen);
  h1RgDgTauSigPP->SetMarkerStyle(24);
  h1RgDgTauSigPP->Draw("Esame");
  h1RgDgTauSigPb->SetLineColor(kGreen+2);
  h1RgDgTauSigPb->SetMarkerColor(kGreen+2);
  h1RgDgTauSigPb->SetMarkerStyle(26);
  h1RgDgTauSigPb->Draw("Esame");

  TLegend *legTauRg = CreateLegend(0.25, 0.45, 0.55, 0.72);
  DrawLatex(0.23,0.75, "#tau reclustering", 0.05);
  legTauRg->AddEntry(h1RgTauSigPP,"vacuum","lp");
  legTauRg->AddEntry(h1RgTauSigPb,"medium","lp");
  legTauRg->AddEntry(h1RgDgTauSigPP,"dyn. vacuum","lp");
  legTauRg->AddEntry(h1RgDgTauSigPb,"dyn. medium","lp");
  legTauRg->Draw();
  
  DrawLatex(0.5, 0.8, "JEWEL @ #sqrt{s} = ...TeV");
  DrawLatex(0.5, 0.72, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.05);
  DrawLatex(0.5, 0.64, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.05);
  DrawLatex(0.5, 0.56, "SD z_{cut} = 0.1, #beta = 0", 0.05);
  DrawLatex(0.5, 0.48, "1st split", 0.05);
  cTRg->cd();

	TPad *padTau2 = new TPad("padTau2", "padTau2", 0, 0, 1, 0.3);
  padTau2->SetTopMargin(0);
  padTau2->SetBottomMargin(0.3);
	padTau2->Draw();
  padTau2->cd();
  h1PgTauSigClone->Divide(h1RgTauSigPP);
  h1PgTauSigClone->SetTitle("");
  h1PgTauSigClone->SetStats(0);
  h1PgTauSigClone->GetYaxis()->SetTitle("med/vac");
  h1PgTauSigClone->GetXaxis()->SetTitle("R_{g} (SD angle)"); 
  h1PgTauSigClone->GetXaxis()->SetLabelSize(0.09);
  h1PgTauSigClone->GetXaxis()->SetTitleSize(0.09);
  h1PgTauSigClone->GetYaxis()->SetLabelSize(0.09);
  h1PgTauSigClone->GetYaxis()->SetTitleSize(0.09);
  h1PgTauSigClone->GetYaxis()->SetTitleOffset(0.4);
  h1PgTauSigClone->SetLineColor(kBlack);
  h1PgTauSigClone->Draw("L");
  h1RgDgTauSigPbClone->Divide(h1RgDgTauSigPP);
  h1RgDgTauSigPbClone->SetLineColor(kGreen);
  h1RgDgTauSigPbClone->Draw("Lsame");

  cTRg->SaveAs("Rg_Tau"+reclusterCut+".png");

  TCanvas *r = new TCanvas("ratios","ratios",500,400);
  h1PgTauSigClone2->Divide(h1RgTauSigPP);
  h1PgTauSigClone2->SetLineColor(kBlack);
  h1PgTauSigClone2->GetYaxis()->SetTitle("med/vac");
  h1PgTauSigClone2->GetXaxis()->SetTitle("R_{g} (SD angle)"); 
  h1PgTauSigClone2->SetTitle(" ");
  h1PgTauSigClone2->Draw("L");
  h1PgSigClone2->Divide(h1RgSigPP);
  h1PgSigClone2->SetLineColor(kMagenta);
  h1PgSigClone2->Draw("Lsame");
  h1RgDgTauSigPbClone2->Divide(h1RgDgTauSigPP);
  h1RgDgTauSigPbClone2->SetLineColor(kGreen+2);
  h1RgDgTauSigPbClone2->Draw("Lsame");
  h1RgDgSigPbClone2->Divide(h1RgDgSigPP);
  h1RgDgSigPbClone2->SetLineColor(kGreen);
  h1RgDgSigPbClone2->Draw("Lsame");

  TLegend *legRatio = CreateLegend(0.15, 0.45, 0.55, 0.72);
  legRatio->SetTextSize(0.05);
  legRatio->AddEntry(h1PgSigClone2,"C/A reclustering","lp");
  legRatio->AddEntry(h1PgTauSigClone2,"#tau reclustering","lp");
  legRatio->AddEntry(h1RgDgSigPbClone2,"C/A history (dyn.)","lp");
  legRatio->AddEntry(h1RgDgTauSigPbClone2,"#tau history (dyn.)","lp");
  legRatio->Draw();

  r->SaveAs("RatiosRg"+reclusterCut+".png");

}

void doRaa(TString strInPP, TString strInPb){
  TFile *finp = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  finp->ls();
  finPb->ls();

  TH1 *h1SigPtWp = dynamic_cast<TH1*>(finp->Get("h1JPtW"));
  TH1 *h1SigPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JPtW"));

  TH1 *h1JsigTauPtWp = dynamic_cast<TH1*>(finp->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JsigTauPtW"));

  TH1 *h1JsigCAPtWp = dynamic_cast<TH1*>(finp->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JsigCAPtW"));


  TCanvas *cPt = new TCanvas(Form("cPt_%d",0),Form("cPt_%d",0),600,500);

  TH1F *frPt = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  //gPad->SetLogy();
  h1SigPtWPb->Sumw2();
  h1SigPtWPb->SetStats(0);
  h1SigPtWPb->SetTitle("");
  h1SigPtWPb->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1SigPtWPb->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtWPb->Divide(h1SigPtWp);
  h1SigPtWPb->SetLineColor(kBlack);
  h1SigPtWPb->SetMarkerStyle(25);
  h1SigPtWPb->GetYaxis()->SetRangeUser(-0.2, 2);
  h1SigPtWPb->Draw("L");
  h1JsigTauPtWPb->Divide(h1JsigTauPtWp);
  h1JsigTauPtWPb->SetLineColor(kRed);
  h1JsigTauPtWPb->SetMarkerColor(kRed);
  h1JsigTauPtWPb->SetMarkerStyle(24);
  h1JsigTauPtWPb->SetTitle("");
  h1JsigTauPtWPb->Draw("same");
  h1JsigCAPtWPb->Divide(h1JsigCAPtWp);
  h1JsigCAPtWPb->SetLineColor(kBlue);
  h1JsigCAPtWPb->SetMarkerColor(kBlue);
  h1JsigCAPtWPb->SetMarkerStyle(26);
  h1JsigCAPtWPb->Draw("same");

  TLegend *leg1 = CreateLegend(0.35, 0.8, 0.75, 0.95);
  //TLegend *leg1 = CreateLegend(0.35, 0.8, 0.15, 0.35);
  leg1->AddEntry(h1JsigTauPtWPb,"#tau-reclustering","l");
  leg1->AddEntry(h1JsigCAPtWPb,"C/A-reclustering","l");
  leg1->AddEntry(h1SigPtWPb,"Signal Jet Collection","l");
  leg1->Draw();

  cPt->SaveAs("Raa_full.png");

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

}

void plotPt(TString strIn, TString suff){
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  std::cout<< "Plot pT info for"<<suff<<std::endl;

  //Signal jet collection
  TH1 *h1SigPtW = dynamic_cast<TH1*>(fin->Get("h1JPtW"));
  TH1 *h1SigPt = dynamic_cast<TH1*>(fin->Get("h1JPt"));
  // tau vs C/A reclustring
  TH1 *h1JsigTauPtW = dynamic_cast<TH1*>(fin->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPt = dynamic_cast<TH1*>(fin->Get("h1JsigTauPt"));

  TH1 *h1JsigCAPtW = dynamic_cast<TH1*>(fin->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPt = dynamic_cast<TH1*>(fin->Get("h1JsigCAPt"));

  //signal jet collection
  TCanvas *cPt = new TCanvas(Form("cPt_%d",0),Form("cPt_%d",0),500,400);
  TH1F *frPt = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  gPad->SetLogy();
  h1SigPt->SetTitle("");
  h1SigPt->GetYaxis()->SetTitle("number of entries");
  h1SigPt->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPt->Draw("E");
  cPt->SaveAs("SigPt_"+suff+".png");

  TCanvas *cPtw = new TCanvas(Form("cPtw_%d",0),Form("cPtw_%d",0),500,400);
  TH1F *frPtw = DrawFrame(0.,1.,0.,1.,"#it{p}_{T} [GeV/#it{c}]", "(event) weighted number of entries");
  gPad->SetLogy();
  h1SigPtW->SetTitle("");
  h1SigPtW->GetYaxis()->SetTitle("#sigma (weighted number of entries)");
  h1SigPtW->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtW->Draw("E");
  cPtw->SaveAs("SigPtW_"+suff+".png", suff);
 
  //Tau
  TCanvas *cPtTau = new TCanvas(Form("cPtTau_%d",0),Form("cPtTau_%d",0),500,400);
  TH1F *frPtTau = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  gPad->SetLogy();
  h1JsigTauPt->SetTitle("");
  h1JsigTauPt->GetYaxis()->SetTitle("number of entries");
  h1JsigTauPt->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigTauPt->Draw("E");
  cPtTau->SaveAs("SigTauPt_"+suff+".png");

  TCanvas *cPtwTau = new TCanvas(Form("cPtwTau_%d",0),Form("cPtwTau_%d",0),500,400);
  TH1F *frPtwTau = DrawFrame(0.,1.,0.,1.,"#it{p}_{T} [GeV/#it{c}]", "(event) weighted number of entries");
  gPad->SetLogy();
  h1JsigTauPtW->SetTitle("");
  h1JsigTauPtW->GetYaxis()->SetTitle("#sigma (weighted number of entries)");
  h1JsigTauPtW->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigTauPtW->Draw("E");
  cPtwTau->SaveAs("SigTauPtW_"+suff+".png", suff);

  //CA
  TCanvas *cPtCA = new TCanvas(Form("cPtCA_%d",0),Form("cPtCA_%d",0),500,400);
  TH1F *frPtCA = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  gPad->SetLogy();
  h1JsigCAPt->SetTitle("");
  h1JsigCAPt->GetYaxis()->SetTitle("number of entries");
  h1JsigCAPt->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigCAPt->Draw("E");
  cPtCA->SaveAs("SigCAPt_"+suff+".png");

  TCanvas *cPtwCA = new TCanvas(Form("cPtwCA_%d",0),Form("cPtwCA_%d",0),500,400);
  TH1F *frPtwCA = DrawFrame(0.,1.,0.,1.,"#it{p}_{T} [GeV/#it{c}]", "(event) weighted number of entries");
  gPad->SetLogy();
  h1JsigCAPtW->SetTitle("");
  h1JsigCAPtW->GetYaxis()->SetTitle("#sigma (weighted number of entries)");
  h1JsigCAPtW->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1JsigCAPtW->Draw("E");
  cPtwCA->SaveAs("SigCAPtW_"+suff+".png", suff);
 
}

void plotTiming(TString strIn, TString suff){
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  std::cout<< "Plot timing info for "<<suff<<std::endl;

  //Tau vs C/A - add projections
  TH2 *h2SigTfTauTf = dynamic_cast<TH2*>(fin->Get("h2SigTfTauTf"));
  TH2 *h2SigTfTauTfZcut = dynamic_cast<TH2*>(fin->Get("h2SigTfTauTfZcut"));
  TH2 *h2SigTfTauTfZcutSoft = dynamic_cast<TH2*>(fin->Get("h2SigTfTauTfZcutSoft"));
  TH2 *h2SigTfDgTf = dynamic_cast<TH2*>(fin->Get("h2SigTfDgTf"));
  TH2 *h2SigTfDgTfSoft = dynamic_cast<TH2*>(fin->Get("h2SigTfDgTfSoft"));

  TH1 *h1LogTfSigZ = dynamic_cast<TH1*>(fin->Get("h1LogTfSigZ"));
  TH1 *h1LogTfSigZTau = dynamic_cast<TH1*>(fin->Get("h1LogTfSigZTau"));
  
  // these are still to be added 
  TH1 *h1SigNCaTf = dynamic_cast<TH1*>(fin->Get("h1SigNCaTf"));
  TH1 *h1SigNCaTfZcut = dynamic_cast<TH1*>(fin->Get("h1SigNCaTfZcut"));
  TH1 *h1SigNTauTfZcut = dynamic_cast<TH1*>(fin->Get("h1SigNTauTfZcut"));
  TH1 *h1SigNTauTfZcutSoft = dynamic_cast<TH1*>(fin->Get("h1SigNTauTfZcutSoft"));
  TH1 *h1SigNTauTfDg = dynamic_cast<TH1*>(fin->Get("h1SigNTauTfDg"));
  TH1 *h1SigNTfDg = dynamic_cast<TH1*>(fin->Get("h1SigNTfDg"));

  TCanvas *c1 = new TCanvas("c1"+suff,"c1"+suff,500,400);
  TH1F *fr1 = DrawFrame(0.,10.,0.,10.,"#tau_{f}^{decluster C/A} (fm/c)","#tau_{f}^{decluster #tau} (fm/c)");
  gPad->SetLogz();
  h2SigTfTauTf->Draw("colz same");
  c1->SaveAs("TfCaTau_"+suff+".png");

  TCanvas *c2 = new TCanvas("c2"+suff,"c2"+suff,500,400);
  TH1F *fr2 = DrawFrame(0.,10.,0.,10.,"#tau_{f}^{decluster C/A} [z_{cut}=0.1] (fm/c)","#tau_{f}^{decluster #tau} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2SigTfTauTfZcut->Draw("colz same");
  c2->SaveAs("TfCaTauZcut_"+suff+".png");

  TCanvas *c2S = new TCanvas("c2SoftLimit"+suff,"c2SoftLimit"+suff,500,400);
  TH1F *fr2S = DrawFrame(0.,10.,0.,10.,"#tau_{f}^{decluster C/A} [z_{cut}=0.1] (fm/c)","#tau_{f}^{decluster #tau} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2SigTfTauTfZcutSoft->Draw("colz same");
  c2S->SaveAs("h2SigTfTauTfZcutSoft"+suff+".png");

  TCanvas *c2D = new TCanvas("c2D"+suff,"c2D"+suff,500,400);
  TH1F *fr2D = DrawFrame(0.,10.,0.,10.,"#tau_{f}^{decluster C/A} [z_{cut}=0.1] (fm/c)","#tau_{f}^{decluster #tau} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2SigTfDgTf->Draw("colz same");
  c2D->SaveAs("h2SigTfDgTf"+suff+".png");

  TCanvas *c2DS = new TCanvas("c2DS"+suff,"c2DS"+suff,500,400);
  TH1F *fr2DS = DrawFrame(0.,10.,0.,10.,"#tau_{f}^{decluster C/A} [z_{cut}=0.1] (fm/c)","#tau_{f}^{decluster #tau} [z_{cut}=0.1] (fm/c)");
  gPad->SetLogz();
  h2SigTfDgTfSoft->Draw("colz same");
  c2DS->SaveAs("h2SigTfDgTfSoft"+suff+".png");

  TCanvas *c3 = new TCanvas("c3"+suff,"c3"+suff,500,400);
  TH1F *fr3 = DrawFrame(-10.,10.,-10.,10.,"log(#tau_{form})","number of entries");
  h1LogTfSigZTau->SetStats(0);
  h1LogTfSigZTau->SetTitle("");
  h1LogTfSigZTau->SetLineColor(kBlack);
  h1LogTfSigZTau->GetYaxis()->SetTitle("number of entries");
  h1LogTfSigZTau->GetXaxis()->SetTitle("log(#tau_{form})");
  h1LogTfSigZTau->Draw("E");
  h1LogTfSigZ->SetLineColor(kMagenta);
  h1LogTfSigZ->Draw("Esame");

  TLegend *leg1 = CreateLegend(0.65, 0.85, 0.75, 0.9);
  leg1->AddEntry(h1LogTfSigZTau,"#tau recl.","l");
  leg1->AddEntry(h1LogTfSigZ,"C/A recl.","l");
  leg1->Draw();

  DrawLatex(0.23, 0.9, "JEWEL @ #sqrt{s} = ...TeV", 0.04);
  DrawLatex(0.23, 0.85, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.03);
  DrawLatex(0.23, 0.8, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.03);
  DrawLatex(0.23, 0.75, "SD z_{cut} = 0.1, #beta = 0", 0.03);
  c3->SaveAs("LogTfCaTauZcut_"+suff+".png");

  TCanvas *c4 = new TCanvas("c4"+suff,"c4"+suff,500,400);
  TH1F *fr4 = DrawFrame(-10.,10.,-10.,10.,"#tau_{form})","number of entries");
  TH1 *tau = h2SigTfTauTfZcut->ProjectionY();
  TH1 *tauS = h2SigTfTauTfZcutSoft->ProjectionY();
  TH1 *ca = h2SigTfTauTfZcut->ProjectionX();
  TH1 *caS = h2SigTfTauTfZcutSoft->ProjectionX();
  TH1 *DgTau = h2SigTfDgTf->ProjectionY();
  TH1 *DgTauS = h2SigTfDgTfSoft->ProjectionY();

  gPad->SetLogy();
  tau->SetStats(0);
  tau->SetTitle("");
  tau->SetLineColor(kBlack);
  tau->GetYaxis()->SetTitle("number of entries");
  tau->GetXaxis()->SetTitle("#tau_{form}");
  tau->Draw("H");
  tauS->SetLineColor(kBlack);
  tauS->SetLineStyle(2);
  ca->SetLineColor(kMagenta);
  caS->SetLineColor(kMagenta);
  caS->SetLineStyle(2);
  DgTauS->SetLineColor(kGreen);
  DgTau->SetLineColor(kGreen);
  DgTauS->SetLineStyle(2);
  tauS->Draw("hsame");
  caS->Draw("hsame");
  ca->Draw("hsame");
  DgTau->Draw("hsame");
  DgTauS->Draw("hsame");

  TLegend *leg2 = CreateLegend(0.27, 0.57, 0.7, 0.93);
  leg2->SetTextSize(0.04);
  leg2->AddEntry(tau,"#tau recl.","l");
  leg2->AddEntry(ca,"C/A recl.","l");
  leg2->AddEntry(DgTau,"DyGroom.","l");
  leg2->AddEntry(DgTauS,"DyGroom. soft","l");
  leg2->AddEntry(tauS,"#tau recl. soft","l");
  leg2->AddEntry(caS,"C/A recl. soft","l");
  leg2->Draw();

  DrawLatex(0.6, 0.7, "JEWEL @ #sqrt{s} = ...TeV", 0.04);
  DrawLatex(0.6, 0.65, "R = 0.4, |#eta_{jet}| #leq 2 ", 0.03);
  DrawLatex(0.6, 0.6, "120 < p_{T, jet} #leq 410 GeV/#it{c}", 0.03);
  DrawLatex(0.6, 0.55, "SD z_{cut} = 0.1, #beta = 0", 0.03);

  c4->SaveAs("ProjTfCaTauZcut_"+suff+".png");

  TCanvas *Ns = new TCanvas("Ns"+suff,"Ns"+suff,500,400);
  TH1F *frNs = DrawFrame(-10.,10.,-10.,10.,"Number of Splits","number of entries");
  h1SigNTauTfZcut->SetStats(0);
  h1SigNTauTfZcut->SetTitle("");
  h1SigNTauTfZcut->SetLineColor(kBlack);
  h1SigNTauTfZcut->GetYaxis()->SetTitle("number of entries");
  h1SigNTauTfZcut->GetXaxis()->SetTitle("Number of Splits in a Jet");
  h1SigNTauTfZcut->Draw("h");
  h1SigNCaTfZcut->SetLineColor(kMagenta);
  h1SigNCaTfZcut->Draw("hsame");
  h1SigNTauTfZcutSoft->SetLineColor(kBlack);
  h1SigNTauTfZcutSoft->SetLineStyle(2);
  h1SigNTauTfZcutSoft->Draw("hsame");
  h1SigNTauTfDg->SetLineColor(kBlue);
  h1SigNTauTfDg->SetLineStyle(2);
  h1SigNTauTfDg->Draw("hsame");
  

  TLegend *legNs = CreateLegend(0.6, 0.8, 0.65, 0.9);
  legNs->SetTextSize(0.03);
  legNs->AddEntry(h1SigNCaTfZcut,"C/A recl. z_{SD} = 0.1","l");
  legNs->AddEntry(h1SigNTauTfZcut,"#tau recl. z_{SD} = 0.1 (hard)","l");
  legNs->AddEntry(h1SigNTauTfZcutSoft,"#tau recl. z_{SD} = 0.1 (soft)","l");
  legNs->AddEntry(h1SigNTauTfDg,"Dynamical grooming","l");
  legNs->Draw();

  Ns->SaveAs("CompareNSplits"+suff+".png");

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

  //large 
  TH1 *h1SigPtWp = dynamic_cast<TH1*>(finp->Get("h1JPtW"));
  TH1 *h1SigPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JPtW"));
  TH1 *h1JsigTauPtWp = dynamic_cast<TH1*>(finp->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JsigTauPtW"));
  TH1 *h1JsigCAPtWp = dynamic_cast<TH1*>(finp->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPtWPb = dynamic_cast<TH1*>(finPb->Get("h1JsigCAPtW"));
  
  //short
  TH1 *h1SigPtWpS = dynamic_cast<TH1*>(finpS->Get("h1JPtW"));
  TH1 *h1SigPtWPbS = dynamic_cast<TH1*>(finPbS->Get("h1JPtW"));
  TH1 *h1JsigTauPtWpS = dynamic_cast<TH1*>(finpS->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPtWPbS = dynamic_cast<TH1*>(finPbS->Get("h1JsigTauPtW"));
  TH1 *h1JsigCAPtWpS = dynamic_cast<TH1*>(finpS->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPtWPbS = dynamic_cast<TH1*>(finPbS->Get("h1JsigCAPtW"));
  //large
  TH1 *h1SigPtWpL = dynamic_cast<TH1*>(finpL->Get("h1JPtW"));
  TH1 *h1SigPtWPbL = dynamic_cast<TH1*>(finPbL->Get("h1JPtW"));
  TH1 *h1JsigTauPtWpL = dynamic_cast<TH1*>(finpL->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPtWPbL = dynamic_cast<TH1*>(finPbL->Get("h1JsigTauPtW"));
  TH1 *h1JsigCAPtWpL = dynamic_cast<TH1*>(finpL->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPtWPbL = dynamic_cast<TH1*>(finPbL->Get("h1JsigCAPtW"));
  //intermed
  TH1 *h1SigPtWpI = dynamic_cast<TH1*>(finpI->Get("h1JPtW"));
  TH1 *h1SigPtWPbI = dynamic_cast<TH1*>(finPbI->Get("h1JPtW"));
  TH1 *h1JsigTauPtWpI = dynamic_cast<TH1*>(finpI->Get("h1JsigTauPtW"));
  TH1 *h1JsigTauPtWPbI = dynamic_cast<TH1*>(finPbI->Get("h1JsigTauPtW"));
  TH1 *h1JsigCAPtWpI = dynamic_cast<TH1*>(finpI->Get("h1JsigCAPtW"));
  TH1 *h1JsigCAPtWPbI = dynamic_cast<TH1*>(finPbI->Get("h1JsigCAPtW"));

  //short vs large formation time
  TCanvas *cPt = new TCanvas(Form("cPt_%d",0),Form("cPt_%d",0),600,500);

  TH1F *frPt = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  //gPad->SetLogy();
  h1SigPtWPbL->Sumw2();
  h1SigPtWPbL->SetStats(0);
  h1SigPtWPbL->SetTitle("");
  h1SigPtWPbL->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1SigPtWPbL->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtWPbL->Divide(h1SigPtWpL);
  h1SigPtWPbL->SetLineColor(kBlack);
  h1SigPtWPbL->SetLineStyle(1);
  h1SigPtWPbL->SetLineWidth(2);  
  h1SigPtWPbL->GetYaxis()->SetRangeUser(0, 1.5);
  h1SigPtWPbL->GetXaxis()->SetRangeUser(110, 400);
  h1SigPtWPbL->Draw("HistL");
  h1JsigTauPtWPbL->Divide(h1JsigTauPtWpL);
  h1JsigTauPtWPbL->SetLineColor(kRed);
  h1JsigTauPtWPbL->SetLineStyle(1);
  h1JsigTauPtWPbL->SetLineWidth(2); 
  h1JsigTauPtWPbL->SetTitle("");
  h1JsigTauPtWPbL->Draw("HistLsame");
  h1JsigCAPtWPbL->Divide(h1JsigCAPtWpL);
  h1JsigCAPtWPbL->SetLineColor(kBlue);
  h1JsigCAPtWPbL->SetMarkerColor(kBlue);
  h1JsigCAPtWPbL->SetLineStyle(1);
  h1JsigCAPtWPbL->SetLineWidth(2); 
  h1JsigCAPtWPbL->Draw("HistLsame");
  h1SigPtWPbS->Divide(h1SigPtWpS);
  h1SigPtWPbS->SetLineColor(kBlack);
  h1SigPtWPbS->SetLineStyle(4);
  h1SigPtWPbS->SetLineWidth(2); 
  h1SigPtWPbS->Draw("HistLsame");
  h1JsigTauPtWPbS->Divide(h1JsigTauPtWpS);
  h1JsigTauPtWPbS->SetLineColor(kRed);
  h1JsigTauPtWPbS->SetLineStyle(4);
  h1JsigTauPtWPbS->SetLineWidth(2); 
  h1JsigTauPtWPbS->Draw("HistLsame");
  h1JsigCAPtWPbS->Divide(h1JsigCAPtWpS);
  h1JsigCAPtWPbS->SetLineColor(kBlue);
  h1JsigCAPtWPbS->SetMarkerColor(kBlue);
  h1JsigCAPtWPbS->SetLineStyle(4);
  h1JsigCAPtWPbS->SetLineWidth(2); 
  h1JsigCAPtWPbS->Draw("HistLsame");

  TLegend *leg1 = CreateLegend(0.25, 0.85, 0.7, 0.95);
  leg1->SetTextSize(0.03);
  leg1->SetNColumns(3);
  leg1->AddEntry(h1JsigTauPtWPbL,"#tau > 3 #tau-recl.","l");
  leg1->AddEntry(h1JsigCAPtWPbL,"#tau > 3 C/A-recl.","l");
  leg1->AddEntry(h1SigPtWPbL,"#tau > 3 Signal Jets","l");
  leg1->AddEntry(h1JsigTauPtWPbS,"#tau < 1 #tau-recl.","l");
  leg1->AddEntry(h1JsigCAPtWPbS,"#tau < 1 C/A-recl.","l");
  leg1->AddEntry(h1SigPtWPbS,"#tau < 1 Signal Jets ","l");
  leg1->Draw();

  cPt->SaveAs("RaaRanges_ShortVsLarge"+recluster+".png");
 
  // Inclusive vs intermediate 
  TCanvas *cPtI = new TCanvas(Form("cPtI_%d",0),Form("cPtI_%d",0),600,500);

  TH1F *frPtI = DrawFrame(0.,1.,0.,1., "#it{p}_{T} [GeV/#it{c}]", "number of entries");
  //gPad->SetLogy();
  h1SigPtWPb->Sumw2();
  h1SigPtWPb->SetStats(0);
  h1SigPtWPb->SetTitle("");
  h1SigPtWPb->GetYaxis()->SetTitle("#sigma_{simple}/#sigma_{vac} (weighted spectra)");
  h1SigPtWPb->GetXaxis()->SetTitle("#it{p}_{T} [GeV/#it{c}]");
  h1SigPtWPb->Divide(h1SigPtWp);
  h1SigPtWPb->SetLineColor(kBlack);
  h1SigPtWPb->SetLineStyle(1);
  h1SigPtWPb->SetLineWidth(2);  
  h1SigPtWPb->GetYaxis()->SetRangeUser(0., 1.5);
  h1SigPtWPb->GetXaxis()->SetRangeUser(110, 400);
  h1SigPtWPb->Draw("HistL");
  h1JsigTauPtWPb->Divide(h1JsigTauPtWp);
  h1JsigTauPtWPb->SetLineColor(kRed);
  h1JsigTauPtWPb->SetLineStyle(1);
  h1JsigTauPtWPb->SetLineWidth(2); 
  h1JsigTauPtWPb->SetTitle("");
  h1JsigTauPtWPb->Draw("HistLsame");
  h1JsigCAPtWPb->Divide(h1JsigCAPtWp);
  h1JsigCAPtWPb->SetLineColor(kBlue);
  h1JsigCAPtWPb->SetMarkerColor(kBlue);
  h1JsigCAPtWPb->SetLineStyle(1);
  h1JsigCAPtWPb->SetLineWidth(2); 
  h1JsigCAPtWPb->Draw("HistLsame");
  h1SigPtWPbI->Divide(h1SigPtWpI);
  h1SigPtWPbI->SetLineColor(kBlack);
  h1SigPtWPbI->SetLineStyle(4);
  h1SigPtWPbI->SetLineWidth(2); 
  h1SigPtWPbI->Draw("HistLsame");
  h1JsigTauPtWPbI->Divide(h1JsigTauPtWpI);
  h1JsigTauPtWPbI->SetLineColor(kRed);
  h1JsigTauPtWPbI->SetLineStyle(4);
  h1JsigTauPtWPbI->SetLineWidth(2); 
  h1JsigTauPtWPbI->Draw("HistLsame");
  h1JsigCAPtWPbI->Divide(h1JsigCAPtWpI);
  h1JsigCAPtWPbI->SetLineColor(kBlue);
  h1JsigCAPtWPbI->SetMarkerColor(kBlue);
  h1JsigCAPtWPbI->SetLineStyle(4);
  h1JsigCAPtWPbI->SetLineWidth(2); 
  h1JsigCAPtWPbI->Draw("HistLsame");

  TLegend *leg1I = CreateLegend(0.25, 0.85, 0.7, 0.95);
  leg1I->SetTextSize(0.03);
  leg1I->SetNColumns(3);
  leg1I->AddEntry(h1JsigTauPtWPb,"incl. #tau-recl.","l");
  leg1I->AddEntry(h1JsigCAPtWPb,"incl. C/A-recl.","l");
  leg1I->AddEntry(h1SigPtWPb,"incl. Signal Jets","l");
  leg1I->AddEntry(h1JsigTauPtWPbI,"1 < #tau < 3 #tau-recl. ","l");
  leg1I->AddEntry(h1JsigCAPtWPbI,"1 < #tau < 3 C/A-recl. ","l");
  leg1I->AddEntry(h1SigPtWPbI,"1 < #tau < 3 Signal Jets","l");
  
  leg1I->Draw();

  cPtI->SaveAs("RaaRanges_IntermedVsFull"+recluster+".png");
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



void compareSplits(TString strIn, TString suff){
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  std::cout<< "Plot timing info for"<<suff<<std::endl;
  TH2 *h2SigRgTauRgZcut = dynamic_cast<TH2*>(fin->Get("h2SigRgTauRgZcut"));

  TCanvas *comp1 = new TCanvas("comp1"+suff,"comp1"+suff,500,400);
  TH1F *fr1 = DrawFrame(0.,0.5,0.,0.5,"R_{g}^{decluster C/A}","R_{g}^{decluster #tau}");
  gPad->SetLogz();
  h2SigRgTauRgZcut->Draw("colz same");
  comp1->SaveAs("h2SigRgTauRgZcut"+suff+".png");

  //  h1TfSigDy[Nsplit]->Write();
  TH1 *h1TfSigTau0 = dynamic_cast<TH1*>(fin->Get("h1TfSigTau[0]"));
  TH1 *h1TfSigTauSoft0 = dynamic_cast<TH1*>(fin->Get("h1TfSigTauSoft[0]"));
  TH1 *h1TfSigCA0 = dynamic_cast<TH1*>(fin->Get("h1TfSigCA[0]"));
  TH1 *h1TfSigTau1 = dynamic_cast<TH1*>(fin->Get("h1TfSigTau[1]"));
  TH1 *h1TfSigTauSoft1 = dynamic_cast<TH1*>(fin->Get("h1TfSigTauSoft[1]"));
  TH1 *h1TfSigCA1 = dynamic_cast<TH1*>(fin->Get("h1TfSigCA[1]"));
  
  TCanvas *ct = new TCanvas("tf_"+suff,"tf_"+suff,800,400);
  ct->Divide(2,1);
  ct->cd(1);
  TH1F *frctf = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
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
  legtf->AddEntry(h1TfSigTau0,"#tau-recl.","l");
  legtf->AddEntry(h1TfSigTauSoft0,"#tau-recl. (soft)","l");
  legtf->AddEntry(h1TfSigCA0,"C/A-recl.","l");
  legtf->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  ct->cd(2);
  TH1F *frctf1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1TfSigTau1->SetTitle("Second Splits");
  h1TfSigTau1->GetYaxis()->SetTitle("number of entries");
  h1TfSigTau1->GetXaxis()->SetTitle("#tau_{form}");
  h1TfSigTau1->SetLineColor(kBlack);
  h1TfSigTau1->Draw("E");
  h1TfSigTauSoft1->SetLineColor(kBlack);
  h1TfSigTauSoft1->SetLineStyle(2);
  h1TfSigTauSoft1->Draw("Esame");
  h1TfSigCA1->SetLineColor(kMagenta);
  h1TfSigCA1->Draw("Esame");

  legtf->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  ct->SaveAs("tform_1And2ndSplits_"+suff+".png");
  
  //h1ZgSigDg[nsplit] = new TH1F(Form("h1ZgSigDg[%d]", nsplit), Form("h1ZgSigDg[%d]",nsplit),50,0.,1); ->could be added once I fixed the issues with filling the vector..
  //h1ZgSigDgTau[nsplit] = new TH1F(Form("h1ZgSigDgTau[%d]", nsplit), Form("h1ZgSigDgTau[%d]",nsplit),50,0.,1);
  TH1 *h1ZgSigCA0 = dynamic_cast<TH1*>(fin->Get("h1ZgSigCA[0]"));
  TH1 *h1ZgSigTau0 = dynamic_cast<TH1*>(fin->Get("h1ZgSigTau[0]"));
  TH1 *h1ZgSigCA1 = dynamic_cast<TH1*>(fin->Get("h1ZgSigCA[1]"));
  TH1 *h1ZgSigTau1 = dynamic_cast<TH1*>(fin->Get("h1ZgSigTau[1]"));

  TCanvas *cS = new TCanvas("cS_"+suff,"cS_"+suff,800,400);
  cS->Divide(2,1);
  cS->cd(1);
  TH1F *frcS = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1ZgSigCA0->SetTitle("First Splits");
  h1ZgSigCA0->GetYaxis()->SetTitle("number of entries");
  h1ZgSigCA0->GetXaxis()->SetTitle("z_{g}");
  h1ZgSigCA0->SetLineColor(kMagenta);
  h1ZgSigCA0->Draw("E");
  h1ZgSigTau0->SetLineColor(kBlack);
  h1ZgSigTau0->Draw("Esame");

  TLegend *legS = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legS->AddEntry(h1ZgSigTau0,"#tau-recl.","l");
  legS->AddEntry(h1ZgSigCA0,"C/A-recl.","l");
  legS->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cS->cd(2);
  TH1F *frcS1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1ZgSigCA1->SetTitle("Second Splits");
  h1ZgSigCA1->GetYaxis()->SetTitle("number of entries");
  h1ZgSigCA1->GetXaxis()->SetTitle("z_{g}");
  h1ZgSigCA1->SetLineColor(kMagenta);
  h1ZgSigCA1->Draw("E");
  h1ZgSigTau1->SetLineColor(kBlack);
  h1ZgSigTau1->Draw("Esame");

  legS->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cS->SaveAs("Zg_1And2ndSplits_"+suff+".png");
  //h1RgDgSig[nsplit] = new TH1F(Form("h1RgDgSig[%d]", nsplit),Form("h1RgDgSig[%d]", nsplit),50,0.,0.5);
  //h1RgDgTauSig[nsplit] = new TH1F(Form("h1RgDgTauSig[%d]", nsplit),Form("h1RgDgTauSig[%d]", nsplit),50,0.,0.5);

  TH1 *h1RgCASig0 = dynamic_cast<TH1*>(fin->Get("h1RgCASig[0]"));
  TH1 *h1RgTauSig0 = dynamic_cast<TH1*>(fin->Get("h1RgTauSig[0]"));
  TH1 *h1RgCASig1 = dynamic_cast<TH1*>(fin->Get("h1RgCASig[1]"));
  TH1 *h1RgTauSig1 = dynamic_cast<TH1*>(fin->Get("h1RgTauSig[1]"));

  TCanvas *cSR = new TCanvas("cSR_"+suff,"cSR_"+suff,800,400);
  cSR->Divide(2,1);
  cSR->cd(1);
  TH1F *frcSR = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1RgCASig0->SetTitle("First Splits");
  h1RgCASig0->GetYaxis()->SetTitle("number of entries");
  h1RgCASig0->GetXaxis()->SetTitle("R_{g}");
  h1RgCASig0->SetLineColor(kMagenta);
  h1RgCASig0->Draw("E");
  h1RgTauSig0->SetLineColor(kBlack);
  h1RgTauSig0->Draw("Esame");

  TLegend *legSR = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSR->AddEntry(h1RgTauSig0,"#tau-recl.","l");
  legSR->AddEntry(h1RgCASig0,"C/A-recl.","l");
  legSR->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSR->cd(2);
  TH1F *frcSR1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1RgCASig1->SetTitle("Second Splits");
  h1RgCASig1->GetYaxis()->SetTitle("number of entries");
  h1RgCASig1->GetXaxis()->SetTitle("R_{g}");
  h1RgCASig1->SetLineColor(kMagenta);
  h1RgCASig1->Draw("E");
  h1RgTauSig1->SetLineColor(kBlack);
  h1RgTauSig1->Draw("Esame");

  legSR->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSR->SaveAs("Rg_1And2ndSplits_"+suff+".png");
  
  //energy of sum of the two branches at each node
  TH1 *h1EradSigCA0 = dynamic_cast<TH1*>(fin->Get("h1EradSigCA[0]"));
  TH1 *h1EradSigTau0 = dynamic_cast<TH1*>(fin->Get("h1EradSigTau[0]"));
  TH1 *h1EradSigCA1 = dynamic_cast<TH1*>(fin->Get("h1EradSigCA[1]"));
  TH1 *h1EradSigTau1 = dynamic_cast<TH1*>(fin->Get("h1EradSigTau[1]"));

  TCanvas *cSE = new TCanvas("cSE_"+suff,"cSE_"+suff,800,400);
  cSE->Divide(2,1);
  cSE->cd(1);
  TH1F *frcSE = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1EradSigCA0->SetTitle("First Splits");
  h1EradSigCA0->GetYaxis()->SetTitle("number of entries");
  h1EradSigCA0->GetXaxis()->SetTitle("E_{rad} (sum of energy of two branches)");
  h1EradSigCA0->SetLineColor(kMagenta);
  h1EradSigCA0->Draw("E");
  h1EradSigTau0->SetLineColor(kBlack);
  h1EradSigTau0->Draw("Esame");

  TLegend *legSE = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSE->AddEntry(h1EradSigTau0,"#tau-recl.","l");
  legSE->AddEntry(h1EradSigCA0,"C/A-recl.","l");
  legSE->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSE->cd(2);
  TH1F *frcSE1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1EradSigCA1->SetTitle("Second Splits");
  h1EradSigCA1->GetYaxis()->SetTitle("number of entries");
  h1EradSigCA1->GetXaxis()->SetTitle("E_{rad} (sum of energy of two branches)");
  h1EradSigCA1->SetLineColor(kMagenta);
  h1EradSigCA1->Draw("E");
  h1EradSigTau1->SetLineColor(kBlack);
  h1EradSigTau1->Draw("Esame");

  legSE->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSE->SaveAs("EnergySum_1And2ndSplits_"+suff+".png");

  //Log(1/angle) in the algorithm
  TH1 *h1LogDr12SigCA0 = dynamic_cast<TH1*>(fin->Get("h1LogDr12SigCA[0]"));
  TH1 *h1LogDr12SigTau0 = dynamic_cast<TH1*>(fin->Get("h1LogDr12SigTau[0]"));
  TH1 *h1LogDr12SigCA1 = dynamic_cast<TH1*>(fin->Get("h1LogDr12SigCA[1]"));
  TH1 *h1LogDr12SigTau1 = dynamic_cast<TH1*>(fin->Get("h1LogDr12SigTau[1]"));

  TCanvas *cSL = new TCanvas("cSL_"+suff,"cSL_"+suff,800,400);
  cSL->Divide(2,1);
  cSL->cd(1);
  TH1F *frcSEL0 = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1LogDr12SigCA0->SetTitle("First Splits");
  h1LogDr12SigCA0->GetYaxis()->SetTitle("number of entries");
  h1LogDr12SigCA0->GetXaxis()->SetTitle("Log(1/angle)");
  h1LogDr12SigCA0->SetLineColor(kMagenta);
  h1LogDr12SigCA0->Draw("E");
  h1LogDr12SigTau0->SetLineColor(kBlack);
  h1LogDr12SigTau0->Draw("Esame");

  TLegend *legSL = CreateLegend(0.23, 0.48, 0.2, 0.4);
  legSL->SetTextSize(0.03);
  legSL->AddEntry(h1LogDr12SigTau0,"#tau-recl.","l");
  legSL->AddEntry(h1LogDr12SigCA0,"C/A-recl.","l");
  legSL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSL->cd(2);
  TH1F *frcSE1L = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1LogDr12SigCA1->SetTitle("Second Splits");
  h1LogDr12SigCA1->GetYaxis()->SetTitle("number of entries");
  h1LogDr12SigCA1->GetXaxis()->SetTitle("Log(1/angle)");
  h1LogDr12SigCA1->SetLineColor(kMagenta);
  h1LogDr12SigCA1->Draw("E");
  h1LogDr12SigTau1->SetLineColor(kBlack);
  h1LogDr12SigTau1->Draw("Esame");

  legSL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSL->SaveAs("Log1overAngle_1And2ndSplits_"+suff+".png");

  // Log(z*angle) in the algorithm
  TH1 *h1LogZthetaSigCA0 = dynamic_cast<TH1*>(fin->Get("h1LogZthetaSigCA[0]"));
  TH1 *h1LogZthetaSigTau0 = dynamic_cast<TH1*>(fin->Get("h1LogZthetaSigTau[0]"));
  TH1 *h1LogZthetaSigCA1 = dynamic_cast<TH1*>(fin->Get("h1LogZthetaSigCA[1]"));
  TH1 *h1LogZthetaSigTau1 = dynamic_cast<TH1*>(fin->Get("h1LogZthetaSigTau[1]"));

  TCanvas *cSEL = new TCanvas("cSEL_"+suff,"cSEL_"+suff,800,400);
  cSEL->Divide(2,1);
  cSEL->cd(1);
  TH1F *frcSEL = DrawFrame(0.,1.,0.,1., "compare 1st splits", "number of entries");
  h1LogZthetaSigCA0->SetTitle("First Splits");
  h1LogZthetaSigCA0->GetYaxis()->SetTitle("number of entries");
  h1LogZthetaSigCA0->GetXaxis()->SetTitle("Log(z*angle)");
  h1LogZthetaSigCA0->SetLineColor(kMagenta);
  h1LogZthetaSigCA0->Draw("E");
  h1LogZthetaSigTau0->SetLineColor(kBlack);
  h1LogZthetaSigTau0->Draw("Esame");

  TLegend *legSEL = CreateLegend(0.43, 0.78, 0.7, 0.9);
  legSEL->AddEntry(h1LogZthetaSigTau0,"#tau-recl.","l");
  legSEL->AddEntry(h1LogZthetaSigCA0,"C/A-recl.","l");
  legSEL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);

  cSEL->cd(2);
  TH1F *frcSEL1 = DrawFrame(0.,1.,0.,1., "compare 2nd splits", "number of entries");
  h1LogZthetaSigCA1->SetTitle("Second Splits");
  h1LogZthetaSigCA1->GetYaxis()->SetTitle("number of entries");
  h1LogZthetaSigCA1->GetXaxis()->SetTitle("Log(z*angle)");
  h1LogZthetaSigCA1->SetLineColor(kMagenta);
  h1LogZthetaSigCA1->Draw("E");
  h1LogZthetaSigTau1->SetLineColor(kBlack);
  h1LogZthetaSigTau1->Draw("Esame");

  legSEL->Draw();
  DrawLatex(0.7,0.5, suff, 0.07);
  cSEL->SaveAs("LogTimesZfrac_1And2ndSplits_"+suff+".png");

}

void plotNsubj(TString strIn, TString suff, TString dy){// add these sigJetRecurZcut_tau21, sigJetRecurZcut_tau32, sigJetRecurTauZcut_tau21, sigJetRecurTauZcut_tau32
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  std::cout<< "Plot sibjettiness info for "<<suff<<std::endl;
  TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2CaTauNsubj21"));// x ca, y tau 
  TH2 *h2Nsubj32 = dynamic_cast<TH2*>(fin->Get("h2CaTauNsubj32"));// x ca, y tau 
  if (dy == "Dy"){ 
    TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2Nsubj21"));// x ca, y tau
    TH2 *h2Nsubj32 = dynamic_cast<TH2*>(fin->Get("h2Nsubj32"));
  }

  TH2 *h2TauNsubj21 = dynamic_cast<TH2*>(fin->Get("h2TauNsubj21"));
  TH2 *h2TauNsubj21Soft = dynamic_cast<TH2*>(fin->Get("h2TauNsubj21"));
  

  TCanvas *cTau21 = new TCanvas("cTau21_"+suff, "cTau21_"+suff,500,400);
  TH1F *frcTau21 = DrawFrame(0.,1.,0.,1.,"formation time hard","Nsubjettiness tau");
  gPad->SetLogz();
  h2TauNsubj21->Draw("colz same");
  cTau21->SaveAs("Nsubj21_HardFormationtime_"+suff+".png");

  TCanvas *cTauSoft21 = new TCanvas("cTauSoft21_"+suff, "cTauSoft21_"+suff,500,400);
  TH1F *frcTauSoft21 = DrawFrame(0.,1.,0.,1.,"formation time soft","Nsubjettiness tau");
  gPad->SetLogz();
  h2TauNsubj21Soft->Draw("colz same");
  cTauSoft21->SaveAs("Nsubj21_SoftFormationtime_"+suff+".png");

  TCanvas *c21 = new TCanvas("c21_"+suff, "c21_"+suff,500,400);
  TH1F *frc21 = DrawFrame(0.,1.,0.,1.,"Nsubjettiness ca","Nsubjettiness tau");
  gPad->SetLogz();
  h2Nsubj21->Draw("colz same");
  c21->SaveAs("h2Nsubj21_"+suff+"_"+dy+".png");

  TCanvas *c32 = new TCanvas("c32_"+suff, "c32_"+suff,500,400);
  TH1F *frc32 = DrawFrame(0.,1.,0.,1.,"Nsubjettiness ca","Nsubjettiness tau");
  gPad->SetLogz();
  h2Nsubj32->Draw("colz same");
  c32->SaveAs("h2Nsubj32_"+suff+"_"+dy+".png");
  
  TCanvas *projNsubj = new TCanvas("projNsubj"+suff,"projNsubj"+suff,500,400);
  TH1F *frproj21 = DrawFrame(0.,1.,0.,1.,"NSubjettiness21","number of entries");
  TH1 *tau21 = h2Nsubj21->ProjectionY();
  //tau21->SetStats(0);
  tau21->SetTitle("");
  tau21->SetLineColor(kBlack);
  tau21->GetYaxis()->SetTitle("number of entries");
  tau21->GetXaxis()->SetTitle("#tau_{21}");
  tau21->Draw("E");
  projNsubj->SaveAs("projNsubj21"+suff+"_"+dy+".png");

  
  TCanvas *projNsubj2 = new TCanvas("projNsubj2"+suff,"projNsubj2"+suff,500,400);
  TH1F *frproj32 = DrawFrame(0.,1.,0.,1.,"NSubjettiness32","number of entries");
  TH1 *tau32 = h2Nsubj32->ProjectionY();
  //tau21->SetStats(0);
  tau32->SetTitle("");
  tau32->SetLineColor(kBlack);
  tau32->GetYaxis()->SetTitle("number of entries");
  tau32->GetXaxis()->SetTitle("#tau_{32}");
  tau32->Draw("E");
  projNsubj2->SaveAs("projNsubj32"+suff+"_"+dy+".png");

}

void plotNsubjInRanges(TString strInPP, TString strInPb, TString strInPPS, TString strInPbS, TString strInPPI, TString strInPbI, TString strInPPL, TString strInPbL, TString dy){
  TFile *finPP = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  TH1 *h1SigPtVac = dynamic_cast<TH1*>(finPP->Get("h1JPt"));// unweighted..histograms from signalJetCollection
  TH1 *h1SigPtMed = dynamic_cast<TH1*>(finPb->Get("h1JPt"));//maybe better to take the weighted...
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


  for(auto i : ifiles){
    TFile *fin = new TFile(i.Data());
    TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2CaTauNsubj21"));// x ca, y tau 
    if (dy == "Dy"){
      std::cout<<"plotting for Dy Result"<<std::endl;
      TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2Nsubj21"));// x ca, y tau h2CaTauNsubj21
    }
    TH1 *tau21 = h2Nsubj21->ProjectionY();
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
  
  // subjettiness ratio 3/2
  TCanvas *projNsubj32 = new TCanvas("projNsubj32","projNsubj32",900,400);
  projNsubj32->Divide(2);
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
    TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2CaTauNsubj32"));// x ca, y tau 
    if (dy == "Dy"){
      std::cout<<"plotting for Dy Result"<<std::endl;
      TH2 *h2Nsubj21 = dynamic_cast<TH2*>(fin->Get("h2Nsubj32"));// x ca, y tau h2CaTauNsubj21
    }
    TH1 *tau21 = h2Nsubj21->ProjectionY();
    tau21->SetLineWidth(2);
    projNsubj32->cd(1);
    if (idx32 < 1){
      padTau132->Draw();
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#tau_{3}/#tau_{2})}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#tau_{3}/#tau_{2}");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "vac "+dy, 0.06);
      leg32->AddEntry(tau21, ilegend[idx32],"lp");

    }
    else if (idx32 == 4){//PbPb
      projNsubj32->cd(2);
      padTau232->Draw();
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32-3);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{d(#tau_{3}/#tau_{2})}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("#tau_{3}/#tau_{2}");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "med "+dy, 0.06);
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else if (idx32 > 4){
      projNsubj32->cd(2);
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
  projNsubj32->SaveAs("projFullNsubj32_"+dy+".png");
}


  //plotRgInRanges(strInPP, strInPb, strInPPSS, strInPbSS, strInPPI, strInPbI, strInPPL, strInPbL, "tau");// to be tested/adjusted
// Rg in ranges as Nsubjettiness 
void plotRgInRanges(TString strInPP, TString strInPb, TString strInPPS, TString strInPbS, TString strInPPI, TString strInPbI, TString strInPPL, TString strInPbL){
  TFile *finPP = new TFile(strInPP.Data());
  TFile *finPb = new TFile(strInPb.Data());
  TH1 *h1SigPtVac = dynamic_cast<TH1*>(finPP->Get("h1JPt"));// unweighted..histograms from signalJetCollection
  TH1 *h1SigPtMed = dynamic_cast<TH1*>(finPb->Get("h1JPt"));//maybe better to take the weighted...
  std::array<double, 2> PtNorm = getNorm(h1SigPtVac, h1SigPtMed);
  double normPP = PtNorm[0];
  double normPb = PtNorm[1];

  list<TString>  ifiles{strInPP, strInPPS, strInPPI, strInPPL, strInPb, strInPbS, strInPbI, strInPbL};  
  TString ilegend[8] = {"vac. incl.", "vac. #tau < 1", "vac. 1 < #tau < 3", "vac. #tau > 3", "med. incl.", "med. #tau < 1", "med. 1 < #tau < 3", "med. #tau > 3"};  
  
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
    TFile *fin = new TFile(i.Data());
    TH2 *h2Rg21 = dynamic_cast<TH2*>(fin->Get("h2SigRgTauRgZcut"));// x ca, y tau
    TH1 *tau21 = h2Rg21->ProjectionY();
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
      DrawLatex(0.6,0.95, "vac #tau", 0.06);
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
      DrawLatex(0.6,0.95, "med #tau", 0.06);
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
  projRg->SaveAs("projFullRgTau.png");
  
  // Rg from CA
  TCanvas *projRg32 = new TCanvas("projRg32","projRg32",900,400);
  projRg32->Divide(2);
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
    TH2 *h2Rg21 = dynamic_cast<TH2*>(fin->Get("h2SigRgTauRgZcut"));// x ca, y tau -- using the 2d 32 histo !
    TH1 *tau21 = h2Rg21->ProjectionX();
    tau21->SetLineWidth(2);
    projRg32->cd(1);
    if (idx32 < 1){
      padTau132->Draw();
      padTau132->cd();
      tau21->Scale(1./normPP,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32+1);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "vac C/A", 0.06);
      leg32->AddEntry(tau21, ilegend[idx32],"lp");

    }
    else if (idx32 == 4){//PbPb
      projRg32->cd(2);
      padTau232->Draw();
      padTau232->cd();
      tau21->Scale(1./normPb,"width");
      tau21->SetStats(0);
      tau21->SetTitle("");
      tau21->SetLineColor(idx32-3);
      tau21->GetYaxis()->SetTitle("#frac{1}{N_{jets, incl}} #frac{dN}{dR_{g}}");
      tau21->GetYaxis()->SetTitleOffset(2.1);
      tau21->GetXaxis()->SetRangeUser(0, 0.6);
      tau21->GetXaxis()->SetTitle("R_{g}");
      tau21->DrawCopy("h");
      DrawLatex(0.6,0.95, "med C/A", 0.06);
      leg232->AddEntry(tau21, ilegend[idx32],"lp");
    }
    else if (idx32 > 4){
      projRg32->cd(2);
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
  projRg32->SaveAs("projFullRgCA.png");
}

void nDropped(TString strIn, TString suff){
  TFile *fin = new TFile(strIn.Data());
  fin->ls();
  std::cout<< "Plot nDroppedSubjets for"<<suff<<std::endl;
  //  h1TfSigDy[Nsplit]->Write();
  TH1 *h1dropTau = dynamic_cast<TH1*>(fin->Get("h1DroppedBrTau"));
  TH1 *h1dropCA = dynamic_cast<TH1*>(fin->Get("h1DroppedBr"));
  
  TCanvas *cdr = new TCanvas("cdr_"+suff,"cdr_"+suff,800,400);
  TH1F *frdr = DrawFrame(0.,1.,0.,1., "compare number of drupped subjets", "number of entries");
  h1dropTau->SetTitle(" ");
  h1dropTau->GetYaxis()->SetTitle("number of entries");
  h1dropTau->GetXaxis()->SetTitle("N dropped subjets (dyn. grooming)");
  h1dropTau->SetLineColor(kBlack);
  h1dropTau->Draw("E");
  h1dropCA->SetLineColor(kMagenta);
  h1dropCA->Draw("Esame");

  TLegend *legdr = CreateLegend(0.25, 0.5, 0.4, 0.78);
  legdr->SetTextSize(0.035);
  legdr->AddEntry(h1dropTau, "#tau history","lp");
  legdr->AddEntry(h1dropCA, "C/A history","lp");
  legdr->Draw();
  cdr->SaveAs("NdroppedSubj_"+suff+".png");
}



//The main function
void jplotTimeClusHistos() { 
  /*
  TString strInPbS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_CA_short.root";
  TString strInPbL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_CA_large.root";
  TString strInPbI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_CA_intermed.root";
  TString strInPb = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple.root";
  TString strInPPS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_CA_short.root";
  TString strInPPL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_CA_large.root";
  TString strInPPI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_CA_intermed.root";
  TString strInPP = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac.root";
 
  TString strInPbS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Dy_short.root";
  TString strInPbL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Dy_large.root";
  TString strInPbI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Dy_intermed.root";
  TString strInPb = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple.root";
  TString strInPPS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Dy_short.root";
  TString strInPPL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Dy_large.root";
  TString strInPPI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Dy_intermed.root";
  TString strInPP = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac.root";
  */
  TString strInPb = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple.root";
  TString strInPbS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Tau_short.root";// < 1 cut
  TString strInPbSS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Tau_supershort.root";// < 0.5 
  TString strInPbL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Tau_large.root";
  TString strInPbI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_simple_Tau_intermed.root";
  TString strInPP = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac.root";
  TString strInPPS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Tau_short.root";// < 1 cut
  TString strInPPSS = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Tau_supershort.root";// < 0.5 cut
  TString strInPPL = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Tau_large.root";
  TString strInPPI = "Johanna/Jewel/JetToyHITimeClusHistosJewel_vac_Tau_intermed.root";
  
  //plotPt(strInPbL, "simple_large_ca");
  //plotPt(strInPPL, "vac_large_ca");
  //plotTiming(strInPb, "simple");
  //plotTiming(strInPP, "vac");
  compareSplits(strInPP, "vac");
  //compareSplits(strInPPS, "vac_short");
  //nDropped(strInPP, "vac");
  //plotNsubj(strInPP, "vac");//sigJetRecurZcut_tau21, sigJetRecurZcut_tau32, sigJetRecurTauZcut_tau21, sigJetRecurTauZcut_tau32
  //plotNsubj(strInPb, "simple");
  //plotRgs(strInPP, strInPb, "incl");//pp, pb
  
 // plotRgInRanges(strInPP, strInPb, strInPPS, strInPbS, strInPPI, strInPbI, strInPPL, strInPbL);// to be tested/adjusted
  //plotRgs(strInPPSS, strInPbSS, "supershort");//pp, pb
  //plotRgs(strInPPL, strInPbL, "long");//pp, pb
  //plotRgs(strInPPI, strInPbI, "intermed");//pp, pb
  
  plotNsubjInRanges(strInPP, strInPb, strInPPSS, strInPbSS, strInPPI, strInPbI, strInPPL, strInPbL, "tau");
  //plotPt(strInPb, "simple");
  //plotPt(strInPP, "vac");
  //plotTiming(strInPbS, "simple_short_ca");
  //plotTiming(strInPPS, "vac_short_ca");
  //doRaa(strInPP, strInPb);
  //doRaaRanges(strInPP, strInPb, strInPPSS, strInPbSS, strInPPL, strInPbL, strInPPI, strInPbI, "Tau");
  return;

}