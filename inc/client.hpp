#pragma once /*auxilia na implementação de include de arquivos com depencencia deste arquivo*/
#include <string> 

#include "udp.hpp"

PacketType cmdToEnum(std::string cmd);
int envia(UDP& udp, PacketType type, std::string payload);
/*Envia para o servidor um protocolo UDP com possibilidade
de ser um pacote MSG ou DADO*/





 