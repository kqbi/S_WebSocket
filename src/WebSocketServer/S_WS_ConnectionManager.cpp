/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_ConnectionManager
//!	Generated Date	: Tue, 20, Aug 2019  
	File Path	: ../../src/WebSocketService/S_WS_ConnectionManager.cpp
*********************************************************************/

#include "S_WS_ConnectionManager.h"
#include "S_WS_Connection.h"
//## package WebSocketService
namespace S_WS {
//## class S_WS_ConnectionManager
    S_WS_ConnectionManager::S_WS_ConnectionManager() {
    }

    S_WS_ConnectionManager::~S_WS_ConnectionManager() {
    }

    void S_WS_ConnectionManager::getConnectionById(std::string &connectionId,
                                                   S_WS_ConnectionManager::connection_ptr &connection) {
        //#[ operation getConnectionById(std::string&,connection_ptr)
        std::lock_guard<std::mutex> lock(_mutex);
        connection = _connections[connectionId];
        //#]
    }

    void S_WS_ConnectionManager::join(connection_ptr connection) {
        //#[ operation join(connection_ptr)
        std::lock_guard<std::mutex> lock(_mutex);
        _connections[connection->_connectionId] = connection;
        //#]
    }

    void S_WS_ConnectionManager::leave(connection_ptr connection) {
        //#[ operation leave(connection_ptr)
        std::lock_guard<std::mutex> lock(_mutex);
        _connections.erase(connection->_connectionId);
        //#]
    }

    void S_WS_ConnectionManager::sendMessage(std::string &msg) {
        //#[ operation sendMessage(std::string&)
        std::lock_guard<std::mutex> lock(_mutex);
        std::unordered_map<std::string, connection_ptr>::iterator it = _connections.begin();
        for (; it != _connections.end(); ++it) {
            if (it->second)
                it->second->send(msg);
        }
        //#]
    }

    void S_WS_ConnectionManager::stop(connection_ptr connection) {
        //#[ operation stop(std::string&)
        std::lock_guard<std::mutex> lock(_mutex);
        //S_LOG_DEBUG(connection->_connectionId  << " use count = " << connection.use_count() << ".");
        _connections.erase(connection->_connectionId);
        connection->closeSocket();
        //#]
    }

    void S_WS_ConnectionManager::stop(std::string &connectionId) {
        //#[ operation stop(std::string&)
        std::lock_guard<std::mutex> lock(_mutex);
        //S_LOG_DEBUG(connection->_connectionId  << " use count = " << connection.use_count() << ".");
        if (_connections[connectionId]) {
            _connections[connectionId]->closeSocket();
            _connections.erase(connectionId);
        }
        //#]
    }

    void S_WS_ConnectionManager::stopAll() {
        //#[ operation stopAll()
        std::lock_guard<std::mutex> lock(_mutex);
        for (auto iter : _connections) {
            if (iter.second)
                iter.second->closeSocket();
        }
        _connections.clear();
        //#]
    }
}
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_ConnectionManager.cpp
*********************************************************************/
