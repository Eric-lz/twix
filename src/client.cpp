#include <iostream>
#include <memory>
#include <cstring>
#include <ctime>
#include <thread>

#include "client.hpp"
#include "udp.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // Welcome text
  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "Bem-vindo ao Twix\n";

  // Endereço do servidor
  sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  // Leitura do IP se não foi fornecido por parâmetro na inicialização
  if(argc == 1){
    string serverIP;

    cout << "Digite o IP do servidor: ";
    cin >> serverIP;

    inet_aton(serverIP.c_str(), &serverAddress.sin_addr);
  }
  else{
    inet_aton(argv[1], &serverAddress.sin_addr);
  }
  
  // UDP object
  UDP udp(serverAddress);
  udp.openSocket();

  // Leitura do nome do perfil com as restrições definidas na especificação do trabalho
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
  cout << "Comandos: SEND msg | FOLLOW profile | UNFOLLOW profile | QUIT" << endl;
  /*comandos tb aceitam somente a letra inicial do comando e nao eh necessario usar capslock
  EXEMPLO DE ENTRADA VALIDA ESPERADA:
  SEND ola!
  unfollow @onze
  f @doze
  u dois 
  follow quatro
  */

  // Thread que recebe pacotes e responde a pings
  thread thReply(threadReply, &udp);

  // Thread principal: trata leitura do teclado e envio de pacotes do usuario
  while(true){
    // Leitura da entrada do teclado
    cout << "> ";
    string input, cmd, arg;
    getline(cin, input);

    // Separa a entrada em comando e argumento
    size_t it = input.find(' ');
    if(it != string::npos){
      cmd = input.substr(0, it);
      arg = input.substr(it+1);
    }
    else cmd = input; // comando sem argumento

    PacketType command;       // Comandos são iguais aos tipos de packet que existem
    command = cmdToEnum(cmd); // Converte comando para Enum
    
    // Toma ações baseado no comando digitado pelo usuário
    switch(command){
    case SEND:
    case FOLLOW:
    case UNFOLLOW:
      envia(udp, command, arg);
      break;

    case LOGOUT:
      logout(udp);
      exit(EXIT_SUCCESS);
      break;

    default:
      cout << "Comando inválido" << endl;
      break;
    }
  }

  return 0;
}

// Thread que recebe notificacoes e imprime na tela
// Também repsonde mensagens de ping
void threadReply(UDP* udp){
  while(true){
    // Endereço do remetente
    sockaddr_in recvAddr;
    memset(&recvAddr, 0, sizeof(recvAddr));

    // Pacote recebido
    auto packet = udp->recebe(&recvAddr);

    switch(packet->type){
      case PING:  // se for ping, envia resposta de volta
        udp->envia(move(packet), &recvAddr);
        break;
      
      case SEND:  // se for notificação, printa
        printNotif(move(packet));
        break;

      default:    // nunca deve chegar nesse ponto
        cout << "unknown action" << endl;
    }
  }
}

/* Como cada comando pode ser digitado de varias formas pelo usuario,
  o cmdToEnum() visa padronizar o envio do comando,
  traduzindo-o para um Enum. */
PacketType cmdToEnum(string cmd){
  for(auto &c : cmd) c = toupper(c);
  if(cmd == "SEND" || cmd == "S")
    return SEND;
  
  if(cmd == "FOLLOW" || cmd == "F")
    return FOLLOW;

  if(cmd == "UNFOLLOW" || cmd == "U")
    return UNFOLLOW;
  
  if(cmd == "EXIT" || cmd == "QUIT") // TODO: CTRL+D ou CTRL+C não funcionam (impossível?)
    return LOGOUT;

  return UNKNOWN;
}

// Imprime notificacao na tela
void printNotif(unique_ptr<Packet> notif){
  // converte timestamp para ASCII
  time_t rawtime = notif->timestamp;
  struct tm *time = localtime(&rawtime);
  char timebuffer[30];

  strftime(timebuffer, sizeof(timebuffer), "%d/%m/%Y %H:%M:%S", time);

  // Printa notificação
  cout << timebuffer << ' ';
  cout << notif->profile << ": ";
  cout << notif->payload << "\n> " << flush;
}

// Envia um pacote via UDP de tipo TYPE com mensagem PAYLOAD
int envia(UDP& udp, PacketType type, string payload){
  // Se payload for vazio, nao envia
  if(payload.empty()){
    cout << "Comando sem argumento" << endl;  // na verdade é payload vazio mas efetivamente da na mesma kkk
    return -1;
  }

  // Pacote que cliente cria para enviar ao servidor acesse udp.hpp e procure por typedef struct packet_t{
  /*
  //Número de sequência
  //Timestamp do dado
  //Tipo do pacote (ex. DATA | CMD)
  //Comprimento do payload
  //Perfil que enviou o pacote
  //Dados da mensagem
  */

  // Cria novo packet
  //unique_ptr<Packet> packet; /*unique_ptr eh um smart pointer*/
  auto packet = make_unique<Packet>(); // Forma otimizada de usar o unique_ptr por isso o trecho acima foi comentado 

  // Se for FOLLOW ou UNFOLLOW e nao tiver @, adiciona (melhor experiencia pro usuario)
  if(type == FOLLOW || type == UNFOLLOW)
    if(payload.at(0) != '@')
      payload.insert(0, 1, '@');

  // Preenche os campos do packet
  packet->timestamp = time(nullptr);
  packet->type = type;
  strncpy(packet->payload, payload.c_str(), MAXLEN); //MAXLEN que limita os 140 caract. implementado em udp.hpp

  // Envia o pacote
  return udp.envia(move(packet)); // move eh uma funcao que move o conteudo de um ponteiro para outro
}

// Informa ao servidor que esta sessão desconectou
int logout(UDP& udp){
  // Cria pacote
  auto packet = make_unique<Packet>();
  packet->type = LOGOUT;
  strncpy(packet->payload, "logout", MAXLEN);

  // Envia o pacote
  return udp.envia(move(packet));
}