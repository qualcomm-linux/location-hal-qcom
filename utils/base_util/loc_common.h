/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

GENERAL DESCRIPTION
  Location Common definitions, macro's

Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause-Clear
=============================================================================*/

#ifndef LOC_COMMON_H
#define LOC_COMMON_H

#include <cstdint>

typedef  uint64_t   uint64;
typedef  int64_t    int64;
typedef  uint32_t   uint32;
typedef  uint16_t   uint16;
typedef  uint8_t    uint8;
typedef  int32_t    int32;
typedef  int16_t    int16;
typedef  int8_t     int8;

typedef  uint8_t    boolean;

#ifndef NULL
  #define NULL  0
#endif

#if defined __GNUC__
#define PACK(x)       x __attribute__((__packed__))
#elif defined __arm
#define PACK(x)       __packed x
#else
#error No PACK() macro defined for this compiler
#endif

#endif // #ifndef LOC_COMMON_H
