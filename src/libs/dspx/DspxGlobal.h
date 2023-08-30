#ifndef DSPXGLOBAL_H
#define DSPXGLOBAL_H

#include <QtGlobal>

#ifndef DSPXMODEL_EXPORT
#  ifdef DSPXMODEL_STATIC
#    define DSPXMODEL_EXPORT
#  else
#    ifdef DSPXMODEL_LIBRARY
#      define DSPXMODEL_EXPORT Q_DECL_EXPORT
#    else
#      define DSPXMODEL_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSPXGLOBAL_H
