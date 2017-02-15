import os
import FWCore.ParameterSet.Config as cms
from getpass import getuser as user

process = cms.Process("ReflectionAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

import sys

runNumber = sys.argv[2]

process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
        #'file:/afs/cern.ch/work/d/dnoonan/public/904_runs/B904_Integration_'+runNumber+'.root'
        'root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run' + runNumber + '/USC_'+ runNumber + '.root'
    )
)

process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(False)
        )

process.tbunpack = cms.EDProducer("HcalTBObjectUnpacker",
        IncludeUnmatchedHits = cms.untracked.bool(False),
        ConfigurationFile=cms.untracked.string('HFcommissioning/Analysis/test/configQADCTDC.txt'),
        HcalSlowDataFED = cms.untracked.int32(14),
        HcalTriggerFED = cms.untracked.int32(1),
        HcalTDCFED = cms.untracked.int32(8),
        HcalQADCFED = cms.untracked.int32(8),
        fedRawDataCollectionTag = cms.InputTag('source')
)

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
                                   #       UnpackHF = cms.untracked.bool(True),
                                   ### Falg to enable unpacking of TTP channels(default = false)
                                   ### UnpackTTP = cms.untracked.bool(True),
                                   FilterDataQuality = cms.bool(False),
                                   InputLabel = cms.InputTag('source'),
                                   HcalFirstFED = cms.untracked.int32(928),
                                   ComplainEmptyData = cms.untracked.bool(False),
                                   #       UnpackCalib = cms.untracked.bool(True),
                                   FEDs = cms.untracked.vint32(
                                   928,930,932,934,1194,1196,
                                   1102,1104,1100,1106,1116,1108,1114,1110,1112,1134
                                   ),
                                   firstSample = cms.int32(0),
                                   lastSample = cms.int32(10)
                                   )


suite_code = 0
if len(sys.argv) > 3:
    suite_code = int(sys.argv[3])

sequencer_flag = 0
if len(sys.argv) == 5:
    sequencer_flag = int(sys.argv[4])

process.hcalAnalyzer = cms.EDAnalyzer('QIE11_testing',
        OutFileName = cms.untracked.string('../dat/QIE11testing_'+runNumber+'_'+str(suite_code)+'.root'),
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
                #file = cms.FileInPath('HFcommissioning/QIE11_Testing/cfg/EMAP_hardware_C53_HF23.txt')
                #file = cms.FileInPath('HFcommissioning/QIE11_Testing/cfg/EMAP_hardware_C53.txt')
                #file = cms.FileInPath('HFcommissioning/QIE11_Testing/cfg/EMAP-C50_fiber.txt')
                #file = cms.FileInPath('HFcommissioning/QIE11_Testing/cfg/EMAP-1CH.txt')
                #file = cms.FileInPath('HFcommissioning/QIE11_Testing/cfg/EMAP-1CH2.txt')
                file = cms.FileInPath('ngHCAL/QIE11_Testing/cfg/emap.txt')
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

