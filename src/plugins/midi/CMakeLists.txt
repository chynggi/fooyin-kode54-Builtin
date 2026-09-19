include(${CMAKE_SOURCE_DIR}/3rdparty/spessasynth.cmake)

if(NOT TARGET spessasynth)
    message(STATUS "SpessaSynth not available; skipping midi plugin.")
    return()
endif()

create_fooyin_plugin_internal(
    midiinput
    DEPENDS Fooyin::Core
            Fooyin::Gui
            spessasynth
    SOURCES midiinputplugin.cpp
            midiinputplugin.h
            midiinput.cpp
            midiinput.h
            midiinputdefs.h
            midiinputsettings.cpp
            midiinputsettings.h
            SpessaPlayer.cpp
            SpessaPlayer.h
            MIDIPlayer.cpp
            MIDIPlayer.h
)

fooyin_append_mimes(audio/midi)
