#include "LibGlobal.h"

/* Windows - set up dll import/export decorators. */
#if defined(BUILDING_LIBRZU)
	/* Building shared library. */
#  define RZU_EXTERN SYMBOL_EXPORT
#elif defined(USING_RZU_SHARED)
	/* Using shared library. */
#  define RZU_EXTERN SYMBOL_IMPORT
#else
	/* Building static library. */
#  define RZU_EXTERN /* nothing */
#endif
