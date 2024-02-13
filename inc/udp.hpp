#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory>
#include <string>

// Porta da aplicacao
#define PORT 4000

// Tamanho da mensagem do packet
#define MAXLEN 140

// Tipo do pacote (ex. DATA, CMD)
enum PacketType{
  PING,
  LOGIN,
  FOLLOW,
  UNFOLLOW,
  SEND,
  LOGOUT,
  UNKNOWN
};

//Estrutura para troca de mensagens entre cliente e servidor
typedef struct packet_t{
  unsigned int seqn;      //Número de sequência
  unsigned int timestamp; //Timestamp do dado
  PacketType type;        //Tipo do pacote (ex. PING | SEND | FOLLOW...)
  int length;             //Comprimento do payload
  char profile[21];       //Perfil que enviou o pacote
  char payload[MAXLEN+1]; //Dados da mensagem 140 CARACTERES +1 detalhe do C++ para contabilidar o /0
} Packet;


// Abstrai criacao de socket e envio de pacotes
class UDP{
private:
  // Socket file descriptor
  int sockfd;
  // Profile name
  std::string name = "server"; //esse nome eh alterado quando o cliente faz login

  // Server address structure
  struct sockaddr_in servaddr;

  // Client address structure
  struct sockaddr_in cliaddr;
  socklen_t cliaddr_len;

  // Sequence number (incoming packets)
  int seqIn;
  // Sequence number (outgoing packets)
  int seqOut;

public:
  UDP();  // Inicializa estruturas
  UDP(sockaddr_in address);
  ~UDP(); // Fecha o socket

  // Create socket file descriptor
  int openSocket();
  
  // (Server) Bind address to socket
  int bindSocket();

  // Envia um packet para o socket aberto, recebe pacote do move, opcionalmente passa o endereco de envio do pacote se nao passar o cliente envia automaticamente
  int envia(std::unique_ptr<Packet> packet, struct sockaddr_in* outaddr = nullptr, const char* sender = nullptr);

  // Recebe packet do socket aberto
  // (optional) inaddr: returns the address of the client
  std::unique_ptr<Packet> recebe(struct sockaddr_in* inaddr = nullptr);

  // Envia um packet de login com o perfil do usuario
  int login(const std::string name);

  // Envia um pacote vazio
  void ping(sockaddr_in address);
};
