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

#endif // _COMMON_H
