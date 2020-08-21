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
#include "S_WS_Server.h"
static bool finished = false;

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

        S_WS_Server* server = new S_WS_Server();
        if(!server->listen("", 6971)){
            std::cerr << "Failed to start Service, exiting..." << std::endl;
            delete server;
            ::exit(-1);
        }

        while (!finished)
        {
#ifdef WIN32
            Sleep(1000);
#else
            usleep(1000*1000);
#endif
        }
        delete server;
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
