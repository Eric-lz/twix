#pragma once
// #include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "data.hpp"

#define PORT 4000

int envia(int sockfd, std::unique_ptr<Packet> packet);
std::unique_ptr<Packet> recebe(int sockfd);