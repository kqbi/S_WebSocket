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
#include "S_WS_Msg.h"
//## package WebSocketService
namespace S_WS {
//## class S_WS_Connection
    S_WS_Connection::S_WS_Connection(boost::asio::io_context &ioc, std::string &connectionId,
                                     S_WS_ServerService &service,
                                     S_WS_ConnectionManager &connectionManager, boost::asio::ip::tcp::socket &&s)
            : _connectionId(connectionId), _connectionManager(connectionManager), _remoteIpAddress(""),
              _remotePort(0), _service(service), _strand(ioc), _ws(std::move(s)) {
        //#[ operation S_WS_Connection(std::string&,S_WS_Service&,S_WS_ConnectionManager&,tcp::socket&&)
        //#]
    }

    S_WS_Connection::~S_WS_Connection() {
        //#[ operation ~S_WS_Connection()
        //S_LOG_DEBUG("~IDW_Connection.id:" << _connectionId);
        closeSocket();
        //#]
    }

    void S_WS_Connection::asyncWrite(std::string &msg) {
        //#[ operation asyncWrite(boost::shared_ptr<std::string const> const&)
        // Always add to queue
        _queue.push(msg);

        // Are we already writing?
        if (_queue.size() > 1)
            return;

        // We are not currently writing, so send this immediately
        if (_ws.is_open()) {
            _ws.async_write(
                    boost::asio::buffer(_queue.front()),
                    _strand.wrap(std::bind(
                            &S_WS_Connection::handleWrite,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2)));
        }
        //#]
    }

    void S_WS_Connection::closeSocket() {
        //#[ operation closeSocket()
        if (_ws.is_open()) {
            boost::system::error_code ec;
            _ws.close(boost::beast::websocket::close_code::normal, ec);
            //S_LOG_DEBUG("closeSocket:" << ec.message() << ".");
        }
        //#]
    }

    void S_WS_Connection::doRead() {
        //#[ operation doRead()
        // Read a message into our buffer
        _ws.async_read(
                _buffer,
                _strand.wrap(std::bind(
                        &S_WS_Connection::handleRead,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2)));
        //#]
    }

    void S_WS_Connection::fail(boost::beast::error_code ec, char const *what) {
        //#[ operation fail(boost::beast::error_code,char const*)
        // Don't report on canceled operations
        if (ec == boost::asio::error::operation_aborted)
            return;
        //S_LOG_ERROR(what << ": " << ec.message() << ":" << ec);
        //#]
    }

    std::string S_WS_Connection::getConnectionId() {
        //#[ operation getConnectionId()
        return _connectionId;
        //#]
    }

    void S_WS_Connection::handleAccept(boost::beast::error_code ec) {
        //#[ operation handleAccept(boost::beast::error_code)
        if (ec)
            return fail(ec, "accept");
        start();
        //#]
    }

    void S_WS_Connection::handleWrite(boost::beast::error_code ec, std::size_t bytesTransferred) {
        //#[ operation handleWrite(boost::beast::error_code,std::size_t)
        // Handle the error, if any
        if (ec) {
            //_queue.clear();
            fail(ec, "write");
            if (ec != boost::asio::error::operation_aborted) {
                //stop();
            }
            return;
        }
        // Remove the string from the queue
        //S_LOG_DEBUG("writePacket: " << *_queue.front() << " to " << "remoteIpAddress: " << _remoteIpAddress << " : " << _connectionId);

        _queue.pop();

        // Send the next message if any
        if (!_queue.empty() && _ws.is_open()) {
            _ws.async_write(
                    boost::asio::buffer(_queue.front()),
                    _strand.wrap(std::bind(
                            &S_WS_Connection::handleWrite,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2)));
        }
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
        _ws.async_accept(
                _strand.wrap(std::bind(
                        &S_WS_Connection::handleAccept,
                        shared_from_this(),
                        std::placeholders::_1)));

        //S_LOG_DEBUG("start success.");
        //#]
    }

    void S_WS_Connection::send(std::string &ss) {
        //#[ operation send(boost::shared_ptr<std::string const> const&)
        _strand.post(std::bind(
                &S_WS_Connection::asyncWrite,
                shared_from_this(),
                ss));
        //#]
    }

    void S_WS_Connection::sendReqMsg(std::string msg) {
        //#[ operation sendReqMsg(std::string)
#if 0
        IDR_Request* message = new IDR_Request();
        message->_content = msg;
        message->toBuffers();
        asyncWrite(message);

        //std::lock_guard<std::mutex> lock(_mutex);
        IDR_Request message;
        message._content = msg;
        message.toBuffers();
        auto const ss = boost::make_shared<std::string const>(std::move(message._msg));
        //asyncWrite(ss);
        send(ss);
#endif
        //#]
    }

    void S_WS_Connection::sendResMsg(std::string msg, int httpErrorCode) {
        //#[ operation sendResMsg(std::string,int)
#if 0
        IDR_Response* message = new IDR_Response(httpErrorCode);
        message->_content = msg;
        message->toBuffers();
        asyncWrite(message);

        //std::lock_guard<std::mutex> lock(_mutex);
        IDR_Response message(httpErrorCode);
        message._content = msg;
        message.toBuffers();
        auto const ss = boost::make_shared<std::string const>(std::move(message._msg));
        //asyncWrite(ss);
        send(ss);
#endif
        //#]
    }

    void S_WS_Connection::shutdown() {
        //#[ operation shutdown()
        //S_LOG_DEBUG("shutdown.");
        _connectionManager.stop(shared_from_this());
        //#]
    }

    void S_WS_Connection::start() {
        //#[ operation start()
        //_connectionService = new IDW_ConnectionService(_connectionId, _server);
        //_connectionService->startBehavior();
        _connectionManager.join(shared_from_this());
        //S_LOG_DEBUG("connecttion count = " << _connectionManager._connections.size() << ".");
        // Read a message
        doRead();
        //#]
    }

    void S_WS_Connection::stop() {
        //#[ operation stop()
        if (_ws.is_open()) {
            boost::system::error_code ec;
            _ws.close(boost::beast::websocket::close_code::normal, ec);
            //S_LOG_DEBUG(_connectionId << " stop:" << ec.message() << ".");
        }
        //#]
    }

    void S_WS_Connection::handleRead(boost::beast::error_code ec, std::size_t bytesTransferred) {
        //#[ operation handleRead(boost::beast::error_code,std::size_t)
        if (ec) {
            fail(ec, "read");
            if (ec != boost::asio::error::operation_aborted) {
                if (_service._pUserNotify && _service._disConnectNotify)
                    _service._disConnectNotify(_service._pUserNotify, _connectionId);
                _connectionManager.stop(shared_from_this());
            }
        }
#if 1
        if (!ec) {
            if (bytesTransferred) {
                if (_service._pUserRead && _service._readFromServer) {
                    S_WS_Msg *msg = new S_WS_Msg(_connectionId,
                                                 std::string(boost::beast::buffers_to_string(_buffer.data())));
                    msg->_remoteIp = _remoteIpAddress;
                    msg->_remotePort = _remotePort;
                    _service._readFromServer(_service._pUserRead, msg);
                }
            }
            _buffer.consume(_buffer.size());
            _ws.async_read(
                    _buffer,
                    _strand.wrap(std::bind(
                            &S_WS_Connection::handleRead,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2)));
        }
#endif
        // Echo the message
#if 0
        _ws.text(_ws.got_text());
        _ws.async_write(
                    _buffer.data(),
                    boost::beast::bind_front_handler(
                        &IDW_Connection::handleWrite,
                        shared_from_this()));
#endif
        //#]
    }
}
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Connection.cpp
*********************************************************************/
