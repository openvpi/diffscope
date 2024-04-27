#ifndef AUDIO_IAUDIO_H
#define AUDIO_IAUDIO_H

#include <audioglobal.h>

#include <QObject>

namespace Audio {

    class AudioPlugin;

    class OutputSystemInterface;

    class IAudioPrivate;

    class AUDIO_EXPORT IAudio : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IAudio)
        friend class AudioPlugin;
    public:
        ~IAudio() override;
        static IAudio *instance();

        OutputSystemInterface *outputSystemInterface() const;
        OutputSystemInterface *vstOutputSystemInterface() const;

    private:
        explicit IAudio(QObject *parent = nullptr);
        QScopedPointer<IAudioPrivate> d_ptr;
    };

}

#endif // AUDIO_IAUDIO_H
