#ifndef SSDLL_GLOBAL_H
#define SSDLL_GLOBAL_H

#ifdef _WIN32
#  if defined(SSDLL_LIBRARY)
#    define SSDLLSHARED_EXPORT __declspec(dllexport)
#  else
#    define SSDLLSHARED_EXPORT __declspec(dllimport)
#  endif
#else
#  define SSDLLSHARED_EXPORT
#endif

#endif // SSDLL_GLOBAL_H
