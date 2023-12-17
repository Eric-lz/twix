#include <memory>
#include <string>
#include <cstring>

#include "udp.hpp"

using std::string;
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
int UDP::envia(unique_ptr<Packet> packet, struct sockaddr_in* outaddr){
  packet->seqn = seqOut++;
  struct sockaddr_in addr;

  // Se for indicado um enderco, enviar pacote para ele
  if(outaddr != nullptr){
    addr.sin_addr = outaddr->sin_addr;
    addr.sin_family = outaddr->sin_family;
    addr.sin_port = outaddr->sin_port;
  }
  // Se nao, usa o endereco do servidor como padrao
  else{
    addr.sin_addr = servaddr.sin_addr;
    addr.sin_family = servaddr.sin_family;
    addr.sin_port = servaddr.sin_port;
  }
  strncpy(packet->profile, name.c_str(), 20);

  return sendto(sockfd, packet.get(), sizeof(Packet),
        MSG_CONFIRM, (struct sockaddr*) &addr,
        sizeof(addr));
}

// Recebe packet do socket aberto
unique_ptr<Packet> UDP::recebe(struct sockaddr_in* inaddr){
  Packet* packet = new Packet;

  recvfrom(sockfd, packet, sizeof(Packet), MSG_WAITALL,
          (struct sockaddr*) &cliaddr, &cliaddr_len);

  seqIn = packet->seqn;

  if(inaddr != nullptr){
    inaddr->sin_addr = cliaddr.sin_addr;
    inaddr->sin_family = cliaddr.sin_family;
    inaddr->sin_port = cliaddr.sin_port;
  }

  return make_unique<Packet>(*packet);
}

// Envia um packet de login com o perfil do usuario
int UDP::login(const string name){
  auto packet = make_unique<Packet>();
  this->name = name;

  // Monta pacote de login
  packet->timestamp = time(NULL);
  packet->type = LOGIN;
  strncpy(packet->payload, name.c_str(), 20);
  packet->length = strnlen(packet->payload, 20);

  // Envia pacote
  return envia(move(packet));
}

void UDP::changeAddr(const sockaddr_in inaddr){
  servaddr = inaddr;
  this->bindSocket();
}

void UDP::ping(){
  auto packet = make_unique<Packet>();
  packet->timestamp = time(NULL);
  packet->type = PING;
  strncpy(packet->payload, "ping", 5);
  envia(move(packet));
}

