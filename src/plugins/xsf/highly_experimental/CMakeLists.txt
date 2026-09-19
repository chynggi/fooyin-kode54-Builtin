cmake_minimum_required(VERSION 3.14)

project(
    highly_experimental
    VERSION 0.1.0
    DESCRIPTION "PS1 and PS2 sound emulator library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/highly_experimental"
    LANGUAGES C
)
set(target highly_experimental)
add_library(${target} STATIC ${sources})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Core")

target_compile_definitions(${target} PRIVATE EMU_COMPILE EMU_LITTLE_ENDIAN HAVE_STDINT_H)

target_sources(${target}
    PRIVATE
        Core/psx.c
        Core/psx.h
        Core/ioptimer.c
        Core/ioptimer.h
        Core/iop.c
        Core/iop.h
        Core/bios.c
        Core/bios.h
        Core/r3000dis.c
        Core/r3000dis.h
        Core/r3000asm.c
        Core/r3000asm.h
        Core/r3000.c
        Core/r3000.h
        Core/vfs.c
        Core/vfs.h
        Core/spucore.c
        Core/spucore.h
        Core/spu.c
        Core/spu.h
        Core/mkhebios.c
        Core/mkhebios.h
        Core/mkhebios_overlays.h
        Core/mkhebios_scripts.h
        Core/emuconfig.h
)
