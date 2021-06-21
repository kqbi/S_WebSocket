//
// Created by kqbi on 2020/5/28.
//

#include "S_WS_ClientService.h"
#include "S_WS_ClientConnect.h"
#include "Utils.h"
#include <Logger.h>
#include <iostream>

namespace S_WS {
    S_WS_ClientService::S_WS_ClientService(boost::asio::io_context &ioc) : _ioc(ioc), _ipAddress(""), _port(""),
                                                                           _connect(0), _target(""), _readFromServer(0),
                                                                           _connected(0), _pUser(0), _pUserRead(0) {
        _extraHeaders.clear();
    }

    S_WS_ClientService::~S_WS_ClientService() {

    }

    void
    S_WS_ClientService::connect(std::string &uri, CONNECTED connected,
                                std::map<std::string, std::string> &extraHeaders, void *pUser) {
        _pUser = pUser;
        _connected = connected;
        _connect = std::make_shared<S_WS_ClientConnect>(_ioc, shared_from_this());
        std::string schema = Utils::FindField(uri.data(), nullptr, "://");
        if (schema != "ws") {
            S_LOG_DEBUG("schema != ws");
            _connected(_pUser, false);
            return;
        }
        std::string remote = Utils::FindField(uri.data(), "://", "/");
        _ipAddress = Utils::FindField(remote.data(), nullptr, ":");
        _port = Utils::FindField(remote.data(), ":", nullptr);
        _target = Utils::FindField(uri.data() + schema.size() + strlen("://") + remote.size(), "/", nullptr);
        _extraHeaders.swap(extraHeaders);
        _connect->connect(_ipAddress, _port);
    }

    void S_WS_ClientService::reConnect() {
        _connect->connect(_ipAddress, _port);
    }

    void S_WS_ClientService::handleStop() {
        _connect->closeSocket();
    }

    void S_WS_ClientService::readFromServer(READFROMSERVER readFromServer, void *pUser) {
        _pUserRead = pUser;
        _readFromServer = readFromServer;
    }

    void S_WS_ClientService::sendMessage(std::string &msg) {
        _connect->sendMessage(msg);
    }
}