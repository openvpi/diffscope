#ifndef AUDIO_FORMATENTRY_H
#define AUDIO_FORMATENTRY_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class AbstractAudioFormatIO;
}

namespace Audio {

    class AUDIO_EXPORT FormatEntry : public QObject {
        Q_OBJECT
    public:
        ~FormatEntry() override;

        virtual QStringList filters() const = 0;
        virtual talcs::AbstractAudioFormatIO *getFormatOpen(const QString &filename, QVariant &userData, const QString &filter, QWidget *win) = 0;
        virtual talcs::AbstractAudioFormatIO *getFormatLoad(const QString &filename, const QVariant &userData, const QString &filter, QWidget *win) = 0;

    protected:
        explicit FormatEntry(QObject *parent = nullptr);
    };

}

#endif // AUDIO_FORMATENTRY_H
