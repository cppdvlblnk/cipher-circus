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

int server_listens(std::string hostname, unsigned short port)
{
    struct sockaddr_in addr; 
    int sd;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    std::memset(&addr, 0x0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sd, (const sockaddr*)&addr, sizeof(addr));
    listen(sd, 10);

    return sd;
}

int server_accepts(int server_socket)
{
    struct sockaddr addr;
    socklen_t addrlen;
    int client = accept(server_socket, &addr, &addrlen);
    return client;
}

int ssl_accept(SSL*pssl, int server_socket)
{
    int peer_socket = server_accepts(server_socket);
    int ret;
    if (!SSL_set_fd(pssl, peer_socket))
    {
        ERR_print_errors_fp(stderr);
        return -1;        
    }
    else if ((ret = SSL_accept(pssl)) < 1)
    {
        SSL_get_error(pssl, ret);
        ERR_print_errors_fp(stderr);
        return ret;
    }
    return peer_socket;    
}

