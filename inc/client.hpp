#pragma once /*auxilia na implementação de include de arquivos com depencencia deste arquivo*/

#include <thread>
#include <string> 

#include "udp.hpp"

PacketType cmdToEnum(std::string cmd);
int envia(UDP &udp, PacketType type, std::string payload);
int logout(UDP &udp);
void threadReply(UDP* udp);
