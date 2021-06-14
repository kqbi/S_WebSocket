#include "S_WS_ClientAPI.h"
#include "S_WS_ClientService.h"
using namespace S_WS;
S_WS_CLIENT_EXPORT ws_client S_WS_CLIENT_CALL S_WSClient_Create(boost::asio::io_context &ioc) {
    S_WS_ClientService *service = new S_WS_ClientService(ioc);
    return (ws_client) service;
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Release(ws_client ctx) {
    assert(ctx);
    ((S_WS_ClientService *) ctx)->handleStop();
    delete (S_WS_ClientService *) ctx;
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_Connect(ws_client ctx, std::string uri, void* pUser, CONNECTED connected, std::map<std::string, std::string> extraHeaders) {
    assert(ctx);
    ((S_WS_ClientService *) ctx)->connect(uri, pUser, connected, extraHeaders);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_ReConnect(ws_client ctx) {
    assert(ctx);
    ((S_WS_ClientService *) ctx)->reConnect();
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL S_WSClient_SendMsg(ws_client ctx, std::string msg) {
    assert(ctx);
    ((S_WS_ClientService *) ctx)->sendMessage(msg);
}

S_WS_CLIENT_EXPORT void S_WS_CLIENT_CALL
S_WSClient_ReadFromServer(ws_client ctx, void *pUser, READFROMSERVER readFromServer) {
    assert(ctx);
    if (readFromServer)
        ((S_WS_ClientService *) ctx)->readFromServer(pUser, readFromServer);
}
