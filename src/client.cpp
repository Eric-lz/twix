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

    // Separa a entrada em comando e argumento
    size_t it = input.find(' ');
    if(it != string::npos){
      cmd = input.substr(0, it);
      arg = input.substr(it+1);
    }
    else cmd = input;

    // Converte entrada para Enum
    // (necessario para usar no switch)
    PacketType command;
    command = cmdToEnum(cmd);

    switch(command){
    case SEND:
    case FOLLOW:
    case UNFOLLOW:
      if(envia(udp, command, arg) < 0)
        cout << "Erro ao enviar comando" << endl;
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

// Envia um pacote via UDP de tipo TYPE com mensagem PAYLOAD
int envia(UDP& udp, PacketType type, string payload){
  // Se payload for vazio, nao envia
  if(payload.empty()) return -1;

  // Cria novo packet
  auto packet = make_unique<Packet>();

  // Se for FOLLOW ou UNFOLLOW e nao tiver @, adiciona
  if(type == FOLLOW || type == UNFOLLOW)
    if(payload.at(0) != '@')
      payload.insert(0, 1, '@');

  // Preenche os campos do packet
  packet->timestamp = time(NULL);
  packet->type = type;
  strncpy(packet->payload, payload.c_str(), MAXLEN);
  packet->length = strnlen(packet->payload, MAXLEN);

  // Envia o pacote
  return udp.envia(move(packet));
}