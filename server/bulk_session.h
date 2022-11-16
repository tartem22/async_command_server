#ifndef BULK_SESSION_H
#define BULK_SESSION_H

#include <memory>
#include <boost/asio.hpp>
#include "../async.h"

namespace bulk
{
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(boost::asio::ip::tcp::socket socket,
                async::handle_t handler);
        ~Session();
        void start();

    private:
        void do_read();
        void do_process(std::size_t length);
        boost::asio::ip::tcp::socket socket_;
        async::handle_t staticHandler = nullptr;
        async::handle_t dynamicHandler = nullptr;
        bool isDynamicBlock = false;
        enum
        {
            max_length = 1024
        };
        char data_[max_length];
    };
} // namespace bulk

#endif // BULK_SESSION_H