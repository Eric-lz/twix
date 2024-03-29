#include <iostream>
#include <memory>
#include <string>
#include <cstring>

#include "udp.hpp"

using namespace std;

UDP::UDP(){
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  servaddr.sin_family      = AF_INET;
  servaddr.sin_port        = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY; // modo servidor

  cliaddr_len = sizeof(cliaddr);

  seqIn = 0;
  seqOut = 0;
}

UDP::UDP(sockaddr_in address){
  memset(&cliaddr, 0, sizeof(cliaddr));
  servaddr = address;

  cliaddr_len = sizeof(cliaddr);

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
int UDP::envia(unique_ptr<Packet> packet, struct sockaddr_in* outaddr, const char* sender){
  // Preenche campos restantes do UDP
  packet->seqn = seqOut++;
  //packet->timestamp = time(nullptr);
  packet->length = strnlen(packet->payload, MAXLEN);
  struct sockaddr_in addr;

  // Se for indicado um enderco, enviar pacote para ele (servidor -> cliente)
  // Se nao, usa o endereco indicado na inicialização (cliente -> servidor)
  if(outaddr != nullptr)
    addr = *outaddr;
  else
    addr = servaddr;
  
  // Envia o nome do perfil que enviou a mensagem
  if(sender != nullptr)
    strncpy(packet->profile, sender, 20);
  else
    strncpy(packet->profile, name.c_str(), 20);

  //Funcao padrao de envio por UDP
  return sendto(sockfd, packet.get(), sizeof(Packet),
        MSG_CONFIRM, (struct sockaddr*) &addr,
        sizeof(addr));
}

// Recebe packet do socket aberto
unique_ptr<Packet> UDP::recebe(struct sockaddr_in* inaddr){
  Packet* packet = new Packet; //ponteiro bruto
  //recvfrom eh uma funcao padrao de recebimento de pacotes UDP precisa de ponteiro bruto
  recvfrom(sockfd, packet, sizeof(Packet), MSG_WAITALL,
          (struct sockaddr*) &cliaddr, &cliaddr_len);

  seqIn = packet->seqn;

  if(inaddr != nullptr){
    inaddr->sin_addr = cliaddr.sin_addr;
    inaddr->sin_family = cliaddr.sin_family; //preenche de acordo o parametrizado no udp envia()
    inaddr->sin_port = cliaddr.sin_port;
  }

  return make_unique<Packet>(*packet); //transforma o ponteiro bruto em um ponteiro inteligente
}

// Envia um packet de login com o perfil do usuario
int UDP::login(const string name){
  auto packet = make_unique<Packet>();
  this->name = name; //troca o nome da classe UDP o string name mais especificamente, ajuda na identificação de sessoes do cliente

  // Monta pacote de login
  packet->type = LOGIN;
  strncpy(packet->payload, name.c_str(), 20);//TEST: verificar se o lenght contempla o @ do perfil, exemplo @victortresvictortres sera aceito? Tem @ + 20 caracteres
  packet->length = strnlen(packet->payload, 20);

  // Envia pacote
  return envia(move(packet));//ponteiro eh copiado para o envia e eh destruido no login
}

//TODO: envia uma mesagem com o contedo "ping", mas ninguem responde ainda, temso que implementar para debug entre maquinas
void UDP::ping(sockaddr_in address){
  auto packet = make_unique<Packet>();
  packet->type = PING;
  strncpy(packet->payload, "ping", 5);
  envia(move(packet), &address);
}

