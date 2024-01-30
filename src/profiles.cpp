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
    if(p.name == name)
      return i;
  }
  
  return -1;
}

void Profiles::login(string profile){
  // Se o perfil não existe na lista, cria
  if(getProfileByName(profile) < 0){
    Profile p;
    p.name = profile;
    profiles.push_back(p);
  }
}

void Profiles::addFollow(string follower, string following){
  // Se encontrar perfil a ser seguido, adicionar na lista de seguidores dele
  int i = getProfileByName(following);

  if(i < 0){
    cout << "profile " << following << " does not exist!" << endl;
  }
  else{
    profiles.at(i).followers.insert(follower);
    cout << follower << " now follows ";
    cout << following << endl;
  }
}

void Profiles::unFollow(string follower, string following){
  // Se encontrar seguidor, remove da lista (tratado automaticamente pelo erase())
  int i = getProfileByName(follower);
  profiles.at(i).followers.erase(following);

  cout << follower << " no longer follows ";
  cout << following << endl;
}

set<string> Profiles::getFollowers(std::string profile){
  int i = getProfileByName(profile);
  return profiles.at(i).followers;
}