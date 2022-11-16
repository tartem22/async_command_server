#ifndef BULK_SERVER_H
#define BULK_SERVER_H

#include "../async.h"

#include <boost/asio.hpp>

namespace bulk
{
    class Server
    {
    public:
        Server(boost::asio::io_context &io_context, short port, int bulkSize);
        ~Server();
    private:
        void do_accept();
        boost::asio::ip::tcp::acceptor acceptor_;
        async::handle_t commandHandler = nullptr;
        int bulkSize = 0;
    };
} // namespace  bulk

#endif // BULK_SERVER_H