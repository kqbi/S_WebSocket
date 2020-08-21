//
// Created by kqbi on 2020/5/28.
//

#include "S_WS_ClientConnect.h"
#include "S_WS_Msg.h"
#include "S_WS_ClientService.h"
namespace S_WS {
    S_WS_ClientConnect::S_WS_ClientConnect(boost::asio::io_context &ioc, S_WS_ClientService &service)
            : _resolver(boost::asio::make_strand(ioc)), _ws(boost::asio::make_strand(ioc)),
              _strand(ioc), _service(service) {

    }

    S_WS_ClientConnect::~S_WS_ClientConnect() {

    }

    void S_WS_ClientConnect::fail(boost::beast::error_code ec, char const *what) {
        //#[ operation fail(boost::beast::error_code,char const*)
        // Don't report on canceled operations
        if (ec == boost::asio::error::operation_aborted)
            return;
        //S_LOG_ERROR(what << ": " << ec.message() << ":" << ec);
        //#]
    }

    void S_WS_ClientConnect::connect(std::string &host, std::string &port) {
        // Look up the domain name
        _resolver.async_resolve(host, port,
                                boost::beast::bind_front_handler(
                                        &S_WS_ClientConnect::onResolve,
                                        this));
    }

    void
    S_WS_ClientConnect::onResolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results) {
        if (ec)
            return fail(ec, "resolve");

        boost::beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(3));

        // Make the connection on the IP address we get from a lookup
        boost::beast::get_lowest_layer(_ws).async_connect(results,
                                                          boost::beast::bind_front_handler(
                                                                  &S_WS_ClientConnect::onConnect,
                                                                  this));
    }

    void S_WS_ClientConnect::onConnect(boost::beast::error_code ec,
                                       boost::asio::ip::tcp::resolver::results_type::endpoint_type) {
        if (ec) {
            _service._disConnectNotify(_service._pUserNotify);
            return fail(ec, "connect");
        }
        // Turn off the timeout on the tcp_stream, because
        // the websocket stream has its own timeout system.
        boost::beast::get_lowest_layer(_ws).expires_never();

        // Set suggested timeout settings for the websocket
        _ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(
                boost::beast::role_type::client));

        // Set a decorator to change the User-Agent of the handshake
        _ws.set_option(boost::beast::websocket::stream_base::decorator(
                [=](boost::beast::websocket::request_type &req) {
                    req.set(boost::beast::http::field::user_agent,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-async");
                    req.set(boost::beast::http::field::host,
                            _service._ipAddress + ":" + _service._port);
                    for (auto it : _service._extraHeaders) {
                        req.set(it.first, it.second);
                    }
                }));

        // Perform the websocket handshake
        _ws.async_handshake(_service._ipAddress, "/" + _service._target,
                            boost::beast::bind_front_handler(
                                    &S_WS_ClientConnect::onHandshake,
                                    this));
    }

    void S_WS_ClientConnect::onHandshake(boost::beast::error_code ec) {
        if (ec) {
            _service._connected(_service._pUserNotify, false);
            return fail(ec, "handshake");
        }
        _service._connected(_service._pUserNotify, true);
        // Send the message
        _ws.async_read(_buffer, _strand.wrap(std::bind(
                &S_WS_ClientConnect::onRead,
                this,
                std::placeholders::_1,
                std::placeholders::_2)));
    }

    void S_WS_ClientConnect::asyncWrite(std::string &msg) {
        _queue.push(msg);

        // Are we already writing?
        if (_queue.size() > 1)
            return;

        // We are not currently writing, so send this immediately
        if (_ws.is_open()) {
            _ws.async_write(
                    boost::asio::buffer(_queue.front()),
                    _strand.wrap(std::bind(
                            &S_WS_ClientConnect::onWrite,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2)));
        }
    }

    void S_WS_ClientConnect::onWrite(boost::beast::error_code ec, std::size_t bytesTransferred) {
        boost::ignore_unused(bytesTransferred);

        if (ec)
            return fail(ec, "write");

        _queue.pop();

        // Send the next message if any
        if (!_queue.empty() && _ws.is_open()) {
            _ws.async_write(
                    boost::asio::buffer(_queue.front()),
                    _strand.wrap(std::bind(
                            &S_WS_ClientConnect::onWrite,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2)));
        }
    }

    void S_WS_ClientConnect::onRead(boost::beast::error_code ec, std::size_t bytesTransferred) {
        if (ec) {
            fail(ec, "read");
            if (ec != boost::asio::error::operation_aborted) {
                if (_service._pUserNotify && _service._disConnectNotify)
                    _service._disConnectNotify(_service._pUserNotify);
                //_connectionManager.stop(shared_from_this());
            }
        }
        if (!ec) {
            if (bytesTransferred) {
                if (_service._pUserRead && _service._readFromServer) {
                    S_WS_Msg *msg = new S_WS_Msg(std::string(""),
                                                 std::string(boost::beast::buffers_to_string(_buffer.data())));
                    _service._readFromServer(_service._pUserRead, msg);
                }
            }
            _buffer.consume(_buffer.size());
            _ws.async_read(_buffer, _strand.wrap(std::bind(
                    &S_WS_ClientConnect::onRead,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2)));
        }
    }

    void S_WS_ClientConnect::sendMessage(std::string &msg) {
        _strand.post(std::bind(
                &S_WS_ClientConnect::asyncWrite,
                this,
                msg));
    }

    void S_WS_ClientConnect::onClose(boost::beast::error_code ec) {
        if (ec)
            return fail(ec, "close");

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        //std::cout << beast::make_printable(buffer_.data()) << std::endl;
    }

    void S_WS_ClientConnect::closeSocket() {
        if (_ws.is_open()) {
            boost::system::error_code ec;
            _ws.close(boost::beast::websocket::close_code::normal, ec);
        }
    }
}