#include <iostream>

#include <network/udp/mcast_recv.hpp>

int main(int argc, char* argv[])
{
    if(argc != 3){
        std::cerr << "Wrong argument count!" << std::endl;
        std::cerr << "Please re-run the application with the following arguments:" << std::endl;
        std::cerr << "\t\t" << "./<app> <PORT> <MCAST ADDR>" << std::endl;
    }
    int port = std::atoi(argv[1]);

    network::McastRecv recv{};
    recv.init(port, argv[2]);

    constexpr int buff_len = 64;
    char buffer[buff_len];
    recv.recv_data(buffer, buff_len);

    std::cout << buffer << std::endl;

    return 0;
}