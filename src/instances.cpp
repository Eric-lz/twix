#include <string>
#include <map>

#include "instances.hpp"

using namespace std;

void Instances::newInstance(string profile, sockaddr_in cliaddr){
  instancesMap[profile] = cliaddr;
}

void Instances::closeInstance(string profile, sockaddr_in cliaddr){
  instancesMap.erase(profile);
}

int Instances::getPort(string profile){
  return instancesMap[profile].sin_port;
}
