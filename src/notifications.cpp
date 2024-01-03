#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <mutex>

#include "notifications.hpp"

using namespace std;

Notifications::Notifications(){
  // Inicializa id em zero
  notif_id = 0;
}

void Notifications::newMessage(unique_ptr<Packet> packet, set<string> followers){
  // Cria nova notificação
  Notification notif;
  notif.id = notif_id++;
  notif.timestamp = packet->timestamp;
  notif.pending = followers.size();
  notif.length = packet->length;
  strncpy(notif.sender, packet->profile, 20);
  strncpy(notif.message, packet->payload, MAXLEN);

  // Adiciona na lista de notificacoes
  notifications.push_back(notif);

  // Adiciona na lista de notificacoes pendentes uma mensagem para cada seguidor
  /*notif.id followers
    0     @doze
    0     @onze
    0     @treze
    1     @doze
  */

 //TODO: validar se o usuario que postou deve visualizar tambem a notificacao que ele mesmo postou
  for(string follower : followers)
    pending_notifs.insert(pair<string, unsigned> (follower, notif.id));

  cout << "id: " << notif.id << ' ';
  cout << packet->profile << ": ";
  cout << notif.message << endl;
}

std::multimap<std::string, unsigned int> Notifications::getPendingNotifs(){
  return pending_notifs;
}

bool Notifications::isEmpty(){
  return pending_notifs.empty();
}

Notification Notifications::getNotifByID(unsigned int notif_id){
  //TEST: tratar excecao caso notif_id nao exista
  return notifications.at(notif_id);
}

void Notifications::deleteNotif(multimap<string, unsigned int>::iterator it){
  mutex m;
  m.lock();
  // pending_notifs.erase(it);
  //TODO: alterar para apagar somente a notificacao do perfil que esta lendo
  pending_notifs.clear();
  m.unlock();
}
