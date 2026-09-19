if(TARGET vgmstream)
    return()
endif()

if(NOT FETCH_VGMSTREAM)
    message(STATUS "FETCH_VGMSTREAM=OFF; not fetching vgmstream")
    return()
endif()

message(STATUS "Using 3rd-party vgmstream")

function(setup_vgmstream)
    include(FetchContent)

    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_STATIC OFF)
    set(BUILD_CLI OFF)
    set(BUILD_V123 OFF)
    set(BUILD_AUDACIOUS OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    FetchContent_Declare(
        vgmstream_upstream
        GIT_REPOSITORY https://github.com/vgmstream/vgmstream.git
        GIT_TAG r2117
        GIT_SHALLOW TRUE EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(vgmstream_upstream)

    set(vgmstream_upstream_SOURCE_DIR
        ${vgmstream_upstream_SOURCE_DIR}
        PARENT_SCOPE
    )
endfunction()

setup_vgmstream()

if(NOT TARGET libvgmstream)
    message(WARNING "vgmstream was fetched but the libvgmstream target is missing")
    return()
endif()

set_property(TARGET libvgmstream PROPERTY POSITION_INDEPENDENT_CODE ON)

# The plugin includes <vgmstream/libvgmstream.h>, matching vgmstream's installed
# header layout. Mirror that layout in the build tree.
set(fooyin_vgmstream_include "${CMAKE_BINARY_DIR}/3rdparty/vgmstream-include")
file(
    COPY "${vgmstream_upstream_SOURCE_DIR}/src/libvgmstream.h" "${vgmstream_upstream_SOURCE_DIR}/src/libvgmstream_streamfile.h"
    DESTINATION "${fooyin_vgmstream_include}/vgmstream"
)

add_library(vgmstream INTERFACE)
target_link_libraries(vgmstream INTERFACE libvgmstream)
target_include_directories(vgmstream SYSTEM INTERFACE "${fooyin_vgmstream_include}")
