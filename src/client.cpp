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

int main() {
  // UDP object
  UDP udp;
  udp.openSocket();

  // Welcome text
  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "Bem-vindo ao Twix\n";
  
  // Leitura do nome do perfil
  string profile;
  while(profile.empty()){
    cout << "Digite seu perfil: @";
    cin >> profile;
    if(profile.size() < 4 || profile.size() > 20){
      cout << "Nome do perfil deve ter 4 a 20 caracteres\n";
      profile.clear();
    }
  }
  // Insere '@' no inicio do perfil
  profile.insert(profile.begin(), '@');

  // Envia um pacote vazio ao servidor
  udp.ping();

  // Tentativa de login
  if(udp.login(profile) < 0){
    perror("login failed");
    exit(EXIT_FAILURE);
  }

  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "Logado como " << profile << endl;
  
  while(true){
    // Leitura da mensagem a ser enviada
    string message;
    cout << "Mensagem: ";
    cin >> message;

    auto packet = make_unique<Packet>();

    packet->timestamp = time(NULL);
    packet->type = SEND;
    strncpy(packet->payload, message.c_str(), MAXLEN);
    packet->length = strnlen(packet->payload, MAXLEN);

    // Envia packet
    int ret = udp.envia(move(packet));

    cout << "envia() = " << ret << endl;
  }

  return 0;
}