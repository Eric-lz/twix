#include <iostream>
#include <memory>
#include <cstring>
#include <ctime>

#include "udp.hpp"
#include "data.hpp"

using std::cout;
using std::endl;

// Driver code
int main() {
  // UDP object
  UDP udp;
  udp.openSocket();
  
  while(true){
    // Leitura da mensagem a ser enviada
    std::string message;
    std::cout << "Mensagem: ";
    std::cin >> message;

    auto packet = std::make_unique<Packet>();

    packet->length = message.size();
    packet->timestamp = time(NULL);
    packet->type = 5;
    strncpy(packet->payload, message.c_str(), message.length()+1);

    // Envia packet
    int ret = udp.envia(std::move(packet));

    std::cout << "envia() = " << ret << std::endl;
  }

  return 0;
}