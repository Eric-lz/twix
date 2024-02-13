#pragma once

#include <thread>
#include <string> 

#include "udp.hpp"

void threadReply(UDP* udp);
PacketType cmdToEnum(std::string cmd);
void printNotif(std::unique_ptr<Packet> notif);
int envia(UDP &udp, PacketType type, std::string payload);
int logout(UDP &udp);
