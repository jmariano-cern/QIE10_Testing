import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("ReflectionAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )

#
#   Command Line Input(Copied from DQM for now)
#
import sys
if ((len(sys.argv) < 3) or (len(sys.argv) > 5)):
    print "### ERROR: Bad Call!"
    print "### Use: cmsRun h2testbeamanalyzer_cfg.py <run number> <suite_code -- (optional)>"
    sys.exit(1)

#
#   Change the filename to process
#
runNumber = sys.argv[2]

#if runNumber not in os.listdir('../img'):
#    os.makedirs('../img/' + runNumber)

#process.source = cms.Source("HcalTBSource",
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/afs/cern.ch/work/j/jmariano/public/904_runs/B904_Integration_'+runNumber+'.root'
        #'/store/data/Run2016B/Tau/USER/LogErrorMonitor-PromptReco-v2/000/273/150/00000/F03041EC-D719-E611-94B4-02163E013760.root'
        #'/store/data/Run2016B/JetHT/RAW/v2/000/275/309/00000/30B76086-B534-E611-A108-02163E0135C7.root'        
        #'/store/data/Run2016B/JetHT/RAW/v2/000/275/309/00000/1EC2D35B-E734-E611-B26F-02163E014393.root'
        #'/store/data/Run2016B/JetHT/RAW/v2/000/275/310/00000/0430BB00-BB35-E611-8343-02163E0135D2.root'
        'root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run' + runNumber + '/USC_'+ runNumber + '.root'
    )
)

process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(False)
        )

#process.tbunpack = cms.EDProducer("HcalTBObjectUnpacker",
#        IncludeUnmatchedHits = cms.untracked.bool(False),
#        ConfigurationFile=cms.untracked.string('HFcommissioning/Analysis/test/configQADCTDC.txt'),
#        HcalSlowDataFED = cms.untracked.int32(14),
#        HcalTriggerFED = cms.untracked.int32(1),
#        HcalTDCFED = cms.untracked.int32(8),
#        HcalQADCFED = cms.untracked.int32(8),
#        fedRawDataCollectionTag = cms.InputTag('source')
#)

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
                                   #       UnpackHF = cms.untracked.bool(True),
                                   ### Falg to enable unpacking of TTP channels(default = false)
                                   ### UnpackTTP = cms.untracked.bool(True),
                                   FilterDataQuality = cms.bool(False),
#                                   InputLabel = cms.InputTag('source'),
                                   InputLabel = cms.InputTag('rawDataCollector'),
#                                   HcalFirstFED = cms.untracked.int32(928), 
                                   HcalFirstFED = cms.untracked.int32(700), 
                                   ComplainEmptyData = cms.untracked.bool(False),
                                   #       UnpackCalib = cms.untracked.bool(True),
                                   FEDs = cms.untracked.vint32(
                                   # 1118, 1120, 1122, 
                                   1132,
                                   61,63,
                                   700, 701, 702, 703, 704,
                                   705, 706, 707, 708, 709,
                                   710, 711, 712, 713, 714,
                                   715, 716, 717, 718, 719,
                                   720, 721, 722, 723, 724,
                                   725, 726, 727, 728, 729,
                                   730, 731, 732,
                                   1118, 1119, # c22
                                   1120, 1121, # c29
                                   1122, 1123, # c32
                                   ),
                                   firstSample = cms.int32(0),
#                                   lastSample = cms.int32(10)
                                   lastSample = cms.int32(14)
                                   )


suite_code = 0
if len(sys.argv) > 3:
    suite_code = int(sys.argv[3])

sequencer_flag = 0
#if len(sys.argv) == 5:
#    sequencer_flag = int(sys.argv[4])

process.hcalAnalyzer = cms.EDAnalyzer('QIE10_testing',
#        OutFileName = cms.untracked.string('../dat/QIE10testing_'+runNumber+'_'+str(suite_code)+'.root'),
        OutFileName = cms.untracked.string('../dat/P5_data.root'),
        Verbosity = cms.untracked.int32(0),
        Suite_Code = cms.untracked.int32(suite_code),
        Sequencer_Flag = cms.untracked.int32(sequencer_flag)
)

#
#   For Debugging: Create a Pool Output Module
#
#process.output = cms.OutputModule(
#        'PoolOutputModule',
#        fileName = cms.untracked.string('HFanalysis_'+runNumber+'.root')
#)

process.load('Configuration.Geometry.GeometryIdeal_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
#from CondCore.DBCommon.CondDBSetup_cfi import *
from CondCore.CondDB.CondDB_cfi import *

process.GlobalTag.globaltag = autoCond['startup'] 

#   EMAP Needed for H2 DATA
process.es_ascii = cms.ESSource('HcalTextCalibrations',
        input = cms.VPSet(
               cms.PSet(
                object = cms.string('ElectronicsMap'),
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP_hardware_C53_HF23.txt')
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP_hardware_C53.txt')
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP-C50_fiber.txt')
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP-1CH.txt')
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP-1CH2.txt')
                #file = cms.FileInPath('HFcommissioning/QIE10_Testing/cfg/EMAP_QIE10_2016.txt')
                file = cms.FileInPath('ngHCAL/QIE10_Testing/cfg/HF_emap_full.txt')
                )
        )
)

process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

process.dump = cms.EDAnalyzer("HcalDigiDump")

process.p = cms.Path(process.hcalDigis
#                     *process.dump
                     *process.hcalAnalyzer
                     )
#process.outpath = cms.EndPath(process.output)

