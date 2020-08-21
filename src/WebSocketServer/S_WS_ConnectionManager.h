/*********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_ConnectionManager
//!	Generated Date	: Tue, 20, Aug 2019  
	File Path	: ../../src/WebSocketService/S_WS_ConnectionManager.h
*********************************************************************/

#ifndef WebSocketService_S_WS_ConnectionManager_H
#define WebSocketService_S_WS_ConnectionManager_H

#include <set>
#include <mutex>
#include <unordered_map>
#include <memory>

namespace S_WS {
    class S_WS_Connection;

//## package WebSocketService

//## class S_WS_ConnectionManager
    class S_WS_ConnectionManager {
    public :

        typedef std::shared_ptr<S_WS_Connection> connection_ptr;
        ////    Constructors and destructors    ////

        S_WS_ConnectionManager();

        ~S_WS_ConnectionManager();

        ////    Operations    ////

        //## operation getConnectionById(std::string&,connection_ptr)
        void getConnectionById(std::string &connectionId, connection_ptr &connection);

        //## operation join(connection_ptr)
        void join(connection_ptr connection);

        //## operation leave(connection_ptr)
        void leave(connection_ptr connection);

        //## operation sendMessage(std::string&)
        void sendMessage(std::string &msg);

        //## operation stop(std::string&)
        void stop(connection_ptr connection);

        void stop(std::string &connectionId);

        //## operation stopAll()
        void stopAll();

        ////    Attributes    ////

        std::unordered_map<std::string, connection_ptr> _connections;        //## attribute _connections

        std::mutex _mutex;        //## attribute _mutex
    };
}
#endif
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_ConnectionManager.h
*********************************************************************/
