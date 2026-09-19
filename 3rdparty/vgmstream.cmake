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

    # Upstream's shared target is Windows-only in practice (its export macro
    # expands to __declspec), so link the static archive instead.
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
    set(vgmstream_upstream_BINARY_DIR
        ${vgmstream_upstream_BINARY_DIR}
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

# libvgmstream is a static archive and upstream only links the codec libraries
# into its shared/CLI targets, so consumers must supply them. A plugin is a
# MODULE, which happily links with undefined symbols and then fails to dlopen,
# so getting this wrong is not caught at build time.
#
# CELT and ATRAC9 are IMPORTED targets private to vgmstream's directory, so
# reference the archives its sub-builds produce instead.
set(fooyin_vgmstream_deps "${vgmstream_upstream_BINARY_DIR}/dependencies")

foreach(
    fooyin_vgmstream_codec IN
    ITEMS "CELT_0061_MAKE;${fooyin_vgmstream_deps}/celt-0061/libcelt/.libs/libcelt.a"
          "CELT_0110_MAKE;${fooyin_vgmstream_deps}/celt-0110/libcelt/.libs/libcelt0.a"
          "ATRAC9_MAKE;${fooyin_vgmstream_deps}/LibAtrac9/bin/libatrac9.a"
)
    list(GET fooyin_vgmstream_codec 0 fooyin_vgmstream_maker)
    list(GET fooyin_vgmstream_codec 1 fooyin_vgmstream_archive)

    if(TARGET ${fooyin_vgmstream_maker})
        add_dependencies(libvgmstream ${fooyin_vgmstream_maker})
        target_link_libraries(vgmstream INTERFACE "${fooyin_vgmstream_archive}")
    endif()
endforeach()

# G.719 is built in-tree the first time and picked up as an IMPORTED target on
# later configures, so handle the archive as well as the global target.
if(TARGET g719_decode)
    target_link_libraries(vgmstream INTERFACE g719_decode)
elseif(EXISTS "${fooyin_vgmstream_deps}/libg719_decode/libg719_decode.a")
    target_link_libraries(vgmstream INTERFACE "${fooyin_vgmstream_deps}/libg719_decode/libg719_decode.a")
endif()

foreach(
    fooyin_vgmstream_lib IN
    ITEMS ${FFMPEG_LIBRARIES}
          ${MPG123_LIBRARIES}
          ${VORBISFILE_LIBRARY}
          ${VORBIS_LIBRARY}
          ${OGG_LIBRARY}
          ${SPEEX_LIBRARY}
)
    if(fooyin_vgmstream_lib)
        target_link_libraries(vgmstream INTERFACE ${fooyin_vgmstream_lib})
    endif()
endforeach()

target_link_libraries(vgmstream INTERFACE m)
