#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <mutex>

#include "notifications.hpp"

using namespace std;

// Inicializa a classe Notifications
Notifications::Notifications(){
  // Inicializa id em zero
  notif_id = 0;
}

// Cria uma nova notificação e insere na lista de notificações
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

 //TODO: validar se o usuario que postou deve visualizar tambem a notificacao que ele mesmo postou
  for(string follower : followers)
    pending_notifs.insert(pair<string, unsigned> (follower, notif.id));

  cout << "notif_id=" << notif.id << ' ';
  cout << packet->profile << ": ";
  cout << notif.message << endl;
}

// Retorna lista de notificações pendentes
std::multimap<std::string, unsigned int> Notifications::getPendingNotifs(){
  return pending_notifs;
}

// Retorna verdadeiro se lista de notificações pendentes está vazia
// Falso caso contrário
bool Notifications::isEmpty(){
  return pending_notifs.empty();
}

// Retorna notificação com ID fornecido
Notification Notifications::getNotifByID(unsigned int notif_id){
  //TEST: tratar excecao caso notif_id nao exista
  return notifications.at(notif_id);
}

// Remove notificação da lista de notificações pendentes
void Notifications::deleteNotif(multimap<string, unsigned int>::iterator it){
  pending_notifs.erase(it->first);
}
