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

/**
 * 创建一个wsServer句柄
 * @param ioc boost asio的iocontext
 * @return 对象指针
 */
S_WS_SERVER_EXPORT ws_server S_WS_SERVER_CALL S_WSServer_Create(boost::asio::io_context &ioc);

/**
 * 初始化日志
 * @param fileName 日志文件名
 * @param type 日志类型 0:控制台，1:文件,默认:控制台+文件
 * @param level 日志级别 0:trace,1:debug,2:info,3:warning,4:error,5:fatal
 * @param maxFileSize 日志文件大小限制,单位:MB
 * @param maxBackupIndex 日志个数限制
 */
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL
S_WSServer_InitLog(std::string &fileName, int type, int level, int maxFileSize, int maxBackupIndex);

/**
 * 创建监听端口
 * @param ipAddress 绑定的的ip地址,可以填空
 * @param port 监听的端口
 * @return bool值，true:成功，false:失败
 */
S_WS_SERVER_EXPORT bool S_WS_SERVER_CALL S_WSServer_Listen(ws_server ctx, std::string &ipAddress, unsigned short port);

/**
 * 发送数据
 * @param ctx wsServer句柄
 * @param connectionId 客户端连接的id标识
 * @param msg 发送的数据
 */
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_SendMsg(ws_server ctx, std::string& connectionId, std::string msg);

typedef void (*DISCONNECTNOTIFY)(std::string& connectionId, void* pUser);
/**
 * 断开连接的通知
 * @param ctx wsServer句柄
 * @param disConnectNotify 断开连接的回调函数
 * @param 用户指针
 */
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_DisConnectNotify(ws_server ctx, DISCONNECTNOTIFY disConnectNotify, void* pUser);

/**
 * 主动断开客户端连接
 * @param ctx wsServer句柄
 * @param connectionId 客户端连接id
 */
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_CloseConnection(ws_server ctx, std::string& connectionId);

typedef void (*READFROMSERVER)(S_WS_Msg* msg, void* pUser);
/**
 * 接收数据
 * @param ctx wsServer句柄
 * @param readFromServer 接收数据回调函数
 * @param pUser 用户指针
 */
S_WS_SERVER_EXPORT void  S_WS_SERVER_CALL S_WSServer_ReadFromServer(ws_server ctx, READFROMSERVER readFromServer, void* pUser);

/**
 * 销毁wsServer句柄
 * @param ctx wsServer句柄
 */
S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_Release(ws_server ctx);

#ifdef __cplusplus
}
#endif

#endif // SWSSERVICESDK_H
