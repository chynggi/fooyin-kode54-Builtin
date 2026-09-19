if(TARGET spessasynth)
    return()
endif()

if(NOT FETCH_SPESSASYNTH)
    message(STATUS "FETCH_SPESSASYNTH=OFF; not fetching SpessaSynth")
    return()
endif()

message(STATUS "Using 3rd-party SpessaSynth")

function(setup_spessasynth)
    include(FetchContent)

    set(BUILD_SHARED_LIBS OFF)
    set(SS_BUILD_SHARED OFF)
    set(SS_BUILD_EXAMPLES OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    FetchContent_Declare(
        spessasynth
        GIT_REPOSITORY https://github.com/kode54/spessasynth_core_c.git
        GIT_TAG 99fc352d8bb38851cb9b13c36eb8eb7a98903c2b
        SOURCE_SUBDIR spessasynth_core EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(spessasynth)
endfunction()

setup_spessasynth()
