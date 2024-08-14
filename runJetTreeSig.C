#include "jetTreeSig.C"

void runJetTreeSig() {
  // gROOT->LoadMacro("");
  jetTreeSig oldtree;
  oldtree.Loop(true, "Tau", "short");
  /*
        if(reduced==true && generator=="CA"){
          if(!(sigTfsZ.size()>0)) continue;
          if(duration == "supershort" && (sigTfsZ[0] >= 0.5)) continue
          if(duration == "short" && (sigTfsZ[0] > 1)) continue;
          if(duration == "large" && (sigTfsZ[0] < 3)) continue;
          if(duration == "intermed" && ((sigTfsZ[0] < 1) || (sigTfsZ[0] > 3))) continue;
        }
        if(reduced==true && generator=="Tau"){
          if(!(sigTfsZTau.size()>0)) continue;
          if(duration == "supershort" && (sigTfsZ[0] >= 0.5)) continue
          if(duration == "short" && (sigTfsZTau[0] > 1)) continue;
          if(duration == "large" && (sigTfsZTau[0] < 3)) continue;
          if(duration == "intermed" && ((sigTfsZTau[0] < 1) || (sigTfsZTau[0] > 3))) continue;
        
  */
}
