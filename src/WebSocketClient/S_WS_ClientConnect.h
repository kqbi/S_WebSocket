//
// Created by kqbi on 2020/5/28.
//

#ifndef WEBSOCKETSERVER_S_WS_CLIENTCONNECT_H
#define WEBSOCKETSERVER_S_WS_CLIENTCONNECT_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <queue>

namespace S_WS {
    class S_WS_ClientService;

    class S_WS_ClientConnect : public std::enable_shared_from_this<S_WS_ClientConnect> {
    public:
        S_WS_ClientConnect(boost::asio::io_context &ioc, std::shared_ptr<S_WS_ClientService> service);

        ~S_WS_ClientConnect();

        void connect(std::string &host, std::string &port);

        void asyncConnect(boost::asio::ip::tcp::resolver::results_type results);

        void asyncHandshake();

        void asyncRead();

        void sendMessage(std::string &msg);

        void closeSocket();

        void stop(boost::system::error_code &err, std::string f);

        boost::asio::ip::tcp::resolver _resolver;
        boost::beast::websocket::stream<boost::beast::tcp_stream> _ws;
        boost::beast::flat_buffer _buffer;
        std::shared_ptr<S_WS_ClientService> _service;
        boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    };
}

#endif //WEBSOCKETSERVER_S_WS_CLIENTCONNECT_H