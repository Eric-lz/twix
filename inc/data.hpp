#pragma once
#include <string>

// Tamanho da mensagem do packet
#define TAM 140

// (Sugestão) Estrutura para troca de mensagens entre cliente e servidor
typedef struct packet_t{
  unsigned int seqn;      //Número de sequência
  unsigned int timestamp; //Timestamp do dado
  int length;             //Comprimento do payload
  int type;               //Tipo do pacote (ex. DATA | CMD)
  char payload[TAM];      //Dados da mensagem
} Packet;

// (Sugestão) Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int length;             // Tamanho da mensagem
  int pending;            // Quantidade de leitores pendentes
  std::string message;    // Mensagem
} Notification;
