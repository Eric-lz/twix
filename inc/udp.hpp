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
  socklen_t servaddr_len;

  // Client address structure
  struct sockaddr_in cliaddr;
  socklen_t cliaddr_len;

  // Sequence number (incoming packets)
  int seqIn;
  // Sequence number (outgoing packets)
  int seqOut;

public:
  UDP();
  ~UDP();

  int openSocket();
  int bindSocket();
  int envia(std::unique_ptr<Packet> packet);
  std::unique_ptr<Packet> recebe();
};
