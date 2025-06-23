cmake_minimum_required(VERSION 3.16)
project(binfetch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
if (MSVC)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
else()
    set(CURL_USE_SCHANNEL OFF CACHE BOOL "" FORCE)
endif()

include(FetchContent)

FetchContent_Declare(
        curl
        GIT_REPOSITORY "https://github.com/curl/curl.git"
        GIT_TAG        "curl-8_6_0"
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(curl)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG        "v3.11.2"
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(binfetch
        src/main.cpp
        src/downloader.cpp
        src/vtscanner.cpp
)

target_precompile_headers(binfetch
        PRIVATE
            src/pch.h
)

target_link_libraries(binfetch
        PRIVATE
            CURL::libcurl_static
            nlohmann_json::nlohmann_json
)
if (UNIX)
    target_link_libraries(binfetch
            PRIVATE
                pthread dl
    )
    target_compile_options(binfetch
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
    )
endif()