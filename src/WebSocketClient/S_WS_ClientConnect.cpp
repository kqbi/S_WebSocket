//
// Created by kqbi on 2020/5/28.
//

#include "S_WS_ClientConnect.h"
#include "S_WS_Msg.h"
#include "S_WS_ClientService.h"
#include <boost/asio/spawn.hpp>
#include "Logger.h"
#include <iostream>

namespace S_WS {
    S_WS_ClientConnect::S_WS_ClientConnect(boost::asio::io_context &ioc, S_WS_ClientService &service)
            : _resolver(boost::asio::make_strand(ioc)), _ws(boost::asio::make_strand(ioc)),
              _strand(ioc.get_executor()), _service(service) {

    }

    S_WS_ClientConnect::~S_WS_ClientConnect() {

    }

    void S_WS_ClientConnect::connect(std::string &host, std::string &port) {
        auto self = shared_from_this();
        _resolver.async_resolve(host, port,
                                [this, self](boost::system::error_code ec,
                                             boost::asio::ip::tcp::resolver::results_type results) {
                                    if (ec) {
                                        stop(ec, "resolve");
                                        return;
                                    }
                                    boost::beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(3));

                                    asyncConnect(results);
                                });
    }


    void S_WS_ClientConnect::asyncConnect(boost::asio::ip::tcp::resolver::results_type results) {
        auto self = shared_from_this();
        boost::beast::get_lowest_layer(_ws).async_connect(
                results,
                [this, self](boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type::endpoint_type) {
                    if (ec) {
                        stop(ec, "connect");
                        return;
                    }
                    // Turn off the timeout on the tcp_stream, because
                    // the websocket stream has its own timeout system.
                    boost::beast::get_lowest_layer(_ws).expires_never();

                    // Set suggested timeout settings for the websocket
                    _ws.set_option(
                            boost::beast::websocket::stream_base::timeout::suggested(
                                    boost::beast::role_type::client));

                    // Set a decorator to change the User-Agent of the handshake
                    _ws.set_option(
                            boost::beast::websocket::stream_base::decorator(
                                    [=](boost::beast::websocket::request_type &req) {
                                        req.set(boost::beast::http::field::user_agent,
                                                std::string(
                                                        BOOST_BEAST_VERSION_STRING) +
                                                " websocket-client-async");
                                        req.set(boost::beast::http::field::host,
                                                _service._ipAddress +
                                                ":" + _service._port);
                                        for (auto it : _service._extraHeaders) {
                                            req.set(it.first, it.second);
                                        }
                                    }));

                    // Perform the websocket handshake
                    asyncHandshake();
                });
    }

    void S_WS_ClientConnect::asyncHandshake() {
        auto self = shared_from_this();
        _ws.async_handshake(_service._ipAddress, "/" + _service._target,
                            [this, self](boost::beast::error_code ec) {
                                if (ec) {
                                   stop(ec, "Handshake");
                                   return;
                                }
                                _service._connected(_service._pUser, true);

                                asyncRead();
                            });
    }

    void S_WS_ClientConnect::asyncRead() {
        auto self = shared_from_this();
        _ws.async_read(_buffer, [this, self](boost::beast::error_code ec, std::size_t bytesTransferred) {
            if (ec) {
                stop(ec, "Read");
            } else {
                if (bytesTransferred) {
                    if (_service._pUserRead && _service._readFromServer) {
                        S_WS_Msg *msg = new S_WS_Msg(std::string(""),
                                                     std::string(boost::beast::buffers_to_string(_buffer.data())));
                        _service._readFromServer(_service._pUserRead, msg);
                    }
                    _buffer.consume(_buffer.size());
                }
                asyncRead();
            }
        });
    }

    void S_WS_ClientConnect::sendMessage(std::string &msg) {
        auto self(shared_from_this());
        boost::asio::post(_strand, [this, msg, self]() {
            boost::asio::spawn(_strand,
                               [this, msg, self](boost::asio::yield_context yield) {
                                   if (_ws.is_open()) {
                                       boost::system::error_code err;
                                       _ws.async_write(boost::asio::buffer(msg),
                                                       yield[err]);
                                       if (err) {
                                           stop(err, "sendMessage");
                                       }
                                   }
                               });
        });
    }

    void S_WS_ClientConnect::closeSocket() {
        if (_ws.is_open()) {
            boost::system::error_code ec;
            _ws.close(boost::beast::websocket::close_code::normal, ec);
        }
    }

    void S_WS_ClientConnect::stop(boost::system::error_code &err, std::string f) {
        S_LOG_DEBUG(f << "error: " << err.message());
        if (err != boost::asio::error::operation_aborted) {
            closeSocket();
            if (_service._pUser && _service._connected)
                _service._connected(_service._pUser, false);
        }
    }

}