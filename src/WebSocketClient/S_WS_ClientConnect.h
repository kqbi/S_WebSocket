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

    class S_WS_ClientConnect {
    public:
        S_WS_ClientConnect(boost::asio::io_context &ioc, S_WS_ClientService &service);

        ~S_WS_ClientConnect();

        void fail(boost::beast::error_code ec, char const *what);

        void connect(std::string &host, std::string &port);

        void onResolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results);

        void onConnect(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type::endpoint_type);

        void onHandshake(boost::beast::error_code ec);

        void onRead(boost::beast::error_code ec, std::size_t bytesTransferred);

        void onClose(boost::beast::error_code ec);

        void onWrite(boost::beast::error_code ec, std::size_t bytesTransferred);

        //## operation sendMessage(std::string&,std::string&)
        void sendMessage(std::string &msg);

        void asyncWrite(std::string &msg);

        void closeSocket();

        std::queue<std::string> _queue;        //## attribute _queue
        boost::asio::ip::tcp::resolver _resolver;
        boost::beast::websocket::stream<boost::beast::tcp_stream> _ws;
        boost::beast::flat_buffer _buffer;
        S_WS_ClientService &_service;
        boost::asio::io_context::strand _strand;
    };
}

#endif //WEBSOCKETSERVER_S_WS_CLIENTCONNECT_H