# g++ -o example example.cpp -L/usr/lib/x86_64-linux-gnu -lssh
cmake_minimum_required(VERSION 3.15)
project(sshexample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXE_LINKER_FLAGS "-L/usr/lib/x86_64-linux-gnu")


#[[add_executable(sshexample 
	src/example.cpp
	src/udsock.cpp
	)
target_link_libraries(sshexample PRIVATE ssh)]]


add_executable(ssl_server_client
    src/ssl_client.cpp)

if (DEFINED ENV{OPENSSL_DIR} OR DEFINED OPENSSL_DIR)
	message (STATUS "OPENSSL_DIR environment variable is set... great")
	message (STATUS "Dir: ${OPENSSL_DIR}")
	target_include_directories(ssl_server_client PRIVATE ${OPENSSL_DIR}/include)
	target_link_directories(ssl_server_client PRIVATE ${OPENSSL_DIR}/lib)
endif()

target_link_libraries(ssl_server_client PRIVATE 
    ssl
    crypto)



