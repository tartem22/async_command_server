#include "bulk_session.h"

using boost::asio::ip::tcp;
using namespace bulk;

static std::vector<std::string> split(const char *c_str, std::size_t size)
{
    std::string str;
    std::vector<std::string> r;
    r.reserve(1000);

    for (std::size_t i = 0; i < size; i++)
    {
        if (*c_str == '\n')
        {
            if (str.length() != 0)
            {
                str += *c_str;
                r.push_back(str);
                str.clear();
            }
        }
        else
            str += *c_str;
        c_str++;
    }
    r.shrink_to_fit();
    return r;
}

Session::Session(boost::asio::ip::tcp::socket socket,
                 async::handle_t handler) : socket_(std::move(socket)),
                                            staticHandler(handler)
{
}

Session::~Session()
{
    if(dynamicHandler)
        async::disconnect(dynamicHandler);
}

void Session::start()
{
    do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    do_process(length);
                                }
                            });
}

void Session::do_process(std::size_t length)
{
    std::vector<std::string> cmds = split(data_, length);
    for (const auto &cmd : cmds)
    {
        if (cmd == "{\n")
            isDynamicBlock = true;
        bool needToEndDynBlock = false;
        if (cmd == "}\n")
            needToEndDynBlock = true;
        if (isDynamicBlock)
        {
            if(dynamicHandler == nullptr)
                dynamicHandler = async::connect(1);
            async::receive(dynamicHandler, cmd.c_str(), cmd.length());
        }
        else
        {
            async::receive(staticHandler, cmd.c_str(), cmd.length());
        }
        if (needToEndDynBlock)
            isDynamicBlock = false;
    }
    do_read();
}