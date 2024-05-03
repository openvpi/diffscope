#ifndef AUDIO_FORMATMANAGER_H
#define AUDIO_FORMATMANAGER_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class AbstractAudioFormatIO;
}

namespace Audio {

    class AudioPlugin;

    class FormatEntry;

    class FormatManagerPrivate;

    class AUDIO_EXPORT FormatManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(FormatManager)
        friend class AudioPlugin;
    public:
        ~FormatManager() override;

        void addEntry(FormatEntry *entry);
        QList<FormatEntry *> entries() const;
        QStringList extensionHints() const;
        QStringList filters() const;

        FormatEntry *hintFromExtension(const QString &extension) const;

        talcs::AbstractAudioFormatIO *getFormatLoad(const QString &filename, const QVariant &userData) const;

    private:
        explicit FormatManager(QObject *parent = nullptr);
        QScopedPointer<FormatManagerPrivate> d_ptr;
    };

}

#endif // AUDIO_FORMATMANAGER_H
