#include "bulk_server.h"
#include "bulk_session.h"
#include <memory>

using boost::asio::ip::tcp;
using namespace bulk;

Server::Server(boost::asio::io_context &io_context, short port, int bulkSize) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
                                                                                bulkSize(bulkSize)
{
    do_accept();
    commandHandler = async::connect(bulkSize);
}

Server::~Server()
{
    if (commandHandler)
        async::disconnect(commandHandler);
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket), commandHandler)->start();
            }

            do_accept();
        });
}