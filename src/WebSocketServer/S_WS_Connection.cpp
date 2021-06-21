/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Connection
//!	Generated Date	: Thu, 20, Jun 2019  
	File Path	: ../../src/WebSocketService/S_WS_Connection.cpp
*********************************************************************/

#include "S_WS_Connection.h"
#include "S_WS_ConnectionManager.h"
#include "S_WS_ServerService.h"
#include <boost/asio/spawn.hpp>
#include "Logger.h"
#include "S_WS_Msg.h"
//## package WebSocketService
namespace S_WS {
//## class S_WS_Connection
    S_WS_Connection::S_WS_Connection(boost::asio::io_context &ioc, std::string &connectionId,
                                     std::shared_ptr<S_WS_ServerService> service,
                                     S_WS_ConnectionManager &connectionManager, boost::asio::ip::tcp::socket &&s)
            : _connectionId(connectionId), _connectionManager(connectionManager), _remoteIpAddress(""),
              _remotePort(0), _service(service), _strand(ioc.get_executor()),
              _ws(std::move(s)) {
        //#[ operation S_WS_Connection(std::string&,S_WS_Service&,S_WS_ConnectionManager&,tcp::socket&&)
        //#]
    }

    S_WS_Connection::~S_WS_Connection() {
        //#[ operation ~S_WS_Connection()
        //S_LOG_DEBUG("~IDW_Connection.id:" << _connectionId);
        closeSocket();
        //#]
    }

    void S_WS_Connection::closeSocket() {
        //#[ operation closeSocket()
        if (_ws.is_open()) {
            boost::system::error_code ec;
            _ws.close(boost::beast::websocket::close_code::normal, ec);
            S_LOG_DEBUG("closeSocket:" << ec.message() << ".");
        }
        //#]
    }

    void S_WS_Connection::doRead() {
        //#[ operation doRead()
        // Read a message into our buffer
        auto self = shared_from_this();
        _ws.async_read(
                _buffer,
                [this, self](boost::system::error_code ec, std::size_t bytesTransferred) {
                    if (ec) {
                        stop(ec, "read");
                    } else {
                        if (bytesTransferred) {
                            if (_service->_pUserRead && _service->_readFromServer) {
                                S_WS_Msg *msg = new S_WS_Msg(_connectionId,
                                                             std::string(
                                                                     boost::beast::buffers_to_string(_buffer.data())));
                                msg->_remoteIp = _remoteIpAddress;
                                msg->_remotePort = _remotePort;
                                S_LOG_DEBUG("read:" << msg->_msg)
                                _service->_readFromServer(msg, _service->_pUserRead);
                            }
                            _buffer.consume(_buffer.size());
                        }
                        doRead();
                    }
                });
        //#]
    }

    std::string S_WS_Connection::getConnectionId() {
        //#[ operation getConnectionId()
        return _connectionId;
        //#]
    }

    void S_WS_Connection::run() {
        //#[ operation run()
        // Set suggested timeout settings for the websocket
        _ws.set_option(
                boost::beast::websocket::stream_base::timeout::suggested(
                        boost::beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        _ws.set_option(boost::beast::websocket::stream_base::decorator(
                [](boost::beast::websocket::response_type &res) {
                    res.set(boost::beast::http::field::server,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-async");
                }));

        // Accept the websocket handshake
        auto self = shared_from_this();
        _ws.async_accept(
                [this, self](boost::system::error_code ec) {
                    if (ec)
                        stop(ec, "accept");
                    else
                        start();
                });

        //S_LOG_DEBUG("start success.");
        //#]
    }

    void S_WS_Connection::send(std::string &ss) {
        //#[ operation send(boost::shared_ptr<std::string const> const&)
        auto self = shared_from_this();
        boost::asio::post(_strand, [this, ss, self]() {
            boost::asio::spawn(_strand,
                               [this, ss, self](boost::asio::yield_context yield) {
                                   if (_ws.is_open()) {
                                       boost::system::error_code err;
                                       _ws.async_write(boost::asio::buffer(ss),
                                                       yield[err]);
                                       if (err) {
                                           stop(err, "sendMessage");
                                       }
                                   }
                               });
        });
        //#]
    }

    void S_WS_Connection::start() {
        //#[ operation start()
        _connectionManager.join(shared_from_this());
        S_LOG_DEBUG("connecttion count = " << _connectionManager._connections.size() << ".");
        doRead();
        //#]
    }

    void S_WS_Connection::stop(boost::system::error_code ec, std::string f) {
        //#[ operation stop()
        S_LOG_DEBUG(f << "error: " << ec.message());
        if (ec != boost::asio::error::operation_aborted) {
            if (_service->_pUserNotify && _service->_disConnectNotify)
                _service->_disConnectNotify(_connectionId, _service->_pUserNotify);
            _connectionManager.stop(shared_from_this());
        } else {
            if (_ws.is_open()) {
                boost::system::error_code ec;
                _ws.close(boost::beast::websocket::close_code::normal, ec);
            }
        }
        //#]
    }

}
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Connection.cpp
*********************************************************************/
