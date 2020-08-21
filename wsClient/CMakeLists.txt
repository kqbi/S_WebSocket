INCLUDE_DIRECTORIES("../include")
INCLUDE_DIRECTORIES("../src/WebSocketMsg")
INCLUDE_DIRECTORIES("../src/WebSocketClient")
INCLUDE_DIRECTORIES("../src/Utils")

aux_source_directory(../src/WebSocketMsg MSG_DIR)
aux_source_directory(../src/WebSocketClient CLIENT_DIR)
aux_source_directory(../src/Utils UTILS_DIR)
if (WIN32)
    ADD_DEFINITIONS("-DS_WS_CLIENT_USE_STATIC_LIB")
endif (WIN32)

add_library(${CMAKE_PROJECT_NAME}Client_static STATIC ${MSG_DIR} ${CLIENT_DIR} ${UTILS_DIR})

set_target_properties(${CMAKE_PROJECT_NAME}Client_static PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}Client")
#安装静态库至系统目录
install(TARGETS ${CMAKE_PROJECT_NAME}Client_static ARCHIVE DESTINATION ${INSTALL_PATH_LIB})

add_subdirectory(test)