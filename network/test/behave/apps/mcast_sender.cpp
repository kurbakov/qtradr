#include <iostream>

#include <network/udp/mcast_send.hpp>

int main(int argc, char* argv[])
{
    if(argc != 4){
        std::cerr << "Wrong argument count!" << std::endl;
        std::cerr << "Please re-run the application with the following arguments:" << std::endl;
        std::cerr << "\t\t" << "./<app> <PORT> <MCAST ADDR> <MESSAGE>" << std::endl;
    }
    int port = std::atoi(argv[1]);

    network::McastSend sender{};
    sender.init(port, argv[2]);

    std::string msg(argv[3]);
    std::cout << "MCAST SENDING: " << msg << std::endl;
    sender.send_data(msg.c_str(), msg.length());

    return 0;
}