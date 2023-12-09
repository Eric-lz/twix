#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include "udp.hpp"
#include "server.hpp"

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
  vector<Profile> profiles;
  map<in_port_t, int> clients;

  while(true){
    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe mensagem do cliente
    unique_ptr<Packet> packet;
    packet = udp.recebe(&cliaddr);

    // Perfil que esta mandando pacotes
    Profile p;
    int p_index;

    switch(packet->type){
    case PING:
      // cout << "ping" << endl;
      break;

    case LOGIN:
      // Nome do perfil
      p.name = packet->payload;
      p_index = profiles.size();
      profiles.push_back(p);
      clients.insert({cliaddr.sin_port, p_index});

      cout << "login: " << p.name;
      cout << " in port: " << cliaddr.sin_port << endl;
      break;

    case FOLLOW:
      // Procura o perfil que enviou a mensagem
      p_index = findPort(clients, cliaddr.sin_port);
      // Adiciona seguidor
      addFollow(profiles.at(p_index), packet->payload);
      break;

    case UNFOLLOW:
      // Procura o perfil que enviou a mensagem
      p_index = findPort(clients, cliaddr.sin_port);
      // Remove seguidor
      unFollow(profiles.at(p_index), packet->payload);
      break;

    case SEND:
      // Procura o perfil que enviou a mensagem
      p_index = findPort(clients, cliaddr.sin_port);
      // Imprime a mensagem no console
      cout << profiles.at(p_index).name << ": ";
      cout << packet->payload << endl;
      // Envia resposta ao cliente
      strncpy(packet->payload, profiles.at(p_index).name.c_str(), MAXLEN);
      udp.envia(move(packet), &cliaddr);
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

void addFollow(Profile& perfil, string follow){
  perfil.follow.insert(follow);
  cout << perfil.name << " now follows ";
  cout << follow << endl;
}

void unFollow(Profile& perfil, string follow){
  auto it = perfil.follow.find(follow);

  // Se encontrar seguidor, remove da lista
  if(it != perfil.follow.end()){
    perfil.follow.erase(it);
    cout << perfil.name << " no longer follows ";
    cout << follow << endl;
  }
  else{
    cout << perfil.name << " does not follow ";
    cout << follow << endl;
  }
}
