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

./generateConfig.sh "${PbSets[@]}"
OPTIONS="--configuration json://generated_config.json -b" # this is produced with generateConfig.sh

function runSpecPb {
        o2-analysis-je-jet-deriveddata-producer $OPTIONS | o2-analysis-tracks-extra-converter $OPTIONS | o2-analysis-bc-converter $OPTIONS | o2-analysis-je-jet-finder-data-charged $OPTIONS | o2-analysis-timestamp $OPTIONS | o2-analysis-event-selection $OPTIONS | o2-analysis-trackselection $OPTIONS | o2-analysis-multiplicity-table $OPTIONS | o2-analysis-centrality-table $OPTIONS | o2-analysis-track-propagation $OPTIONS | o2-analysis-je-jet-lund-reclustering $OPTIONS --workflow-suffix $1
        mv AnalysisResults.root AnalysisResults_Pb_$1.root
	mv dpl-config.json dpl-config-Pb_$1.json
    }

for cut in "${PbSets[@]}"; do
    echo "cut in createConfig: "$cut
    runSpecPb $cut
done

