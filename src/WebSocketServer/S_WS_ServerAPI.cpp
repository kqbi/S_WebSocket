#include "S_WS_ServerAPI.h"
#include "S_WS_ServerService.h"

S_WS_SERVER_EXPORT ws_server S_WS_SERVER_CALL S_WSServer_Create(boost::asio::io_context &ioc) {
    S_WS_ServerService::Ptr *service(new S_WS_ServerService::Ptr(new S_WS_ServerService(ioc)));
    return (ws_server) service;
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_Release(ws_server ctx) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    (*c)->handleStop();
    delete (S_WS_ServerService::Ptr *) ctx;
}

S_WS_SERVER_EXPORT bool S_WS_SERVER_CALL S_WSServer_Listen(ws_server ctx, std::string &ipAddress, unsigned short port) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    return (*c)->listen(ipAddress, port);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL
S_WSServer_DisConnectNotify(ws_server ctx, void *pUser, DISCONNECTNOTIFY disConnectNotify) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    if (disConnectNotify)
        (*c)->disConnectNotify(pUser, disConnectNotify);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_CloseConnection(ws_server ctx, std::string &connectionId) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    (*c)->closeConnection(connectionId);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_SendMsg(ws_server ctx, std::string &connectionId, std::string msg) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    (*c)->sendMessage(connectionId, msg);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL
S_WSServer_ReadFromServer(ws_server ctx, void *pUser, READFROMSERVER readFromServer) {
    assert(ctx);
    S_WS_ServerService::Ptr *c = (S_WS_ServerService::Ptr *)ctx;
    if (readFromServer)
        (*c)->readFromServer(pUser, readFromServer);
}
