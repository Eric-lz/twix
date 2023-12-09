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
      for(unsigned long i = 0; i < data.size(); i++){
        if(data.at(i).port == cliaddr.sin_port){
          // Adiciona perfil a lista de seguidos
          data.at(i).follow.push_back(packet->payload);
          cout << data.at(i).profile << " now follows ";
          cout << packet->payload << endl;
        }
      }
      break;

    case UNFOLLOW:
      cout << "UNFOLLOW" << endl;
      break;

    case SEND:
      // Procura o perfil que enviou a mensagem
      for(unsigned long i = 0; i < data.size(); i++){
        if(data.at(i).port == cliaddr.sin_port){
          cout << data.at(i).profile << ": ";
          cout << packet->payload << endl;
        }
      }
      break;

    default:
      perror("unknown packet type");
      break;
    }
  }

  return 0;
}
