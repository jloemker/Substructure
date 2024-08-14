#include "plotUtils.C"

//void plotTimeClusHistos(TString strIn = "Johanna/JetToyHIResultTimeClusBkg_tau.root", double zcut = 0.1) {
void plotTimeClusHistos(TString strIn = "Johanna/radii/JetToyHITimeClusHistosVacR0p2.root", double zcut = 0.1) {

  TFile *fin = new TFile(strIn.Data());
  fin->ls();

  TH1 *h1JESCS = dynamic_cast<TH1*>(fin->Get("h1JESCS"));
  TH2 *h2PtJESCS = dynamic_cast<TH2*>(fin->Get("h2PtJESCS"));

  TH1 *h1JESCSFull = dynamic_cast<TH1*>(fin->Get("h1JESCSFull"));
  TH2 *h2PtJESCSFull = dynamic_cast<TH2*>(fin->Get("h2PtJESCSFull"));

  TH1 *h1ZgSig = dynamic_cast<TH1*>(fin->Get("h1ZgSig"));
  TH1 *h1ZgCS = dynamic_cast<TH1*>(fin->Get("h1ZgCS"));
  TH1 *h1ZgCSFull = dynamic_cast<TH1*>(fin->Get("h1ZgCSFull"));

  TH1 *h1RgSig = dynamic_cast<TH1*>(fin->Get("h1RgSig"));
  TH1 *h1RgCS = dynamic_cast<TH1*>(fin->Get("h1RgCS"));
  TH1 *h1RgCSFull = dynamic_cast<TH1*>(fin->Get("h1RgCSFull"));

  TH1 *h1LogTfSig = dynamic_cast<TH1*>(fin->Get("h1LogTfSig"));
  TH1 *h1LogTfSigCh = dynamic_cast<TH1*>(fin->Get("h1LogTfSigCh"));
  TH1 *h1LogTfCS = dynamic_cast<TH1*>(fin->Get("h1LogTfCS"));
  TH1 *h1LogTfCSFull = dynamic_cast<TH1*>(fin->Get("h1LogTfCSFull"));

  TH2 *h2PartonSigTf = dynamic_cast<TH2*>(fin->Get("h2PartonSigTf"));
  TH2 *h2PartonSigChTf = dynamic_cast<TH2*>(fin->Get("h2PartonSigChTf"));
  TH2 *h2PartonCSTf = dynamic_cast<TH2*>(fin->Get("h2PartonCSTf"));
  TH2 *h2PartonCSFullTf = dynamic_cast<TH2*>(fin->Get("h2PartonCSFullTf"));

  TH2 *h2SigCSTf = dynamic_cast<TH2*>(fin->Get("h2SigCSTf"));
  TH2 *h2SigCSFullTf = dynamic_cast<TH2*>(fin->Get("h2SigCSFullTf"));

  TH1 *h1TfScaleCS = dynamic_cast<TH1*>(fin->Get("h1TfScaleCS"));
  TH1 *h1TfScaleCSFull = dynamic_cast<TH1*>(fin->Get("h1TfScaleCSFull"));

  TH1 *h1TfScaleSigp = dynamic_cast<TH1*>(fin->Get("h1TfScaleSigp"));
  TH1 *h1TfScaleCSp = dynamic_cast<TH1*>(fin->Get("h1TfScaleCSp"));
  TH1 *h1TfScaleCSFullp = dynamic_cast<TH1*>(fin->Get("h1TfScaleCSFullp"));

  TH2 *h2TfScaleRgCS = dynamic_cast<TH2*>(fin->Get("h2TfScaleRgCS"));
  TH2 *h2TfScaleRgCSFull = dynamic_cast<TH2*>(fin->Get("h2TfScaleRgCSFull"));

  //normalize
  h1LogTfSig->Scale(1./h1LogTfSig->Integral(),"width");
  h1LogTfCS->Scale(1./h1LogTfCS->Integral(),"width"); 
  h1LogTfCSFull->Scale(1./h1LogTfCSFull->Integral(),"width");
  h1LogTfSigCh->Scale(1./h1LogTfSigCh->Integral(),"width");

  TCanvas *c0 = new TCanvas(Form("c0_%d",0),Form("c0_%d",0),500,400);

  TH1F *fr0 = DrawFrame(0.,2.,1e-3,10.,"p_{T,jet}^{sub}/p_{T,jet}^{true}","a.u.");
  gPad->SetLogy();
  h1JESCS->Scale(1./h1JESCS->Integral(),"width");
  h1JESCSFull->Scale(1./h1JESCSFull->Integral(),"width");
  
  h1JESCS->SetLineColor(GetColor(0));
  h1JESCS->SetMarkerColor(GetColor(0));
  h1JESCS->SetLineWidth(3);


  h1JESCSFull->SetLineColor(GetColor(1));
  h1JESCSFull->SetMarkerColor(GetColor(1));
  h1JESCSFull->SetLineWidth(3);
  
  h1JESCS->Draw("same hist");
  h1JESCSFull->Draw("same hist");

  //return;
  
  TCanvas *c1 = new TCanvas(Form("c1_%d",0),Form("c1_%d",0),500,400);

  TH1F *fr1 = DrawFrame(-10.,10.,1e-3,0.4,"Log(#tau_{f}^{decluster}) (fm/c)","1/N dN/dLog(#tau_{f}^{decluster})");

  h1LogTfSig->SetLineColor(GetColor(0));
  h1LogTfSig->SetMarkerColor(GetColor(0));
  h1LogTfSig->SetLineWidth(3);


  h1LogTfSigCh->SetLineColor(GetColor(1));
  h1LogTfSigCh->SetMarkerColor(GetColor(1));
  h1LogTfSigCh->SetLineWidth(3);
  
  h1LogTfSig->Draw("same hist");
  h1LogTfSigCh->Draw("same hist");

  TLegend *leg1a = CreateLegend(0.25, 0.6, 0.65, 0.92);
  leg1a->AddEntry(h1LogTfSig,"charged+neutral, p_{T,jet}>130 GeV","l");
  leg1a->AddEntry(h1LogTfSigCh,"charged, p_{T,jet}>100 GeV","l");
  leg1a->Draw();

  c1->SaveAs("TfSigSigCh.png");
  
  TCanvas *c2 = new TCanvas(Form("c2_%d",0),Form("c2_%d",0),500,400);

  TH1F *fr2 = DrawFrame(-10.,10.,1e-3,0.4,"Log(#tau_{f}^{decluster}) (fm/c)","1/N dN/dLog(#tau_{f}^{decluster})");

  h1LogTfSig->SetLineColor(GetColor(0));
  h1LogTfSig->SetMarkerColor(GetColor(0));
  h1LogTfSig->SetLineWidth(3);

  h1LogTfCS->SetLineColor(GetColor(1));
  h1LogTfCS->SetMarkerColor(GetColor(1));
  h1LogTfCS->SetLineWidth(3);
 
  h1LogTfCSFull->SetLineColor(GetColor(3));
  h1LogTfCSFull->SetMarkerColor(GetColor(3));
  h1LogTfCSFull->SetLineWidth(3);
    
  h1LogTfCS->Draw("same hist");
  h1LogTfCSFull->Draw("same hist");
  h1LogTfSig->Draw("same hist");


  DrawLatex(0.26,0.87,"PYTHIA8 #sqrt{s}=5.02 TeV");
  DrawLatex(0.26,0.81,"anti-k_{T} R=0.5");
  DrawLatex(0.26,0.75,"#tau declustering");
  DrawLatex(0.26,0.69,Form("z_{cut}=%.1f  #beta=0",zcut));

  TLegend *leg2 = CreateLegend(0.6, 0.8, 0.5, 0.82, "p_{T,jet}>130 GeV");
  leg2->AddEntry(h1LogTfSig,"truth","l");
  leg2->AddEntry(h1LogTfCS,"CS jet-by-jet","l");
  leg2->AddEntry(h1LogTfCSFull,"CS full event","l");
  leg2->Draw();

  c2->SaveAs("TfSigCSCSFull.png");


  //correlation to parton split
  TCanvas *c3 = new TCanvas(Form("c3_%d",0),Form("c3_%d",0),500,400);

  TH1F *fr3 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster} [no bkg] (fm/c)");
  gPad->SetLogz();
  h2PartonSigTf->Draw("colz same");

  c3->SaveAs("TfPartonSig.png");

  TCanvas *c4 = new TCanvas(Form("c4_%d",0),Form("c4_%d",0),500,400);

  TH1F *fr4 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster} [CS jet-by-jet] (fm/c)");
  gPad->SetLogz();
  h2PartonCSTf->Draw("colz same");

  c4->SaveAs("TfPartonCS.png");

  TCanvas *c5 = new TCanvas(Form("c5_%d",0),Form("c5_%d",0),500,400);

  TH1F *fr5 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster} [CS full-event] (fm/c)");
  gPad->SetLogz();
  h2PartonCSFullTf->Draw("colz same");

  c5->SaveAs("TfPartonCSFull.png");
  
  TCanvas *c6 = new TCanvas(Form("c6_%d",0),Form("c6_%d",0),500,400);

  TH1F *fr6 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{parton split} (fm/c)","#tau_{f}^{decluster} [no bkg, ch jet] (fm/c)");
  gPad->SetLogz();
  h2PartonSigChTf->Draw("colz same");

  c6->SaveAs("TfPartonSigCh.png");



  
  //correlation to signal jet
  TCanvas *c7 = new TCanvas(Form("c7_%d",0),Form("c7_%d",0),500,400);

  TH1F *fr7 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{decluster} [truth] (fm/c)","#tau_{f}^{decluster} [CS jet-by-jet] (fm/c)");
  gPad->SetLogz();
  h2SigCSTf->Draw("colz same");

  c7->SaveAs("TfSigCS.png");

  TCanvas *c8 = new TCanvas(Form("c8_%d",0),Form("c8_%d",0),500,400);

  TH1F *fr8 = DrawFrame(0.,1.,0.,1.,"#tau_{f}^{decluster} [truth] (fm/c)","#tau_{f}^{decluster} [CS full-event] (fm/c)");
  gPad->SetLogz();
  h2SigCSFullTf->Draw("colz same");

  c8->SaveAs("TfSigCSFull.png");
  
  //Tf scale vs Rg
  TCanvas *c9 = new TCanvas(Form("c9_%d",0),Form("c9_%d",0),500,400);

  TH1F *fr9 = DrawFrame(0.,2.,0.,0.5,"CS jet-by-jet/truth [#tau_{f}^{decluster}] ","R_{g}");
  gPad->SetLogz();
  h2TfScaleRgCS->Draw("colz same");

  c9->SaveAs("TfSigCSScaleRg.png");

  TCanvas *c10 = new TCanvas(Form("c10_%d",0),Form("c10_%d",0),500,400);

  TH1F *fr10 = DrawFrame(0.,2.,0.,0.5,"CS full-event/truth [#tau_{f}^{decluster}] ","R_{g}");
  gPad->SetLogz();
  h2TfScaleRgCSFull->Draw("colz same");

  c10->SaveAs("TfSigCSFullScaleRg.png");

  return;
  
  // DrawLatex(0.5,0.87,"PYTHIA8 #sqrt{s}=5.02 TeV",0.06,2);
  // DrawLatex(0.5,0.81,"anti-k_{T} R=0.5");
  // DrawLatex(0.5,0.75,"#tau declustering");
  // DrawLatex(0.5,0.69,"z_{cut}=0.1  #beta=0");

}
