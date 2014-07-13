#ifndef RAPPELZLIB_GLOBAL_H
#define RAPPELZLIB_GLOBAL_H

#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_RAPPELZLIB)
	/* Building shared library. */
#   define RAPPELZLIB_EXTERN __declspec(dllexport)
# elif defined(USING_RAPPELZLIB)
	/* Using shared library. */
#   define RAPPELZLIB_EXTERN __declspec(dllimport)
# else
	/* Building static library. */
#   define RAPPELZLIB_EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define RAPPELZLIB_EXTERN __attribute__((visibility("default")))
#else
# define RAPPELZLIB_EXTERN /* nothing */
#endif

#ifdef _MSC_VER
#    pragma warning(disable: 4251) /* class 'A' needs to have dll interface for to be used by clients of class 'B'. dll and users must use same crt dll and compiler flags. */
#endif

#ifdef __GNUC__
#define PRINTFCHECK(formatidx, argsidx) __attribute__((format (printf, formatidx, argsidx)))
#else
#define PRINTFCHECK(formatidx, argsidx)
#endif

#endif // RAPPELZLIB_GLOBAL_H
