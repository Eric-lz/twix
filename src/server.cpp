// #include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "udp.hpp"
#include "data.hpp"

#define PORT    4000
#define MAXLINE 1024

using std::cout;
using std::endl;

// Driver code
int main() {
  int sockfd;
  struct sockaddr_in servaddr; //, cliaddr;
  
  // Creating socket file descriptor
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  
  // memset(&servaddr, 0, sizeof(servaddr));
  // memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family       = AF_INET; // IPv4
  servaddr.sin_addr.s_addr  = INADDR_ANY;
  servaddr.sin_port         = htons(PORT);
  
  // Bind the socket with the server address
  if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  
while(true){
  // Recebe mensagem do cliente
  // auto packet = std::make_unique<Packet>();
  Packet* packet = nullptr;
  packet = recebe(sockfd);

  // std::string msg = packet->payload;

  // Imprime a mensagem recebida
  // std::cout << "Mensagem recebida: " << msg << std::endl;
  cout << "seqn: " << packet->seqn << "\n";
  cout << "timestamp: " << packet->timestamp << "\n";
  cout << "length: " << packet->length << "\n";
  cout << "type: " << packet->type << "\n";
  // cout << "payload: " << packet->payload << endl;
  printf("payload: %s\n", packet->payload);
}

  close(sockfd);
  return 0;
}