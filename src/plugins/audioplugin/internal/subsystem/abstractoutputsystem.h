#ifndef AUDIO_ABSTRACTOUTPUTSYSTEM_H
#define AUDIO_ABSTRACTOUTPUTSYSTEM_H

#include <memory>

#include <QObject>

namespace talcs {
    class AbstractOutputContext;
}

namespace Audio::Internal {

    class AbstractOutputSystem : public QObject {
        Q_OBJECT
    public:
        explicit AbstractOutputSystem(QObject *parent = nullptr);
        ~AbstractOutputSystem() override;

        virtual bool initialize() = 0;

        talcs::AbstractOutputContext *context() const;

        void setFileBufferingReadAheadSize(qint64 size);
        qint64 fileBufferingReadAheadSize() const;

        bool isReady() const;

    Q_SIGNALS:
        void fileBufferingReadAheadSizeChanged(qint64 readAheadSize);

    protected:
        void setContext(talcs::AbstractOutputContext *context);

    private:
        qint64 m_fileBufferingReadAheadSize = 0;
        talcs::AbstractOutputContext *m_context = nullptr;
    };

}

#endif // AUDIO_ABSTRACTOUTPUTSYSTEM_H
