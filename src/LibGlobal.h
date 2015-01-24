#ifndef LIBGLOBAL_H
#define LIBGLOBAL_H

#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_LIBRZU)
	/* Building shared library. */
#   define LIB_EXTERN __declspec(dllexport)
# elif defined(USING_RZU_SHARED)
	/* Using shared library. */
#   define LIB_EXTERN __declspec(dllimport)
# else
	/* Building static library. */
#   define LIB_EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define LIB_EXTERN __attribute__((visibility("default")))
#else
# define LIB_EXTERN /* nothing */
#endif

#ifdef _MSC_VER
#    pragma warning(disable: 4251) /* class 'A' needs to have dll interface for to be used by clients of class 'B'. dll and users must use same crt dll and compiler flags. */
#    pragma warning(disable: 4200) /* nonstandard extension used : zero-sized array in struct/union */
#endif

#ifdef __GNUC__
#define PRINTFCHECK(formatidx, argsidx) __attribute__((format (printf, formatidx, argsidx)))
#else
#define PRINTFCHECK(formatidx, argsidx)
#endif

#endif // LIBGLOBAL_H
