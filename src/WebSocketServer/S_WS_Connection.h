/*********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Connection
//!	Generated Date	: Thu, 20, Jun 2019  
	File Path	: ../../src/WebSocketService/S_WS_Connection.h
*********************************************************************/

#ifndef WebSocketService_S_WS_Connection_H
#define WebSocketService_S_WS_Connection_H

#include <boost/beast.hpp>
#include <queue>
#include <boost/asio/strand.hpp>
namespace S_WS {
    class S_WS_ConnectionManager;

    class S_WS_ServerService;

//## package WebSocketService

//## class S_WS_Connection
    class S_WS_Connection : public std::enable_shared_from_this<S_WS_Connection> {
    public :

        typedef std::shared_ptr<S_WS_Connection> connection_ptr;
        ////    Constructors and destructors    ////

        //## operation S_WS_Connection(std::string&,S_WS_Service&,S_WS_ConnectionManager&,tcp::socket&&)
        S_WS_Connection(boost::asio::io_context &ioc, std::string &connectionId, S_WS_ServerService &service,
                        S_WS_ConnectionManager &connectionManager, boost::asio::ip::tcp::socket &&s);

        //## operation ~S_WS_Connection()
        ~S_WS_Connection();

        ////    Operations    ////

        //## operation asyncWrite(boost::shared_ptr<std::string const> const&)
        void asyncWrite(std::string &msg);

        //## operation closeSocket()
        void closeSocket();

        //## operation doRead()
        void doRead();

        //## operation fail(boost::beast::error_code,char const*)
        void fail(boost::beast::error_code ec, char const *what);

        //## operation getConnectionId()
        std::string getConnectionId();

        //## operation handleAccept(boost::beast::error_code)
        void handleAccept(boost::beast::error_code ec);

        //## operation handleWrite(boost::beast::error_code,std::size_t)
        void handleWrite(boost::beast::error_code ec, std::size_t bytesTransferred);

        //## operation run()
        void run();

        //## operation send(boost::shared_ptr<std::string const> const&)
        void send(std::string &ss);

        //## operation sendReqMsg(std::string)
        void sendReqMsg(std::string msg);

        //## operation sendResMsg(std::string,int)
        void sendResMsg(std::string msg, int httpErrorCode = 200);

        //## operation shutdown()
        void shutdown();

        //## operation start()
        void start();

        //## operation stop()
        void stop();

    private :

        //## operation handleRead(boost::beast::error_code,std::size_t)
        void handleRead(boost::beast::error_code ec, std::size_t bytesTransferred);

        ////    Attributes    ////

    public :

        boost::beast::flat_buffer _buffer;        //## attribute _buffer

        std::string _connectionId;        //## attribute _connectionId

        S_WS_ConnectionManager &_connectionManager;        //## attribute _connectionManager

        boost::optional<boost::beast::http::request_parser<boost::beast::http::string_body> > _parser;        //## attribute _parser

        std::queue<std::string> _queue;        //## attribute _queue

        std::string _remoteIpAddress;        //## attribute _remoteIpAddress

        unsigned short _remotePort;        //## attribute _remotePort

        S_WS_ServerService &_service;        //## attribute _service

        boost::beast::websocket::stream<boost::beast::tcp_stream> _ws;        //## attribute _ws

        boost::asio::io_context::strand _strand;
    };
}
#endif
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Connection.h
*********************************************************************/
