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
        explicit IAudio(QObject *parent = nullptr);
        ~IAudio() override;
        static IAudio *instance();

        OutputSystemInterface *outputSystemInterface() const;
        OutputSystemInterface *vstOutputSystemInterface() const;
        void installOutputSystemAddOn(const QMetaObject *clazz);
        QList<const QMetaObject *> outputSystemAddons() const;

    private:
        QScopedPointer<IAudioPrivate> d_ptr;
    };

} // Audio

#endif // AUDIO_IAUDIO_H
