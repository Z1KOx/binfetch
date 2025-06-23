cmake_minimum_required(VERSION 3.16)
project(binfetch
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if (MSVC)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "Use Windows native SSL" FORCE)
else()
    set(CURL_USE_SCHANNEL OFF CACHE BOOL "Disable Windows native SSL" FORCE)
endif()

include(FetchContent)
function(fetch_dep name repo tag)
    message(STATUS "Fetching ${name} from ${repo} (tag ${tag})")
    FetchContent_Declare(
            ${name}
            GIT_REPOSITORY ${repo}
            GIT_TAG ${tag}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(${name})
endfunction()

fetch_dep(
        curl
        https://github.com/curl/curl.git
        curl-8_6_0
)
fetch_dep(
        nlohmann_json
        https://github.com/nlohmann/json.git
        v3.11.2
)

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
    target_link_libraries(binfetch PRIVATE pthread dl)
    target_compile_options(binfetch
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
    )
endif()