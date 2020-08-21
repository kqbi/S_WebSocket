#ifndef SWSSERVICESDK_H
#define SWSSERVICESDK_H

#include "S_WS_Msg.h"
#include <boost/asio/io_context.hpp>
using namespace S_WS;
#if defined( _WIN32 ) || defined( __MINGW32__ )
#   if defined( S_WS_SERVER_API_EXPORTS )
#       define S_WS_SERVER_EXPORT __declspec(dllexport)
#       define S_WS_SERVER_CALL __stdcall
#   elif defined( S_WS_SERVER_USE_DLL_IMPORT ) || !defined( S_WS_SERVER_USE_STATIC_LIB )
#       define S_WS_SERVER_EXPORT __declspec(dllimport)
#       define S_WS_SERVER_CALL __stdcall
#   else
#       define S_WS_SERVER_EXPORT
#       define S_WS_SERVER_CALL
#   endif
#elif defined(__linux__) || defined(__APPLE__) //linux
#   define S_WS_SERVER_EXPORT
#   define S_WS_SERVER_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef void *ws_server;

S_WS_SERVER_EXPORT ws_server S_WS_SERVER_CALL S_WSServer_Create(boost::asio::io_context &ioc);

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_Release(ws_server ctx);

S_WS_SERVER_EXPORT bool S_WS_SERVER_CALL S_WSServer_Listen(ws_server ctx, std::string &ipAddress, unsigned short port);

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_SendMsg(ws_server ctx, std::string& connectionId, std::string msg);

typedef void (*DISCONNECTNOTIFY)(void* pUser, std::string& connectionId);
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_DisConnectNotify(ws_server ctx, void* pUser, DISCONNECTNOTIFY disConnectNotify);

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_CloseConnection(ws_server ctx, std::string& connectionId);

typedef void (*READFROMSERVER)(void* pUser, S_WS_Msg* msg);
S_WS_SERVER_EXPORT void  S_WS_SERVER_CALL S_WSServer_ReadFromServer(ws_server ctx, void* pUser, READFROMSERVER readFromServer);

#ifdef __cplusplus
}
#endif

#endif // SWSSERVICESDK_H
