/*********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Service
//!	Generated Date	: Tue, 20, Aug 2019  
	File Path	: ../../src/WebSocketService/S_WS_Service.h
*********************************************************************/

#ifndef WebSocketService_S_WS_Service_H
#define WebSocketService_S_WS_Service_H

#include "S_WS_ConnectionManager.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
namespace S_WS {
    class S_WS_Connection;

    class S_WS_Msg;

//## package WebSocketService

//## class S_WS_Service
    class S_WS_ServerService {
    public :
        typedef std::shared_ptr<S_WS_Connection> connection_ptr;

        typedef void (*READFROMSERVER)(void *pUser, S_WS_Msg *msg);

        typedef void (*DISCONNECTNOTIFY)(void *pUser, std::string &connectionId);
        ////    Constructors and destructors    ////

        //## operation S_WS_Service(const std::string&,int,const std::string&,std::size_t)
        S_WS_ServerService(boost::asio::io_context &ioc);

        //## operation ~S_WS_Service()
        ~S_WS_ServerService();

        ////    Operations    ////

        //## operation closeConnection(std::string&)
        void closeConnection(std::string &connectionId);

        //## operation disConnectNotify(void*,DISCONNECTNOTIFY)
        void disConnectNotify(void *pUser, DISCONNECTNOTIFY disConnectNotify);

        //## operation execProcessIDWProtocol(std::string&)
        void execProcessIDWProtocol(std::string &connectionId);

        //## operation handleAccept(boost::beast::error_code,tcp::socket)
        void handleAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket s);

        //## operation handleStop()
        void handleStop();

        //## operation listen()
        bool listen(std::string &ipAddress, unsigned short port);

        //## operation readFromServer(void*,READFROMSERVER)
        void readFromServer(void *pUser, READFROMSERVER readFromServer);

        //## operation sendMessage(std::string)
        void sendMessage(std::string &msg);

        //## operation sendMessage(std::string&,std::string&)
        void sendMessage(std::string &connectionId, std::string &msg);

        //## operation startAccept()
        void startAccept();

        ////    Attributes    ////

        boost::asio::io_context &_ioc;        //## attribute _ioContext

    public :

        std::string _ipAddress;        //## attribute _ipAddress

        boost::asio::ip::tcp::acceptor _acceptor;        //## attribute _acceptor

        int _port;        //## attribute _port

        S_WS_ConnectionManager _connectionManager;        //## attribute _connectionManager

        void *_pUserRead;        //## attribute _pUserRead

        READFROMSERVER _readFromServer;        //## attribute _readFromServer

        DISCONNECTNOTIFY _disConnectNotify;        //## attribute _disConnectNotify

        void *_pUserNotify;        //## attribute _pUserNotify
    };
}
#endif
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Service.h
*********************************************************************/
