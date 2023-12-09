#include <iostream>
#include <memory>
#include <cstring>
#include <ctime>

#include "client.hpp"
#include "udp.hpp"
#include "data.hpp"

using std::make_unique;
using std::string;
using std::toupper;
using std::getline;
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
    cin.ignore();
    if(profile.size() < 4 || profile.size() > 20){
      cout << "Nome do perfil deve ter 4 a 20 caracteres\n";
      profile.clear();
    }
  }
  // Insere '@' no inicio do perfil
  profile.insert(profile.begin(), '@');

  // Envia um pacote vazio ao servidor
  // (endereco do cliente só atualiza no servidor após o segundo packet)
  // (não sei resolver kk )
  udp.ping();

  // Tentativa de login
  if(udp.login(profile) < 0){
    perror("login failed");
    exit(EXIT_FAILURE);
  }

  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "Logado como " << profile << '\n';
  cout << "Comandos: SEND msg | FOLLOW profile | UNFOLLOW profile" << endl;
  
  while(true){
    // Leitura da mensagem a ser enviada
    cout << "> ";
    string input, cmd, arg;
    getline(cin, input);

    size_t it = input.find(' ');
    if(it != string::npos){
      cmd = input.substr(0, it);
      arg = input.substr(it+1);
    }
    else cmd = input;

    auto packet = make_unique<Packet>();
    int ret;

    PacketType command;
    command = cmdToEnum(cmd);
    switch(command){
    case SEND:
      packet->timestamp = time(NULL);
      packet->type = SEND;
      strncpy(packet->payload, arg.c_str(), MAXLEN);
      packet->length = strnlen(packet->payload, MAXLEN);
      ret = udp.envia(move(packet));
      cout << "envia() = " << ret << endl;
      break;

    case FOLLOW:
      packet->timestamp = time(NULL);
      packet->type = FOLLOW;
      strncpy(packet->payload, arg.c_str(), MAXLEN);
      packet->length = strnlen(packet->payload, MAXLEN);
      ret = udp.envia(move(packet));
      cout << "follow" << endl;
      break;

    case UNFOLLOW:
      packet->timestamp = time(NULL);
      packet->type = UNFOLLOW;
      strncpy(packet->payload, arg.c_str(), MAXLEN);
      packet->length = strnlen(packet->payload, MAXLEN);
      ret = udp.envia(move(packet));
      cout << "unfollow" << endl;
      break;
    
    case QUIT:
      cout << "quit" << endl;
      exit(0);
      break;

    default:
      cout << "Comando inválido" << endl;
      break;
    }

  }

  return 0;
}

PacketType cmdToEnum(string cmd){
  for(auto &c : cmd) c = toupper(c);
  if(cmd == "SEND" || cmd == "S")
    return SEND;
  
  if(cmd == "FOLLOW" || cmd == "F")
    return FOLLOW;

  if(cmd == "UNFOLLOW" || cmd == "U")
    return UNFOLLOW;
  
  if(cmd == "EXIT" || cmd == "QUIT" ||
     cmd == "^C" || cmd == "^D")
    return QUIT;

  return UNKNOWN;
}