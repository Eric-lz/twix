#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include "udp.hpp"
#include "server.hpp"

using std::unique_ptr;
using std::pair;
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
  vector<Profile> data;

  while(true){
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe mensagem do cliente
    unique_ptr<Packet> packet;
    packet = udp.recebe(&cliaddr);

    // Perfil que esta mandando pacotes
    Profile p;
    int index;

    switch(packet->type){
    case PING:
      cout << "ping" << endl;
      break;

    case LOGIN:
      p.port = cliaddr.sin_port;
      p.profile = packet->payload;
      data.push_back(p);
      cout << "login: " << p.profile;
      cout << " in port: " << p.port << endl;
      break;

    case FOLLOW:
      // Procura o perfil que enviou a mensagem
      index = findPort(data, cliaddr.sin_port);
      // Adiciona seguidor
      addFollow(data.at(index), packet->payload);
      break;

    case UNFOLLOW:
      // Procura o perfil que enviou a mensagem
      index = findPort(data, cliaddr.sin_port);
      // Remove seguidor
      unFollow(data.at(index), packet->payload);
      break;

    case SEND:
      // Procura o perfil que enviou a mensagem
      index = findPort(data, cliaddr.sin_port);
      // Imprime a mensagem no console
      cout << data.at(index).profile << ": ";
      cout << packet->payload << endl;
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
int findPort(vector<Profile>& data, in_port_t port){
  unsigned long i = 0;
  // Procura o perfil que enviou a mensagem
  for(auto p : data){
    if(p.port == port)
      return i;

    i++;
  }

  return -1;
}

void addFollow(Profile& perfil, string follow){
  perfil.follow.insert(follow);
  cout << perfil.profile << " now follows ";
  cout << follow << endl;
}

void unFollow(Profile& perfil, string follow){
  auto it = perfil.follow.find(follow);

  // Se encontrar seguidor, remove da lista
  if(it != perfil.follow.end()){
    perfil.follow.erase(it);
    cout << perfil.profile << " no longer follows ";
    cout << follow << endl;
  }
  else{
    cout << perfil.profile << " does not follow ";
    cout << follow << endl;
  }
}
