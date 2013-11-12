#ifndef RAPPELZLIB_GLOBAL_H
#define RAPPELZLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RAPPELZLIB_LIBRARY)
#  define RAPPELZLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RAPPELZLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(WIN32) && !defined(WIN64)
#  define IFACECALLCONV __stdcall
#else
#  define IFACECALLCONV
#endif

#endif // RAPPELZLIB_GLOBAL_H
