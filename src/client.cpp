#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
  struct sockaddr_in servaddr;
  
  // Creating socket file descriptor
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  
  // Filling server information
  servaddr.sin_family       = AF_INET;
  servaddr.sin_port         = htons(PORT);
  servaddr.sin_addr.s_addr  = INADDR_ANY;
  
  // Leitura da mensagem a ser enviada
  std::string message;
  std::cout << "Mensagem: ";
  std::cin >> message;

  // Envia mensagem ao servidor
  sendto(sockfd, message.c_str(), message.size(),
        MSG_CONFIRM, (struct sockaddr*) &servaddr,
        sizeof(servaddr));

  std::cout << "Mensagem enviada." << std::endl;
  
  // Recebe resposta do servidor
  int n;
  socklen_t len;
  n = recvfrom(sockfd, (char*) buffer, MAXLINE,
              MSG_WAITALL, (struct sockaddr*) &servaddr,
              &len);
  
  // String deve terminar com null
  buffer[n] = '\0';

  // Imprime resposta do servidor
  std::cout << "Resposta: " << buffer << std::endl;
  
  close(sockfd);
  return 0;
}