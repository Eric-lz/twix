#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#include "profiles.hpp"

using namespace std;

int Profiles::getProfileByName(string name){
  for(int i = 0; auto p : profiles){
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
  int i = getProfileByName(following);
  profiles.at(i).follow.insert(follower);

  cout << follower << " now follows ";
  cout << following << endl;
}

void Profiles::unFollow(string follower, string following){
  // Se encontrar seguidor, remove da lista
  int i = getProfileByName(follower);
  profiles.at(i).follow.erase(following);

  cout << follower << " no longer follows ";
  cout << following << endl;
}

set<string> Profiles::getFollowers(std::string profile){
  int i = getProfileByName(profile);
  return profiles.at(i).follow;
}