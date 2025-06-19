cmake_minimum_required(VERSION 3.15)
project(binfetch)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS    OFF CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL     ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
        curl
            GIT_REPOSITORY "https://github.com/curl/curl.git"
            GIT_TAG        "curl-8_6_0"
            GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(curl)

add_executable(binfetch src/main.cpp)
target_link_libraries(binfetch PRIVATE CURL::libcurl_static)
