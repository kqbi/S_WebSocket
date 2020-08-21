INCLUDE_DIRECTORIES("../../include")
INCLUDE_DIRECTORIES("../../src/WebSocketServer/test")

aux_source_directory(../../src/WebSocketServer/test SERVERTEST_DIR)

if (WIN32)
    ADD_DEFINITIONS("-DWIN32_LEAN_AND_MEAN")
endif (WIN32)

add_executable(ServerTest ${SERVERTEST_DIR})

if (WIN32)
    target_link_libraries(ServerTest ${CMAKE_PROJECT_NAME}Server_static ${LINK_LIB_LIST} Winmm)
else (WIN32)
    target_link_libraries(ServerTest ${CMAKE_PROJECT_NAME}Server_static ${LINK_LIB_LIST} uuid pthread dl stdc++ m crypt)
endif (WIN32)
