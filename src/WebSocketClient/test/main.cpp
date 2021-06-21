/********************************************************************
    Rhapsody	: 8.4
    Login		: Administrator
    Component	: DefaultComponent
    Configuration 	: S_HTTP_Config
    Model Element	: S_HTTP_Config
//!	Generated Date	: Fri, 14, Jun 2019  
    File Path	: ../../src/MainDefaultComponent.cpp
*********************************************************************/

#include <iostream>
#include <signal.h>
#include <boost/asio/executor_work_guard.hpp>
#include "S_WS_Client.h"

static bool finished = false;

void connectNotify(bool connected, void *pUser) {
    S_WS_Client *client = (S_WS_Client *) pUser;
    if (!connected) {
		printf("disconnected\n");
        client->reConnect();
    } else {
        printf("connected\n");
    }
}

static void
signalHandler(int signo) {
    std::cerr << "Shutting down" << std::endl;
    finished = true;
}


int main(int argc, char *argv[]) {
    int status = 0;
#ifndef _WIN32
    if ( signal( SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        std::cerr << "Couldn't install signal handler for SIGPIPE" << std::endl;
        exit(-1);
    }
    //        if ( signal( SIGHUP,signalHandler ) == SIG_ERR )
    //        {
    //            std::cerr << "Couldn't install signal handler for SIGHUP" << std::endl;
    //            exit( -1 );
    //        }
#endif

    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << "Couldn't install signal handler for SIGINT" << std::endl;
        exit(-1);
    }

    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        std::cerr << "Couldn't install signal handler for SIGTERM" << std::endl;
        exit(-1);
    }
    boost::asio::io_context ioc;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work(boost::asio::make_work_guard(ioc));
    S_WS_Client *client = new S_WS_Client(ioc);
    client->init();
    std::string uri = "ws://127.0.0.1:6971/";
    std::map<std::string, std::string> t;
    client->connect(uri, connectNotify, client, t);
    std::thread a([&](){ioc.run();});
    while (!finished) {
#ifdef WIN32
        Sleep(1000);
#else
        usleep(1000*1000);
#endif
    }
    delete client;
    ::exit(status);
    status = 0;
    return status;
}

/*********************************************************************
    File Path	: ../../src/MainDefaultComponent.cpp
*********************************************************************/
