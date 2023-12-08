#pragma once
#include <string>

// Tamanho da mensagem do packet
#define MAXLEN 140

// (Sugestão) Estrutura para troca de mensagens entre cliente e servidor
typedef struct packet_t{
  unsigned int seqn;      //Número de sequência
  unsigned int timestamp; //Timestamp do dado
  int type;               //Tipo do pacote (ex. DATA | CMD)
  int length;             //Comprimento do payload
  char payload[MAXLEN+1]; //Dados da mensagem
} Packet;

// (Sugestão) Estrutura de metadados das notificações
typedef struct notification_t{
  unsigned int id;        // Identificador da notificação
  unsigned int timestamp; // Timestamp da notificação
  int pending;            // Quantidade de leitores pendentes
  int length;             // Tamanho da mensagem
  char message[MAXLEN+1]; //Dados da mensagem
} Notification;
