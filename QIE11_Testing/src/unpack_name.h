#include <iostream>
#include <sstream>
#include <map>

struct name_data
{
  vector<std::string> parameter;
  vector<float> val;
};

name_data unpack_name(edm::Handle<FEDRawDataCollection> raw_collection, hcaltb::HcalTBSlowDataUnpacker sdp, int _event_num, int dim) {

  std::map<std::string,std::string> strings;
  std::map<std::string,double> numerics;
  
  name_data slow_data;
  //slow_data.parameter.push_back("NULL");
  //slow_data.val.push_back(-999.0);

  sdp.unpackMaps(raw_collection->FEDData(14),strings,numerics);

  int splitpoint;
  int splitpoint2;
  int splitpoint3;
  int splitpoint2_prev;
  int splitpoint3_prev;

  for (std::map<std::string,std::string>::const_iterator j = strings.begin() ; j != strings.end() ; j++) {
    if (j->first == "NAME") {
      splitpoint = j->second.find("_");
      if (splitpoint != -1) {
	splitpoint2 = -1;
	splitpoint3 = -1;
	splitpoint2_prev = -1;
	splitpoint3_prev = -1;
	for (int d=0 ; d<dim ; d++) {
	  splitpoint2 = j->second.substr(0,splitpoint).find("-",splitpoint2+1);
	  splitpoint3 = j->second.substr(splitpoint+1).find("-",splitpoint3+1);
	  //slow_data.parameter = j->second.substr(0,splitpoint);
	  //slow_data.val = std::stof(j->second.substr(splitpoint+1));
	  slow_data.parameter.push_back(j->second.substr(0,splitpoint).substr(splitpoint2_prev+1,splitpoint2));
	  slow_data.val.push_back(std::stof(j->second.substr(splitpoint+1).substr(splitpoint3_prev+1,splitpoint3)));
	  splitpoint2_prev = splitpoint2;
	  splitpoint3_prev = splitpoint3;
	}
      }
    }
  }

  if (_event_num == 0) {  
    cout << "****************************************************" << endl;
  }
  if (_event_num%100 == 0) {  
    cout << "****************************************************" << endl;
    for (int d=0 ; d<dim ; d++) {
      cout << "* Scan Parameter " << d << " -- " << slow_data.parameter.at(d) << " : " << slow_data.val.at(d) << endl;
    }
    cout << "****************************************************" << endl;
  }
  return slow_data;

}
