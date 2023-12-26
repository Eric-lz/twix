# Twix

Trabalho Prático da disciplina de Sistemas Operacionais II

INF01151

UFRGS - 2023/2

# Descrição

A proposta de trabalho é implementar um serviço de envio de notificações estilo
“feed de notícias”, muito similar ao Twitter.

O projeto compreenderá um app para usuários (cliente) e um servidor.
O cliente permitirá ao usuário fazer uso do serviço de notificações.
Quando o usuário iniciar um processo cliente, o cliente estabelecerá uma conexão
com o servidor e iniciará uma “sessão de uso” atrelada ao “perfil” do usuário.


# Esturtura do projeto
Pastas com include (definição) de cada classe e pasta com source (código fonte).

# Estrutura Makefile
INC_DIR pasta include
CXX compilador
CXXFlAGS = para execução normal
DPLYFLAGS = para execução com deploy

SERVER_O para criar o servidor
CLIENT_O par criar o cliente

COMANDOS:
Make all: complila tudo

make server: servidor

make client: cliente

make clean: limpa os arquivos intermediarios para manter organizado durante desenvolvimentos

make test:re-compila tudo e inicia o servidor de forma padronizada

make deploy: mesmo que make all, porém com otimizações

make deploy_s: servidor otimizado

make deploy_c: cliente otimizado