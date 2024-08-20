#!/bin/bash
#reset

# these cuts will be automatically converted to a) generate the config file and b) run the function runSpec
PbSets=( 
    "Pb_1hard"
    "Pb_1hard_short"
    "Pb_1hard_intermed"
    "Pb_1hard_large"
    "Pb_2soft"
    "Pb_2soft_short"
    "Pb_2soft_intermed"
    "Pb_2soft_large"
    "Pb_trans"
    "Pb_trans_short"
    "Pb_trans_intermed"
    "Pb_trans_large"
)

PPSets=( 
    "PP_1hard"
    "PP_1hard_short"
    "PP_1hard_intermed"
    "PP_1hard_large"
    "PP_2soft"
    "PP_2soft_short"
    "PP_2soft_intermed"
    "PP_2soft_large"
    "PP_trans"
    "PP_trans_short"
    "PP_trans_intermed"
    "PP_trans_large"
)

./generateConfigPb.sh "${PbSets[@]}"
OPTIONSPb="--configuration json://generated_configPb.json -b" 

function runSpecPb {
        o2-analysis-je-jet-deriveddata-producer $OPTIONSPb | o2-analysis-tracks-extra-converter $OPTIONSPb | o2-analysis-bc-converter $OPTIONSPb | o2-analysis-je-jet-finder-data-charged $OPTIONSPb | o2-analysis-timestamp $OPTIONSPb | o2-analysis-event-selection $OPTIONSPb | o2-analysis-trackselection $OPTIONSPb | o2-analysis-multiplicity-table $OPTIONSPb | o2-analysis-centrality-table $OPTIONSPb | o2-analysis-track-propagation $OPTIONSPb | o2-analysis-je-jet-lund-reclustering $OPTIONSPb --workflow-suffix $1
        mv AnalysisResults.root AnalysisResults_$1.root
	    mv dpl-config.json dpl-config-Pb_$1.json
    }

./generateConfigPP.sh "${PPSets[@]}"
OPTIONSPP="--configuration json://generated_configPP.json -b" 

function runSpecPP {
        o2-analysis-je-jet-deriveddata-producer $OPTIONSPP | o2-analysis-je-jet-finder-data-charged $OPTIONSPP | o2-analysis-timestamp $OPTIONSPP | o2-analysis-event-selection $OPTIONSPP | o2-analysis-trackselection $OPTIONSPP | o2-analysis-multiplicity-table $OPTIONSPP | o2-analysis-centrality-table $OPTIONSPP | o2-analysis-track-propagation $OPTIONSPP | o2-analysis-je-jet-lund-reclustering $OPTIONSPP --workflow-suffix $1
        mv AnalysisResults.root AnalysisResults_$1.root
	    mv dpl-config.json dpl-config-PP_$1.json
    }

for cut in "${PbSets[@]}"; do
    echo "Pb cut in createConfig: "$cut
    runSpecPb $cut
done

for cut in "${PPSets[@]}"; do
    echo "PP cut in createConfig: "$cut
    runSpecPP $cut
done

