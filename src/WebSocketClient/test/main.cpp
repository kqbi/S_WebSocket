/********************************************************************
    Rhapsody	: 8.4
    Login		: Administrator
    Component	: DefaultComponent
    Configuration 	: S_HTTP_Config
    Model Element	: S_HTTP_Config
//!	Generated Date	: Fri, 14, Jun 2019  
    File Path	: ../../src/MainDefaultComponent.cpp
*********************************************************************/

#include <oxf.h>
#include <iostream>
#include <signal.h>
#include "S_WS_Client.h"
static bool finished = false;

void connectNotify(void *pUser, bool connected) {
    S_WS_Client* client = (S_WS_Client*)pUser;

}

static void
signalHandler(int signo)
{
    std::cerr << "Shutting down" << std::endl;
    finished = true;
}


int main(int argc, char* argv[]) {
    int status = 0;
    if(OXF::Instance().Initialize())
    {
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

        if ( signal( SIGINT, signalHandler  ) == SIG_ERR )
        {
            std::cerr << "Couldn't install signal handler for SIGINT" << std::endl;
            exit( -1 );
        }

        if ( signal( SIGTERM, signalHandler  ) == SIG_ERR )
        {
            std::cerr << "Couldn't install signal handler for SIGTERM" << std::endl;
            exit( -1 );
        }

        S_WS_Client* client = new S_WS_Client();
        client->init();
        std::string uri = "127.0.0.1:6971";
        client->connect(uri,client,connectNotify);

        while (!finished)
        {
#ifdef WIN32
            Sleep(1000);
#else
            usleep(1000*1000);
#endif
        }
        delete client;
        ::exit(status);
        status = 0;
    }
    else
    {
        status = 1;
    }
    return status;
}

/*********************************************************************
    File Path	: ../../src/MainDefaultComponent.cpp
*********************************************************************/
