#pragma once
#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

/* TODO
 * Across every file, make sure that the way the byte ordering is handled
 * is consistant to reduce confusion in the future
 */

// Vorbis spec refers to bytes as octets alot so this is to make it more clear
typedef uint8_t byte;
typedef byte octet;

#endif // _COMMON_H
