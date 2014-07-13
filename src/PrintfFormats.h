#ifndef PRINTFFORMATS_H
#define PRINTFFORMATS_H

#ifdef _MSC_VER
# define PRId8		"d"
# define PRId16		"d"
# define PRId32		"I32d"
# define PRId64		"I64d"
# define PRIdS		"Id"

# define PRIu8		"u"
# define PRIu16		"u"
# define PRIu32		"I32u"
# define PRIu64		"I64u"
# define PRIuS		"Iu"
#else
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#define PRIdS		"zd"
#define PRIuS		"zu"
#endif

#endif // PRINTFFORMATS_H
