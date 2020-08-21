#ifndef SWSCLIENTSDK_H
#define SWSCLIENTSDK_H

#include "S_WS_Msg.h"
#include <boost/asio/io_context.hpp>
using namespace S_WS;
#if defined( _WIN32 ) || defined( __MINGW32__ )
#   if defined( S_WS_CLIENT_API_EXPORTS )
#       define S_WS_CLIENT_EXPORT __declspec(dllexport)
#       define S_WS_CLIENT_CALL __stdcall
#   elif defined( S_WS_CLIENT_USE_DLL_IMPORT ) || !defined( S_WS_CLIENT_USE_STATIC_LIB )
#       define S_WS_CLIENT_EXPORT __declspec(dllimport)
#       define S_WS_CLIENT_CALL __stdcall
#   else
#       define S_WS_CLIENT_EXPORT
#       define S_WS_CLIENT_CALL
#   endif
#elif defined(__linux__) || defined(__APPLE__) //linux
#   define S_WS_CLIENT_EXPORT
#   define S_WS_CLIENT_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef void *ws_client;

S_WS_CLIENT_EXPORT ws_client S_WS_CLIENT_CALL S_WSClient_Create(boost::asio::io_context &ioc);

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Release(ws_client ctx);

typedef void (*CONNECTED)(void* pUser, bool connected);
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Connect(ws_client ctx, std::string uri, void* pUser, CONNECTED connected, std::map<std::string, std::string> extraHeaders = {});

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_SendMsg(ws_client ctx, std::string msg);

typedef void (*DISCONNECTNOTIFY)(void* pUser);
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_DisConnectNotify(ws_client ctx, void* pUser, DISCONNECTNOTIFY disConnectNotify);

typedef void (*READFROMSERVER)(void* pUser, S_WS_Msg* msg);
S_WS_CLIENT_EXPORT void  S_WS_CLIENT_CALL S_WSClient_ReadFromServer(ws_client ctx, void* pUser, READFROMSERVER readFromServer);

#ifdef __cplusplus
}
#endif

#endif // SWSCLIENTSDK_H
