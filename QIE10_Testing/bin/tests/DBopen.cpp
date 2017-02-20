#include <iostream>
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sqlite/3.15.1/include/sqlite3.h"

using namespace std;

void DBopen() {

  // source:
  // https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("../../src/ngHF2017LMap_20170206_pre05.db", &db);
  
  if( rc ){
    cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
  } else {
    cout << "Opened database successfully" << endl;
  }
  sqlite3_close(db);

}
