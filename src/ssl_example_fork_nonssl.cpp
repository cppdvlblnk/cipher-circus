#include "ssl_common.h"




int main(int argc, char**argv)
{

    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto hostname = std::string{argv[1]};
    auto certfilepath = argv[3];
    auto certpkeypath = argv[4];

    if ( !fork() )
    {
        //Client
        print_string("I'm the kid -- client!!!");
        init_open_ssl();
        auto [pS,pC] = init_ssl_connection_role(ROLE_TYPE::CLIENT);
        auto ssl_suite_str = SSL_get_cipher(SSL_new(pC));

        std::stringstream ss;
        ss << "SSL Suite:\n" << ssl_suite_str << std::endl;
        print_string(ss);
        

        std::string loremipsum{"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum sed consectetur neque, a tempor sem."};
        

        ss.str(loremipsum); 
        print_string(ss);       
        

        ss << "Attempting connection to server localhost on PORT: " << port << std::endl;
        print_string(ss);
        auto server = connect_to_server(hostname, port);


        write(server, loremipsum.data(), loremipsum.size());
        close(server);

    }
    else 
    {
        //Server
        print_string("I'm the parent -- server!!!!");
        init_open_ssl();
        auto [pS,pC] = init_ssl_connection_role(ROLE_TYPE::SERVER);
        auto connectionIsSecure = load_certificate_and_priv_key_files(pC, argv[1], argv[2]);
        
        //Listen till a client is found
        auto listener = server_listens(hostname, port);
        auto client = server_accepts(listener);
        
        //Send Information.
        auto const nbytestop = 2 << 6;
        auto buffer = std::vector<char>(2 << 6, '\0');
        auto pbuffer = buffer.data();
        
        auto byteslefttoread = nbytestop;
        
        std::stringstream ss;
        ss << "Left To Read: " << byteslefttoread << std::endl;
        print_string(ss);
        auto zero_counter = 0;
        do
        {

            auto nbytes = read(client, pbuffer, byteslefttoread);
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

        }
        while (byteslefttoread > 0);
        
        ss << buffer.data(); 
        print_string(ss);

        close(client);
        close(listener);

    }

    return 0;

}

