#include "S_WS_ClientAPI.h"
#include "S_WS_ClientService.h"
#include "Logger.h"

using namespace S_WS;
S_WS_CLIENT_EXPORT ws_client S_WS_CLIENT_CALL S_WSClient_Create(boost::asio::io_context &ioc) {
    S_WS_ClientService::Ptr *service(new S_WS_ClientService::Ptr(new S_WS_ClientService(ioc)));
    return (ws_client) service;
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL
S_WSClient_InitLog(std::string &fileName, int type, int level, int maxFileSize, int maxBackupIndex) {
    Logger::Instance().Init(fileName, type, level, maxFileSize, maxBackupIndex);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL
S_WSClient_Connect(ws_client ctx, std::string uri, CONNECTED connected, void *pUser,
                   std::map<std::string, std::string> extraHeaders) {
    assert(ctx);
    S_WS_ClientService::Ptr *c = (S_WS_ClientService::Ptr *) ctx;
    (*c)->connect(uri, connected, extraHeaders, pUser);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_ReConnect(ws_client ctx) {
    assert(ctx);
    S_WS_ClientService::Ptr *c = (S_WS_ClientService::Ptr *) ctx;
    (*c)->reConnect();
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_SendMsg(ws_client ctx, std::string msg) {
    assert(ctx);
    S_WS_ClientService::Ptr *c = (S_WS_ClientService::Ptr *) ctx;
    (*c)->sendMessage(msg);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL
S_WSClient_ReadFromServer(ws_client ctx, READFROMSERVER readFromServer, void *pUser) {
    assert(ctx);
    S_WS_ClientService::Ptr *c = (S_WS_ClientService::Ptr *) ctx;
    if (readFromServer)
        (*c)->readFromServer(readFromServer, pUser);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Release(ws_client ctx) {
    assert(ctx);
    S_WS_ClientService::Ptr *c = (S_WS_ClientService::Ptr *) ctx;
    (*c)->handleStop();
    delete (S_WS_ClientService::Ptr *) ctx;
}