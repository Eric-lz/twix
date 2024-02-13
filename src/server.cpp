#include <iostream>
#include <thread>
#include <memory>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <set>

#include "udp.hpp"
#include "server.hpp"
#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

using namespace std;

int main() {
  // Server memory
  // Instances
  Instances* instances = new Instances; /*Relaciona perfil a portas*/
  // Profiles
  Profiles* profiles = new Profiles;  /*Perfil e seus seguidores*/
  // Notifications
  Notifications* notifications = new Notifications; /*Notificacoes | quem enviou | uid | mensagem |  */
  /* TODO: Implementar logica de notificacoes a serem enviadas 1)enviar de fato 2)marcar como enviada */

  // Open UDP socket to listen for logins
  UDP* udp = new UDP;
  udp->openSocket();
  udp->bindSocket();

  // Thread que gerencia notificacoes e envios
  // (mutexes precisam ser criados aqui e passados para a outra thread?)
  // (usar mutex global é uma solução?)
  thread thEnvia(threadSession, udp, instances, notifications);

  // Thread que verifica se o cliente ainda está com a sessão aberta (keep alive)
  thread thKeepAlive(threadKeepAlive, udp, instances);

  //Thread que escuta o recebimento de mensagens
  while(true){
    // Endereco do cliente que enviou a mensagem
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Recebe pacote do cliente
    unique_ptr<Packet> packet;
    packet = udp->recebe(&cliaddr); //udp recebe pacote e &cliaddr recebe endereco do cliente

    // Lista de seguidores
    set<string> followers;

    auto inst = instances->getInstances();

    // Ação do servidor baseada no tipo de packet recebido do cliente
    switch(packet->type){
    case PING: //TODO: esse metodo pode ser usado para validar conexao entre cliente e servidor, pq udp nao tem conceito de conexcao
      instances->setAlive(packet->profile);
      break;

    case LOGIN:
      // Login
      // Create new thread to handle messages
      // (pass UDP socket and server memory)
      //TODO: Implementar verificacao de quantidade de sessoes por perfil, nao podemos deixar o mesmo perfil ter mais de dois acessos simultaneos, se tentar acessar em 3 maquina o cliente recebe negativa
      instances->newInstance(packet->payload, cliaddr);
      inst = instances->getInstances();
      profiles->login(packet->payload);
      cout << packet->payload << " logged in from ";
      cout << inet_ntoa(cliaddr.sin_addr) << cliaddr.sin_port << endl;
      break;

    case FOLLOW:
      // Adiciona seguidor
      profiles->addFollow(packet->profile, packet->payload);
      break;

    case UNFOLLOW:
      // Remove seguidor
      profiles->unFollow(packet->profile, packet->payload);
      break;

    case SEND:
      // Adiciona mensagem na lista de notificacoes
      // Pega lista de seguidores do perfil que enviou a mensagem
      followers = profiles->getFollowers(packet->profile); //pega seguidores do perfil que enviou a mensagem para enviar para eles na lista de notificacoes
      notifications->newMessage(move(packet), followers);//prepara noticacao para enviar para seguidores
      break;

    case LOGOUT:
      instances->closeInstance(packet->profile, cliaddr);
      break;

    default:
      perror("unknown packet type");
      break;
    }
  }

  return 0;
}


//Thread so envia notificacoes
//TODO: Refatorar funcao para enviar notificacoes
void threadSession(UDP* udp, Instances* instances, Notifications* notifications){
  while(true){ //loop infinito
    if(!notifications->isEmpty()){ // se lista estiver vazia, nao faz nada
      // Cria lista de notificacoes pendentes local
      auto pending = notifications->getPendingNotifs();

      // Percorre lista de notificacoes pendentes
      for (multimap<string, unsigned int>::iterator notif_it = pending.begin(); notif_it != pending.end(); ++notif_it) { 
        /* Explicando esse loop:
          Um iterador chamado notif_it é inicializado no início do multimap (pending.begin()) e o loop continua
          até que este iterador alcance o final do multimap (pending.end()).
          Dentro do corpo do loop (que não é mostrado no código fornecido), você teria acesso ao par chave-valor atual através do iterador notif_it.
          Você pode acessar a chave com notif_it->first e o valor com notif_it->second.
        */

        // cout << "send notif.id " << notif_it->second << " to "; // second é o id da notificacao
        // cout << notif_it->first << endl; // first é o perfil que vai receber a notificacao

        //PROXIMOS PASSOS:
        //TODO: se instancia não existir, guarda notificação e não envia
        auto notifToSend = notifications->getNotifByID(notif_it->second);

        // build packet
        auto packet = make_unique<Packet>();
        strncpy(packet->payload, notifToSend.message, MAXLEN);
        packet->type = SEND;

        // address to send notification
        sockaddr_in cliaddr = instances->getPort(notif_it->first);

        // se instancia possui um endereco valido (diferente de zero)
        // envia a notificação e remove da lista de pendentes
        if(cliaddr.sin_addr.s_addr != 0){
          udp->envia(move(packet), &cliaddr, notifToSend.sender);
          notifications->deleteNotif(notif_it);
          cout << "send notif.id " << notif_it->second << " to ";
          cout << notif_it->first << endl;
        }
      }
    }

    // Executa a thread de 1 em 1 segundo para não pinar a CPU em 100%
    sleep(1);
  }
}

// Thread que envia pings periodicamente para verificar se os clientes ainda estão conectados
void threadKeepAlive(UDP* udp, Instances* inst){
  // send ping packet to every instance
  while(true){
    // get all instances
    auto instances = inst->getInstances();

    // ping every instance
    for(auto i : instances){
      // cout << "Pinging " << i.first << " at ";
      // cout << inet_ntoa(i.second.sin_addr) << ':' << i.second.sin_port << endl;
      udp->ping(i.second);
    }

    // wait for 400 ms to receive pong from all instances
    this_thread::sleep_for(chrono::milliseconds(400));

    inst->checkAlive();

    // wait for 2 seconds to perform next Keep Alive check
    this_thread::sleep_for(chrono::seconds(2));
  }
}
