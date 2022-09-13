cmake_minimum_required(VERSION 3.0.0)
project(MicrochipSmartHub VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(spdlog CONFIG REQUIRED)

find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set (SRC_PATH
                main.cpp 
                usb_smart_hub_manager.cpp
                i2c_smart_hub_manager.cpp
                http_client.cpp
                log/logger.cpp
                )

add_executable(${PROJECT_NAME} ${SRC_PATH})

target_link_libraries(${PROJECT_NAME} PRIVATE usb-1.0)
target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog::spdlog
    )
target_include_directories(${PROJECT_NAME} PRIVATE ${CPP_HTTPLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL) 
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
