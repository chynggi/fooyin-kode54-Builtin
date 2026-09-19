cmake_minimum_required(VERSION 3.14)

project(
    vio2sf
    VERSION 0.1.0
    DESCRIPTION "Nintendo DS sound emulator library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/vio2sf"
    LANGUAGES C CXX
)
set(target vio2sf)
add_library(${target} STATIC ${sources})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/vio2sf/desmume")

target_sources(${target}
    PRIVATE
        src/vio2sf/desmume/arm_instructions.c
        src/vio2sf/desmume/arm_instructions.h
        src/vio2sf/desmume/ARM9.h
        src/vio2sf/desmume/armcpu.c
        src/vio2sf/desmume/armcpu.h
        src/vio2sf/desmume/barray.c
        src/vio2sf/desmume/barray.h
        src/vio2sf/desmume/bios.c
        src/vio2sf/desmume/bios.h
        src/vio2sf/desmume/bits.h
        src/vio2sf/desmume/config.h
        src/vio2sf/desmume/cp15.c
        src/vio2sf/desmume/cp15.h
        src/vio2sf/desmume/debug.h
        src/vio2sf/desmume/dscard.h
        src/vio2sf/desmume/FIFO.c
        src/vio2sf/desmume/FIFO.h
        src/vio2sf/desmume/GPU.c
        src/vio2sf/desmume/GPU.h
        src/vio2sf/desmume/isqrt.c
        src/vio2sf/desmume/isqrt.h
        src/vio2sf/desmume/matrix.c
        src/vio2sf/desmume/matrix.h
        src/vio2sf/desmume/mc.c
        src/vio2sf/desmume/mc.h
        src/vio2sf/desmume/mem.h
        src/vio2sf/desmume/MMU.c
        src/vio2sf/desmume/MMU.h
        src/vio2sf/desmume/NDSSystem.c
        src/vio2sf/desmume/NDSSystem.h
        src/vio2sf/desmume/registers.h
        src/vio2sf/desmume/resampler.c
        src/vio2sf/desmume/resampler.h
        src/vio2sf/desmume/spu_exports.h
        src/vio2sf/desmume/SPU.cpp
        src/vio2sf/desmume/SPU.h
        src/vio2sf/desmume/state.c
        src/vio2sf/desmume/state.h
        src/vio2sf/desmume/thumb_instructions.c
        src/vio2sf/desmume/thumb_instructions.h
        src/vio2sf/desmume/types.h
)
