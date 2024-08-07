#ifndef AUDIO_PROJECTADDON_H
#define AUDIO_PROJECTADDON_H

#include <TalcsCore/MetronomeAudioSource.h>
#include <TalcsCore/BufferingAudioSource.h>

#include <QMCore/qmchronomap.h>

#include <CoreApi/iwindow.h>

namespace talcs {
    class DspxProjectContext;
}

namespace QDspx {
    class AudioClipEntity;
}

namespace Audio {
    class AudioContextInterface;

    class IAudioContextAddOn;
}

namespace Audio::Internal {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        bool isVST() const;

        talcs::DspxProjectContext *projectContext() const;
        AudioContextInterface *audioContextInterface() const;

        void updateFileCounter(const QString &filename, int count);

        void setAudioClipToOpenFile(QDspx::AudioClipEntity *entity, const QString &selectedFilter);
        bool checkAudioClipIsToOpenFile(QDspx::AudioClipEntity *entity, QString &selectedFilter);

        void addFailedAudioClipToAlert(QDspx::AudioClipEntity *entity);
        QList<QDspx::AudioClipEntity *> takeFailedAudioClipsToAlert();

    private:
        AudioContextInterface *m_audioContextInterface;
        talcs::DspxProjectContext *m_projectContext;

        QHash<QDspx::AudioClipEntity *, QString> *m_audioClipsToOpenFile;

        QList<QDspx::AudioClipEntity *> m_failedAudioClipsToAlert;
    };

}

#endif // AUDIO_PROJECTADDON_H
