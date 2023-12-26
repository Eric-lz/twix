#include <iostream>
#include <memory>
#include <cstring>
#include <ctime>

#include "client.hpp"
#include "udp.hpp"

using std::unique_ptr;
using std::make_unique;
using std::move;
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
  
  // Leitura do nome do perfil com limitações de nome de usuário definidas no escopo do trabalh
  string name;
  while(name.empty()){
    cout << "Digite seu perfil: @";
    cin >> name;
    cin.ignore();
    if(name.size() < 4 || name.size() > 20){
      cout << "Nome do perfil deve ter 4 a 20 caracteres\n";
      name.clear();
    }
  }
  // Insere '@' no inicio do perfil
  name.insert(name.begin(), '@');

  // Tentativa de login
  if(udp.login(name) < 0){
    /*TODO: TRATAR RESPOSTA SERVIDOR QUANDO TIVER 2 SESSOES REALIZADAS*/
    perror("login failed");
    exit(EXIT_FAILURE);
  }

  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "Logado como " << name << '\n';
  cout << "Cada mensagem deve conter no max 140 caracteres \n";
  cout << "Comandos: SEND msg | FOLLOW profile | UNFOLLOW profile" << endl;
  /*comandos tb aceitam somente a letra inicial do comanto e nao eh necessario usar capslock*/
  while(true){
    // Leitura da mensagem a ser enviada
    cout << "> ";
    string input, cmd, arg;
    getline(cin, input);

    // Separa a entrada em comando e argumento
    /*
    EXEMPLO DE ENTRADA VALIDA ESPERADA:
    SEND ola!       (press enter)
    unfollow @onze  (press enter)
    f @doze         (press enter)
    u dois  (press enter)
    follow quatro         (press enter)
    */
    size_t it = input.find(' ');
    if(it != string::npos){
      cmd = input.substr(0, it);
      arg = input.substr(it+1);
    }
    else cmd = input;

    // Converte entrada para Enum
    /*Como cada cmd pode ser digitado de varias formas pelo o usuario, o cmdToEnum visa padronizar o envio desses que traduz o comando para seu numero correspondente */
    // (necessario para usar no switch)
    PacketType command;
    command = cmdToEnum(cmd);

    
    switch(command){
    case SEND:
    case FOLLOW:
    case UNFOLLOW:
      envia(udp, command, arg);
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
  if(payload.empty()){
    cout << "Comando sem argumento" << endl;
    return -1;
  }

  // Cria novo packet
  // Pacote que cliente cria para enviar ao servidor acesse udp.hpp e procure por typedef struct packet_t{
  /*
  //Número de sequência
  //Timestamp do dado
  //Tipo do pacote (ex. DATA | CMD)
  //Comprimento do payload
  //Perfil que enviou o pacote
  //Dados da mensagem
  */
  //unique_ptr<Packet> packet; /*unique_ptr eh um smart pointer*/

  auto packet = make_unique<Packet>(); /*Forma otimizada de usar o unique_ptr por isso o trecho acima foi comentado*/

  // Se for FOLLOW ou UNFOLLOW e nao tiver @, adiciona tratamento para conforto do usuario
  if(type == FOLLOW || type == UNFOLLOW)
    if(payload.at(0) != '@')
      payload.insert(0, 1, '@');

  // Preenche os campos do packet
  packet->timestamp = time(NULL); //TEST: caso de erro na ordem de cronologia das mensagens, verificar se o timestamp esta sendo preenchido corretamente 
  packet->type = type;
  strncpy(packet->payload, payload.c_str(), MAXLEN); //MAXLEN que limita os 140 caract. implementado em udp.hpp
  packet->length = strnlen(packet->payload, MAXLEN);

  // Envia o pacote
  return udp.envia(move(packet)); //move eh uma funcao que move o conteudo de um ponteiro para outro
}
