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
- **Diretório src/** : arquivos fonte (*.cpp);
- **Diretório inc/** : arquivos cabeçalho (*.hpp);
- **Diretório build/** : arquivos objeto (*.o);

Os binários do servidor e do cliente são compilados na pasta raíz do projeto.

# Estrutura Makefile
- **INC_DIR**: diretório com arquivos cabeçalho (*.hpp);
- **CXX**: compilador C++ utilizado;
- **CXXFLAGS**: opções do compilador para debug;
- **DPLYFLAGS**: opções do compilador para deploy;
- **SERVER_O**: arquivos objeto necessários para o servidor;
- **CLIENT_O**: arquivos objeto necessários para o cliente.

## Comandos `make`:
- `all`: compila servidor e cliente (sem otimizações);
- `server` e `client`: para compilar separadamente o servidor ou o cliente;
- `test`: re-compila tudo e inicia o servidor;
- `clean`: limpa arquivos binários e objetos;
- `deploy`: compila servidor e cliente com otimizações e sem opções de debug.