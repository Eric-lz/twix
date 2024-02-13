#pragma once

#include <netinet/in.h>
#include <string>
#include <map>
#include <vector>

// Classe usada para gerenciar as instancias (ou sessoes) dos clients
class Instances{
private:
  // Alterar map para multimap para poder salvar duas instancias por perfil?
  // Usar um map <perfil, semaforo(2)> para limitar até duas instancias? 
  std::map<std::string, struct sockaddr_in> instancesMap;
  std::vector<std::string> keepAliveMap;

public:
  // Abre nova instancia do cliente
  void newInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Fecha instancia do cliente
  void closeInstance(std::string profile, struct sockaddr_in cliaddr);
  
  // Retorna a porta de um perfil
  struct sockaddr_in getPort(std::string profile);

  // Adiciona uma instancia na lista de keep alive
  void setAlive(std::string profile);

  // Remove da lista de instancias aquelas que não estão em keepAliveMap
  void checkAlive();

  // Retorna lista de instancias
  std::map<std::string, struct sockaddr_in> getInstances();
};
