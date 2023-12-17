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
  // Session threads
  vector<thread> threads;

  while(true){
    // Open UDP socket to listen for logins
    UDP* udp = new UDP;
    udp->openSocket();
    udp->bindSocket();
    cout << "new socket opened" << endl;

    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe pacote do cliente
    unique_ptr<Packet> packet;
    packet = udp->recebe(&cliaddr);
    cout << "packet received" << endl;

    // Listen for login packets
    if(packet->type == LOGIN){
      instances->newInstance(packet->payload, cliaddr);
      profiles->login(packet->payload);

      // change socket address
      // udp->changeAddr(cliaddr);

      cout << "login: " << packet->payload;
      cout << " in port " << cliaddr.sin_port << endl;
      
      // Create new thread to handle messages
      // (pass UDP socket and server memory)
      thread thSession(threadSession, udp, instances, profiles, notifications);

      // Insert thread into Threads option
      threads.push_back(move(thSession));
    }
  }

  return 0;
}

void threadSession(UDP* udp, Instances* instances, Profiles* profiles, Notifications* notifications){
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
      perror("login already handled");
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