#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#include "profiles.hpp"

using namespace std;

int ProfilesList::getProfileByName(string name){
  for(int i = 0; auto p : profiles){
    if(p.name == name)
      return i;
  }
  
  return -1;
}

void ProfilesList::login(string profile){
  // Se o perfil não existe na lista, cria
  if(getProfileByName(profile) < 0){
    Profile p;
    p.name = profile;
    profiles.push_back(p);
  }
}

void ProfilesList::addFollow(string follower, string following){
  int i = getProfileByName(follower);
  profiles.at(i).follow.insert(following);

  cout << follower << " now follows ";
  cout << following << endl;
}

void ProfilesList::unFollow(string follower, string following){
  // Se encontrar seguidor, remove da lista
  int i = getProfileByName(follower);
  profiles.at(i).follow.erase(following);

  cout << follower << " no longer follows ";
  cout << following << endl;
}

void ProfilesList::recebeMensagem(unique_ptr<Packet> packet){
  // Indice do perfil que enviou a mensagem
  auto perfil = profiles.at(getProfileByName(packet->profile));

  // Adiciona na lista de notificações recebidas
  Notification notif;
  notif.id = notif_id++;
  notif.timestamp = packet->timestamp;
  notif.length = packet->length;
  notif.pending = perfil.follow.size();
  strncpy(notif.message, packet->payload, MAXLEN);
  notifications.push_back(notif);
  
  // Adiciona na lista de notificações pendentes
  for(string follower : perfil.follow)
    pending_notifs.insert(pair<string, unsigned> (follower, notif.id));

  cout << "id: " << notif.id << ' ';
  cout << packet->profile << ": ";
  cout << packet->payload << endl;
}