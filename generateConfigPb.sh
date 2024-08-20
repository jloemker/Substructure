#!/bin/bash

use_cuts=("$@")
if [ ${#use_cuts[@]} -eq 0 ]; then
    echo "ERROR: No cuts provided. Please provide cuts in runCutVar.sh script"
    exit 1
fi

#Function to generate the standard configuration
generate_standard_config() {
    local configFile="generated_configPb.json"
    cat << EOF > "$configFile"
{
    "internal-dpl-clock": "",
    "internal-dpl-aod-reader": {
        "aod-file-private": "..\/AO2D_LHC22q_pass4_lowIR_PbPb.root",
        "time-limit": "0",
        "orbit-offset-enumeration": "0",
        "orbit-multiplier-enumeration": "0",
        "start-value-enumeration": "0",
        "end-value-enumeration": "-1",
        "step-value-enumeration": "1"
    },
    "internal-dpl-aod-spawner": "",
    "bc-converter": "",
    "internal-dpl-aod-index-builder": "",
    "timestamp-task": {
        "verbose": "false",
        "rct-path": "RCT\/Info\/RunInformation",
        "orbit-reset-path": "CTP\/Calib\/OrbitReset",
        "ccdb-url": "http:\/\/alice-ccdb.cern.ch",
        "isRun2MC": "false"
    },
    "tracks-extra-converter": "",
    "bc-selection-task": {
        "triggerBcShift": "999",
        "ITSROFrameStartBorderMargin": "-1",
        "ITSROFrameEndBorderMargin": "-1",
        "TimeFrameStartBorderMargin": "-1",
        "TimeFrameEndBorderMargin": "-1",
        "processRun2": "false",
        "processRun3": "true"
    },
    "tracks-extra-spawner": "",
    "track-propagation": {
        "ccdb-url": "http:\/\/alice-ccdb.cern.ch",
        "lutPath": "GLO\/Param\/MatLUT",
        "geoPath": "GLO\/Config\/GeometryAligned",
        "grpmagPath": "GLO\/Config\/GRPMagField",
        "mVtxPath": "GLO\/Calib\/MeanVertex",
        "minPropagationDistance": "83.0999985",
        "useTrackTuner": "false",
        "fillTrackTunerTable": "false",
        "trackTunerParams": "debugInfo=0|updateTrackDCAs=1|updateTrackCovMat=1|updateCurvature=0|updateCurvatureIU=0|updatePulls=0|isInputFileFromCCDB=1|pathInputFile=Users\/m\/mfaggin\/test\/inputsTrackTuner\/PbPb2022|nameInputFile=trackTuner_DataLHC22sPass5_McLHC22l1b2_run529397.root|pathFileQoverPt=Users\/h\/hsharma\/qOverPtGraphs|nameFileQoverPt=D0sigma_Data_removal_itstps_MC_LHC22b1b.root|usePvRefitCorrections=0|qOverPtMC=-1.|qOverPtData=-1.",
        "axisPtQA": {
            "values": [
                "0",
                "0",
                "0.10000000149011612",
                "0.20000000298023224",
                "0.30000001192092896",
                "0.40000000596046448",
                "0.5",
                "0.60000002384185791",
                "0.69999998807907104",
                "0.80000001192092896",
                "0.89999997615814209",
                "1",
                "1.1000000238418579",
                "1.2000000476837158",
                "1.2999999523162842",
                "1.3999999761581421",
                "1.5",
                "1.6000000238418579",
                "1.7000000476837158",
                "1.7999999523162842",
                "1.8999999761581421",
                "2",
                "2.2000000476837158",
                "2.4000000953674316",
                "2.5999999046325684",
                "2.7999999523162842",
                "3",
                "3.2000000476837158",
                "3.4000000953674316",
                "3.5999999046325684",
                "3.7999999523162842",
                "4",
                "4.4000000953674316",
                "4.8000001907348633",
                "5.1999998092651367",
                "5.5999999046325684",
                "6",
                "6.5",
                "7",
                "7.5",
                "8",
                "9",
                "10",
                "11",
                "12",
                "13",
                "14",
                "15",
                "17",
                "19",
                "21",
                "23",
                "25",
                "30",
                "35",
                "40",
                "50"
            ]
        },
        "processStandard": "false",
        "processStandardWithPID": "false",
        "processCovarianceMc": "false",
        "processCovariance": "true",
        "processCovarianceWithPID": "false"
    },
    "event-selection-task": {
        "syst": "PbPb",
        "muonSelection": "0",
        "maxDiffZvtxFT0vsPV": "1",
        "isMC": "false",
        "TimeIntervalForOccupancyCalculationMin": "-40",
        "TimeIntervalForOccupancyCalculationMax": "100",
        "TimeBinsForOccupancyCalculation": {
            "values": [
                "-40",
                "-20",
                "0",
                "25",
                "50",
                "75",
                "100"
            ]
        },
        "ReferenceOccupanciesInTimeBins": {
            "values": [
                "3000",
                "1400",
                "750",
                "1000",
                "1750",
                "4000"
            ]
        },
        "TimeRangeVetoOnCollStandard": "10",
        "TimeRangeVetoOnCollNarrow": "4",
        "UseWeightsForOccupancyEstimator": "true",
        "processRun2": "false",
        "processRun3": "true"
    },
    "track-selection": {
        "isRun3": "true",
        "produceTable": "-1",
        "produceFBextendedTable": "-1",
        "compatibilityIU": "false",
        "itsMatching": "0",
        "dcaSetup": "0",
        "ptMin": "0.100000001",
        "ptMax": "1e+10",
        "etaMin": "-0.800000012",
        "etaMax": "0.800000012"
    },
    "multiplicity-table": {
        "doVertexZeq": "1",
        "fractionOfEvents": "2",
        "enabledTables": {
            "labels_rows": [
                "FV0Mults",
                "FT0Mults",
                "FDDMults",
                "ZDCMults",
                "TrackletMults",
                "TPCMults",
                "PVMults",
                "MultsExtra",
                "MultSelections",
                "FV0MultZeqs",
                "FT0MultZeqs",
                "FDDMultZeqs",
                "PVMultZeqs",
                "MultsExtraMC"
            ],
            "labels_cols": [
                "Enable"
            ],
            "values": [
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ]
            ]
        },
        "ccdburl": "http:\/\/alice-ccdb.cern.ch",
        "ccdbpath": "Centrality\/Calibration",
        "produceHistograms": "false",
        "processRun2": "false",
        "processRun3": "true",
        "processGlobalTrackingCounters": "false",
        "processMC": "false"
    },
    "centrality-table": {
        "enabledTables": {
            "labels_rows": [
                "CentRun2V0Ms",
                "CentRun2V0As",
                "CentRun2SPDTrks",
                "CentRun2SPDClss",
                "CentRun2CL0s",
                "CentRun2CL1s",
                "CentFV0As",
                "CentFT0Ms",
                "CentFT0As",
                "CentFT0Cs",
                "CentFDDMs",
                "CentNTPVs"
            ],
            "labels_cols": [
                "Enable"
            ],
            "values": [
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ],
                [
                    "-1"
                ]
            ]
        },
        "ccdburl": "http:\/\/alice-ccdb.cern.ch",
        "ccdbpath": "Centrality\/Estimators",
        "genname": "",
        "doNotCrashOnNull": "false",
        "embedINELgtZEROselection": "false",
        "fatalizeMultCalibSanity": "false",
        "produceHistograms": "false",
        "binsPercentile": {
            "values": [
                "0",
                "0",
                "0.001",
                "0.002",
                "0.0030000000000000001",
                "0.0040000000000000001",
                "0.0050000000000000001",
                "0.0060000000000000001",
                "0.0070000000000000001",
                "0.0080000000000000002",
                "0.0089999999999999993",
                "0.01",
                "0.010999999999999999",
                "0.012",
                "0.012999999999999999",
                "0.014",
                "0.014999999999999999",
                "0.016",
                "0.017000000000000001",
                "0.017999999999999999",
                "0.019",
                "0.02",
                "0.021000000000000001",
                "0.021999999999999999",
                "0.023",
                "0.024",
                "0.025000000000000001",
                "0.025999999999999999",
                "0.027",
                "0.028000000000000001",
                "0.029000000000000001",
                "0.029999999999999999",
                "0.031",
                "0.032000000000000001",
                "0.033000000000000002",
                "0.034000000000000002",
                "0.035000000000000003",
                "0.035999999999999997",
                "0.036999999999999998",
                "0.037999999999999999",
                "0.039",
                "0.040000000000000001",
                "0.041000000000000002",
                "0.042000000000000003",
                "0.042999999999999997",
                "0.043999999999999997",
                "0.044999999999999998",
                "0.045999999999999999",
                "0.047",
                "0.048000000000000001",
                "0.049000000000000002",
                "0.050000000000000003",
                "0.050999999999999997",
                "0.051999999999999998",
                "0.052999999999999999",
                "0.053999999999999999",
                "0.055",
                "0.056000000000000001",
                "0.057000000000000002",
                "0.058000000000000003",
                "0.058999999999999997",
                "0.059999999999999998",
                "0.060999999999999999",
                "0.062",
                "0.063",
                "0.064000000000000001",
                "0.065000000000000002",
                "0.066000000000000003",
                "0.067000000000000004",
                "0.068000000000000005",
                "0.069000000000000006",
                "0.070000000000000007",
                "0.070999999999999994",
                "0.071999999999999995",
                "0.072999999999999995",
                "0.073999999999999996",
                "0.074999999999999997",
                "0.075999999999999998",
                "0.076999999999999999",
                "0.078",
                "0.079000000000000001",
                "0.080000000000000002",
                "0.081000000000000003",
                "0.082000000000000003",
                "0.083000000000000004",
                "0.084000000000000005",
                "0.085000000000000006",
                "0.085999999999999993",
                "0.086999999999999994",
                "0.087999999999999995",
                "0.088999999999999996",
                "0.089999999999999997",
                "0.090999999999999998",
                "0.091999999999999998",
                "0.092999999999999999",
                "0.094",
                "0.095000000000000001",
                "0.096000000000000002",
                "0.097000000000000003",
                "0.098000000000000004",
                "0.099000000000000005",
                "0.10000000000000001",
                "0.11",
                "0.12",
                "0.13",
                "0.14000000000000001",
                "0.14999999999999999",
                "0.16",
                "0.17000000000000001",
                "0.17999999999999999",
                "0.19",
                "0.20000000000000001",
                "0.20999999999999999",
                "0.22",
                "0.23000000000000001",
                "0.23999999999999999",
                "0.25",
                "0.26000000000000001",
                "0.27000000000000002",
                "0.28000000000000003",
                "0.28999999999999998",
                "0.29999999999999999",
                "0.31",
                "0.32000000000000001",
                "0.33000000000000002",
                "0.34000000000000002",
                "0.34999999999999998",
                "0.35999999999999999",
                "0.37",
                "0.38",
                "0.39000000000000001",
                "0.40000000000000002",
                "0.40999999999999998",
                "0.41999999999999998",
                "0.42999999999999999",
                "0.44",
                "0.45000000000000001",
                "0.46000000000000002",
                "0.46999999999999997",
                "0.47999999999999998",
                "0.48999999999999999",
                "0.5",
                "0.51000000000000001",
                "0.52000000000000002",
                "0.53000000000000003",
                "0.54000000000000004",
                "0.55000000000000004",
                "0.56000000000000005",
                "0.56999999999999995",
                "0.57999999999999996",
                "0.58999999999999997",
                "0.59999999999999998",
                "0.60999999999999999",
                "0.62",
                "0.63",
                "0.64000000000000001",
                "0.65000000000000002",
                "0.66000000000000003",
                "0.67000000000000004",
                "0.68000000000000005",
                "0.68999999999999995",
                "0.69999999999999996",
                "0.70999999999999996",
                "0.71999999999999997",
                "0.72999999999999998",
                "0.73999999999999999",
                "0.75",
                "0.76000000000000001",
                "0.77000000000000002",
                "0.78000000000000003",
                "0.79000000000000004",
                "0.80000000000000004",
                "0.81000000000000005",
                "0.81999999999999995",
                "0.82999999999999996",
                "0.83999999999999997",
                "0.84999999999999998",
                "0.85999999999999999",
                "0.87",
                "0.88",
                "0.89000000000000001",
                "0.90000000000000002",
                "0.91000000000000003",
                "0.92000000000000004",
                "0.93000000000000005",
                "0.93999999999999995",
                "0.94999999999999996",
                "0.95999999999999996",
                "0.96999999999999997",
                "0.97999999999999998",
                "0.98999999999999999",
                "1",
                "1.1000000000000001",
                "1.2",
                "1.3",
                "1.3999999999999999",
                "1.5",
                "1.6000000000000001",
                "1.7",
                "1.8",
                "1.8999999999999999",
                "2",
                "2.1000000000000001",
                "2.2000000000000002",
                "2.2999999999999998",
                "2.3999999999999999",
                "2.5",
                "2.6000000000000001",
                "2.7000000000000002",
                "2.7999999999999998",
                "2.8999999999999999",
                "3",
                "3.1000000000000001",
                "3.2000000000000002",
                "3.2999999999999998",
                "3.3999999999999999",
                "3.5",
                "3.6000000000000001",
                "3.7000000000000002",
                "3.7999999999999998",
                "3.8999999999999999",
                "4",
                "4.0999999999999996",
                "4.2000000000000002",
                "4.2999999999999998",
                "4.4000000000000004",
                "4.5",
                "4.5999999999999996",
                "4.7000000000000002",
                "4.7999999999999998",
                "4.9000000000000004",
                "5",
                "5.0999999999999996",
                "5.2000000000000002",
                "5.2999999999999998",
                "5.4000000000000004",
                "5.5",
                "5.5999999999999996",
                "5.7000000000000002",
                "5.7999999999999998",
                "5.9000000000000004",
                "6",
                "6.0999999999999996",
                "6.2000000000000002",
                "6.2999999999999998",
                "6.4000000000000004",
                "6.5",
                "6.5999999999999996",
                "6.7000000000000002",
                "6.7999999999999998",
                "6.9000000000000004",
                "7",
                "7.0999999999999996",
                "7.2000000000000002",
                "7.2999999999999998",
                "7.4000000000000004",
                "7.5",
                "7.5999999999999996",
                "7.7000000000000002",
                "7.7999999999999998",
                "7.9000000000000004",
                "8",
                "8.0999999999999996",
                "8.1999999999999993",
                "8.3000000000000007",
                "8.4000000000000004",
                "8.5",
                "8.5999999999999996",
                "8.6999999999999993",
                "8.8000000000000007",
                "8.9000000000000004",
                "9",
                "9.0999999999999996",
                "9.1999999999999993",
                "9.3000000000000007",
                "9.4000000000000004",
                "9.5",
                "9.5999999999999996",
                "9.6999999999999993",
                "9.8000000000000007",
                "9.9000000000000004",
                "10",
                "11",
                "12",
                "13",
                "14",
                "15",
                "16",
                "17",
                "18",
                "19",
                "20",
                "21",
                "22",
                "23",
                "24",
                "25",
                "26",
                "27",
                "28",
                "29",
                "30",
                "31",
                "32",
                "33",
                "34",
                "35",
                "36",
                "37",
                "38",
                "39",
                "40",
                "41",
                "42",
                "43",
                "44",
                "45",
                "46",
                "47",
                "48",
                "49",
                "50",
                "51",
                "52",
                "53",
                "54",
                "55",
                "56",
                "57",
                "58",
                "59",
                "60",
                "61",
                "62",
                "63",
                "64",
                "65",
                "66",
                "67",
                "68",
                "69",
                "70",
                "71",
                "72",
                "73",
                "74",
                "75",
                "76",
                "77",
                "78",
                "79",
                "80",
                "81",
                "82",
                "83",
                "84",
                "85",
                "86",
                "87",
                "88",
                "89",
                "90",
                "91",
                "92",
                "93",
                "94",
                "95",
                "96",
                "97",
                "98",
                "99",
                "100"
            ]
        },
        "processRun2": "false",
        "processRun3": "true",
        "processRun3FT0": "false"
    },
    "jet-deriveddata-producer": {
        "processBunchCrossings": "false",
        "processCollisions": "true",
        "processCollisionsWithoutCentralityAndMultiplicity": "false",
        "processCollisionsRun2": "false",
        "processCollisionsALICE3": "false",
        "processMcCollisionLabels": "false",
        "processMcCollisions": "false",
        "processTracks": "true",
        "processMcTrackLabels": "false",
        "processParticles": "false",
        "processClusters": "false",
        "processMcClusterLabels": "false",
        "processD0Collisions": "false",
        "processD0McCollisions": "false",
        "processD0": "false",
        "processD0MC": "false",
        "processLcCollisions": "false",
        "processLcMcCollisions": "false",
        "processLc": "false",
        "processLcMC": "false",
        "processV0": "false",
        "processV0MC": "false",
        "processDielectronCollisions": "false",
        "processDielectron": "false",
        "processDielectronMc": "false"
    },
    "jet-finder-data-charged": {
        "vertexZCut": "10",
        "centralityMin": "-999",
        "centralityMax": "999",
        "trackPtMin": "0.150000006",
        "trackPtMax": "1000",
        "trackEtaMin": "-0.899999976",
        "trackEtaMax": "0.899999976",
        "trackPhiMin": "-999",
        "trackPhiMax": "999",
        "trackingEfficiency": "1",
        "trackSelections": "globalTracks",
        "eventSelections": "sel8",
        "particleSelections": "PhysicalPrimary",
        "clusterDefinition": "kV3Default",
        "clusterEtaMin": "-0.709999979",
        "clusterEtaMax": "0.709999979",
        "clusterPhiMin": "1.38999999",
        "clusterPhiMax": "3.26999998",
        "clusterEnergyMin": "0.5",
        "clusterTimeMin": "-25",
        "clusterTimeMax": "25",
        "clusterRejectExotics": "true",
        "doEMCALEventSelection": "true",
        "jetRadius": {
            "values": [
                "0.3"
            ]
        },
        "jetPtMin": "5",
        "jetPtMax": "200",
        "jetEWSPtMin": "0",
        "jetEWSPtMax": "1000",
        "jetEtaMin": "-0.5",
        "jetEtaMax": "0.5",
        "jetAlgorithm": "2",
        "jetRecombScheme": "0",
        "jetGhostArea": "0.00499999989",
        "ghostRepeat": "1",
        "DoTriggering": "false",
        "jetAreaFractionMin": "-99",
        "jetPtBinWidth": "1",
        "fillTHnSparse": "true",
        "processChargedJets": "true",
        "processChargedEvtWiseSubJets": "false",
        "processNeutralJets": "false",
        "processFullJets": "false",
        "processParticleLevelChargedJets": "false",
        "processParticleLevelNeutralJets": "false",
        "processParticleLevelFullJets": "false"
    },
    "jet-lund-reclustering": {
        "eventSelections": "sel8",
        "Nsub": "2",
        "Zcut": "0.10000000000000001",
        "Beta": "0",
        "Tfcut_min": "0",
        "Tfcut_max": "20",
        "Rgcut_trans": "0.10000000000000001",
        "Tf_limit": "0",
        "jetPtMin": "5",
        "jetR": "0.300000012",
        "jet_min_eta": "-0.5",
        "jet_max_eta": "0.5",
        "genKTp": "0.5",
        "vertexZCut": "10",
        "Nsplits": "3",
        "pt-binning": {
            "values": [
                "0",
                "0.10000000000000001",
                "0.20000000000000001",
                "0.29999999999999999",
                "0.40000000000000002",
                "0.5",
                "0.59999999999999998",
                "0.69999999999999996",
                "0.80000000000000004",
                "0.90000000000000002",
                "1",
                "1.1000000000000001",
                "1.2",
                "1.3",
                "1.3999999999999999",
                "1.5",
                "1.6000000000000001",
                "1.7",
                "1.8",
                "1.8999999999999999",
                "2",
                "2.2000000000000002",
                "2.3999999999999999",
                "2.6000000000000001",
                "2.7999999999999998",
                "3",
                "3.2000000000000002",
                "3.3999999999999999",
                "3.6000000000000001",
                "3.7999999999999998",
                "4",
                "4.4000000000000004",
                "4.7999999999999998",
                "5.2000000000000002",
                "5.5999999999999996",
                "6",
                "6.5",
                "7",
                "7.5",
                "8",
                "9",
                "10",
                "11",
                "12",
                "13",
                "14",
                "15",
                "17",
                "19",
                "21",
                "23",
                "25",
                "30",
                "35",
                "40",
                "50",
                "60",
                "70",
                "80",
                "90",
                "100",
                "115",
                "130",
                "145",
                "160",
                "180",
                "200"
            ]
        },
        "phi-binning": {
            "values": [
                "90",
                "0",
                "6.2831853071795862"
            ]
        },
        "eta-binning": {
            "values": [
                "90",
                "-0.90000000000000002",
                "0.90000000000000002"
            ]
        },
        "lnkt-binning": {
            "values": [
                "10",
                "-10",
                "10"
            ]
        },
        "Delta-R-binning": {
            "values": [
                "50",
                "0",
                "0.5"
            ]
        },
        "NSub-Ratio-binning": {
            "values": [
                "50",
                "0",
                "1.2000000476837158"
            ]
        },
        "zg-binning": {
            "values": [
                "50",
                "0",
                "1.2"
            ]
        },
        "erad-binning": {
            "values": [
                "1000",
                "0",
                "10000"
            ]
        },
        "lnDeltaRBinning-binning": {
            "values": [
                "50",
                "-5",
                "10"
            ]
        },
        "lnzTheta-binning": {
            "values": [
                "50",
                "-10",
                "10"
            ]
        },
        "tf-binning": {
            "values": [
                "100",
                "0",
                "22"
            ]
        },
        "lnTf-binning": {
            "values": [
                "100",
                "10",
                "10"
            ]
        },
        "processChargedJets": "true"
    },
EOF
    echo "$configFile"
}

#Function to append to the standard configuration the configuration for each single cut
append_config_for_cut() {
    local cutName=$1
    echo "local var: cutName "$cutName
    local configFile=$2
    local tf_limit="0"
    local tfcut_max="20"
    local tfcut_min="0"
    local configSectionName="jet-lund-reclustering$cutName" 
    echo "local section name: "$configSectionName
    if [[ $cutName = *'trans'* ]]; then
        echo 'transition of limits'
        tf_limit="0"
        if [[ $cutName = *'large'* ]]; then
            echo 'large time'
            tfcut_max="20"
            tfcut_min="3"
        elif [[ $cutName = *'intermed'* ]]; then
            echo 'intermedite time'
            tfcut_max="3"
            tfcut_min="1"
        elif [[ $cutName = *'short'* ]]; then
            echo 'short time'
            tfcut_max="1"
            tfcut_min="0"
        fi
    elif [[ $cutName = *'1hard'* ]]; then
        echo 'hard limit'
        tf_limit="1"
        if [[ $cutName = *'large'* ]]; then
            echo 'large time'
            tfcut_max="20"
            tfcut_min="3"
        elif [[ $cutName = *'intermed'* ]]; then
            echo 'intermedite time'
            tfcut_max="3"
            tfcut_min="1"
        elif [[ $cutName = *'short'* ]]; then
            echo 'short time'
            tfcut_max="1"
            tfcut_min="0"
        fi
    elif [[ $cutName = *'2soft'* ]]; then
        echo 'soft limit'
        tf_limit="2"
        if [[ $cutName = *'large'* ]]; then
            echo 'large time'
            tfcut_max="20"
            tfcut_min="3"
        elif [[ $cutName = *'intermed'* ]]; then
            echo 'intermedite time'
            tfcut_max="3"
            tfcut_min="1"
        elif [[ $cutName = *'short'* ]]; then
            echo 'short time'
            tfcut_max="1"
            tfcut_min="0"
        fi
    fi
    sed "/\"jet-lund-reclustering\"/,\$!d; s/\"jet-lund-reclustering\"/\"$configSectionName\"/; s/\"Tf_limit\": \"[^\"]*\"/\"Tf_limit\": \"$tf_limit\"/; s/\"Tfcut_max\": \"[^\"]*\"/\"Tfcut_max\": \"$tfcut_max\"/; s/\"Tfcut_min\": \"[^\"]*\"/\"Tfcut_min\": \"$tfcut_min\"/" "$configFile" >> "cut_config.json"
}

#Function to finalize the overall configuration
finalize_config_file() {
    local configFile=$1
    cat "cut_config.json" >> "$configFile"
    rm "cut_config.json"
    cat << EOF >> "$configFile"
    "internal-dpl-aod-writer": "",
    "internal-dpl-aod-global-analysis-file-sink": "",
    "internal-dpl-injected-dummy-sink": ""
}
EOF
}

#Generate the standard configuration
configFile=$(generate_standard_config)

#Append each cut variation to the configuration
for cut in "${use_cuts[@]}"; do
    IFS='=' read -r cutName <<< "$cut"
    echo "cut in generateConfig: "$cut
    echo "cutName in generateConfig: "$cutName
    append_config_for_cut "$cutName" "$configFile"
done

#Finalize the configuration file
finalize_config_file "$configFile"

