#ifndef AUDIO_IAUDIOCONTEXTADDON_H
#define AUDIO_IAUDIOCONTEXTADDON_H

#include <QObject>

namespace Audio {

    class AudioContextInterfacePrivate;

    class IAudioContextAddOnPrivate;

    class IAudioContextAddOn : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IAudioContextAddOn)
        friend class AudioContextInterfacePrivate;
    public:
        ~IAudioContextAddOn() override;
        virtual void initialize() = 0;
        virtual void extensionInitialized() = 0;
        virtual bool delayedInitialize();

    protected:
        explicit IAudioContextAddOn(QObject *parent = nullptr);

    private:
        QScopedPointer<IAudioContextAddOnPrivate> d_ptr;
    };

} // Audio

#endif // AUDIO_IAUDIOCONTEXTADDON_H
