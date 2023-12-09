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

// Busca o client pela porta e retorna seu indice
// Retorna -1 se nao encontra
int findPort(std::vector<Profile>& data, in_port_t port);

// Adiciona um perfil na lista de seguidos
void addFollow(Profile& perfil, std::string follow);

// Remove um perfil da lista de seguidos
void unFollow(Profile& perfil, std::string follow);
