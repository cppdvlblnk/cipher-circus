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


#define PORT 9876
#define SHIFT 6




int ssl_connect(SSL*pssl, int server_socket)
{
    int ret;
    if(!SSL_set_fd(pssl, server_socket))
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    else if ((ret = SSL_connect(pssl)) < 1)
    {
        SSL_get_error(pssl, ret);
        ERR_print_errors_fp(stderr);
        return ret;
    }
    return server_socket;
}




int connect_to_server(std::string hostname, unsigned short port)
{
    struct sockaddr_in addr;
    struct hostent* host = gethostbyname(hostname.c_str());
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    std::memset(&addr, 0x0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = (reinterpret_cast<long*>(host->h_addr))[0];
    if (connect(sd, (const sockaddr*)&addr, sizeof(addr)) == -1)
    {
        close(sd);
    }
    return sd;
    
}
