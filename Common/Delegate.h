#ifndef DELEGATE_H
#define DELEGATE_H

#include <functional>
#if !(__cplusplus > 199711L || __GXX_EXPERIMENTAL_CXX0X__ || _MSC_VER >= 1600 || defined( _LIBCPP_VERSION ))
#define USE_TR1
#endif

#ifdef USE_TR1
#  ifdef __GNUC__
#    include <tr1/functional>
#  endif
namespace cpp11x = std::tr1;
#else
namespace cpp11x = std;
#endif

#endif // DELEGATE_H
