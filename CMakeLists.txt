cmake_minimum_required(VERSION 3.22.1)
project(NovaCore C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED libwebsockets)
pkg_check_modules(JSONC REQUIRED json-c)
find_package(OpenSSL REQUIRED)


include_directories(
    ${LWS_INCLUDE_DIRS}
    ${JSONC_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

link_directories(
    ${LWS_LIBRARY_DIRS}
    ${JSONC_LIBRARY_DIRS}
)

add_executable(NovaCore src/main.c)

target_link_libraries(NovaCore
    ${LWS_LIBRARIES}
    ${JSONC_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    curl
)