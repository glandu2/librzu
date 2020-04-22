#pragma once

#if defined(_MSC_VER) && _MSC_VER < 1900
#define PRId8 "d"
#define PRId16 "d"
#define PRId32 "I32d"
#define PRId64 "I64d"
#define PRIdS "Id"

#define PRIu8 "u"
#define PRIu16 "u"
#define PRIu32 "I32u"
#define PRIu64 "I64u"
#define PRIuS "Iu"

#define PRIx8 "x"
#define PRIx16 "x"
#define PRIx32 "I32x"
#define PRIx64 "I64x"
#define PRIxS "Ix"
#else
#include <inttypes.h>
#ifndef PRIdS
#define PRIdS "zd"
#endif
#ifndef PRIuS
#define PRIuS "zu"
#endif
#endif

