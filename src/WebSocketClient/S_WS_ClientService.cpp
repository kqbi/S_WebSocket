//
// Created by kqbi on 2020/5/28.
//

#include "S_WS_ClientService.h"
#include "S_WS_ClientConnect.h"
#include "Util.h"
#include <iostream>
namespace S_WS {
    S_WS_ClientService::S_WS_ClientService(boost::asio::io_context &ioc) : _ioc(ioc), _ipAddress(""), _port(""),
                                                                           _connect(0), _target(""), _readFromServer(0),
                                                                           _connected(0), _disConnectNotify(0),
                                                                           _pUserNotify(0),
                                                                           _pUserRead(0) {
        _extraHeaders.clear();
        _connect = new S_WS_ClientConnect(ioc, *this);
    }

    S_WS_ClientService::~S_WS_ClientService() {
        if (_connect) {
            delete _connect;
            _connect = 0;
        }
    }

    void
    S_WS_ClientService::connect(std::string &uri, void *pUser, CONNECTED connected,
                                std::map<std::string, std::string> &extraHeaders) {
        _pUserNotify = pUser;
        _connected = connected;
        std::string schema = FindField(uri.data(), nullptr, "://");
        if (schema != "ws") {
            _disConnectNotify(_pUserNotify);
            return;
        }
        std::string remote = FindField(uri.data(), "://", "/");
        _ipAddress = FindField(remote.data(), nullptr, ":");
        _port = FindField(remote.data(), ":", nullptr);
        _target = FindField(uri.data() + schema.size() + strlen("://") + remote.size(), "/", nullptr);
        _extraHeaders.swap(extraHeaders);
        _connect->connect(_ipAddress, _port);
    }

    void S_WS_ClientService::handleStop() {
        _connect->closeSocket();
    }

    void S_WS_ClientService::disConnectNotify(void *pUser, DISCONNECTNOTIFY disConnectNotify) {
        _pUserNotify = pUser;
        _disConnectNotify = disConnectNotify;
    }

    void S_WS_ClientService::readFromServer(void *pUser, READFROMSERVER readFromServer) {
        _pUserRead = pUser;
        _readFromServer = readFromServer;
    }

    void S_WS_ClientService::sendMessage(std::string &msg) {
        _connect->sendMessage(msg);
    }
}