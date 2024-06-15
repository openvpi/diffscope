#ifndef DEBUG_DEBUGGLOBAL_H
#define DEBUG_DEBUGGLOBAL_H

#include <QtGlobal>

#if defined(DEBUG_LIBRARY)
#  define DEBUG_EXPORT Q_DECL_EXPORT
#else
#  define DEBUG_EXPORT Q_DECL_IMPORT
#endif

#endif // DEBUG_DEBUGGLOBAL_H
