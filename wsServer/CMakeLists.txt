INCLUDE_DIRECTORIES("../include")
INCLUDE_DIRECTORIES("../src/WebSocketMsg")
INCLUDE_DIRECTORIES("../src/WebSocketServer")
INCLUDE_DIRECTORIES("../src/Utils")

aux_source_directory(../src/WebSocketMsg MSG_DIR)
aux_source_directory(../src/WebSocketServer SERVER_DIR)
aux_source_directory(../src/Utils UTILS_DIR)

if (WIN32)
    ADD_DEFINITIONS("-DS_WS_SERVER_USE_STATIC_LIB")
endif (WIN32)

add_library(${CMAKE_PROJECT_NAME}Server_static STATIC ${MSG_DIR} ${SERVER_DIR} ${UTILS_DIR})

set_target_properties(${CMAKE_PROJECT_NAME}Server_static PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}Server")
#安装静态库至系统目录
install(TARGETS ${CMAKE_PROJECT_NAME}Server_static ARCHIVE DESTINATION ${INSTALL_PATH_LIB})

add_subdirectory(test)
