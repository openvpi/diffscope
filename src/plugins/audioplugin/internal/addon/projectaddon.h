#ifndef AUDIO_PROJECTADDON_H
#define AUDIO_PROJECTADDON_H

#include <QMCore/qmchronomap.h>

#include <CoreApi/iwindow.h>

namespace Core {
    class IProjectWindow;
}

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

        static ProjectAddOn *get(Core::IProjectWindow *windowHandle);

        bool isVST() const;

        talcs::DspxProjectContext *projectContext() const;
        AudioContextInterface *audioContextInterface() const;

        void updateFileCounter(const QString &filename, int count);

        void setAudioClipToOpenFile(QDspx::AudioClipEntity *entity, const QString &selectedFilter);
        bool checkAudioClipIsToOpenFile(QDspx::AudioClipEntity *entity, QString &selectedFilter);

        void addFailedAudioClipToAlert(QDspx::AudioClipEntity *entity);
        QList<QDspx::AudioClipEntity *> takeFailedAudioClipsToAlert();

        void setData(const QString &key, const QVariant &value);
        QVariant data(const QString &key) const;
        bool hasData(const QString &key) const;

    private:
        AudioContextInterface *m_audioContextInterface;
        talcs::DspxProjectContext *m_projectContext;

        QHash<QDspx::AudioClipEntity *, QString> m_audioClipsToOpenFile;

        QList<QDspx::AudioClipEntity *> m_failedAudioClipsToAlert;

        QHash<QString, QVariant> m_dataDict;
    };

}

#endif // AUDIO_PROJECTADDON_H
