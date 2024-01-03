#include <string>
#include <map>

#include "instances.hpp"

using namespace std;

void Instances::newInstance(string profile, sockaddr_in cliaddr){
  instancesMap[profile] = cliaddr;
}

void Instances::closeInstance(string profile, sockaddr_in cliaddr){
  instancesMap.erase(profile);
  //TEST: deleta o perfil inteiro entao para deletar uma instancia, entao como deletar somente uma instancia e nao o perfil inteiro?
  // talvez mudar estrutura para multimap
}

int Instances::getPort(string profile){
  return instancesMap[profile].sin_port;
  //se mudar para multimap, como retornar a porta de uma instancia especifica?
}
