import os
import re

##### IO FILES ######

cfgfile_name = '../cfg/QIE10_testing.cfg'
initfile_name = './QIE10_init.h'
loopfile_name = './QIE10_loop.h'
infofile_name = '../bin/QIE10_testing.info'

##### DEFINITIONS #####

# definitions[type][0] = 'type' name
# definitions[type][1] = array of parameter names for 'type'
definitions = [

    ['null'       , ['name'] ] ,
    ['TH1F_perEV' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex'] ] ,
    ['TH2F_perEV' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex' , 'nbinsy' , 'lowy' , 'highy' , 'titley'] ] ,
    ['TH1F_perCH' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex'] ] ,
    ['TH2F_perCH' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex' , 'nbinsy' , 'lowy' , 'highy' , 'titley'] ] ,
    ['TH1F_PerTS' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex'] ] ,
    ['TH2F_PerTS' , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex' , 'nbinsy' , 'lowy' , 'highy' , 'titley'] ] ,
    ['TProfile'   , ['name' , 'nbinsx' , 'lowx' , 'highx' , 'titlex' , 'nbinsy' , 'lowy' , 'highy' , 'titley'] ] ,
    ['TTree'      , ['name'] ] ,
    ['logger'     , ['name' , 'log_file'] ] 

]

# maps parameter name to c++ type
cpp_types = {

    'name'     : 'char'  ,
    'nbinsx'   : 'int'   ,
    'lowx'     : 'float' ,
    'highx'    : 'float' ,
    'titlex'   : 'char'  ,
    'nbinsy'   : 'int'   ,
    'lowy'     : 'float' ,
    'highy'    : 'float' ,
    'titley'   : 'char'  ,
    'log_file' : 'char'
    
}

# types of blocks
block_definitions = [

    'loop_vars'       , # non-local variables to be used in loops
    'pre_event_loop'  , # runs once per event
    'pre_loop'        , # runs once per channel per event
    'loop'            , # runs once per timeslice per channel per event
    'post_loop'       , # runs once per channel per event (after the TS loop)
    'post_event_loop'   # runs once per event (after TS loop)

]

# ordered parameters for branches
branch_parameters = [

    'name'  , 
    'type'  ,
    'scope' ,

]

# possible scopes for branch data
branch_scopes = [

    'perEvent'   , # one entry per event - link to pre_outer_loop
    'perChannel' , # one entry per channel per event - link to pre_loop
    'perTS'        # one entry per channel per timeslice per event - link to loop

]

# ttree branch data type for branch declarations
ttree_types = {

    'int'    : 'I' ,
    'double' : 'D' ,
    'bool'   : 'O'

}

##### PRINTING FUNCTIONS #####

def get_num_tests(suite_code,test_num):
    num_out = 0
    for j in range(len(data[test_num])):
        if suite_code in data[test_num][j][2]:
            num_out = num_out + 1
    return num_out        

def print_header(infile,infile_name):
    infile.write('// #############################################################\n')
    infile.write('// #############################################################\n')
    infile.write('// ###' + ''.center(55) + '###\n')
    infile.write('// ###' + infile_name.center(55) + '###\n')
    infile.write('// ###' + ''.center(55) + '###\n')
    infile.write('// ###' + 'This file was written by a robot!'.center(55) + '###\n')
    infile.write('// ###' + 'Do not edit by hand!'.center(55) + '###\n')
    infile.write('// ###' + ''.center(55) + '###\n')
    infile.write('// ###' + 'Instead, modify the configuration file:'.center(55) + '###\n')
    infile.write('// ###' + ' QIE10_testing.cfg'.center(55) + '###\n')
    infile.write('// ###' + 'and then rebuild with:'.center(55) + '###\n')
    infile.write('// ###' + './rebuild'.center(55) + '###\n')
    infile.write('// ###' + ''.center(55) + '###\n')
    infile.write('// ###' + 'Joseph Mariano -- jmariano@cern.ch'.center(55) + '###\n')
    infile.write('// ###' + '27-04-2016'.center(55) + '###\n')
    infile.write('// ###' + ''.center(55) + '###\n')
    infile.write('// #############################################################\n')
    infile.write('// #############################################################\n')
    infile.write('\n')

def print_info():

    os.system('rm ./' + infofile_name)
    os.system('touch ./' + infofile_name)
    infofile = open(infofile_name,'w')

    infofile.write('#############'.center(55) + '\n')
    infofile.write('#############'.center(55) + '\n')
    infofile.write('### TESTS ###'.center(55) + '\n')
    infofile.write('#############'.center(55) + '\n')
    infofile.write('#############'.center(55) + '\n')
    infofile.write('\n')
    infofile.write('\n')
    for i in range(len(definitions)):
        infofile.write('#########################################################\n')        
        infofile.write('#' + (definitions[i][0] + 's').center(55) + '#\n')
        infofile.write('#########################################################\n')        
        infofile.write('#' +''.center(55) + '#\n')
        for j in range(len(data[i])):
            infofile.write('#' + data[i][j][0][0].center(55) + '#\n')
        infofile.write('#########################################################\n')        
        infofile.write('\n')
        infofile.write('\n')
    infofile.write('##############'.center(55) + '\n')
    infofile.write('##############'.center(55) + '\n')
    infofile.write('### SUITES ###'.center(55) + '\n')
    infofile.write('##############'.center(55) + '\n')
    infofile.write('##############'.center(55) + '\n')
    infofile.write('\n')
    infofile.write('\n')
    for s in range(len(suites)):
        infofile.write('#########################################################\n')        
        infofile.write('#' + ('SUITE ' + str(suites[s][0]) + ': ' + suites[s][1]).center(55) + '#\n')
        infofile.write('#########################################################\n')        
        for i in range(len(definitions)):
            num_in_suite = 0
            if get_num_tests(suites[s][0],i) > 0:
                infofile.write('#' + (definitions[i][0] + 's').center(55,'#') + '#\n')                    
                infofile.write('#########################################################\n')
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    infofile.write('# ' + str(num_in_suite).ljust(2) + ': ' + (data[i][j][0][0]).center(46) + '    #\n')
                    num_in_suite = num_in_suite + 1
            if get_num_tests(suites[s][0],i) > 0:        
                infofile.write('#########################################################\n')
        infofile.write('\n')
        infofile.write('\n')
    infofile.close()

def print_loop():

    os.system('rm ./' + loopfile_name)
    os.system('touch ./' + loopfile_name)
    loopfile = open(loopfile_name,'w')

    print_header(loopfile, loopfile_name)

    loopfile.write('#include <vector>\n')
    loopfile.write('#include <fstream>\n')
    loopfile.write('#include <sstream>\n')
    loopfile.write('#include <iostream>\n')
    loopfile.write('#include <iomanip>\n')
    loopfile.write('#include <math.h>\n')
    loopfile.write('\n')
    loopfile.write('#include "TH1D.h"\n')
    loopfile.write('#include "TH2D.h"\n')
    loopfile.write('#include "TProfile.h"\n')
    loopfile.write('\n')
    loopfile.write('using namespace std;\n')
    loopfile.write('\n')
    loopfile.write('// ########################\n')
    loopfile.write('// # LOOP_VARS STRUCTURES #\n')
    loopfile.write('// ########################\n')
    loopfile.write('\n')
    s = 0
    loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
    loopfile.write('\n')
    loopfile.write('struct loop_vars {\n')
    loopfile.write('\n')
    for i in range(len(data)):
        for j in range(len(data[i])):
            if suites[s][0] in data[i][j][2]:
                for k in range(len(block_definitions)):
                    if block_definitions[k] == 'loop_vars':
                        for line in data[i][j][1][k]:
                            loopfile.write(line + '\n')                              
    loopfile.write('\n')
    loopfile.write('};\n')
    loopfile.write('\n')
    loopfile.write('// ###################\n')
    loopfile.write('// # PRE_EVENT_LOOPS #\n')
    loopfile.write('// ###################\n')
    loopfile.write('\n')
    loopfile.write('loop_vars pre_event_loop(vector<std::string> parameter, vector<float> val, int suite_code, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {\n')
    loopfile.write('\n')
    loopfile.write('  loop_vars global;\n')
    loopfile.write('\n')
    for s in range(len(suites)):
        loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        loopfile.write('\n')
        loopfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        loopfile.write('\n')
        for i in range(len(data)):
            test_num_in_suite = 0
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(block_definitions)):
                        if block_definitions[k] == 'pre_event_loop':
                            for line in data[i][j][1][k]:
                                if ((definitions[i][0] == 'TTree') and (line.strip() == '<fill>')):
                                    loopfile.write('  ' + line.split('<fill>')[0] + '_trees[' + str(test_num_in_suite) + ']->Fill();\n')
                                elif line.find('<set_branch>') != -1:
                                    loopfile.write('    _qie10Info.' + line.replace('<set_branch>','').split('=')[0].strip()[1:-1] + '_' + data[i][j][0][0][1:-1] + '[nCH] = ' + line.replace('<set_branch>','').split('=')[1].strip() + ';\n')
                                elif re.search('<branch=".*">',line) is not None:
                                    loopfile.write('  ' + re.sub('<branch=".*">','_qie10Info.' + re.search('<branch=".*">',line).group().split('"')[1] + '_' + data[i][j][0][0][1:-1],line) + '\n')
                                elif line.find('<log>') != -1:
                                    loopfile.write('  ' + line.replace('<log>','loggers[' + str(test_num_in_suite) + '] <<') + '\n')
                                else:
                                    loopfile.write('  ' + line.replace('<fill_perEV>',definitions[i][0]+'s['+str(test_num_in_suite)+']->Fill').replace('<fill_perCH>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nCH]->Fill').replace('<fill_perTS>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nTS][nCH]->Fill') + '\n')
                    test_num_in_suite = test_num_in_suite + 1
        loopfile.write('\n')
        loopfile.write('  }\n')
        loopfile.write('\n')
    loopfile.write('  return global;\n')
    loopfile.write('\n')
    loopfile.write('}\n')
    loopfile.write('\n')
    loopfile.write('// #############\n')
    loopfile.write('// # PRE_LOOPS #\n')
    loopfile.write('// #############\n')
    loopfile.write('\n')
    loopfile.write('loop_vars pre_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE10DataFrame digis, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {\n')
    loopfile.write('\n')
    for s in range(len(suites)):
        loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        loopfile.write('\n')
        loopfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        loopfile.write('\n')
        for i in range(len(data)):
            test_num_in_suite = 0
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(block_definitions)):
                        if block_definitions[k] == 'pre_loop':
                            for line in data[i][j][1][k]:
                                if ((definitions[i][0] == 'TTree') and (line.strip() == '<fill>')):
                                    loopfile.write('  ' + line.split('<fill>')[0] + '_trees[' + str(test_num_in_suite) + ']->Fill();\n')
                                elif line.find('<set_branch>') != -1:
                                    loopfile.write('    _qie10Info.' + line.replace('<set_branch>','').split('=')[0].strip()[1:-1] + '_' + data[i][j][0][0][1:-1] + '[nCH] = ' + line.replace('<set_branch>','').split('=')[1].strip() + ';\n')
                                elif re.search('<branch=".*">',line) is not None:
                                    loopfile.write('  ' + re.sub('<branch=".*">','_qie10Info.' + re.search('<branch=".*">',line).group().split('"')[1] + '_' + data[i][j][0][0][1:-1],line) + '\n')
                                elif line.find('<log>') != -1:
                                    loopfile.write('  ' + line.replace('<log>','loggers[' + str(test_num_in_suite) + '] <<') + '\n')
                                else:
                                    loopfile.write('  ' + line.replace('<fill_perEV>',definitions[i][0]+'s['+str(test_num_in_suite)+']->Fill').replace('<fill_perCH>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nCH]->Fill').replace('<fill_perTS>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nTS][nCH]->Fill') + '\n')
                    test_num_in_suite = test_num_in_suite + 1
        loopfile.write('\n')
        loopfile.write('  }\n')
        loopfile.write('\n')
    loopfile.write('  return global;\n')
    loopfile.write('\n')
    loopfile.write('}\n')
    loopfile.write('\n')
    loopfile.write('// #########\n')
    loopfile.write('// # LOOPS #\n')
    loopfile.write('// #########\n')
    loopfile.write('\n')
    loopfile.write('loop_vars loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE10DataFrame digis, int nTS, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {\n')
    loopfile.write('\n')
    for s in range(len(suites)):
        loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        loopfile.write('\n')
        loopfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        loopfile.write('\n')
        for i in range(len(data)):
            test_num_in_suite = 0
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(block_definitions)):
                        if block_definitions[k] == 'loop':
                            for line in data[i][j][1][k]:
                                if ((definitions[i][0] == 'TTree') and (line.strip() == '<fill>')):
                                    loopfile.write('  ' + line.split('<fill>')[0] + '_trees[' + str(test_num_in_suite) + ']->Fill();\n')
                                elif line.find('<set_branch>') != -1:
                                    loopfile.write('    _qie10Info.' + line.replace('<set_branch>','').split('=')[0].strip()[1:-1] + '_' + data[i][j][0][0][1:-1] + '[nTS][nCH] = ' + line.replace('<set_branch>','').split('=')[1].strip() + ';\n')
                                elif re.search('<branch=".*">',line) is not None:
                                    loopfile.write('  ' + re.sub('<branch=".*">','_qie10Info.' + re.search('<branch=".*">',line).group().split('"')[1] + '_' + data[i][j][0][0][1:-1],line) + '\n')
                                elif line.find('<log>') != -1:
                                    loopfile.write('  ' + line.replace('<log>','loggers[' + str(test_num_in_suite) + '] <<') + '\n')
                                else:
                                    loopfile.write('  ' + line.replace('<fill_perEV>',definitions[i][0]+'s['+str(test_num_in_suite)+']->Fill').replace('<fill_perCH>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nCH]->Fill').replace('<fill_perTS>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nTS][nCH]->Fill') + '\n')
                    test_num_in_suite = test_num_in_suite + 1
        loopfile.write('\n')
        loopfile.write('  }\n')
        loopfile.write('\n')
    loopfile.write('  return global;\n')
    loopfile.write('\n')
    loopfile.write('}\n')
    loopfile.write('\n')
    loopfile.write('// ##############\n')
    loopfile.write('// # POST_LOOPS #\n')
    loopfile.write('// ##############\n')
    loopfile.write('\n')
    loopfile.write('loop_vars post_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, QIE10DataFrame digis, int nCH, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {\n')
    loopfile.write('\n')
    for s in range(len(suites)):
        loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        loopfile.write('\n')
        loopfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        loopfile.write('\n')
        for i in range(len(data)):
            test_num_in_suite = 0
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(block_definitions)):
                        if block_definitions[k] == 'post_loop':
                            for line in data[i][j][1][k]:
                                if ((definitions[i][0] == 'TTree') and (line.strip() == '<fill>')):
                                    loopfile.write('  ' + line.split('<fill>')[0] + '_trees[' + str(test_num_in_suite) + ']->Fill();\n')
                                elif line.find('<set_branch>') != -1:
                                    loopfile.write('    _qie10Info.' + line.replace('<set_branch>','').split('=')[0].strip()[1:-1] + '_' + data[i][j][0][0][1:-1] + '[nCH] = ' + line.replace('<set_branch>','').split('=')[1].strip() + ';\n')
                                elif re.search('<branch=".*">',line) is not None:
                                    loopfile.write('  ' + re.sub('<branch=".*">','_qie10Info.' + re.search('<branch=".*">',line).group().split('"')[1] + '_' + data[i][j][0][0][1:-1],line) + '\n')
                                elif line.find('<log>') != -1:
                                    loopfile.write('  ' + line.replace('<log>','loggers[' + str(test_num_in_suite) + '] <<') + '\n')
                                else:
                                    loopfile.write('  ' + line.replace('<fill_perEV>',definitions[i][0]+'s['+str(test_num_in_suite)+']->Fill').replace('<fill_perCH>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nCH]->Fill').replace('<fill_perTS>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nTS][nCH]->Fill') + '\n')
                    test_num_in_suite = test_num_in_suite + 1
        loopfile.write('\n')
        loopfile.write('  }\n')
        loopfile.write('\n')
    loopfile.write('  return global;\n')
    loopfile.write('\n')
    loopfile.write('}\n')
    loopfile.write('\n')
    loopfile.write('// ####################\n')
    loopfile.write('// # POST_EVENT_LOOPS #\n')
    loopfile.write('// ####################\n')
    loopfile.write('\n')
    loopfile.write('void post_event_loop(vector<std::string> parameter, vector<float> val, int suite_code, loop_vars global, int _event_num, TQIE10Info &_qie10Info, TTree** _trees, vector<TH1F*> &TH1F_perEVs, vector<vector<TH1F*> > &TH1F_perCHs, vector<vector<vector<TH1F*> > > &TH1F_PerTSs, vector<TH2F*> &TH2F_perEVs, vector<vector<TH2F*> > &TH2F_perCHs, vector<vector<vector<TH2F*> > > &TH2F_PerTSs, vector<vector<TH2F*> > &TProfiles, ofstream *loggers) {\n')
    loopfile.write('\n')
    for s in range(len(suites)):
        loopfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        loopfile.write('\n')
        loopfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        loopfile.write('\n')
        for i in range(len(data)):
            test_num_in_suite = 0
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(block_definitions)):
                        if block_definitions[k] == 'post_event_loop':
                            for line in data[i][j][1][k]:
                                if ((definitions[i][0] == 'TTree') and (line.strip() == '<fill>')):
                                    loopfile.write('  ' + line.split('<fill>')[0] + '_trees[' + str(test_num_in_suite) + ']->Fill();\n')
                                elif line.find('<set_branch>') != -1:
                                    loopfile.write('    _qie10Info.' + line.replace('<set_branch>','').split('=')[0].strip()[1:-1] + '_' + data[i][j][0][0][1:-1] + '[nCH] = ' + line.replace('<set_branch>','').split('=')[1].strip() + ';\n')
                                elif re.search('<branch=".*">',line) is not None:
                                    loopfile.write('  ' + re.sub('<branch=".*">','_qie10Info.' + re.search('<branch=".*">',line).group().split('"')[1] + '_' + data[i][j][0][0][1:-1],line) + '\n')
                                elif line.find('<log>') != -1:
                                    loopfile.write('  ' + line.replace('<log>','loggers[' + str(test_num_in_suite) + '] <<') + '\n')
                                else:
                                    loopfile.write('  ' + line.replace('<fill_perEV>',definitions[i][0]+'s['+str(test_num_in_suite)+']->Fill').replace('<fill_perCH>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nCH]->Fill').replace('<fill_perTS>',definitions[i][0]+'s['+str(test_num_in_suite)+'][nTS][nCH]->Fill') + '\n')
                    test_num_in_suite = test_num_in_suite + 1
        loopfile.write('\n')
        loopfile.write('  }\n')
        loopfile.write('\n')
    loopfile.write('}\n')
    loopfile.write('\n')
    loopfile.close()

def print_init():

    os.system('rm ./' + initfile_name)
    os.system('touch ./' + initfile_name)
    initfile = open(initfile_name,'w')

    print_header(initfile,initfile_name)

    initfile.write('#include <vector>\n')
    initfile.write('#include <fstream>\n')
    initfile.write('#include <sstream>\n')
    initfile.write('#include <iostream>\n')
    initfile.write('#include <iomanip>\n')
    initfile.write('\n')
    initfile.write('#include "TH1D.h"\n')
    initfile.write('#include "TH2D.h"\n')
    initfile.write('#include "TProfile.h"\n')
    initfile.write('\n')
    initfile.write('// #########\n')
    initfile.write('// # TESTS #\n')
    initfile.write('// #########\n')
    initfile.write('\n')
    for i in range(len(data)):
        if (definitions[i][0] != 'null'):
            initfile.write('// #####  ' + definitions[i][0] + 's  #####\n')
            initfile.write('\n')
            for j in range(len(data[i])):
                initfile.write('// --- ' + definitions[i][0] + ' ' + str(j) + ' ---\n')
                for k in range(len(definitions[i][1])):
                    initfile.write('#define ' + definitions[i][0].upper() + '_' + str(j) + '_' + definitions[i][1][k].upper() + ' ' + data[i][j][0][k] + '\n')
                initfile.write('\n')
    initfile.write('// ##########\n')
    initfile.write('// # SUITES #\n')
    initfile.write('// ##########\n')
    initfile.write('\n')
    for s in range(len(suites)):
        initfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        for i in range(len(data)):
            num_tests = get_num_tests(suites[s][0],i)
            initfile.write('#define SUITE' + suites[s][0] + '_' + definitions[i][0].upper() + '_NUM ' + str(num_tests) + '\n')
            if ((num_tests > 0) and (definitions[i][0] != 'null')):
                for k in range(len(definitions[i][1])):
                    initfile.write('#define SUITE' + suites[s][0] + '_' + definitions[i][0].upper() + '_' + definitions[i][1][k].upper() + ' ')
                    for j in range(len(data[i])):
                        if suites[s][0] in data[i][j][2]:
                            initfile.write(definitions[i][0].upper() + '_' + str(j) + '_' + definitions[i][1][k].upper() + ' , ')
                    initfile.write('\n')
        initfile.write('\n')
    initfile.write('// #######################\n')
    initfile.write('// # TREE DATA STRUCTURE #\n')
    initfile.write('// #######################\n')
    initfile.write('\n')
    s = 0
    initfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
    initfile.write('\n')
    initfile.write('struct TQIE10Info\n')
    initfile.write('{\n')
    initfile.write('\n')
    initfile.write('  int numChs;\n')
    initfile.write('  int numTS;\n')
    for i in range(len(definitions)):
        if definitions[i][0] == 'TTree':
            for j in range(len(data[i])):
                if suites[s][0] in data[i][j][2]:
                    for k in range(len(data[i][j][3])):
                        for l in range(len(branch_parameters)):
                            if branch_parameters[l] == 'type':
                                initfile.write('  ' + data[i][j][3][k][l] + ' ')
                        for l in range(len(branch_parameters)):
                            if branch_parameters[l] == 'scope':
                                if data[i][j][3][k][l] == 'perChannel':
                                    initfile.write('*')
                                if data[i][j][3][k][l] == 'perTS':
                                    initfile.write('**')
                        for l in range(len(branch_parameters)):
                            if branch_parameters[l] == 'name':
                                initfile.write(data[i][j][3][k][l][1:-1] + '_' + data[i][j][0][0][1:-1] + ';\n')
    initfile.write('\n')
    initfile.write('};\n')
    initfile.write('\n')
    initfile.write('// #################\n')
    initfile.write('// # INIT FUNCTION #\n')
    initfile.write('// #################\n')
    initfile.write('\n')
    initfile.write('using namespace std;\n')
    initfile.write('\n')
    initfile.write('void init(int suite_code, int tTS, int nCH,\n')
    initfile.write('           ofstream*& loggers, int &num_loggers, vector<string> &logger_name, vector<string> &logger_log_file,\n')
    initfile.write('          TFile* _file, TTree**& _trees, TQIE10Info &_qie10Info, vector<string> &TTree_name, int &num_TTrees,\n')
    initfile.write('          vector<TH1F*> &TH1F_perEVs, int &num_TH1F_perEVs, vector<string> &TH1F_perEV_name, vector<int> &TH1F_perEV_nbinsx, vector<float> &TH1F_perEV_lowx, vector<float> &TH1F_perEV_highx, vector<string> &TH1F_perEV_titlex,\n')
    initfile.write('          vector<vector<TH1F*> > &TH1F_perCHs, int &num_TH1F_perCHs, vector<string> &TH1F_perCH_name, vector<int> &TH1F_perCH_nbinsx, vector<float> &TH1F_perCH_lowx, vector<float> &TH1F_perCH_highx, vector<string> &TH1F_perCH_titlex,\n')
    initfile.write('          vector<vector<vector<TH1F*> > > &TH1F_PerTSs, int &num_TH1F_PerTSs, vector<string> &TH1F_PerTS_name, vector<int> &TH1F_PerTS_nbinsx, vector<float> &TH1F_PerTS_lowx, vector<float> &TH1F_PerTS_highx, vector<string> &TH1F_PerTS_titlex,\n')
    initfile.write('          vector<TH2F*> &TH2F_perEVs, int &num_TH2F_perEVs, vector<string> &TH2F_perEV_name, vector<int> &TH2F_perEV_nbinsx, vector<float> &TH2F_perEV_lowx, vector<float> &TH2F_perEV_highx, vector<string> &TH2F_perEV_titlex, vector<int> &TH2F_perEV_nbinsy, vector<float> &TH2F_perEV_lowy, vector<float> &TH2F_perEV_highy, vector<string> &TH2F_perEV_titley,\n')
    initfile.write('          vector<vector<TH2F*> > &TH2F_perCHs, int &num_TH2F_perCHs, vector<string> &TH2F_perCH_name, vector<int> &TH2F_perCH_nbinsx, vector<float> &TH2F_perCH_lowx, vector<float> &TH2F_perCH_highx, vector<string> &TH2F_perCH_titlex, vector<int> &TH2F_perCH_nbinsy, vector<float> &TH2F_perCH_lowy, vector<float> &TH2F_perCH_highy, vector<string> &TH2F_perCH_titley,\n')
    initfile.write('          vector<vector<vector<TH2F*> > > &TH2F_PerTSs, int &num_TH2F_PerTSs, vector<string> &TH2F_PerTS_name, vector<int> &TH2F_PerTS_nbinsx, vector<float> &TH2F_PerTS_lowx, vector<float> &TH2F_PerTS_highx, vector<string> &TH2F_PerTS_titlex, vector<int> &TH2F_PerTS_nbinsy, vector<float> &TH2F_PerTS_lowy, vector<float> &TH2F_PerTS_highy, vector<string> &TH2F_PerTS_titley,\n')
    initfile.write('          vector<vector<TH2F*> > &TProfiles, int &num_TProfiles, vector<string> &TProfile_name, vector<int> &TProfile_nbinsx, vector<float> &TProfile_lowx, vector<float> &TProfile_highx, vector<string> &TProfile_titlex, vector<int> &TProfile_nbinsy, vector<float> &TProfile_lowy, vector<float> &TProfile_highy, vector<string> &TProfile_titley){\n')
    header = ''
    header = header + 'void init(int suite_code, int tTS, int nCH,\n'
    header = header + '          ofstream*& loggers, int &num_loggers, vector<string> &logger_name, vector<string> &logger_log_file,\n'
    header = header + '          TFile* _file, TTree**& _trees, TQIE10Info &_qie10Info, vector<string> &TTree_name, int &num_TTrees,\n'
    header = header + '          vector<TH1F*> &TH1F_perEVs, int &num_TH1F_perEVs, vector<string> &TH1F_perEV_name, vector<int> &TH1F_perEV_nbinsx, vector<float> &TH1F_perEV_lowx, vector<float> &TH1F_perEV_highx, vector<string> &TH1F_perEV_titlex,\n'
    header = header + '          vector<vector<TH1F*> > &TH1F_perCHs, int &num_TH1F_perCHs, vector<string> &TH1F_perCH_name, vector<int> &TH1F_perCH_nbinsx, vector<float> &TH1F_perCH_lowx, vector<float> &TH1F_perCH_highx, vector<string> &TH1F_perCH_titlex,\n' 
    header = header + '          vector<vector<vector<TH1F*> > > &TH1F_PerTSs, int &num_TH1F_PerTSs, vector<string> &TH1F_PerTS_name, vector<int> &TH1F_PerTS_nbinsx, vector<float> &TH1F_PerTS_lowx, vector<float> &TH1F_PerTS_highx, vector<string> &TH1F_PerTS_titlex,\n'
    header = header + '          vector<TH2F*> &TH2F_perEVs, int &num_TH2F_perEVs, vector<string> &TH2F_perEV_name, vector<int> &TH2F_perEV_nbinsx, vector<float> &TH2F_perEV_lowx, vector<float> &TH2F_perEV_highx, vector<string> &TH2F_perEV_titlex, vector<int> &TH2F_perEV_nbinsy, vector<float> &TH2F_perEV_lowy, vector<float> &TH2F_perEV_highy, vector<string> &TH2F_perEV_titley,\n'
    header = header + '          vector<vector<TH2F*> > &TH2F_perCHs, int &num_TH2F_perCHs, vector<string> &TH2F_perCH_name, vector<int> &TH2F_perCH_nbinsx, vector<float> &TH2F_perCH_lowx, vector<float> &TH2F_perCH_highx, vector<string> &TH2F_perCH_titlex, vector<int> &TH2F_perCH_nbinsy, vector<float> &TH2F_perCH_lowy, vector<float> &TH2F_perCH_highy, vector<string> &TH2F_perCH_titley,\n'
    header = header + '          vector<vector<vector<TH2F*> > > &TH2F_PerTSs, int &num_TH2F_PerTSs, vector<string> &TH2F_PerTS_name, vector<int> &TH2F_PerTS_nbinsx, vector<float> &TH2F_PerTS_lowx, vector<float> &TH2F_PerTS_highx, vector<string> &TH2F_PerTS_titlex, vector<int> &TH2F_PerTS_nbinsy, vector<float> &TH2F_PerTS_lowy, vector<float> &TH2F_PerTS_highy, vector<string> &TH2F_PerTS_titley){\n'
    header = header + '          vector<vector<TH2F*> > &TProfiles, int &num_TProfiles, vector<string> &TProfile_name, vector<int> &TProfile_nbinsx, vector<float> &TProfile_lowx, vector<float> &TProfile_highx, vector<string> &TProfile_titlex, vector<int> &TProfile_nbinsy, vector<float> &TProfile_lowy, vector<float> &TProfile_highy, vector<string> &TProfile_titley){\n'
    initfile.write('\n')
    for s in range(len(suites)):
        initfile.write('// --- Suite ' + suites[s][0] + ': ' + suites[s][1] + ' ---\n')
        initfile.write('\n')
        initfile.write('  if (suite_code == ' + str(suites[s][0]) + ') {\n')
        initfile.write('\n')
        for i in range(len(definitions)):
            if definitions[i][0] == 'TTree':
                if get_num_tests(suites[s][0],i) > 0:
                    initfile.write('    _file->mkdir("Trees");\n')
                    initfile.write('    _file->cd("Trees");\n')
                    initfile.write('\n')
                    initfile.write('    char branchName[100];\n')
#                    initfile.write('    branchName = branchName;\n')                    
                    initfile.write('    char branchDescription[100];\n')
 #                   initfile.write('    branchDescription = branchDescription;\n')
                    initfile.write('\n')
                    initfile.write('    _qie10Info.numChs = nCH;\n')
                    initfile.write('    _qie10Info.numTS = tTS;\n')
                    initfile.write('\n')
        for i in range(len(definitions)):
            if definitions[i][0] == 'TTree':
                for j in range(len(data[i])):
                    if suites[s][0] in data[i][j][2]:
                        for k in range(len(data[i][j][3])):
                            for l in range(len(branch_parameters)):
                                if branch_parameters[l] == 'scope':
                                    if data[i][j][3][k][l] == 'perChannel':
                                        for m in range(len(branch_parameters)):
                                            if branch_parameters[m] == 'name':
                                                initfile.write('    _qie10Info.')
                                                initfile.write(data[i][j][3][k][m][1:-1] + '_' + data[i][j][0][0][1:-1] + ' = new ')
                                                for n in range(len(branch_parameters)):
                                                    if branch_parameters[n] == 'type':
                                                        initfile.write(data[i][j][3][k][n] + '[_qie10Info.numChs];\n')
                                    if data[i][j][3][k][l] == 'perTS':
                                        for m in range(len(branch_parameters)):
                                            if branch_parameters[m] == 'name':
                                                initfile.write('    _qie10Info.')
                                                branch_name = data[i][j][3][k][m][1:-1];
                                                initfile.write(branch_name + '_' + data[i][j][0][0][1:-1] + '= new ')
                                                for n in range(len(branch_parameters)):
                                                    if branch_parameters[n] == 'type':
                                                        branch_type = data[i][j][3][k][n]
                                                        initfile.write(branch_type + '*[_qie10Info.numTS];\n')
                                                        initfile.write('    for (int i = 0; i < _qie10Info.numTS ; i++) {\n')
                                                        initfile.write('      _qie10Info.' + branch_name + '_' + data[i][j][0][0][1:-1] + '[i] = new ' + branch_type + '[_qie10Info.numChs];\n')
                                                        initfile.write('    }\n')

                                                        #initfile.write('    sprintf(branchName,"' + branch_name + '_TS%i",i);\n')
                                                        #initfile.write('    sprintf(branchDescription,"' + branch_name + '_TS%i[numChs]/' + ttree_types[branch_type]  + '",i);\n')
  
        initfile.write('\n')
        for i in range(len(data)):
            num_tests = get_num_tests(suites[s][0],i)
            if header.find('num_' + definitions[i][0] + 's') != -1:
                initfile.write('    num_' + definitions[i][0] + 's = SUITE' + str(suites[s][0]) +'_' + definitions[i][0].upper() + '_NUM;\n')    
            else:
                initfile.write('    int num_' + definitions[i][0] + 's = SUITE' + str(suites[s][0]) +'_' + definitions[i][0].upper() + '_NUM;\n')    
            initfile.write('    num_' + definitions[i][0] + 's = num_' + definitions[i][0] + 's;\n')
            if ((num_tests > 0) and (definitions[i][0] != 'null')):
                for k in range(len(definitions[i][1])):
                    initfile.write('    ' + cpp_types[definitions[i][1][k]] + ' temp_' + definitions[i][0] + '_' + definitions[i][1][k] + '[num_' + definitions[i][0] + 's]')
                    if (cpp_types[definitions[i][1][k]] == 'char'):
                        initfile.write('[100]')
                    initfile.write(' = {SUITE' + str(s) + '_' + definitions[i][0].upper() + '_' + definitions[i][1][k].upper() + '};\n')
#                    initfile.write('    temp_' + definitions[i][0] + '_' + definitions[i][1][k] + ' = temp_' + definitions[i][0] + '_' + definitions[i][1][k] + ';\n')
                initfile.write('\n')
            else:
                initfile.write('\n')                
        for i in range(len(definitions)):
            if ( ( get_num_tests( suites[s][0], i ) > 0 ) and (definitions[i][0] != 'null') ):
                initfile.write('    for (int i=0 ; i < num_' + definitions[i][0] + 's ; i++) {\n')
                if ((definitions[i][0] == 'TProfile')):
                    initfile.write('      vector<TH2F*> temp_' + definitions[i][0] + '_vector;\n')
                    initfile.write('      ' + definitions[i][0] + 's.push_back(temp_' + definitions[i][0] + '_vector);\n')
#                if ((definitions[i][0] == 'TH1F_perEV') or (definitions[i][0] == 'TH2F_perEV')):
#                    initfile.write('      ' + definitions[i][0].split('_')[0] + '* temp_' + definitions[i][0] + ';\n')
#                    initfile.write('      ' + definitions[i][0] + 's.push_back(temp_' + definitions[i][0] + ');\n')
                if ((definitions[i][0] == 'TH1F_perCH') or (definitions[i][0] == 'TH2F_perCH')):
                    initfile.write('      vector<' + definitions[i][0].split('_')[0] + '*> temp_' + definitions[i][0] + '_vector;\n')
                    initfile.write('      ' + definitions[i][0] + 's.push_back(temp_' + definitions[i][0] + '_vector);\n')
                if ((definitions[i][0] == 'TH1F_PerTS') or (definitions[i][0] == 'TH2F_PerTS')):
                    initfile.write('      vector<vector<' + definitions[i][0].split('_')[0] + '*> > temp_' + definitions[i][0] + '_vector_vector;\n')
                    initfile.write('      ' + definitions[i][0] + 's.push_back(temp_' + definitions[i][0] + '_vector_vector);\n')
                for j in range(len(definitions[i][1])):
                    initfile.write('      ' + definitions[i][0] + '_' + definitions[i][1][j] + '.push_back(temp_' + definitions[i][0] + '_' + definitions[i][1][j] + '[i]);\n')
                initfile.write('    }\n')
                initfile.write('\n')
            if definitions[i][0] == 'logger':
                if get_num_tests(suites[s][0],i) > 0:
                    initfile.write('    loggers = new ofstream[num_loggers];\n')
                    initfile.write('\n')
            if definitions[i][0] == 'TTree':
                if get_num_tests(suites[s][0],i) > 0:
                    initfile.write('    _trees = new TTree*[num_TTrees];\n')
                    initfile.write('    for (int i=0 ; i < num_TTrees ; i++) {\n')                
                    initfile.write('      TTree *temp_tree = new TTree(temp_TTree_name[i],temp_TTree_name[i]);\n')
                    initfile.write('      _trees[i] = temp_tree;\n')                
                    initfile.write('    }\n')                
                    initfile.write('\n')
                    tree_num_in_suite = 0
                    for j in range(len(data[i])):
                        if suites[s][0] in data[i][j][2]:
                            initfile.write('    _trees[' + str(tree_num_in_suite) + ']->Branch("numChs", &_qie10Info.numChs, "numChs/I");\n')
                            initfile.write('    _trees[' + str(tree_num_in_suite) + ']->Branch("numTS", &_qie10Info.numTS, "numTS/I");\n')
                            for k in range(len(data[i][j][3])):
                                for l in range(len(branch_parameters)):
                                    if branch_parameters[l] == 'scope':
                                        if data[i][j][3][k][l] == 'perChannel':
                                            for m in range(len(branch_parameters)):
                                                if branch_parameters[m] == 'name':
                                                    initfile.write('    _trees[' + str(tree_num_in_suite) + ']->Branch("')
                                                    initfile.write(data[i][j][3][k][m][1:-1] + '", _qie10Info.' + data[i][j][3][k][m][1:-1] + '_' + data[i][j][0][0][1:-1] + ', "' + data[i][j][3][k][m][1:-1] + '/')
                                                    for n in range(len(branch_parameters)):
                                                        if branch_parameters[n] == 'type':
                                                            initfile.write(ttree_types[data[i][j][3][k][n]] + '");\n')
                                        if data[i][j][3][k][l] == 'perTS':
                                            for m in range(len(branch_parameters)):
                                                if branch_parameters[m] == 'name':
                                                    branch_name = data[i][j][3][k][m][1:-1];
                                                    initfile.write('    for (int i = 0; i < _qie10Info.numTS ; i++) {\n')
                                                    initfile.write('      sprintf(branchName,"' + branch_name + '_TS%i",i);\n')
                                                    initfile.write('      sprintf(branchDescription,"' + branch_name + '_TS%i[numChs]/')
                                                    for n in range(len(branch_parameters)):
                                                        if branch_parameters[n] == 'type':
                                                            branch_type = data[i][j][3][k][n]
                                                            initfile.write(ttree_types[branch_type] + '",i);\n')
                                                            initfile.write('      _trees[' + str(tree_num_in_suite) +']->Branch(branchName, _qie10Info.' + data[i][j][3][k][m][1:-1] + '_' + data[i][j][0][0][1:-1] + '[i],branchDescription);\n')
                                                    initfile.write('    }\n')
                            tree_num_in_suite = tree_num_in_suite + 1
        initfile.write('  }\n')
        initfile.write('\n')
    initfile.write('}\n')
    initfile.write('\n')
    initfile.close()

##### PARSING FUNCTIONS #####

def fill_data(temp_data):
    for i in range(len(definitions)):
        if definitions[i][0] == temp_data['type']:
            data[i].append([[],[],['0'],[]])
            for j in range(len(block_definitions)):
                data[i][-1][1].append([])
            for j in range(len(definitions[i][1])):
                data[i][-1][0].append(temp_data[definitions[i][1][j]]) 

branches = []
def fill_branches():
    for b in range(len(branches)):
        for i in range(len(definitions)):
            if definitions[i][0] == 'TTree':
                for j in range(len(data[i])):
                    if data[i][j][0][0] == branches[b][0]:
                        data[i][j][3].append(['','',''])
                        for branch_parameter in branches[b][1]:
                            for k in range(len(branch_parameters)):
                                if branch_parameters[k] == branch_parameter:
                                    data[i][j][3][-1][k] = branches[b][1][branch_parameter]

temp_suites = []
def fill_suites():
    for temp_suite in temp_suites:
        suites.append([ temp_suite['code'] , temp_suite['name'] ])
        for key in temp_suite:
            if ((key != 'name') and (key != 'code')):
                for i in range(len(definitions)):
                    if definitions[i][0] == key:
                        for test in temp_suite[key].split(','):
                            for j in range(len(data[i])):
                                if data[i][j][0][0] == test.strip():
                                    data[i][j][2].append(temp_suite['code'])

# blocks[block_number][0] = test_type
# blocks[block_number][1] = test_name
# blocks[block_number][2] = block_type
# blocks[block_number][3] = array of lines in block
blocks = []
def fill_blocks():
    for block in blocks:
        for i in range(len(definitions)):
            if definitions[i][0] == block[0]:
                for j in range(len(data[i])):
                    if data[i][j][0][0] == block[1]:
                        for k in range(len(block_definitions)):
                            if block_definitions[k] == block[2]:
                                for line in block[3]:
                                    data[i][j][1][k].append(line)

##### MAIN #####


###### INITIALIZE  ######

cfg_file = open(cfgfile_name,'r')
lines = cfg_file.readlines()
cfg_file.close()

# data[type] = array of tests of 'type'
# data[type][test][0] = array of parameters for 'test'
# data[type][test][0][0] = 'name' of 'test'
# data[type][test][1] = array of blocks for 'test'
# data[type][test][2] = array of suites which include 'test'
# data[type][test][3] = array of branches for 'test' (TTree type only)
data = []
for i in range(len(definitions)):
    data.append([])

# suites[suite_num][0] = suite_code
# suites[suite_num][1] = suite_name
suites = [
    [ '0' , '"full"']
]

# clean start
in_def = 0
in_blo = 0
in_sui = 0
in_bra = 0
in_com = 0
line_num = 0

##### READ CFG FILE #####

for line in lines:

    if (line.strip() != '') and (line.strip()[0] != '#'): # if not comment or blank line

        if (line.strip() == '<start_definition>'): # starting definition
            in_def = 1
            temp_data = {}
        if (line.strip() == '<end_definition>'): # ending definition 
            if (in_def == 1): # if was in definition
                in_def = 0
                fill_data(temp_data)
            elif (in_com == 1): # end commented block
                in_com == 0
            else:
                print 'ERROR: <end_definition> without <start_definition>: line number ' + str(line_num)

        if (line.strip() == '<start_suite>'): # starting suite
            in_sui = 1
            temp_suite = {}
        if (line.strip() == '<end_suite>'): # ending suite
            if (in_sui == 1): # if was in suite
                in_sui = 0
                temp_suites.append(temp_suite)
            elif (in_com == 1): # end commented block
                in_com == 0
            else:
                print 'ERROR: <end_suite> without <start_suite>: line number ' + str(line_num)

        if ((in_def == 1) and (line.strip() != '<start_definition>')): # if in a definition (but not at the start)

            if (line.find('<add_branch>') != -1): # starting branch
                temp_branch = {}
                in_bra = 1
            if (line.strip() == '<end_branch>'): # ending block
                if (in_bra == 1):
                    in_bra = 0
                    branches.append([])
                    branches[-1].append(temp_data['name'])
                    branches[-1].append(temp_branch)
                else:
                    print 'ERROR: <end_branch> without <add_branch>: line number ' + str(line_num)

            if (line.find('<start_block>') != -1): # starting block
                in_blo = 1
                temp_block = []
                block_name = line.split('=')[0].strip()
            if (line.strip() == '<end_block>'): # ending block
                if (in_blo == 1):
                    in_blo = 0
                    blocks.append( [ temp_data['type'] , temp_data['name'] , block_name , temp_block ] )
                else:
                    print 'ERROR: <end_block> without <start_block>: line number ' + str(line_num)

            if ((in_blo == 1) and (line.find('<start_block>') == -1)): # if in a block (but not at the start)
                temp_block.append(line.rstrip())

            if ((in_bra == 1) and (line.find('<add_branch>') == -1)): # if in a branch (but not at the start)
                temp_branch[line.split('=')[0].strip()] = line.split('=')[1].strip()

            if ((in_blo != 1) and (in_bra != 1) and (line.strip() != '<end_block>') and (line.strip() != '<end_branch>')): # if not in a block or a branch
                temp_data[line.split('=')[0].strip()] = line.split('=')[1].strip()

        elif ((in_sui == 1) and (line.strip() != '<start_suite>')): # if not in a definition, but in a suite
            temp_suite[line.split('=')[0].strip()] = line.split('=')[1].strip()

        elif ((line.strip() != '<end_definition>') and (line.strip() != '<start_definition>') and (line.strip() != '<start_suite>') and (line.strip() != '<end_suite>') and (in_com == 0)):
            print 'ERROR: statement outside of definition or suite: line ' + str(line_num) + ': ' + line.strip()

    elif ((line.strip() != '') and (line.strip()[0] == '#') and (line.find('<start') != -1)): # if commented block
        in_com = 1
            
    line_num = line_num + 1

fill_suites()
fill_blocks()
fill_branches()
print_init()
print_loop()
print_info()
