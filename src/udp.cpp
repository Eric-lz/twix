// #include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "data.hpp"
#include "udp.hpp"

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::move;

int envia(int sockfd, unique_ptr<Packet> packet){
  struct sockaddr_in servaddr{
    .sin_family      = AF_INET,
    .sin_port        = htons(PORT)
  };
  servaddr.sin_addr.s_addr = INADDR_ANY;

  // Creating socket file descriptor
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    return -2;
  }

  // Envia mensagem ao servidor
  return sendto(sockfd, packet.get(), sizeof(Packet),
        MSG_CONFIRM, (struct sockaddr*) &servaddr,
        sizeof(servaddr));
}

// Recebe resposta do servidor
unique_ptr<Packet> recebe(int sockfd){
  Packet* packet = new Packet;
  socklen_t len;

  struct sockaddr_in servaddr{
    .sin_family      = AF_INET,
    .sin_port        = htons(PORT)
  };
  servaddr.sin_addr.s_addr = INADDR_ANY;

  recvfrom(sockfd, packet, sizeof(Packet),
              MSG_WAITALL, (struct sockaddr*) &servaddr,
              &len);

  return make_unique<Packet>(*packet);
}