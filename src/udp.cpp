#include <memory>
#include <cstring>

#include "data.hpp"
#include "udp.hpp"

using std::unique_ptr;
using std::make_unique;
using std::move;
using std::memset;

UDP::UDP(){
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  servaddr.sin_family      = AF_INET;
  servaddr.sin_port        = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  seqIn = 0;
  seqOut = 0;
}

UDP::~UDP(){
  close(sockfd);
}

// Create socket file descriptor
int UDP::openSocket(){
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    return -1;
  }
  
  return 0;
}

// (Server) Bind address to socket
int UDP::bindSocket(){
  if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0){
    perror("bind failed");
    return -1;
  }

  return 0;
}

// Envia um packet para o socket aberto
int UDP::envia(unique_ptr<Packet> packet){
  packet->seqn = seqOut++;

  return sendto(sockfd, packet.get(), sizeof(Packet),
        MSG_CONFIRM, (struct sockaddr*) &servaddr,
        sizeof(servaddr));
}

// Recebe packet do socket aberto
unique_ptr<Packet> UDP::recebe(){
  Packet* packet = new Packet;

  recvfrom(sockfd, packet, sizeof(Packet), MSG_WAITALL,
          (struct sockaddr*) &cliaddr, &cliaddr_len);

  seqIn = packet->seqn;

  return make_unique<Packet>(*packet);
}