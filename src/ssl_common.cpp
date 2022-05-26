#include "ssl_common.h"
void print_string(std::string str) 
{
    std::cout << getpid() << " " << str << std::endl; 
}

void print_string(std::stringstream& ss)
{
    std::cout << getpid() << " " << ss.str() << std::endl;
    ss.str(std::string{});
}

void init_open_ssl()
{
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


bool load_certificate_and_priv_key_files(SSL_CTX* pCtxt, char* pCertFile, char* pPrKeyFile)
{
    if (!pCtxt || !pCertFile || !pPrKeyFile)
    {
        return false;
    }
    SSL_CTX_use_certificate_file(pCtxt, pCertFile, SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(pCtxt, pPrKeyFile, SSL_FILETYPE_PEM);
    if (!SSL_CTX_check_private_key(pCtxt))
    {
        std::cout << "Key & Cert don't match" << std::endl;
        return false;
    }
    return true;
}

std::pair<SSL*, SSL_CTX*> init_ssl_connection_role(ROLE_TYPE role)
{
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    auto mthd = role == ROLE_TYPE::CLIENT ? SSLv23_client_method() : SSLv23_server_method();
    SSL_CTX* pCtxt = SSL_CTX_new(mthd);
    return std::make_pair(SSL_new(pCtxt), pCtxt);
}

