#ifndef AUDIO_OUTPUTSYSTEM_H
#define AUDIO_OUTPUTSYSTEM_H

#include <memory>

#include <TalcsDevice/OutputContext.h>

#include <audioplugin/internal/abstractoutputsystem.h>

namespace Audio::Internal {

    class OutputSystem : public AbstractOutputSystem {
        Q_OBJECT
    public:
        explicit OutputSystem(QObject *parent = nullptr);
        ~OutputSystem() override;

        bool initialize() override;

        inline talcs::OutputContext *outputContext() const {
            return m_outputContext.get();
        }

        bool setDriver(const QString &driverName);
        bool setDevice(const QString &deviceName);
        bool setAdoptedBufferSize(qint64 bufferSize);
        bool setAdoptedSampleRate(double sampleRate);
        void setHotPlugNotificationMode(talcs::OutputContext::HotPlugNotificationMode mode);

    private:
        void postSetDevice() const;

        std::unique_ptr<talcs::OutputContext> m_outputContext;
    };

}

#endif // AUDIO_OUTPUTSYSTEM_H
