#pragma once
#include <vector>
#include <set>
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
  std::set<std::string> follow;
  std::vector<Notification> notif;
  std::vector<std::pair<std::string, int>> pending_notif;
} Profile;
