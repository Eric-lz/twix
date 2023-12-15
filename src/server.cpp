#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>

#include "udp.hpp"
#include "server.hpp"
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
  ProfilesList profiles;

  // Perfil - Porta
  map<string, sockaddr_in> clients;

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
      profiles.login(packet->payload);
      clients[packet->payload] = cliaddr;
      cout << "login: " << packet->payload;
      cout << " in port " << clients[packet->payload].sin_port << endl;
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
      // Recebe mensagem do client 
      profiles.recebeMensagem(move(packet));
      break;

    default:
      perror("unknown packet type");
      break;
    }
  }

  return 0;
}

// Busca o client pela porta e retorna seu indice
// Retorna -1 se nao encontra
int findPort(map<in_port_t, int>& clients, in_port_t port){
  if(clients.find(port) != clients.end())
    return clients[port];

  return -1;
}
