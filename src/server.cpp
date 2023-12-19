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
  // Open UDP socket to listen for logins
  UDP* udp = new UDP;
  udp->openSocket();
  udp->bindSocket();

  // Thread que gerencia notificacoes e envios
  thread thEnvia(threadSession, udp, instances, notifications);

  while(true){
    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe pacote do cliente
    unique_ptr<Packet> packet;
    packet = udp->recebe(&cliaddr);

    // Lista de seguidores
    set<string> followers;

    // Ação do servidor baseada no tipo de packet recebido
    switch(packet->type){
    case PING: // useless
      cout << "ping from " << cliaddr.sin_port << endl;
      break;

    case LOGIN:
      // Login
      // Create new thread to handle messages
      // (pass UDP socket and server memory)
      instances->newInstance(packet->payload, cliaddr);
      profiles->login(packet->payload);
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

  return 0;
}

void threadSession(UDP* udp, Instances* instances, Notifications* notifications){
  while(true){
    if(!notifications->isEmpty()){
      // Lista de notificacoes pendentes
      auto pending = notifications->getPendingNotifs();

      for (multimap<string, unsigned int>::iterator notif_it = pending.begin(); notif_it != pending.end(); ++notif_it) { 
        // consume notification
        cout << "send notif.id " << notif_it->second << " to ";
        cout << notif_it->first << endl;

        // get notification
        // auto notif = notifications->getNotifByID(notif_it->second);

        // build packet
        // unique_ptr<Packet> packet;

        // consume notification
        notifications->deleteNotif(notif_it);
      }
    }

    sleep(1);
  }
}