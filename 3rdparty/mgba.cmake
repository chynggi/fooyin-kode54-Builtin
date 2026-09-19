if(TARGET mgba)
    return()
endif()

if(NOT FETCH_MGBA)
    message(STATUS "FETCH_MGBA=OFF; not fetching mGBA")
    return()
endif()

message(STATUS "Using 3rd-party mGBA")

function(setup_mgba)
    include(FetchContent)

    # The fork still declares cmake_minimum_required(VERSION 3.1), which CMake 4
    # rejects outright.
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

    # LIBMGBA_ONLY skips every frontend and tool, leaving just the core library.
    set(LIBMGBA_ONLY ON)
    set(SKIP_LIBRARY OFF)
    set(BUILD_SHARED OFF)
    set(BUILD_STATIC ON)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # kode54's gsfplayer branch exposes blip_buf.h and keeps the audio mixer in
    # minimal-core builds, which GSF playback needs.
    FetchContent_Declare(
        mgba
        GIT_REPOSITORY https://github.com/kode54/mgba.git
        GIT_TAG 183a2e5b74a443e48c3a89b26e3a74b8493ba347 EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(mgba)
endfunction()

setup_mgba()

if(TARGET mgba)
    set_property(TARGET mgba PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()
