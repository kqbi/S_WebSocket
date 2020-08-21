#include "S_WS_ServerAPI.h"
#include "S_WS_ServerService.h"

S_WS_SERVER_EXPORT ws_server S_WS_SERVER_CALL S_WSServer_Create(boost::asio::io_context &ioc) {
    S_WS_ServerService *service = new S_WS_ServerService(ioc);
    return (ws_server) service;
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_Release(ws_server ctx) {
    assert(ctx);
    ((S_WS_ServerService *) ctx)->handleStop();
    delete (S_WS_ServerService *) ctx;
}

S_WS_SERVER_EXPORT bool S_WS_SERVER_CALL S_WSServer_Listen(ws_server ctx, std::string &ipAddress, unsigned short port) {
    assert(ctx);
    return ((S_WS_ServerService *) ctx)->listen(ipAddress, port);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL
S_WSServer_DisConnectNotify(ws_server ctx, void *pUser, DISCONNECTNOTIFY disConnectNotify) {
    assert(ctx);
    if (disConnectNotify)
        ((S_WS_ServerService *) ctx)->disConnectNotify(pUser, disConnectNotify);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_CloseConnection(ws_server ctx, std::string &connectionId) {
    assert(ctx);
    ((S_WS_ServerService *) ctx)->closeConnection(connectionId);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL S_WSServer_SendMsg(ws_server ctx, std::string &connectionId, std::string msg) {
    assert(ctx);
    ((S_WS_ServerService *) ctx)->sendMessage(connectionId, msg);
}

S_WS_SERVER_EXPORT void S_WS_SERVER_CALL
S_WSServer_ReadFromServer(ws_server ctx, void *pUser, READFROMSERVER readFromServer) {
    assert(ctx);
    if (readFromServer)
        ((S_WS_ServerService *) ctx)->readFromServer(pUser, readFromServer);
}
