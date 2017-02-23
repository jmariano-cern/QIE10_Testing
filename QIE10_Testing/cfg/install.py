#!/usr/bin/python

from os import system as call
from os import chdir as cd
from os import environ as env
from getpass import getuser as user
from sys import exit

print 'Building map files...'
cd(env['QIE10ROOT'] + '/src')
call('./remap')

print 'Building the analyzer core...'
call('python generate.py')
cd(env['QIE10ROOT'])

print 'Compiling the analyzer...'
call('scram b -j 8')
print 'Analyzer compiled.'

print 'Installion complete.'
