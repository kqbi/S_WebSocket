/*********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_MainSession
//!	Generated Date	: Fri, 21, Jun 2019  
	File Path	: ../../src/ServiceTest/S_WS_MainSession.h
*********************************************************************/

#ifndef ServiceTest_S_WS_MainSession_H
#define ServiceTest_S_WS_MainSession_H

#include "S_WS_ClientAPI.h"
class S_WS_Msg;

class S_WS_Protocol;

//## package ServiceTest

//## class S_WS_MainSession
class S_WS_Client {
    ////    Constructors and destructors    ////
    
public :

    //## operation S_WS_MainSession()
    S_WS_Client(boost::asio::io_context &ioc);
    
    //## operation ~S_WS_MainSession()
    ~S_WS_Client();
    
    ////    Operations    ////

    void init();

    //## operation listen(unsigned short)
    void connect(std::string uri, CONNECTED connected, void* pUser, std::map<std::string, std::string> extraHeaders = {});

    void reConnect();
    ////    Attributes    ////
    
    ws_client _wsClient;
};

#endif
/*********************************************************************
	File Path	: ../../src/ServiceTest/S_WS_MainSession.h
*********************************************************************/
