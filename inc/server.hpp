#pragma once
#include <vector>
#include <list>
#include <string>

// (Sugestão) Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int pending;            // Quantidade de leitores pendentes
  int length;             // Tamanho da mensagem
  char message[MAXLEN+1]; //Dados da mensagem
} Notification;

// Estrutura para armazenar os perfis
typedef struct profile_t{
  in_port_t port;
  std::string profile;
  std::list<std::string> follow;
  std::list<Notification> notif;
  std::list<std::pair<std::string, int>> pending_notif;
} Profile;
