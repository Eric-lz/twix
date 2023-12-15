#pragma once
#include <map>

// Busca o client pela porta e retorna seu indice
// Retorna -1 se nao encontra
int findPort(std::map<in_port_t, int>& clients, in_port_t port);