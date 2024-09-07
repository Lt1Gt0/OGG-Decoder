#pragma once
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <stdint.h>
#include <iostream>
#include "logger.hpp"

/**
 * Different Ogg Codec types that I know exist
 */
enum class Codec : int {
    Dirac,
    FLAC,
    Skeleton,
    Writ,
    Opus,
    Theora,
    Vorbis,
    Clet,
    CMML,
    JNG,
    Kate,
    MIDI,
    MNG,
    PCM,
    PNG,
    Speex,
    Yuv4Mpeg,
    Unknown
};

constexpr int CODEC_COUNT {17};

enum Severity : int {
    low,
    medium,
    high
};

inline void error(Severity severity)
{
    LOG_ERROR << "Exiting with severity: " << (int)severity << std::endl;
    std::cerr << '\n';
    std::exit((int)severity);
}

template<typename T, typename... Ts>
inline constexpr void error(Severity severity, T head, Ts... tail)
{
    std::cerr << head << " ";
    error(severity, tail...);
}
#endif // _COMMON_HPP_
