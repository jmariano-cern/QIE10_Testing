#!/usr/bin/python

from os import system as call
from os import chdir as cd
from os import environ as env
from getpass import getuser as user
from sys import exit

yn = 0

print('Before installation, you MUST have enabled your lxplus work area.')
yn = raw_input('Is your work area enabled? -- ')

if ((yn != 'Yes') and (yn != 'yes')and (yn != 'Y') and (yn != 'y') and (yn != 'OK')and (yn != 'Ok') and (yn != 'ok') and (yn != '1')):
    print 'Installation failed. Please enable your work area (see: https://resources.web.cern.ch/resources/Help/?kbid=067040) and try again.'
    exit()

print 'Creating folder to store raw run data: /afs/cern.ch/work/' + user()[0] + '/' + user() + '/public/904_runs...'
call('mkdir /afs/cern.ch/work/' + user()[0] + '/' + user() + '/public/904_runs')

print 'Building map files...'
cd(env['QIE10ROOT'] + '/src')
call('./remap HFM01,HFM02,HFM03,HFM04,HFM05,HFM06,HFM07,HFM08,HFP01,HFP02,HFP03,HFP04,HFP05,HFP06,HFP07,HFP08')

print 'Building the analyzer core...'
call('python generate.py')
cd(env['QIE10ROOT'])

print 'Compiling the analyzer...'
call('scram b -j 8')
print 'Analyzer compiled.'

print 'Installion complete.'
