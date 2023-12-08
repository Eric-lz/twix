#include <iostream>
#include <memory>

#include "udp.hpp"
#include "data.hpp"

#define PORT    4000
#define MAXLINE 1024

using std::cout;
using std::endl;

// Driver code
int main() {
  UDP udp;
  udp.openSocket();
  udp.bindSocket();

  while(true){
    // Recebe mensagem do cliente
    std::unique_ptr<Packet> packet;
    packet = udp.recebe();

    // Imprime a mensagem recebida
    cout << "seqn: " << packet->seqn << "\n";
    cout << "timestamp: " << packet->timestamp << "\n";
    cout << "length: " << packet->length << "\n";
    cout << "type: " << packet->type << "\n";
    cout << "payload: " << packet->payload << endl;
  }

  return 0;
}