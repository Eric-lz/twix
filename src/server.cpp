#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT    4000
#define MAXLINE 1024

// Driver code
int main() {
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;
  
  // Creating socket file descriptor
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family       = AF_INET; // IPv4
  servaddr.sin_addr.s_addr  = INADDR_ANY;
  servaddr.sin_port         = htons(PORT);
  
  // Bind the socket with the server address
  if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  
  // Recebe mensagem do cliente
  int n;
  socklen_t len = sizeof(cliaddr);
  n = recvfrom(sockfd, (char*) buffer, MAXLINE,
              MSG_WAITALL, (struct sockaddr*) &cliaddr,
              &len);
  
  // String deve terminar com null
  buffer[n] = '\0';

  // Imprime a mensagem recebida
  std::cout << "Mensagem recebida: " << buffer << std::endl;

  // Envia resposta ao cliente
  std::string response = "Hello from server.";
  sendto(sockfd, response.c_str(), response.size(),
        MSG_CONFIRM, (struct sockaddr*) &cliaddr,
        len);
  std::cout << "Resposta enviada." << std::endl;

  return 0;
}