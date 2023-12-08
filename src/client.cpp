#include <iostream>
#include <memory>
#include <cstring>
#include <ctime>

#include "udp.hpp"
#include "data.hpp"

using std::make_unique;
using std::string;
using std::cin;
using std::cout;
using std::endl;

// Driver code
int main() {
  // UDP object
  UDP udp;
  udp.openSocket();
  
  while(true){
    // Leitura da mensagem a ser enviada
    string message;
    cout << "Mensagem: ";
    cin >> message;

    auto packet = make_unique<Packet>();

    packet->length = message.size();
    packet->timestamp = time(NULL);
    packet->type = 5;
    strncpy(packet->payload, message.c_str(), message.length()+1);

    // Envia packet
    int ret = udp.envia(move(packet));

    cout << "envia() = " << ret << endl;
  }

  return 0;
}