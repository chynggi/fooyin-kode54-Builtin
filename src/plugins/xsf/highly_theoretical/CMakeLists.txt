cmake_minimum_required(VERSION 3.14)

project(
    highly_theoretical
    VERSION 0.1.0
    DESCRIPTION "Sega Saturn and Dreamcast sound emulator library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/highly_theoretical"
    LANGUAGES C
)
set(target highly_theoretical)
add_library(${target} STATIC ${sources})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Core")

target_compile_definitions(${target} PRIVATE EMU_COMPILE EMU_LITTLE_ENDIAN HAVE_STDINT_H USE_M68K LSB_FIRST HAVE_MPROTECT)

target_sources(${target}
    PRIVATE
        Core/sega.c
        Core/sega.h
        Core/dcsound.c
        Core/dcsound.h
        Core/satsound.c
        Core/satsound.h
        Core/yam.c
        Core/yam.h
        Core/arm.c
        Core/arm.h
        Core/emuconfig.h
        Core/m68k/m68kops.c
        Core/m68k/m68kops.h
        Core/m68k/m68kcpu.c
        Core/m68k/m68kcpu.h
        Core/m68k/m68k.h
        Core/m68k/m68kconf.h
        Core/m68k/macros.h
)
