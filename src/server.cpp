#include <iostream>
#include <memory>
#include <cstring>

#include "udp.hpp"
#include "data.hpp"

using std::unique_ptr;
using std::cout;
using std::endl;

int main() {
  // UDP object
  UDP udp;
  udp.openSocket();
  udp.bindSocket();

  while(true){
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe mensagem do cliente
    unique_ptr<Packet> packet;
    packet = udp.recebe(&cliaddr);

    // Imprime a mensagem recebida
    cout << "cli.port: " << cliaddr.sin_port << "\n";
    cout << "seqn: " << packet->seqn << "\n";
    cout << "timestamp: " << packet->timestamp << "\n";
    cout << "type: " << packet->type << "\n";
    cout << "length: " << packet->length << "\n";
    cout << "payload: " << packet->payload << endl;
  }

  return 0;
}