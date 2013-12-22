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

#endif // RAPPELZLIB_GLOBAL_H
