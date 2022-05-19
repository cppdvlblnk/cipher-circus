#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <utility>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>


SSL_METHOD* pMethod;
SSL_CTX* pCtx;


std::vector<std::string> ssl_error_strings{};




void init_open_ssl()
{
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}


std::pair<const SSL_METHOD*, SSL_CTX*> init_ssl_mthd_ctxt()
{
    const SSL_METHOD* mthd = SSLv23_client_method();
    SSL_CTX* ctxt = SSL_CTX_new(mthd);
    return std::make_pair(mthd, ctxt);
}


SSL* ssl_protocol_up(SSL_CTX* ctxt)
{
    auto ssl = SSL_new(ctxt);
    return ssl;
}

void ssl_protocol_teardown(SSL*pssl, int socket)
{

    SSL_free(pssl);
    close(socket);
}

int bind_ssl_socket_and_connect(SSL*pssl, int socket)
{
    int ret;
    if(!SSL_set_fd(pssl, socket))
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
    return ret;
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
    if (connect(sd, (const sockaddr*)&addr, sizeof(addr)) == -1) close(sd);
    return sd;
    
}

int main(int argc, char**argv)
{

    init_open_ssl();
    auto [m,c] = init_ssl_mthd_ctxt();
    
    auto ssl_suite_str = SSL_get_cipher(SSL_new(c));
    std::cout << "SSL Suite:\n" << ssl_suite_str << std::endl;


    return 0;
}

