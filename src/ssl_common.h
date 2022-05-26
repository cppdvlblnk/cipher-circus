#pragma once 

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iostream>
#include <utility>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>


enum class ROLE_TYPE{

    SERVER,
    CLIENT
    
};

//COMMON Utilities
bool check_parms(int,char**);
void print_string(std::stringstream&);
void print_string(std::string);


//COMMON SSL 
void init_open_ssl();
SSL* ssl_protocol_up(SSL_CTX* pCtxt);
void ssl_protocol_teardown(SSL* pssl, int socket);


//COMMON CERTIFICATE OPERATIONS.
bool load_certificate_and_priv_key_files(SSL_CTX*, char* pCertFile, char* pPrKeyFile);


//SSL CONNECTION START.
std::pair<SSL*, SSL_CTX*> init_ssl_connection_role(ROLE_TYPE role);


//SSL CLIENT.
int ssl_connect(SSL*, int server_socket);
int connect_to_server(std::string hostname, unsigned short port);

//SSL SERVER.
int server_listens(std::string hostname, unsigned short port);
int server_accepts(int server_socket);
int ssl_accept(SSL*, int server_socket);

//sa

