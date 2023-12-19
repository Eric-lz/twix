#pragma once
#include <memory>
#include <vector>
#include <map>
#include <set>

#include "udp.hpp"

// Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int pending;            // Quantidade de leitores pendentes
  int length;             // Tamanho da mensagem
  char sender[21];        // Perfil que enviou a mensagem
  char message[MAXLEN+1]; // Dados da mensagem
} Notification;

class Notifications{
private:
  std::vector<Notification> notifications;
  std::multimap<std::string, unsigned int> pending_notifs;
  unsigned int notif_id;

public:
  // Constructor
  Notifications();

  // Recebe mensagem de um client
  void newMessage(std::unique_ptr<Packet> packet, std::set<std::string> followers);
  
  // Retorna a lista de notificações pendentes
  std::multimap<std::string, unsigned int> getPendingNotifs();

  // Retorna se lista de notificações pendentes esta vazia
  bool isEmpty();
  
  // Remove uma notificação da lista 
  void deleteNotif(std::multimap<std::string, unsigned int>::iterator it);
};