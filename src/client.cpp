// #include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <memory>
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
  
  // Creating socket file descriptor
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  
while(true){
  // Leitura da mensagem a ser enviada
  std::string message;
  std::cout << "Mensagem: ";
  std::cin >> message;

  // auto packet = std::make_unique<Packet>();
  Packet* packet = new Packet;

  packet->seqn = 111;
  packet->length = message.size();
  packet->timestamp = 123456;
  packet->type = 0;
  // packet->payload = strdup(message.c_str());
  strncpy(packet->payload, message.c_str(), message.length()+1);

  cout << "seqn: " << packet->seqn << "\n";
  cout << "timestamp: " << packet->timestamp << "\n";
  cout << "length: " << packet->length << "\n";
  cout << "type: " << packet->type << "\n";
  cout << "payload: " << packet->payload << endl;

  // Envia packet
  // int ret = envia(sockfd, std::move(packet));
  int ret = envia(sockfd, packet);

  std::cout << "envia() = " << ret << std::endl;
}

  close(sockfd);
  return 0;
}