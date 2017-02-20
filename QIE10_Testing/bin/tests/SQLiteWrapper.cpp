// CALL EXAMPLE
// root -l -q -x -b SQLiteWrapper.cpp+(\"DBopen.cpp\")

#include "TSystem.h"
#include "TROOT.h"

void SQLiteWrapper(const char *macroName) {
  gSystem->Load("/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sqlite/3.15.1/lib/libsqlite3.so");
  gSystem->ListLibraries();
  gROOT->Macro(macroName);
}
