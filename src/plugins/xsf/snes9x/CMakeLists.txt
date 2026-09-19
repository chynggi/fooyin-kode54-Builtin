cmake_minimum_required(VERSION 3.14)

project(
    snes9x
    VERSION 0.1.0
    DESCRIPTION "Super Nintendo sound emulator library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/libsnes9x"
    LANGUAGES CXX
)
set(target snes9x)
add_library(${target} STATIC ${sources})
set_property(TARGET snes9x PROPERTY CXX_STANDARD 17)

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/snes9x")

target_sources(${target}
    PRIVATE
        src/cpu.cpp
        src/cpuaddr.h
        src/cpuexec.cpp
        src/cpumacro.h
        src/cpuops.cpp
        src/cpuops.h
        src/dma.cpp
        src/getset.cpp
        src/globals.cpp
        src/memmap.cpp
        src/ppu.cpp
        src/sdd1.cpp
        src/sdd1.h
        src/apu/apu.cpp
        src/apu/bapu/dsp/SPC_DSP.cpp
        src/apu/bapu/dsp/sdsp.cpp
        src/apu/bapu/smp/algorithms.cpp
        src/apu/bapu/smp/core.cpp
        src/apu/bapu/smp/iplrom.cpp
        src/apu/bapu/smp/memory.cpp
        src/apu/bapu/smp/smp.cpp
        src/apu/bapu/smp/timing.cpp
        include/snes9x/snes9x.h
        include/snes9x/port.h
        include/snes9x/windowsh_wrapper.h
        include/snes9x/memmap.h
        include/snes9x/getset.h
        include/snes9x/65c816.h
        include/snes9x/cpuexec.h
        include/snes9x/dma.h
        include/snes9x/ppu.h
        include/snes9x/apu/apu.h
        include/snes9x/apu/resampler.h
        include/snes9x/apu/bapu/snes/snes.hpp
        include/snes9x/apu/bapu/dsp/sdsp.hpp
        include/snes9x/apu/bapu/dsp/SPC_DSP.h
        include/snes9x/apu/bapu/dsp/blargg_common.h
        include/snes9x/apu/bapu/dsp/blargg_config.h
        include/snes9x/apu/bapu/dsp/blargg_endian.h
)
