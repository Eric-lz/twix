#pragma once
#include <string>

#include "udp.hpp"
#include "data.hpp"

PacketType cmdToEnum(std::string cmd);
int envia(UDP& udp, PacketType type, std::string payload);