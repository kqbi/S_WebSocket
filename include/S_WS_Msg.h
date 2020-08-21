/*********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Msg
//!	Generated Date	: Fri, 28, Jun 2019  
	File Path	: ../../src/WebSocketService/S_WS_Msg.h
*********************************************************************/

#ifndef WebSocketService_S_WS_Msg_H
#define WebSocketService_S_WS_Msg_H

#include <string>
//## package WebSocketService
namespace S_WS {
//## class S_WS_Msg
    class S_WS_Msg {
        ////    Constructors and destructors    ////

    public :

        //## operation S_WS_Msg(std::string&,std::string&)
        S_WS_Msg(std::string &connectionId, std::string &msg);

        ~S_WS_Msg();

        ////    Attributes    ////

        std::string _connectionId;        //## attribute _connectionId

        std::string _msg;        //## attribute _msg

        std::string _remoteIp;

        uint16_t _remotePort;
    };
}
#endif
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Msg.h
*********************************************************************/
