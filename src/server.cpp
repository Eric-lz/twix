#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>

#include "udp.hpp"
#include "server.hpp"
#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

using std::unique_ptr;
using std::pair;
using std::map;
using std::vector;
using std::string;
using std::find;
using std::cout;
using std::endl;

int main() {
  // UDP object
  UDP udp;
  udp.openSocket();
  udp.bindSocket();

  // Server memory
  // Instances
  Instances instances;
  // Profiles
  ProfilesList profiles;
  // Notifications
  Notifications notifications;

  while(true){
    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe pacote do cliente
    unique_ptr<Packet> packet;
    packet = udp.recebe(&cliaddr);

    // Ação do servidor baseada no tipo de packet recebido
    switch(packet->type){
    case PING: // useless
      cout << "ping from " << cliaddr.sin_port << endl;
      break;

    case LOGIN:
      // Login
      instances.newInstance(packet->payload, cliaddr);
      profiles.login(packet->payload);
      cout << "login: " << packet->payload;
      cout << " in port " << cliaddr.sin_port << endl;
      break;

    case FOLLOW:
      // Adiciona seguidor
      profiles.addFollow(packet->profile, packet->payload);
      break;

    case UNFOLLOW:
      // Remove seguidor
      profiles.unFollow(packet->profile, packet->payload);
      break;

    case SEND:
      // Adiciona mensagem na lista de notificacoes 
      notifications.newMessage(move(packet));
      break;

    default:
      perror("unknown packet type");
      break;
    }
  }

  return 0;
}
