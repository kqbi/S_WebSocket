/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_MainSession
//!	Generated Date	: Fri, 21, Jun 2019  
	File Path	: ../../src/ServiceTest/S_WS_MainSession.cpp
*********************************************************************/

#include "S_WS_Server.h"
#include "S_WS_Msg.h"
#include "OXFWorkPoller.h"
#include <iostream>
//## package ServiceTest

//## class S_WS_MainSession

void readFromServer(void *pUser, S_WS::S_WS_Msg *msg) {
    S_WS_Server* server = (S_WS_Server*)pUser;
    std::cout<<msg->_msg << std::endl;
}

void disConnectNotify(void *pUser, std::string &connectionId) {
    printf("%s disConnect!!!\n", connectionId.c_str());
}

S_WS_Server::S_WS_Server() : _wsServer(0) {
    //#[ operation S_WS_MainSession()
    _wsServer = S_WSServer_Create(OXFWorkPollerPool::Instance()._ioc);
    //#]
}

S_WS_Server::~S_WS_Server() {
    //#[ operation ~S_WS_MainSession()
    S_WSServer_Release(_wsServer);
    //#]
}

bool S_WS_Server::listen(std::string ip, unsigned short port) {
    //#[ operation listen(unsigned short)
    bool ret = false;
    if (ret = S_WSServer_Listen(_wsServer, ip, port)) {
        S_WSServer_ReadFromServer(_wsServer, this, readFromServer);
        S_WSServer_DisConnectNotify(_wsServer, this, disConnectNotify);
    }
    return ret;
    //#]
}

/*********************************************************************
	File Path	: ../../src/ServiceTest/S_WS_MainSession.cpp
*********************************************************************/
