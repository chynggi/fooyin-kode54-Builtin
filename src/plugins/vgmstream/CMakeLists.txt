include(${CMAKE_SOURCE_DIR}/3rdparty/vgmstream.cmake)

if(NOT TARGET vgmstream)
    message(STATUS "vgmstream not available; skipping vgmstream plugin.")
    return()
endif()

create_fooyin_plugin_internal(
    vgmstreaminput
    DEPENDS Fooyin::Core
            Fooyin::Gui
            vgmstream
    SOURCES vgmstreaminputplugin.cpp
            vgmstreaminputplugin.h
            vgmstreaminput.cpp
            vgmstreaminput.h
            vgmstreaminputdefs.h
            vgmstreaminputsettings.cpp
            vgmstreaminputsettings.h
)
