#ifndef SCRIPT_MANAGER_SCRIPTMANAGERGLOBAL_H
#define SCRIPT_MANAGER_SCRIPTMANAGERGLOBAL_H

#include <QtGlobal>

#if defined(SCRIPT_MANAGER_LIBRARY)
#  define SCRIPT_MANAGER_EXPORT Q_DECL_EXPORT
#else
#  define SCRIPT_MANAGER_EXPORT Q_DECL_IMPORT
#endif

#endif // SCRIPT_MANAGER_SCRIPTMANAGERGLOBAL_H
