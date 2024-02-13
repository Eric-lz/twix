// debug
#include <iostream>

#include <string>
#include <map>

#include "instances.hpp"

using namespace std;

void Instances::newInstance(string profile, sockaddr_in cliaddr){
  instancesMap[profile] = cliaddr;
  setAlive(profile);
}

void Instances::closeInstance(string profile, sockaddr_in cliaddr){
  instancesMap.erase(profile);
}

struct sockaddr_in Instances::getPort(string profile){
  return instancesMap[profile];
}

void Instances::setAlive(string profile){
  keepAliveMap.push_back(profile);
}

void Instances::checkAlive(){
  // close every instance that didn't respond to the ping
  // (isn't on the keepAliveMap)
  for(auto i : instancesMap){
    bool found = false;

    for(size_t j = 0; j < keepAliveMap.size(); j++){
      // cout << i.first << " == " << keepAliveMap[j] << endl;
      if(i.first == keepAliveMap[j])
        found = true;
    }

    if(!found){
      closeInstance(i.first, i.second);
      //cout << i.first << " is offline." << endl;
    }
  }

  keepAliveMap.clear();
}

std::map<std::string, struct sockaddr_in> Instances::getInstances(){
  return instancesMap;
}
