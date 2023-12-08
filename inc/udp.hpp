#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory>

#include "data.hpp"

#define PORT 4000

class UDP{
private:
  // Socket file descriptor
  int sockfd;

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
  ~UDP(); // Fecha o socket

  // Create socket file descriptor
  int openSocket();
  
  // (Server) Bind address to socket
  int bindSocket();

  // Envia um packet para o socket aberto
  int envia(std::unique_ptr<Packet> packet);

  // Recebe packet do socket aberto
  // (optional) inaddr: returns the address of the client
  std::unique_ptr<Packet> recebe(struct sockaddr_in* inaddr = nullptr);

  // Envia um packet de login com o perfil do usuario
  int login(const std::string profile);

  // Envia um pacote vazio
  void ping();
};
