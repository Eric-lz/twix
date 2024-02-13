#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#include "profiles.hpp"

using namespace std;

int Profiles::getProfileByName(string name){
  for(int i = 0; auto p : profiles){  // initializer in range-based for loop was introduced in C++23
    if(p.name == name){
      return i;
    }
    i++;
  }
  
  return -1;
}

// Se o perfil não existe na lista, cria
void Profiles::login(string profile){
  if(getProfileByName(profile) < 0){
    Profile p;
    p.name = profile;
    profiles.push_back(p);
    cout << profile << " created." << endl;
  }
}

// Adiciona um seguidor a um perfil (follower está seguindo following)
// Retorna 
int Profiles::addFollow(string follower, string following){
  // Se encontrar perfil a ser seguido, adicionar na lista de seguidores dele
  int i = getProfileByName(following);

  if(i < 0){
    cout << "profile " << following << " does not exist!" << endl;
    return -1;
  }
  else{
    profiles.at(i).followers.insert(follower);
    cout << follower << " now follows ";
    cout << following << endl;
    return 0;
  }
}

void Profiles::unFollow(string follower, string following){
  // Se encontrar seguidor, remove da lista (tratado automaticamente pelo erase())
  int i = getProfileByName(following);
  profiles.at(i).followers.erase(follower);

  cout << follower << " no longer follows ";
  cout << following << endl;
}

set<string> Profiles::getFollowers(std::string profile){
  int i = getProfileByName(profile);
  return profiles.at(i).followers;
}