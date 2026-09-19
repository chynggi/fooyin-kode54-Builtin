include(${CMAKE_SOURCE_DIR}/3rdparty/mgba.cmake)

if(NOT TARGET mgba)
    message(STATUS "mGBA not available; skipping xsf plugin.")
    return()
endif()

find_package(ZLIB QUIET)

if(NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found; skipping xsf plugin.")
    return()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(psflib)
add_subdirectory(highly_experimental)
add_subdirectory(highly_theoretical)
add_subdirectory(highly_quixotic)
add_subdirectory(lazyusf2)
add_subdirectory(vio2sf)
add_subdirectory(sseqplayer)
add_subdirectory(snes9x)

create_fooyin_plugin_internal(
    xsfinput
    DEPENDS Fooyin::Core
            Fooyin::Gui
            psflib
            highly_experimental
            highly_theoretical
            highly_quixotic
            lazyusf2
            vio2sf
            sseqplayer
            snes9x
            mgba
            ZLIB::ZLIB
    SOURCES xsfinputplugin.cpp
            xsfinputplugin.h
            xsfinput.cpp
            xsfinput.h
            xsfinputdefs.h
            xsfinputsettings.cpp
            xsfinputsettings.h
            circular_buffer.h
)

fooyin_append_mimes(
    audio/x-psf
    audio/x-minipsf
    audio/x-psflib
)
