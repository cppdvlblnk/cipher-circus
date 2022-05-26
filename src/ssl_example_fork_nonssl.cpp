#include "ssl_common.h"




int main(int argc, char**argv)
{

    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto hostname = std::string{argv[1]};

    if ( !fork() )
    {
        //Client
        print_string("I'm the kid -- client!!!");
        

        std::string loremipsum{"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum sed consectetur neque, a tempor sem."};
        
		std::stringstream ss;
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

