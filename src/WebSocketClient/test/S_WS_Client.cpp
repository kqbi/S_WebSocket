/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_MainSession
//!	Generated Date	: Fri, 21, Jun 2019  
	File Path	: ../../src/ServiceTest/S_WS_MainSession.cpp
*********************************************************************/

#include "S_WS_Client.h"
#include "S_WS_Msg.h"
#include <iostream>
//## package ServiceTest

void readFromServer(void *pUser, S_WS::S_WS_Msg *msg) {
    S_WS_Client* server = (S_WS_Client*)pUser;
    std::cout<<msg->_msg << std::endl;
}

S_WS_Client::S_WS_Client(boost::asio::io_context &ioc) : _wsClient(0) {
    //#[ operation S_WS_MainSession()
    _wsClient = S_WSClient_Create(ioc);
    //#]
}

S_WS_Client::~S_WS_Client() {
    //#[ operation ~S_WS_MainSession()
    S_WSClient_Release(_wsClient);
    //#]
}

void S_WS_Client::init() {
    S_WSClient_ReadFromServer(_wsClient,this, readFromServer);
}

void S_WS_Client::connect(std::string uri, void* pUser, CONNECTED connected, std::map<std::string, std::string> extraHeaders) {
    //#[ operation listen(unsigned short)
    S_WSClient_Connect(_wsClient, uri, pUser, connected,extraHeaders);
    //#]
}

void S_WS_Client::reConnect() {
    //#[ operation listen(unsigned short)
    S_WSClient_ReConnect(_wsClient);
    //#]
}

/*********************************************************************
	File Path	: ../../src/ServiceTest/S_WS_MainSession.cpp
*********************************************************************/
