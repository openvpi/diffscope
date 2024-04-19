#ifndef AUDIO_IOUTPUTSYSTEMADDON_H
#define AUDIO_IOUTPUTSYSTEMADDON_H

#include <QObject>

namespace talcs {
    class AudioSource;
}

namespace Audio {

    class OutputSystemInterface;
    class OutputSystemInterfacePrivate;

    class IOutputSystemAddOnPrivate;

    class IOutputSystemAddOn : public QObject {
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

        bool substituteSource(talcs::AudioSource *source);
        bool resetSource();
        virtual void handleSubstitutedSourceAfterControl(float gain, float pan);

        OutputSystemInterface *outputSystemInterface() const;

    protected:
        explicit IOutputSystemAddOn(QObject *parent = nullptr);

    private:
        QScopedPointer<IOutputSystemAddOnPrivate> d_ptr;

    };

} // Audio

#endif // AUDIO_IOUTPUTSYSTEMADDON_H
