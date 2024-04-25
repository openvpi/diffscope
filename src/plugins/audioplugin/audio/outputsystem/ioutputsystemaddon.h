#ifndef AUDIO_IOUTPUTSYSTEMADDON_H
#define AUDIO_IOUTPUTSYSTEMADDON_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class AudioSource;
}

namespace Audio {

    class OutputSystemInterface;
    class OutputSystemInterfacePrivate;

    class IOutputSystemAddOnPrivate;

    class AUDIO_EXPORT IOutputSystemAddOn : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IOutputSystemAddOn)
        friend class OutputSystemInterfacePrivate;
    public:
        ~IOutputSystemAddOn() override;

        virtual void initialize() = 0;
        virtual void extensionInitialized() = 0;
        virtual bool delayedInitialize();

        virtual void handleBufferSizeChange();
        virtual void handleSampleRateChange();
        virtual void handleDeviceChange();

        OutputSystemInterface *outputSystemInterface() const;

    protected:
        explicit IOutputSystemAddOn(QObject *parent = nullptr);

    private:
        QScopedPointer<IOutputSystemAddOnPrivate> d_ptr;

    };

}

#endif // AUDIO_IOUTPUTSYSTEMADDON_H
