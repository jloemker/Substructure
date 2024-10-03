#!/bin/bash

reset

inputs=( 
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/001/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/002/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/003/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/004/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/005/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/006/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/007/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/008/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/009/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/010/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/011/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/012/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/013/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/014/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/015/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/016/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/017/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/018/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/019/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/020/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/021/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/022/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/023/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/024/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/025/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/026/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/027/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/028/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/029/AO2D.root"
"alien:///alice/sim/2023/LHC23k4g/0/539908/AOD/030/AO2D.root"
)

Cfg="--configuration json://configRUN1MC.json -b" 

rm -rf inputsMerge.list

function runSpec {
        o2-analysis-mccollision-converter $Cfg |o2-analysis-timestamp $Cfg | o2-analysis-track-propagation $Cfg | o2-analysis-trackselection $Cfg | o2-analysis-event-selection $Cfg | o2-analysis-multiplicity-table $Cfg | o2-analysis-pid-tpc-base $Cfg | o2-analysis-pid-tpc $Cfg | o2-analysis-pid-tof-base $Cfg | o2-analysis-pid-tof $Cfg | o2-analysis-pid-tof-full $Cfg | o2-analysistutorial-mm-my-task $Cfg

        mv AnalysisResults.root AnalysisResults_$1.root
        echo AnalysisResults_$1.root >> inputsMerge.list
    }
    
i=0 

function merge {
	hadd -fk FinalResults.root @inputsMerge.list
}

for cut in "${inputs[@]}"; do
    echo $cut > inputsMC.txt
    runSpec $i
    ((i++))
    echo $i
done

merge 
mv inputsMerge.list inputsMergeFinal.list
