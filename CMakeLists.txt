# g++ -o example example.cpp -L/usr/lib/x86_64-linux-gnu -lssh
cmake_minimum_required(VERSION 3.15)
project(sshexample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_EXE_LINKER_FLAGS "-L/usr/lib/x86_64-linux-gnu")


#[[add_executable(sshexample 
	src/example.cpp
	src/udsock.cpp
	)
target_link_libraries(sshexample PRIVATE ssh)]]


set(sources_ssl_layer 
    src/ssl_client.cpp
    src/ssl_server.cpp
    src/ssl_common.cpp 
    src/ssl_common.h
    )


add_executable(example_fork_nonssl
    src/ssl_example_fork_nonssl.cpp 
    ${sources_ssl_layer}
)
add_executable(example_fork
    src/ssl_example_fork.cpp 
    ${sources_ssl_layer})





if (APPLE)
    message(STATUS "Apple found")
else()
    message(STATUS "Non Apple Found")
endif()

if (DEFINED ENV{OPENSSL_DIR} OR DEFINED OPENSSL_DIR)
	message (STATUS "OPENSSL_DIR environment variable is set... great")
	message (STATUS "Dir: ${OPENSSL_DIR}")
    if (UNIX AND NOT APPLE)
        target_link_directories(example_fork_nonssl PRIVATE ${OPENSSL_DIR}/lib)
        target_include_directories(example_fork_nonssl PRIVATE /usr/include)
        target_link_directories(example_fork PRIVATE ${OPENSSL_DIR}/lib)
        target_include_directories(example_fork PRIVATE /usr/include)
    elseif(APPLE)
        target_link_directories(example_fork_nonssl PRIVATE ${OPENSSL_DIR}/lib)
        target_include_directories(example_fork_nonssl PRIVATE ${OPENSSL_DIR}/include)
        target_link_directories(example_fork PRIVATE ${OPENSSL_DIR}/lib)
        target_include_directories(example_fork PRIVATE ${OPENSSL_DIR}/include)
    endif()
else()
    message (STATUS "No OPENSSL_DIR environment variable set.")

    if (WINDOWS)
    elseif(UNIX AND NOT APPLE)
        message(FATAL_ERROR "Unix detected: Do:\n\n\tdpkg -L libssl-dev |grep lib\n\nand export the variable with the path to the folder containing libcrypto.a and libssl.a, into OPENSSL_DIR")
    elseif(APPLE)
        message(FATAL_ERROR "Apple detected: Do:\n\n\tbrew ls openssl@1.1\n\nand export the variable with the path to the folder containing both include and lib folders, into OPENSSL_DIR")
    endif()
endif()

target_link_libraries(example_fork PRIVATE 
    ssl
    crypto)
target_link_libraries(example_fork_nonssl PRIVATE 
    ssl
    crypto)
























