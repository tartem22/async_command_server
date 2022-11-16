#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

#include "server/bulk_server.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            // std::cerr << "Usage: async_tcp_echo_server <port>\n";
            // return 1;
            argv[1] = "9000";
            argv[2] = "5";
        }

        boost::asio::io_context io_context;

        bulk::Server server(io_context, std::atoi(argv[1]), std::atoi(argv[2]));

        io_context.run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
