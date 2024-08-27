#!/bin/bash
#reset

# these cuts will be automatically converted to a) generate the config file and b) run the function runSpec
PbSets=( 
    "Pb_3single"
    "Pb_3single_short"
    "Pb_3single_intermed"
    "Pb_3single_large"
)

PPSets=( 
    "PP_3single"
    "PP_3single_short"
    "PP_3single_intermed"
    "PP_3single_large"
)

./generateConfigPb.sh "${PbSets[@]}"
OPTIONSPb="--configuration json://generated_configPb.json -b" 

function runSpecPb {
        o2-analysis-je-jet-deriveddata-producer $OPTIONSPb | o2-analysis-tracks-extra-converter $OPTIONSPb | o2-analysis-bc-converter $OPTIONSPb | o2-analysis-je-jet-finder-data-charged $OPTIONSPb | o2-analysis-timestamp $OPTIONSPb | o2-analysis-event-selection $OPTIONSPb | o2-analysis-trackselection $OPTIONSPb | o2-analysis-multiplicity-table $OPTIONSPb | o2-analysis-centrality-table $OPTIONSPb | o2-analysis-track-propagation $OPTIONSPb | o2-analysis-je-jet-lund-reclustering $OPTIONSPb --workflow-suffix $1
        mv AnalysisResults.root AnalysisResults_$1.root
	    mv dpl-config.json dpl-config-$1.json
    }

./generateConfigPP.sh "${PPSets[@]}"
OPTIONSPP="--configuration json://generated_configPP.json -b" 

function runSpecPP {
        o2-analysis-je-jet-deriveddata-producer $OPTIONSPP | o2-analysis-je-jet-finder-data-charged $OPTIONSPP | o2-analysis-timestamp $OPTIONSPP | o2-analysis-event-selection $OPTIONSPP | o2-analysis-trackselection $OPTIONSPP | o2-analysis-multiplicity-table $OPTIONSPP | o2-analysis-centrality-table $OPTIONSPP | o2-analysis-track-propagation $OPTIONSPP | o2-analysis-je-jet-lund-reclustering $OPTIONSPP --workflow-suffix $1
        mv AnalysisResults.root AnalysisResults_$1.root
	    mv dpl-config.json dpl-config-$1.json
    }

for cut in "${PbSets[@]}"; do
    echo "Pb cut in createConfig: "$cut
    runSpecPb $cut
done

for cut in "${PPSets[@]}"; do
    echo "PP cut in createConfig: "$cut
    runSpecPP $cut
done

