#pragma once
#include <memory>
#include <map>

#include "udp.hpp"

// Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int pending;            // Quantidade de leitores pendentes
  int length;             // Tamanho da mensagem
  char message[MAXLEN+1]; //Dados da mensagem
} Notification;

class Notifications{
private:
  std::vector<Notification> notifications;
  std::multimap<std::string, unsigned> pending_notifs;
  unsigned int notif_id;

public:
  // Recebe mensagem de um client
  void newMessage(std::unique_ptr<Packet> packet);
};