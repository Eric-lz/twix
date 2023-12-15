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
  std::set<std::string> follow;
} Profile;

class ProfilesList{
private:
  // list of profiles
  std::vector<Profile> profiles;
  unsigned int notif_id = 0;

  // Procura perfil pelo nome e retorna seu indice
  int getProfileByName(std::string name);

public:
  // Loga com um perfil
  void login(std::string profile);

  // Segue novo perfil
  void addFollow(std::string follower, std::string following);

  // Para de seguir um perfil
  void unFollow(std::string follower, std::string following);
};