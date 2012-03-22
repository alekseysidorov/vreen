#ifndef API_GLOBAL_H
#define API_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VK_LIBRARY)
#  define VK_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define VK_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // API_GLOBAL_H
