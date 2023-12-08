#pragma once
#include <vector>
#include <list>
#include <string>

#include "data.hpp"

using std::pair;
using std::list;
using std::string;

// Estrutura para armazenar os perfis
typedef struct profile_t{
  string profile;
  list<int> port;
  list<string> follow;
  list<Notification> notif;
  list<pair<string, int>> pending_notif;
} Profile;