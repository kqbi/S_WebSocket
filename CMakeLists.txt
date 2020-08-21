project(S_WebSocket)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE "Release")
set (BUILD_USE_64BITS on)
set(platform x64)

set(LINK_LIB_LIST)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(system linux)
    #set(THIRDPARTY "/home/vmns/thirdparty/Linux")
    set(BOOST_DATE_TIME_LIB libboost_date_time.a)
    set(BOOST_THREAD_LIB libboost_thread.a)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(system win)
    set(THIRDPARTY "D:/work/projects/3rdparty")
    set(BOOST_DATE_TIME_LIB libboost_date_time-vc141-mt-x64-1_72.lib)
    set(BOOST_THREAD_LIB libboost_thread-vc141-mt-x64-1_72.lib)
    set(OXF_LIB oxf-md.lib)
endif ()

#加载自定义模块
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#设置库文件路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#设置可执行程序路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#设置子目录

#安装目录
set(INSTALL_PATH_LIB ${THIRDPARTY}/${CMAKE_PROJECT_NAME}/lib/${platform})
set(INSTALL_PATH_INCLUDE ${THIRDPARTY}/${CMAKE_PROJECT_NAME}/include)

#安装头文件至系统目录
install(DIRECTORY src DESTINATION ${INSTALL_PATH_INCLUDE} FILES_MATCHING PATTERN "*.h")

#oxf
set(OXF_ROOT_PATH ${THIRDPARTY}/oxf)
set(OXF_INCLUDE_PATH ${OXF_ROOT_PATH}/include)
set(OXF_LIB_PATH ${OXF_ROOT_PATH}/lib/${system}/${platform}/${CMAKE_BUILD_TYPE})
INCLUDE_DIRECTORIES(${OXF_INCLUDE_PATH})
link_directories(${OXF_LIB_PATH})
list(APPEND LINK_LIB_LIST ${OXF_LIB})

#boost
#set(BOOST_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/boost)
set(BOOST_ROOT_PATH ${THIRDPARTY}/boost)
set(BOOST_INCLUDE_PATH ${BOOST_ROOT_PATH}/include)
set(BOOST_LIB_PATH ${BOOST_ROOT_PATH}/lib/${system}/${platform})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_PATH})
link_directories(${BOOST_LIB_PATH})
list(APPEND LINK_LIB_LIST ${BOOST_DATE_TIME_LIB} ${BOOST_THREAD_LIB})

if (WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} /MDd /Od /ZI /DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} /MD /O2")
else (WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")
endif (WIN32)

if (WIN32)
    #DLL
    #ADD_DEFINITIONS("-DS_HTTP_SERVICE_DLL")
    #ADD_DEFINITIONS("-DS_HTTP_SERVICE_API_IMPORTS")
    ADD_DEFINITIONS("-DWIN32_LEAN_AND_MEAN")
endif (WIN32)

add_subdirectory(wsClient)
add_subdirectory(wsServer)
