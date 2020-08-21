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

#include <oxf.h>
#include "S_WS_ServerAPI.h"
class S_WS_Msg;

class S_WS_Protocol;

//## package ServiceTest

//## class S_WS_MainSession
class S_WS_Server {
    ////    Constructors and destructors    ////
    
public :

    //## operation S_WS_MainSession()
    S_WS_Server();
    
    //## operation ~S_WS_MainSession()
    ~S_WS_Server();
    
    ////    Operations    ////

    //## operation listen(unsigned short)
    bool listen(std::string ip, unsigned short port);
    
    ////    Attributes    ////
    
    ws_server _wsServer;
};

#endif
/*********************************************************************
	File Path	: ../../src/ServiceTest/S_WS_MainSession.h
*********************************************************************/
