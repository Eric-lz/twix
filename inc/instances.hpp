#pragma once

#include <netinet/in.h>
#include <string>
#include <map>

//ainda nao foi utilizada no projeto, provavelmente sera utilizada
//usado para gerenciar as instancias do cliente que no maximo deve ter 2
class Instances{
private: 
  std::map<std::string, struct sockaddr_in> instancesMap;

public:
  // Abre nova instancia do cliente
  void newInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Fecha instancia do cliente
  void closeInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Retorna a porta de um perfil
  //TODO como resolver se o perfil tem mais de uma instancia?
  int getPort(std::string profile);
};