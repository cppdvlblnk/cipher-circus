#include "ssl_common.h"





int main (int argc, char ** argv)
{

    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto hostname = std::string{argv[1]};
    auto certfilepath = argv[3];
    auto certpkeypath = argv[4];

    if ( !fork())
    {
        std::stringstream ss;
        print_string("I'm the kid -- client!!!");
        
        auto [pS, pC] = init_ssl_connection_role(ROLE_TYPE::CLIENT);

        auto socket_to_server = connect_to_server(hostname, port);
        
        socket_to_server = ssl_connect(pS, socket_to_server);

        
        std::string loremipsum{"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum sed consectetur neque, a tempor sem."};
        ss.str(loremipsum);
        print_string(ss);

        SSL_write(pS, loremipsum.data(), loremipsum.size());
        ssl_protocol_teardown(pS, socket_to_server);
    }
    else
    {
        print_string("I'm the parent -- server!!!");

        auto [pS, pC] = init_ssl_connection_role(ROLE_TYPE::SERVER);

        auto server_socket = server_listens(hostname, port);

        auto peer_socket = ssl_accept(pS, server_socket);

        auto connectionIsSecure = load_certificate_and_priv_key_files(pC, certfilepath, certpkeypath);

        auto buffer = std::vector<char>(2 << 6, '\0');
        auto pbuffer = buffer.data();
        auto byteslefttoread = 2 << 6;

        std::stringstream ss;
        auto zero_counter = 0;
        do{
            auto nbytes = SSL_read(pS, pbuffer, byteslefttoread);
            if (nbytes == 0)
            {
                ++zero_counter;
                if (zero_counter == 3) break;                
            }
            else if (nbytes == -1)
                break;
            
            byteslefttoread -= nbytes;
            
            ss << "Left To Read: " << byteslefttoread << ", Bytes Read: " << nbytes << std::endl;
            print_string(ss);

            pbuffer += nbytes;

        }while ( byteslefttoread > 0);

        ss << buffer.data();
        print_string(ss);

        ssl_protocol_teardown(pS, peer_socket);
        close(server_socket);
    }

    
}
