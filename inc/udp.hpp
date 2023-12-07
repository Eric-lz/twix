#pragma once
#include <memory>

#include "data.hpp"

#define PORT 4000

int envia(int sockfd, std::unique_ptr<Packet> packet);
std::unique_ptr<Packet> recebe(int sockfd);