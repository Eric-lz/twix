#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <memory>

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
    std::unique_ptr<Packet> packet;
    packet = recebe(sockfd);

    // Imprime a mensagem recebida
    cout << "seqn: " << packet->seqn << "\n";
    cout << "timestamp: " << packet->timestamp << "\n";
    cout << "length: " << packet->length << "\n";
    cout << "type: " << packet->type << "\n";
    cout << "payload: " << packet->payload << endl;
  }

  close(sockfd);
  return 0;
}