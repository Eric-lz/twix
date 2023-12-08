#pragma once
#include <vector>
#include <list>
#include <string>

#include "data.hpp"

// Estrutura para armazenar os perfis
typedef struct profile_t{
  in_port_t port;
  std::string profile;
  std::list<std::string> follow;
  std::list<Notification> notif;
  std::list<std::pair<std::string, int>> pending_notif;
} Profile;