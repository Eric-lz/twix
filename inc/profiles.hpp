#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "notifications.hpp"
#include "udp.hpp"

// Estrutura para armazenar os perfis
typedef struct profile_t{
  std::string name;
  std::set<std::string> follow; //TODO: FOLLOW PARA FOLLWER MUDAR LISTA DE SEGUINDO PARA LISTA DE SEGUIDORES, FICA MAIS FACIL DE SABER PARA QUEM TEM QUE MANDAR NOTIFICACAO E DIMINUI CONDICAO DE CORRIDA
  } Profile;

class Profiles{
private:
  // list of profiles
  std::vector<Profile> profiles;

  // Procura perfil pelo nome e retorna seu indice
  int getProfileByName(std::string name);

public:
  // Loga com um perfil
  void login(std::string profile);

  // Segue novo perfil
  void addFollow(std::string follower, std::string following);

  // Para de seguir um perfil
  void unFollow(std::string follower, std::string following);

  // Retorna lista de seguidores de um perfil
  std::set<std::string> getFollowers(std::string profile);
};