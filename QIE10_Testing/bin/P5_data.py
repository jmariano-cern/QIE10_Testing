import os
import FWCore.ParameterSet.Config as cms
from getpass import getuser as user
import subprocess
import sys

process = cms.Process("ReflectionAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#
#   Command Line Input(Copied from DQM for now)
#
import sys
#if ((len(sys.argv) < 3) or (len(sys.argv) > 5)):
#    print "### ERROR: Bad Call!"
#    print "### Use: cmsRun QIE10_testing.py <run number> <suite_code (see QIE10_testing.info)> <dimensionality_of_scan>"
#    sys.exit(1)

#
#   Change the filename to process
#

isCopiedLocal = False
if 'Local' in sys.argv:
    isCopiedLocal = True
    sys.argv.remove('Local')

runNumber = sys.argv[2]



#if runNumber not in os.listdir('../img'):
#    os.makedirs('../img/' + runNumber)

if not isCopiedLocal:
    cmd = 'echo "cd /eos/cms/store/group/dpg_hcal/comm_hcal/USC/" | eos && echo "ls ." | eos'

    eos_runs = subprocess.check_output(cmd, shell=True)

    if (eos_runs.find(str(runNumber)) == -1):
        print "ERROR: run " + str(runNumber) + " not found in /eos/cms/store/group/dpg_hcal/comm_hcal/USC/"
        sys.exit()
    
    process.source = cms.Source("HcalTBSource",
        fileNames = cms.untracked.vstring(
            'root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run' + runNumber + '/USC_'+ runNumber + '.root'
        )
    )

else:
    process.source = cms.Source("HcalTBSource",
        fileNames = cms.untracked.vstring(
            'file:/afs/cern.ch/work/' + user()[0] + '/' + user() + '/USC_'+ runNumber + '.root'
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
                                   1118, 1119,
                                   1120, 1121,
                                   1122, 1123,
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

process.hcalAnalyzer = cms.EDAnalyzer('QIE10_testing',
        OutFileName = cms.untracked.string('$QIE10ROOT/dat/QIE10testing_'+runNumber+'_'+str(suite_code)+'.root'),
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
                #file = cms.FileInPath('ngHCAL/QIE10_Testing/cfg/emap.txt')
                file = cms.FileInPath('ngHCAL/QIE10_Testing/cfg/ngHF_emap.txt')
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

