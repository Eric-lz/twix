#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>

// Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int pending;            // Quantidade de leitores pendentes
  int length;             // Tamanho da mensagem
  char message[MAXLEN+1]; //Dados da mensagem
} Notification;

// Estrutura para armazenar os perfis
typedef struct profile_t{
  std::string name;
  std::set<std::string> follow;
} Profile;

class ProfilesList{
private:
  // list of profiles
  std::vector<Profile> profiles;
  std::vector<Notification> notifications;
  std::multimap<std::string, unsigned> pending_notifs;
  unsigned int notif_id = 0;

  // Procura perfil pelo nome e retorna seu indice
  int getProfileByName(std::string name);

public:
  // Loga com um perfil
  void login(std::string profile);

  // Segue novo perfil
  void addFollow(std::string follower, std::string following);

  // Para de seguir um perfil
  void unFollow(std::string follower, std::string following);

  // Recebe mensagem de um client
  void recebeMensagem(std::unique_ptr<Packet> packet);
};

// Busca o client pela porta e retorna seu indice
// Retorna -1 se nao encontra
int findPort(std::map<in_port_t, int>& clients, in_port_t port);