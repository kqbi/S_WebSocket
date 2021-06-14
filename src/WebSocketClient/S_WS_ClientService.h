//
// Created by kqbi on 2020/5/28.
//

#ifndef WEBSOCKETSERVER_S_WS_CLIENTSERVICE_H
#define WEBSOCKETSERVER_S_WS_CLIENTSERVICE_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace S_WS {
    class S_WS_Msg;

    class S_WS_ClientConnect;

    class S_WS_ClientService {
    public :
        typedef void (*READFROMSERVER)(void *pUser, S_WS_Msg *msg);

        typedef void (*CONNECTED)(void *pUser, bool connected);

        ////    Constructors and destructors    ////

        //## operation S_WS_Service(const std::string&,int,const std::string&,std::size_t)
        S_WS_ClientService(boost::asio::io_context &ioc);

        //## operation ~S_WS_Service()
        ~S_WS_ClientService();

        ////    Operations    ////

        //## operation execProcessIDWProtocol(std::string&)
        void execProcessIDWProtocol(std::string &connectionId);

        //## operation getNewConnectionId()
        std::string getNewConnectionId();

        //## operation handleAccept(boost::beast::error_code,tcp::socket)
        void handleAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket s);

        //## operation handleStop()
        void handleStop();

        //## operation listen()
        void
        connect(std::string &uri, void *pUser, CONNECTED connected, std::map<std::string, std::string> &extraHeaders);

        void reConnect();

        //## operation readFromServer(void*,READFROMSERVER)
        void readFromServer(void *pUser, READFROMSERVER readFromServer);

        //## operation sendMessage(std::string)
        void sendMessage(std::string &msg);

        //## operation startAccept()
        void startAccept();

        ////    Attributes    ////

        boost::asio::io_context &_ioc;        //## attribute _ioContext

    public :

        std::string _ipAddress;        //## attribute _ipAddress

        std::string _port;        //## attribute _port

        void *_pUserRead;        //## attribute _pUserRead

        READFROMSERVER _readFromServer;        //## attribute _readFromServer

        CONNECTED _connected;

        void *_pUser;        //## attribute _pUserNotify

        std::shared_ptr<S_WS_ClientConnect> _connect;

        std::string _target;

        std::map<std::string, std::string> _extraHeaders;
    };
}
#endif //WEBSOCKETSERVER_S_WS_CLIENTSERVICE_H
