cmake_minimum_required(VERSION 3.14)

project(
    sseqplayer
    VERSION 0.1.0
    DESCRIPTION "Nintendo DS sequence player library"
    HOMEPAGE_URL "https://bitbucket.org/losnoco/sseqplayer"
    LANGUAGES CXX
)
set(target sseqplayer)
add_library(${target} STATIC ${sources})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_sources(${target}
    PRIVATE
        Channel.cpp
        Channel.h
        codecvt.h
        common.h
        consts.h
        convert.h
        FATSection.cpp
        FATSection.h
        INFOEntry.cpp
        INFOEntry.h
        INFOSection.cpp
        INFOSection.h
        NDSStdHeader.cpp
        NDSStdHeader.h
        Player.cpp
        Player.h
        SBNK.cpp
        SBNK.h
        SDAT.cpp
        SDAT.h
        SSEQ.cpp
        SSEQ.h
        SWAR.cpp
        SWAR.h
        SWAV.cpp
        SWAV.h
        SYMBSection.cpp
        SYMBSection.h
        Track.cpp
        Track.h
        wstring_convert.h
)
