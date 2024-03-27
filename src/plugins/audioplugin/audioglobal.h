#ifndef AUDIO_AUDIOGLOBAL_H
#define AUDIO_AUDIOGLOBAL_H

#include <QtGlobal>

#if defined(AUDIO_LIBRARY)
#  define AUDIO_EXPORT Q_DECL_EXPORT
#else
#  define AUDIO_EXPORT Q_DECL_IMPORT
#endif

#endif // AUDIO_AUDIOGLOBAL_H
