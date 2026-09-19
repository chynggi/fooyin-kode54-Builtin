cmake_minimum_required(VERSION 3.14)

project(
    highly_quixotic
    VERSION 0.1.0
    DESCRIPTION "QSound arcade sound emulator library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/highly_quixotic"
    LANGUAGES C
)
set(target highly_quixotic)
add_library(${target} STATIC ${sources})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Core")

target_compile_definitions(${target} PRIVATE EMU_COMPILE EMU_LITTLE_ENDIAN HAVE_STDINT_H)

target_sources(${target}
    PRIVATE
        Core/qsound_ctr.c
        Core/qsound_ctr.h
        Core/qsound.c
        Core/qsound.h
        Core/qmix.c
        Core/qmix.h
        Core/kabuki.c
        Core/kabuki.h
        Core/z80.c
        Core/z80.h
        Core/emuconfig.h
)
