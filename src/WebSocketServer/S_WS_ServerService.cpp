/********************************************************************
	Rhapsody	: 8.4 
	Login		: Administrator
	Component	: DefaultComponent 
	Configuration 	: DefaultConfig
	Model Element	: S_WS_Service
//!	Generated Date	: Tue, 20, Aug 2019  
	File Path	: ../../src/WebSocketService/S_WS_Service.cpp
*********************************************************************/

#include "S_WS_ServerService.h"
#include "S_WS_Connection.h"
#include "S_WS_Msg.h"
#include "Logger.h"
#include "Utils.h"
//## package WebSocketService
namespace S_WS {
//## class S_WS_Service

    S_WS_ServerService::S_WS_ServerService(boost::asio::io_context &ioc) : _ioc(ioc), _acceptor(ioc), _ipAddress(""),
                                                                           _port(0), _pUserRead(0), _readFromServer(0),
                                                                           _disConnectNotify(0), _pUserNotify(0) {
        //#[ operation S_WS_Service(const std::string&,int,const std::string&,std::size_t)
        Logger::Instance().Init("server.log",0,0,50, 5);
        //#]
    }

    S_WS_ServerService::~S_WS_ServerService() {
        //#[ operation ~S_WS_Service()
        //S_LOG_DEBUG("~IDW_ConnectionServer.");
        //#]
    }

    void S_WS_ServerService::closeConnection(std::string &connectionId) {
        //#[ operation closeConnection(std::string&)
        _connectionManager.stop(connectionId);
        //#]
    }

    void S_WS_ServerService::disConnectNotify(DISCONNECTNOTIFY disConnectNotify, void *pUser) {
        //#[ operation disConnectNotify(void*,DISCONNECTNOTIFY)
        _pUserNotify = pUser;
        _disConnectNotify = disConnectNotify;
        //#]
    }

    void S_WS_ServerService::execProcessIDWProtocol(std::string &connectionId) {
        //#[ operation execProcessIDWProtocol(std::string&)
#if 0
        //S_LOG_DEBUG("execProcessIDRProtocol ProtocolType: " << protocol->getProtocolType());
        int errorCode = 0;
        IDR_Protocol::ProtocolType protocolType = protocol->getProtocolType();
        connection_ptr connection = 0;
        _connectionManager.getConnectionById(connectionId, connection);
        if (connection) {
            if (!connection->_connectionService->_registerFlags && (protocol->getProtocolType() != IDR_Protocol::LoginRegisterReq_IDR)) {
                if (protocol)
                    delete protocol;
                return;
            }
            switch (protocolType) {
            case IDR_Protocol::LoginRegisterReq_IDR:
            {
                IDR_LoginRegisterReq *req = (IDR_LoginRegisterReq *)protocol;
                S_LOG_INFO("LoginRegisterReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::UserInfoRecord record;
                errorCode = checkAuthentication(req->_userName, req->_password, record);
                if (!errorCode) {
                    connection->_connectionService->_userName = req->_userName;
                    connection->_connectionService->_roleId = record._roleId;
                    connection->_connectionService->_registerFlags = true;
                    connection->_connectionService->GEN(evIDWRegisterSuccess);
                }
                sendLoginRegisterRes(connection, protocol->_seq, errorCode, record);
            }
                break;
            case IDR_Protocol::KeepaliveReq_IDR:
            {
                S_LOG_INFO("KeepaliveReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                connection->_connectionService->_keepAliveTimeoutTimes = 0;
                sendKeepAliveRes(connection, protocol->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryDeviceBoxInfoReq_IDR:
            {
                IDR_QueryDeviceBoxInfoReq *req = (IDR_QueryDeviceBoxInfoReq *)protocol;
                S_LOG_INFO("QueryDeviceBoxInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceBoxInfoRecord record;
                errorCode = queryDeviceBoxInfo(req->_deviceId, record);
                sendQueryDeviceBoxInfoRes(connection, req->_seq, errorCode, record);
            }
                break;
            case IDR_Protocol::QueryDeviceBoxListReq_IDR:
            {
                IDR_QueryDeviceBoxListReq *req = (IDR_QueryDeviceBoxListReq *)protocol;
                S_LOG_INFO("QueryDeviceBoxListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceBoxInfoRecordList recordList;
                queryDeviceBoxList(req->_groupId, recordList);
                sendQueryDeviceBoxListRes(connection, req->_seq, errorCode, recordList);
            }
                break;
            case IDR_Protocol::QueryDeviceRealtimeStatusReq_IDR:
            {
                IDR_QueryDeviceRealtimeStatusReq *req = (IDR_QueryDeviceRealtimeStatusReq *)protocol;
                S_LOG_INFO("QueryDeviceRealtimeStatusReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                int ret = checkDeviceStatus(req->_deviceId);
                if (!ret)
                    connection->_server._controlLayerInterface->queryDeviceRealtimeStatus(connection->_connectionId,
                                                                                          protocol->_seq,
                                                                                          req->_deviceId,
                                                                                          boost::bind(&IDW_ConnectionServer::recvDeviceRealtimeStatusRes,
                                                                                                      this,
                                                                                                      _1,
                                                                                                      _2,
                                                                                                      _3,
                                                                                                      _4));
                else {
                    IDS_Protocol::DeviceRealtimeStatusInfo statusInfo;
                    _server._config->queryDeviceDefaultStatus(req->_deviceId, statusInfo);
                    statusInfo.deviceStatus = 1;
                    statusInfo.deviceId = req->_deviceId;
                    recvDeviceRealtimeStatusRes(connection->_connectionId, req->_seq, errorCode, statusInfo);
                }
            }
                break;
            case IDR_Protocol::AddUserReq_IDR:
            {
                IDR_AddUserReq *req = (IDR_AddUserReq *)protocol;
                S_LOG_INFO("AddUserReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (addUser(req->_userName, req->_password, req->_contact, req->_roleId, req->_mobilePhone))
                    errorCode = IPOSS_STATUS_ADD_ERROR;
                sendAddUserRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeleteUserReq_IDR:
            {
                IDR_DeleteUserReq *req = (IDR_DeleteUserReq *)protocol;
                S_LOG_INFO("DeleteUserReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::UserInfoRecord userInfoRecord;
                queryUserInfo(req->_userId, userInfoRecord);
                if (userInfoRecord._userName.compare("admin"))
                {
                    if (deleteUser(req->_userId))
                        errorCode = IPOSS_STATUS_DELETE_ERROR;
                }
                else
                    errorCode = IPOSS_STATUS_THIS_USER_CANNOT_DELETED;
                sendDeleteUserRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::UpdateUserInfoReq_IDR:
            {
                IDR_UpdateUserInfoReq *req = (IDR_UpdateUserInfoReq *)protocol;
                S_LOG_INFO("UpdateUserInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (updateUserInfo(req->_userId, req->_password, req->_contact, req->_roleId, req->_mobilePhone))
                    errorCode = IPOSS_STATUS_UPDATE_ERROR;
                sendUpdateUserInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryUserInfoReq_IDR:
            {
                IDR_QueryUserInfoReq *req = (IDR_QueryUserInfoReq *)protocol;
                S_LOG_INFO("QueryUserInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::UserInfoRecord userInfoRecord;
                errorCode = queryUserInfo(req->_userId, userInfoRecord);
                sendQueryUserInfoRes(connection, req->_seq, errorCode, userInfoRecord);
            }
                break;
            case IDR_Protocol::QueryUserInfoListReq_IDR:
            {
                IDR_QueryUserInfoListReq *req = (IDR_QueryUserInfoListReq *)protocol;
                S_LOG_INFO("QueryUserInfoListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::UserInfoRecordList userInfoRecordList;
                queryUserInfoList(userInfoRecordList);
                sendQueryUserInfoListRes(connection, req->_seq, errorCode, userInfoRecordList);
            }
                break;
            case IDR_Protocol::AddDeviceFactoryReq_IDR:
            {
                IDR_AddDeviceFactoryReq *req = (IDR_AddDeviceFactoryReq *)protocol;
                S_LOG_INFO("AddDeviceFactoryReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (addDeviceFactory(req->_factoryName, req->_contact, req->_phone))
                    errorCode = IPOSS_STATUS_ADD_ERROR;
                sendAddDeviceFactoryRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeleteDeviceFactoryReq_IDR:
            {
                IDR_DeleteDeviceFactoryReq *req = (IDR_DeleteDeviceFactoryReq *)protocol;
                S_LOG_INFO("DeleteDeviceFactoryReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (deleteDeviceFactory(req->_factoryId))
                    errorCode = IPOSS_STATUS_DELETE_ERROR;
                sendDeleteDeviceFactoryRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::UpdateDeviceFactoryInfoReq_IDR:
            {
                IDR_UpdateDeviceFactoryInfoReq *req = (IDR_UpdateDeviceFactoryInfoReq *)protocol;
                S_LOG_INFO("UpdateDeviceFactoryInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (updateDeviceFactoryInfo(req->_factoryId, req->_contact, req->_phone))
                    errorCode = IPOSS_STATUS_UPDATE_ERROR;
                sendUpdateDeviceFactoryInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryDeviceFactoryInfoReq_IDR:
            {
                IDR_QueryDeviceFactoryInfoReq *req = (IDR_QueryDeviceFactoryInfoReq *)protocol;
                S_LOG_INFO("QueryDeviceFactoryInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceFactoryInfoRecord deviceFactoryInfoRecord;
                errorCode = queryDeviceFactoryInfo(req->_factoryId, deviceFactoryInfoRecord);
                sendQueryDeviceFactoryInfoRes(connection, req->_seq, errorCode, deviceFactoryInfoRecord);
            }
                break;
            case IDR_Protocol::QueryDeviceFactoryInfoListReq_IDR:
            {
                IDR_QueryDeviceFactoryInfoListReq *req = (IDR_QueryDeviceFactoryInfoListReq *)protocol;
                S_LOG_INFO("QueryDeviceFactoryInfoListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceFactoryInfoRecordList devicefactoryInfoRecordList;
                queryDeviceFactoryInfoList(devicefactoryInfoRecordList);
                sendQueryDeviceFactoryInfoListRes(connection, req->_seq, errorCode, devicefactoryInfoRecordList);
            }
                break;
            case IDR_Protocol::ProtectionControlReq_IDR:
            {
                IDR_DefenseControlReq *req = (IDR_DefenseControlReq *)protocol;
                S_LOG_INFO("DefenseControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->protectionRemovalControl(connection->_connectionId,
                                                                                         req->_seq,
                                                                                         req->_deviceId,
                                                                                         req->_msgDataStr,
                                                                                         boost::bind(&IDW_ConnectionServer::recvDefenseControlRes,
                                                                                                     this,
                                                                                                     _1,
                                                                                                     _2,
                                                                                                     _3));
                else
                    recvDefenseControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::RelayControlReq_IDR:
            {
                IDR_RelayControlReq *req = (IDR_RelayControlReq *)protocol;
                S_LOG_INFO("RelayControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->relayControl(connection->_connectionId,
                                                                             req->_seq,
                                                                             req->_deviceId,
                                                                             req->_msgDataStr,
                                                                             boost::bind(&IDW_ConnectionServer::recvRelayControlRes,
                                                                                         this,
                                                                                         _1,
                                                                                         _2,
                                                                                         _3));
                else
                    recvRelayControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::GPIOControlReq_IDR:
            {
                IDR_GPIOControlReq *req = (IDR_GPIOControlReq *)protocol;
                S_LOG_INFO("GPIOControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->gpioControl(connection->_connectionId,
                                                                            req->_seq,
                                                                            req->_deviceId,
                                                                            req->_msgDataStr,
                                                                            boost::bind(&IDW_ConnectionServer::recvGPIOControlRes,
                                                                                        this,
                                                                                        _1,
                                                                                        _2,
                                                                                        _3));
                else
                    recvGPIOControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::StrongPowerControlReq_IDR:
            {
                IDR_StrongPowerControlReq *req = (IDR_StrongPowerControlReq *)protocol;
                S_LOG_INFO("StrongPowerControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->strongPowerControl(connection->_connectionId,
                                                                                   req->_seq,
                                                                                   req->_deviceId,
                                                                                   req->_msgDataStr,
                                                                                   boost::bind(&IDW_ConnectionServer::recvStrongPowerControlRes,
                                                                                               this,
                                                                                               _1,
                                                                                               _2,
                                                                                               _3));
                else
                    recvStrongPowerControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DCPowerControlReq_IDR:
            {
                IDR_DCPowerControlReq *req = (IDR_DCPowerControlReq *)protocol;
                S_LOG_INFO("DCPowerControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->dcPowerControl(connection->_connectionId,
                                                                               req->_seq,
                                                                               req->_deviceId,
                                                                               req->_msgDataStr,
                                                                               boost::bind(&IDW_ConnectionServer::recvDCPowerControlRes,
                                                                                           this,
                                                                                           _1,
                                                                                           _2,
                                                                                           _3));
                else
                    recvDCPowerControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::ACPowerControlReq_IDR:
            {
                IDR_ACPowerControlReq *req = (IDR_ACPowerControlReq *)protocol;
                S_LOG_INFO("ACPowerControlReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->acPowerControl(connection->_connectionId,
                                                                               req->_seq,
                                                                               req->_deviceId,
                                                                               req->_msgDataStr,
                                                                               boost::bind(&IDW_ConnectionServer::recvACPowerControlRes,
                                                                                           this,
                                                                                           _1,
                                                                                           _2,
                                                                                           _3));
                else
                    recvACPowerControlRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::RemoteResetConfigReq_IDR:
            {
                IDR_RemoteResetConfigReq *req = (IDR_RemoteResetConfigReq *)protocol;
                S_LOG_INFO("RemoteResetConfigReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->remoteResetConfig(connection->_connectionId,
                                                                                  req->_seq,
                                                                                  req->_deviceId,
                                                                                  boost::bind(&IDW_ConnectionServer::recvRemoteResetConfigRes,
                                                                                              this,
                                                                                              _1,
                                                                                              _2,
                                                                                              _3));
                else
                    recvRemoteResetConfigRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::RemoteRestartConfigReq_IDR:
            {
                IDR_RemoteRestartConfigReq *req = (IDR_RemoteRestartConfigReq *)protocol;
                S_LOG_INFO("RemoteRestartConfigReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->remoteRestartConfig(connection->_connectionId,
                                                                                    req->_seq,
                                                                                    req->_deviceId,
                                                                                    boost::bind(&IDW_ConnectionServer::recvRemoteRestartConfigRes,
                                                                                                this,
                                                                                                _1,
                                                                                                _2,
                                                                                                _3));
                else
                    recvRemoteRestartConfigRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::TThresholdConfigReq_IDR:
            {
                IDR_TemperatureThresholdConfigReq *req = (IDR_TemperatureThresholdConfigReq *)protocol;
                S_LOG_INFO("TThresholdConfigReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->tThresholdConfig(connection->_connectionId,
                                                                                 req->_seq,
                                                                                 req->_deviceId,
                                                                                 req->_temperatureThreshold,
                                                                                 boost::bind(&IDW_ConnectionServer::recvTThresholdConfigRes,
                                                                                             this,
                                                                                             _1,
                                                                                             _2,
                                                                                             _3));
                else
                    recvTThresholdConfigRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::ChangeDeviceDefaultSettingReq_IDR:
            {
                IDR_ChangeDeviceDefaultSettingReq *req = (IDR_ChangeDeviceDefaultSettingReq *)protocol;
                S_LOG_INFO("ChangeDeviceDefaultSettingReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = checkDeviceStatus(req->_deviceId);
                if (!errorCode)
                    connection->_server._controlLayerInterface->changeDeviceDefaultSetting(connection->_connectionId,
                                                                                           req->_seq,
                                                                                           req->_deviceId,
                                                                                           req->_gpioStatuss,
                                                                                           req->_protectionStatus,
                                                                                           req->_relayStatuss,
                                                                                           req->_strongPowerACStatuss,
                                                                                           req->_temperatureThreshold,
                                                                                           req->_weakPowerDCStatuss,
                                                                                           req->_weakPowerACStatuss,
                                                                                           boost::bind(&IDW_ConnectionServer::recvChangeDeviceDefaultSettingRes,
                                                                                                       this,
                                                                                                       _1,
                                                                                                       _2,
                                                                                                       _3));
                else
                    recvChangeDeviceDefaultSettingRes(connection->_connectionId, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::UpdateAlarmMsgToAssignedReq_IDR:
            {
                IDR_UpdateAlarmMsgToAssignedReq *req = (IDR_UpdateAlarmMsgToAssignedReq *)protocol;
                S_LOG_INFO("UpdateAlarmMsgToAssignedReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::string time = iDUtils::getIITimeStampStr();
                std::string timestamp = iDUtils::getTimeStampStr((unsigned char *)time.data(), 7);
                errorCode = updateFaultAssignInfo(req->_alarmMsgId, req->_userId, req->_handleSuggestion, timestamp);
                sendUpdateFaultAssignInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::AddGroupReq_IDR:
            {
                IDR_AddGroupReq *req = (IDR_AddGroupReq *)protocol;
                S_LOG_INFO("AddGroupReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = addGroup(req->_deviceGroupInfoRecord, req->_parentGroupId);
                sendAddGroupRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeleteGroupReq_IDR:
            {
                IDR_DeleteGroupReq *req = (IDR_DeleteGroupReq *)protocol;
                S_LOG_INFO("IDR_DeleteGroupReq From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (req->_groupId.compare("001"))
                {
                    if (deleteGroup(req->_groupId))
                        errorCode = IPOSS_STATUS_DELETE_ERROR;
                }
                else
                    errorCode = IPOSS_STATUS_THIS_GROUP_CANNOT_DELETED;
                sendDeleteGroupRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryGroupListReq_IDR:
            {
                IDR_QueryGroupListReq *req = (IDR_QueryGroupListReq *)protocol;
                S_LOG_INFO("QueryGroupListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceGroupInfoRecordList recordList;
                queryGroupList(req->_farentId, recordList);
                sendQueryGroupListRes(connection, req->_seq, errorCode, recordList);
            }
                break;
            case IDR_Protocol::QueryGroupInfoReq_IDR:
            {
                IDR_QueryGroupInfoReq *req = (IDR_QueryGroupInfoReq *)protocol;
                S_LOG_INFO("QueryGroupInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DeviceGroupInfoRecord record;
                errorCode = queryGroupInfo(req->_groupId, record);
                sendQueryGroupInfoRes(connection, req->_seq, errorCode, record);
            }
                break;
            case IDR_Protocol::UpdateGroupInfoReq_IDR:
            {
                IDR_UpdateGroupInfoReq *req = (IDR_UpdateGroupInfoReq *)protocol;
                S_LOG_INFO("UpdateGroupInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (updateGroupInfo(req->_deviceGroupInfoRecord))
                    errorCode = IPOSS_STATUS_UPDATE_ERROR;
                sendUpdateGroupInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::UpdateDeviceBoxInfoReq_IDR:
            {
                IDR_UpdateDeviceBoxInfoReq *req = (IDR_UpdateDeviceBoxInfoReq *)protocol;
                S_LOG_INFO("UpdateDeviceBoxIfo_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = updateDeviceBoxInfo(req->_deviceBoxInfoRecord);
                sendUpdateDeviceBoxInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryRoleListReq_IDR:
            {
                IDR_QueryRoleListReq *req = (IDR_QueryRoleListReq *)protocol;
                S_LOG_INFO("QueryRoleListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::RoleInfoRecordList recordList;
                queryRoleList(recordList);
                sendQueryRoleListRes(connection, req->_seq, errorCode, recordList);
            }
                break;
            case IDR_Protocol::QueryRoleInfoReq_IDR:
            {
                IDR_QueryRoleInfoReq *req = (IDR_QueryRoleInfoReq *)protocol;
                S_LOG_INFO("QueryRoleInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::RoleInfoRecord roleInfoRecord;
                errorCode = queryRoleInfo(req->_roleId, roleInfoRecord);
                sendQueryRoleInfoRes(connection, req->_seq, errorCode, roleInfoRecord);
            }
                break;
            case IDR_Protocol::QueryAlarmMsgNumReq_IDR:
            {
                IDR_QueryAlarmMsgNumReq *req = (IDR_QueryAlarmMsgNumReq *)protocol;
                S_LOG_INFO("QueryAlarmMsgNumReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                printf("req _alarmHandleStatus ===== %d\n", req->_alarmHandleStatus);
#if 0
                AbstractDb::RoleInfoRecord  roleInfoRecord;
                queryRoleInfo(req->_roleId, roleInfoRecord);
                sendQueryRoleInfoRes(req->_seq, errorCode, roleInfoRecord);
#endif
            }
                break;
            case IDR_Protocol::QueryAlarmMsgReq_IDR:
            {
                IDR_QueryAlarmMsgReq *req = (IDR_QueryAlarmMsgReq *)protocol;
                S_LOG_INFO("QueryAlarmMsgReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                int sum = 0;
                AbstractDb::DeviceAlarmMsgRecordList recordList;
                errorCode = queryAlarmMsg(req->_alarmHandleStatus, req->_alarmType, req->_startTime, req->_endTime,
                                          req->_startNum, req->_endNum, req->_queryCondition, req->_queryCondition,
                                          req->_queryCondition, req->_queryCondition, req->_orderByStr, req->_sortFlags, sum, recordList);
                sendQueryAlarmMsgRes(connection, req->_seq, errorCode, sum, recordList);
            }
                break;
            case IDR_Protocol::AddSlavedDeviceReq_IDR:
            {
                IDR_AddSlavedDeviceReq *req = (IDR_AddSlavedDeviceReq *)protocol;
                S_LOG_INFO("AddSlavedDeviceReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                if (addSlavedDevice(req->_record))
                    errorCode = IPOSS_STATUS_ADD_ERROR;
                sendAddSlavedDeviceRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeleteSlavedDeviceReq_IDR:
            {
                IDR_DeleteSlavedDeviceReq *req = (IDR_DeleteSlavedDeviceReq *)protocol;
                S_LOG_INFO("DeleteSlavedDeviceReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = deleteSlavedDevice(req->_deviceId);
                sendDeleteSlavedDeviceRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::UpdateSlavedDeviceInfoReq_IDR:
            {
                IDR_UpdateSlavedDeviceInfoReq *req = (IDR_UpdateSlavedDeviceInfoReq *)protocol;
                S_LOG_INFO("UpdateSlavedDeviceInfoReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = updateSlavedDeviceInfo(req->_record);
                sendUpdateSlavedDeviceInfoRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QuerySlavedDeviceInfoReq_IDR:
            {
                IDR_QuerySlavedDeviceInfoReq *req = (IDR_QuerySlavedDeviceInfoReq *)protocol;
                S_LOG_INFO("IDR_QuerySlavedDeviceInfoReq From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::SlavedDeviceInfoRecord record;
                errorCode = querySlavedDeviceInfo(req->_deviceId, record);
                sendQuerySlavedDeviceInfoRes(connection, req->_seq, errorCode, record);
            }
                break;
            case IDR_Protocol::QuerySlavedDeviceListReq_IDR:
            {
                IDR_QuerySlavedDeviceListReq *req = (IDR_QuerySlavedDeviceListReq *)protocol;
                S_LOG_INFO("QuerySlavedDeviceListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::SlavedDeviceInfoRecordList recordList;
                querySlavedDeviceList(recordList);
                sendQuerySlavedDeviceListRes(connection, req->_seq, errorCode, recordList);
            }
                break;
            case IDR_Protocol::UpdateSlavedDeviceInfoOfDeviceBoxReq_IDR:
            {
                IDR_UpdateSlavedDeviceInfoOfDeviceBoxReq *req = (IDR_UpdateSlavedDeviceInfoOfDeviceBoxReq *)protocol;
                S_LOG_INFO("UpdateSlavedDeviceInfoOfDeviceBoxReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = updateSlavedDeviceInfoOfDeviceBox(req->_deviceId, req->_slavedDeviceInfos);
                sendUpdateSlavedDeviceInfoOfDeviceBoxRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QuerySlavedDeviceOfDeviceBoxReq_IDR:
            {
                IDR_QuerySlavedDeviceOfDeviceBoxReq *req = (IDR_QuerySlavedDeviceOfDeviceBoxReq *)protocol;
                S_LOG_INFO("QuerySlavedDeviceOfDeviceBoxReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::string oldSlavedDeviceInfos = "";
                std::string newSlavedDeviceInfos = "";
                errorCode = querySlavedDeviceOfDeviceBox(req->_deviceId, oldSlavedDeviceInfos);
                if (!oldSlavedDeviceInfos.empty())
                {
                    parseSlavedDeviceInfoJsonStr(oldSlavedDeviceInfos, newSlavedDeviceInfos);
                    updateSlavedDeviceInfoOfDeviceBox(req->_deviceId, newSlavedDeviceInfos);
                }
                sendQuerySlavedDeviceOfDeviceBoxRes(connection, req->_seq, errorCode, newSlavedDeviceInfos);
            }
                break;
            case IDR_Protocol::UpdateAlarmMsgToProcessingCompletedReq_IDR:
            {
                IDR_UpdateAlarmMsgToProcessingCompletedReq *req = (IDR_UpdateAlarmMsgToProcessingCompletedReq *)protocol;
                S_LOG_INFO("UpdateAlarmMsgToProcessingCompletedReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::string time = iDUtils::getIITimeStampStr();
                std::string timestamp = iDUtils::getTimeStampStr((unsigned char *)time.data(), 7);
                int alarm = AbstractDb::DeviceAlarmMsgRecord::Alarm_Finished;
                errorCode = updateAlarmMsgToProcessingCompleted(req->_alarmMsgId, alarm, timestamp);
                sendUpdateAlarmMsgToProcessingCompletedRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::AddInspectionPlanningReq_IDR:
            {
                IDR_AddInspectionPlanningReq *req = (IDR_AddInspectionPlanningReq *)protocol;
                S_LOG_INFO("addInspectionPlanningReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::string uuid = iDUtils::createUUID();
                errorCode = addInspectionPlanning(uuid,
                                                  req->_cronExpression,
                                                  req->_cronExpressionDesc,
                                                  req->_inspectionPlanningName,
                                                  req->_inspectionTypeListDesc,
                                                  req->_deviceIdList,
                                                  req->_inspectionTypeList,
                                                  connection->_connectionService->_userName);

                try
                {
                    auto cron = cron::make_cron(req->_cronExpression.c_str());

                    std::time_t now = std::time(0);
                    std::time_t next = cron::cron_next(cron, now);
                    struct tm local;
                    time_t t = next;

#ifdef _WIN32
                    localtime_s(&local, &t);
#else
                    localtime_r(&t, &local);
#endif

                    uint16_t year = local.tm_year + 1900;
                    uint8_t mon = local.tm_mon + 1;

                    unsigned char timeStr[7];
                    timeStr[0] = (year >> 8) & 0xFF;
                    timeStr[1] = year & 0xFF;
                    timeStr[2] = mon & 0xFF;
                    timeStr[3] = local.tm_mday & 0xFF;
                    timeStr[4] = local.tm_hour & 0xFF;
                    timeStr[5] = local.tm_min & 0xFF;
                    timeStr[6] = local.tm_sec & 0xFF;

                    std::string timeCron = iDUtils::getTimeStampStr(timeStr, 7);
                    S_LOG_DEBUG("timeCron: " << timeCron);
                }

                catch (cron::bad_cronexpr const &ex)
                {
                    std::cerr << ex.what() << '\n';
                }
                if (!errorCode)
                    connection->_server._controlLayerInterface->addInspectionPlanning(uuid,
                                                                                      req->_cronExpression,
                                                                                      req->_deviceIdList,
                                                                                      req->_inspectionTypeList,
                                                                                      0);
                sendAddInspectionPlanningRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeleteInspectionPlanningReq_IDR:
            {
                IDR_DeleteInspectionPlanningReq *req = (IDR_DeleteInspectionPlanningReq *)protocol;
                S_LOG_INFO("deleteInspectionPlanningReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = deleteInspectionPlanning(req->_inspectionPlanningId);
                connection->_server._controlLayerInterface->deleteInspectionPlanning(req->_inspectionPlanningId);
                sendDeleteInspectionPlanningRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::QueryInspectionPlanningListReq_IDR :
            {
                IDR_QueryInspectionPlanningListReq *req = (IDR_QueryInspectionPlanningListReq *)protocol;
                S_LOG_INFO("QueryInspectionPlanningListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DevicePlanningTaskRecordList recordList;
                queryInspectionPlanningList(recordList);
                sendQueryInspectionPlanningListRes(connection, req->_seq, errorCode, recordList);
            }
                break;
            case IDR_Protocol::QueryInspectionDeviceListReq_IDR :
            {
                IDR_QueryInspectionDeviceListReq *req = (IDR_QueryInspectionDeviceListReq*)protocol;
                S_LOG_INFO("QueryInspectionDeviceListReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                int sum = 0;
                AbstractDb::DevicePlanningTaskDeviceRecordList recordList;
                queryInspectionDeviceList(req->_inspectionPlanningId, req->_startNum, req->_endNum, sum, recordList);
                sendQueryInspectionDeviceListRes(connection, req->_seq, errorCode, sum, recordList);
            }
                break;
            case IDR_Protocol::QueryInspectionRecordsReq_IDR :
            {
                IDR_QueryInspectionRecordsReq *req = (IDR_QueryInspectionRecordsReq*)protocol;
                S_LOG_INFO("QueryInspectionRecordsReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                int sum = 0;
                AbstractDb::DevicePlanningTaskLogRecordList recordList;
                queryInspectionRecordList(req->_inspectionPlanningId,
                                          req->_startNum,
                                          req->_endNum,
                                          sum,
                                          recordList);
                sendQueryInspectionRecordsRes(connection, req->_seq, errorCode, sum, recordList);
            }
                break;
            case IDR_Protocol::QueryInspectionResultsReq_IDR :
            {
                IDR_QueryInspectionResultsReq *req = (IDR_QueryInspectionResultsReq*)protocol;
                S_LOG_INFO("QueryInspectionResultsReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                AbstractDb::DevicePlanningTaskResultRecordList  recordList;
                int sum = 0;
                queryInspectionResultList(req->_recordId,
                                          req->_startNum,
                                          req->_endNum,
                                          sum,
                                          recordList);
                sendQueryInspectionResultsRes(connection, req->_seq, errorCode, sum, recordList);
            }
                break;
            case IDR_Protocol::UpdateInspectionPlanningStatusReq_IDR:
            {
                IDR_UpdateInspectionPlanningStatusReq *req = (IDR_UpdateInspectionPlanningStatusReq *)protocol;
                S_LOG_INFO("UpdatePatrolPlanningStatusReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                errorCode = updateInspectionPlanningStatus(req->_inspectionPlanningId, req->_status);
                sendUpdateInspectionPlanningStatusRes(connection, req->_seq, errorCode);
            }
                break;
            case IDR_Protocol::DeviceUpgradeConfigReq_IDR:
            {
                IDR_DeviceUpgradeConfigReq *req = (IDR_DeviceUpgradeConfigReq *)protocol;
                S_LOG_INFO("DeviceUpgradeConfigReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::string upgradeData = "";

                int ret = checkUpgradeData(req->_deviceId,
                                           req->_upgradeData,
                                           req->_upgradeDataMD5Num,
                                           upgradeData);
                if (!ret) {
                    connection->_server._controlLayerInterface->deviceUpgradeConfig(connection->_connectionId,
                                                                                    req->_seq,
                                                                                    req->_deviceId,
                                                                                    upgradeData,
                                                                                    boost::bind(&IDW_ConnectionServer::recvDeviceUpgradeConfigRes,
                                                                                                this,
                                                                                                _1,
                                                                                                _2,
                                                                                                _3));
                } else {
                    errorCode = IPOSS_STATUS_DEVICE_UPGRADE_FAILED;
                    recvDeviceUpgradeConfigRes(connection->_connectionId, req->_seq, errorCode);
                }
            }
                break;
            case IDR_Protocol::QueryDeviceOnlineEquipmentStatisticsReq_IDR :
            {
                IDR_QueryDeviceOnlineEquipmentStatisticsReq *req = (IDR_QueryDeviceOnlineEquipmentStatisticsReq*)protocol;
                S_LOG_INFO("QueryDeviceOnlineEquipmentStatisticsReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                int onlineNum = 0;
                int offlineNum = 0;
                queryDeviceOnlineEquipmentStatistics(onlineNum, offlineNum);
                sendQueryDeviceOnlineEquipmentStatisticsRes(connection, req->_seq, errorCode, onlineNum, offlineNum);
            }
                break;
            case IDR_Protocol::QueryDeviceFaultNumberReq_IDR :
            {
                IDR_QueryDeviceFaultNumberReq *req = (IDR_QueryDeviceFaultNumberReq*)protocol;
                S_LOG_INFO("QueryDeviceFaultNumberReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::vector<AbstractDb::DeviceAlarmMsgRecord::DeviceFaultNumber> deviceFaultNumberList;
                queryDeviceFaultNumberList(req->_startTime, req->_endTime, deviceFaultNumberList);
                sendQueryDeviceFaultNumberRes(connection, req->_seq, errorCode, deviceFaultNumberList);
            }
                break;
            case IDR_Protocol::QueryDeviceFaultNumberByGroupReq_IDR :
            {
                IDR_QueryDeviceFaultNumberByGroupReq *req = (IDR_QueryDeviceFaultNumberByGroupReq*)protocol;
                S_LOG_INFO("QueryDeviceFaultNumberByGroupReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::vector<AbstractDb::DeviceAlarmMsgRecord::DeviceFaultNumberByGroup> deviceFaultNumberByGroupList;
                queryDeviceFaultNumberByGroupList(req->_startTime, req->_endTime, deviceFaultNumberByGroupList);
                sendQueryDeviceFaultNumberByGroupRes(connection, req->_seq, errorCode, deviceFaultNumberByGroupList);
            }
                break;
            case IDR_Protocol::QueryDeviceFaultNumberByAlarmTypeReq_IDR :
            {
                IDR_QueryDeviceFaultNumberByAlarmTypeReq *req = (IDR_QueryDeviceFaultNumberByAlarmTypeReq*)protocol;
                S_LOG_INFO("QueryDeviceFaultNumberByAlarmTypeReq_IDR From " << connection->_remoteIpAddress << " : " << connection->_connectionId);
                std::vector<AbstractDb::DeviceAlarmMsgRecord::DeviceFaultNumberByAlarmType> deviceFaultNumberByAlarmTypeList;
                queryDeviceFaultNumberByAlarmTypeList(req->_startTime, req->_endTime, deviceFaultNumberByAlarmTypeList);
                sendQueryDeviceFaultNumberByAlarmTypeRes(connection, req->_seq, errorCode, deviceFaultNumberByAlarmTypeList);
            }
                break;
            default:
                break;
            }
        }
        if (protocol)
            delete protocol;
#endif
        //#]
    }



    void S_WS_ServerService::handleAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket s) {
        //#[ operation handleAccept(boost::beast::error_code,tcp::socket)
        if (!_acceptor.is_open()) {
            S_LOG_ERROR("accept is falied.");
            return;
        }
        if (!ec) {
            std::string connectId = Utils::getNewConnectionId();
            //S_LOG_DEBUG("start.");
            boost::system::error_code ec;

            boost::asio::ip::tcp::endpoint endpoint = s.remote_endpoint(ec);
            if (ec) {
                //S_LOG_ERROR("get remote endpoint Error: " << ec.message() << ".");
                //handleStop();
                return;
            }

            //S_LOG_DEBUG("_remoteIpAddress ******:" << endpoint.address().to_string() << ":" << endpoint.port() << ":" << connectId);

            connection_ptr newConnection(new S_WS_Connection(_ioc,
                                                             connectId,
                                                             shared_from_this(),
                                                             _connectionManager,
                                                             std::move(s)));
            newConnection->_remoteIpAddress = endpoint.address().to_string();
            newConnection->_remotePort = endpoint.port();
            newConnection->run();
        }
        if (ec != boost::asio::error::operation_aborted)
            startAccept();
        //#]
    }

    void S_WS_ServerService::handleStop() {
        //#[ operation handleStop()
        //S_LOG_DEBUG("handleStop.");
        boost::system::error_code ec;
        _acceptor.close(ec);
        //S_LOG_DEBUG("acceptor.close Error: " << ec.message() << ".");
        _connectionManager.stopAll();
        //#]
    }

    bool S_WS_ServerService::listen(std::string &ipAddress, unsigned short port) {
        //#[ operation listen()
        S_LOG_INFO("listen " << port);
        _ipAddress = ipAddress;
        _port = port;
        boost::asio::ip::tcp::endpoint endpoint;
        if (_ipAddress.empty()) {
            endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port);
        } else {
            boost::asio::ip::address addr;
            addr = boost::asio::ip::make_address(_ipAddress);
            endpoint = boost::asio::ip::tcp::endpoint(addr, _port);
        }
        try {
            _acceptor.open(endpoint.protocol());
            boost::asio::socket_base::reuse_address option(true);
            _acceptor.set_option(option);
            _acceptor.bind(endpoint);
            _acceptor.listen();
        } catch (std::exception &e) {
            S_LOG_ERROR("listen Error: " << e.what() << ".");
            return false;
        }
        startAccept();
        S_LOG_INFO("startAccept");
        return true;
        //#]
    }

    void S_WS_ServerService::readFromServer(READFROMSERVER readFromServer, void *pUser) {
        //#[ operation readFromServer(void*,READFROMSERVER)
        _pUserRead = pUser;
        _readFromServer = readFromServer;
        //#]
    }

    void S_WS_ServerService::sendMessage(std::string &msg) {
        //#[ operation sendMessage(std::string)
        _connectionManager.sendMessage(msg);
        //#]
    }

    void S_WS_ServerService::sendMessage(std::string &connectionId, std::string &msg) {
        //#[ operation sendMessage(std::string&,std::string&)
        connection_ptr connection = 0;
        _connectionManager.getConnectionById(connectionId, connection);
        if (connection)
            connection->send(msg);
        //#]
    }

    void S_WS_ServerService::startAccept() {
        //#[ operation startAccept()
        _acceptor.async_accept(
                boost::asio::make_strand(_ioc),
                boost::beast::bind_front_handler(
                        &S_WS_ServerService::handleAccept,
                        this));
        //#]
    }
}
/*********************************************************************
	File Path	: ../../src/WebSocketService/S_WS_Service.cpp
*********************************************************************/
