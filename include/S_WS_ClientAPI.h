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

/**
 * 创建一个wsClient句柄
 * @param ioc boost asio的iocontext
 * @return 对象指针
 */
S_WS_CLIENT_EXPORT ws_client S_WS_CLIENT_CALL S_WSClient_Create(boost::asio::io_context &ioc);

/**
 * 初始化日志
 * @param fileName 日志文件名
 * @param type 日志类型 0:控制台，1:文件,默认:控制台+文件
 * @param level 日志级别 0:trace,1:debug,2:info,3:warning,4:error,5:fatal
 * @param maxFileSize 日志文件大小限制,单位:MB
 * @param maxBackupIndex 日志个数限制
 */
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_InitLog(std::string &fileName, int type, int level, int maxFileSize, int maxBackupIndex);

typedef void (*CONNECTED)(bool connected, void* pUser);

/**
 * 创建连接
 * @param ctx wsClient句柄
 * @param uri 访问的服务端地址,例如:ws://127.0.0.1:6971/
 * @param connected 连接回调函数
 * @param pUser 用户指针
 * @param extraHeaders header中的key-value
 */
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Connect(ws_client ctx, std::string uri, CONNECTED connected, void* pUser, std::map<std::string, std::string> extraHeaders = {});

/**
 * 重连接
 * @param ctx wsClient句柄
 */
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_ReConnect(ws_client ctx);

/**
 * 发送数据
 * @param ctx wsClient句柄
 * @param msg 发送数据
 */
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_SendMsg(ws_client ctx, std::string msg);

typedef void (*READFROMSERVER)(S_WS_Msg* msg, void* pUser);
/**
 * 接收数据
 * @param ctx wsClient句柄
 * @param readFromServer 接收数据的回调
 * @param pUser 用户指针
 */
S_WS_CLIENT_EXPORT void  S_WS_CLIENT_CALL S_WSClient_ReadFromServer(ws_client ctx, READFROMSERVER readFromServer, void* pUser);

/**
 * 销毁句柄
 * @param ctx wsClient句柄
 */
S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Release(ws_client ctx);

#ifdef __cplusplus
}
#endif

#endif // SWSCLIENTSDK_H
