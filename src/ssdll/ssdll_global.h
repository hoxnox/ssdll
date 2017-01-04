#ifndef SSDLL_GLOBAL_H
#define SSDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SSDLL_LIBRARY)
#  define SSDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SSDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SSDLL_GLOBAL_H
