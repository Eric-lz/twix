#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>

#include "notifications.hpp"

using namespace std;

void Notifications::newMessage(unique_ptr<Packet> packet, set<string> followers){
  // Cria nova notificação
  Notification notif;
  notif.id = notif_id++;
  notif.timestamp = packet->timestamp;
  notif.pending = followers.size();
  notif.length = packet->length;
  strncpy(notif.message, packet->payload, MAXLEN);

  // Adiciona na lista de notificacoes
  notifications.push_back(notif);

  // Adiciona na lista de notificacoes pendentes
  for(string follower : followers)
    pending_notifs.insert(pair<string, unsigned> (follower, notif.id));

  cout << "id: " << notif.id << ' ';
  cout << packet->profile << ": ";
  cout << notif.message << endl;
}

Notifications::Notifications(){
  // Inicializa id em zero
  notif_id = 0;
}