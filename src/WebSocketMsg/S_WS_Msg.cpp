/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Msg
//!	Generated Date	: Thu, 20, Jun 2019  
	File Path	: ../../src/WebSocketService/S_WS_Msg.cpp
*********************************************************************/

#include "S_WS_Msg.h"
//## package WebSocketService
namespace S_WS {
//## class S_WS_Msg
    S_WS_Msg::S_WS_Msg(std::string &connectionId, std::string &msg) : _connectionId(connectionId), _msg(msg),
                                                                      _remoteIp(""),
                                                                      _remotePort(0) {
        //#[ operation S_WS_Msg(std::string&,std::string&)
        //#]
    }

    S_WS_Msg::~S_WS_Msg() {
    }
}
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Msg.cpp
*********************************************************************/
