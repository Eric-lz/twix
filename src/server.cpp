#include <iostream>
#include <thread>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <set>

#include "udp.hpp"
#include "server.hpp"
#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

using std::thread;
using std::unique_ptr;
using std::pair;
using std::map;
using std::multimap;
using std::set;
using std::vector;
using std::string;
using std::find;
using std::cout;
using std::endl;

int main() {
  // Server memory
  // Instances
  Instances* instances = new Instances;
  // Profiles
  Profiles* profiles = new Profiles;
  // Notifications
  Notifications* notifications = new Notifications;

  // Cria thread que recebe pacotes
  thread thRecebe(threadRecebe, instances, profiles, notifications);
  thread thEnvia(threadEnvia, instances, profiles, notifications);

  thRecebe.join();
  thEnvia.join();

  return 0;
}

void threadRecebe(Instances* instances, Profiles* profiles, Notifications* notifications){
  // UDP socket
  UDP udp;
  udp.openSocket();
  udp.bindSocket();

  while(true){
    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe pacote do cliente
    unique_ptr<Packet> packet;
    packet = udp.recebe(&cliaddr);

    // Lista de seguidores
    set<string> followers;

    // Ação do servidor baseada no tipo de packet recebido
    switch(packet->type){
    case PING: // useless
      cout << "ping from " << cliaddr.sin_port << endl;
      break;

    case LOGIN:
      // Login
      instances->newInstance(packet->payload, cliaddr);
      profiles->login(packet->payload);
      cout << "login: " << packet->payload;
      cout << " in port " << cliaddr.sin_port << endl;
      break;

    case FOLLOW:
      // Adiciona seguidor
      profiles->addFollow(packet->profile, packet->payload);
      break;

    case UNFOLLOW:
      // Remove seguidor
      profiles->unFollow(packet->profile, packet->payload);
      break;

    case SEND:
      // Adiciona mensagem na lista de notificacoes
      // Pega lista de seguidores do perfil que enviou a mensagem
      followers = profiles->getFollowers(packet->profile);
      notifications->newMessage(move(packet), followers);
      break;

    default:
      perror("unknown packet type");
      break;
    }
  }
}

void threadEnvia(Instances* instances, Profiles* profiles, Notifications* notifications){
  // UDP socket
  UDP udp;
  udp.openSocket();
  cout << "socket aberto\n";
  while(true){
    auto pending = notifications->getPendingNotifs();

    if(!pending.empty()){    
      for (multimap<string, unsigned int>::iterator it = pending.begin(); it != pending.end(); ++it) {
        cout << "send to: " << it->first << ": ";
        cout << it->second << endl;
        // consume notification
        notifications->deleteNotif(it);
      }
    }
  }
}