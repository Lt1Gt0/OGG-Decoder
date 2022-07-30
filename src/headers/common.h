#pragma once
#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <iostream>
#include "Debug/logger.h"

enum Severity : int {
    low,
    medium,
    high
};

inline void error(int severity)
{
    LOG_ERROR << "Exiting with severity: " << (int)severity << std::endl;
    std::cerr << '\n';
    std::exit(severity);
}

template<typename T, typename... Ts>
inline constexpr void error(int severity, T head, Ts... tail)
{
    std::cerr << head << " ";
    error(severity, tail...);
}

/* TODO
 * Across every file, make sure that the way the byte ordering is handled
 * is consistant to reduce confusion in the future
 */

// Vorbis spec refers to bytes as octets alot so this is to make it more clear
typedef uint8_t byte;
typedef byte octet;

#endif // _COMMON_H
