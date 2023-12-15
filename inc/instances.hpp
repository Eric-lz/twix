#pragma once
#include <netinet/in.h>
#include <string>
#include <map>

class Instances{
private: 
  std::map<std::string, struct sockaddr_in> instancesMap;

public:
  // Abre nova instancia do cliente
  void newInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Fecha instancia do cliente
  void closeInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Retorna a porta de um perfil
  int getPort(std::string profile);
};