#ifndef AUDIO_BUILTINFORMATENTRY_H
#define AUDIO_BUILTINFORMATENTRY_H

#include <audioplugin/formatentry.h>

namespace Audio {

    class BuiltInFormatEntry : public FormatEntry {
        Q_OBJECT
    public:
        explicit BuiltInFormatEntry(QObject *parent = nullptr);
        ~BuiltInFormatEntry();

        QStringList filters() const override;
        QStringList extensionHints() const override;
        talcs::AbstractAudioFormatIO *getFormatOpen(const QString &filename, QVariant &userData, QWidget *win) override;
        talcs::AbstractAudioFormatIO *getFormatLoad(const QString &filename, const QVariant &userData) override;

    private:
        QStringList m_filters;
        QStringList m_extensionHints;
        QList<QPair<QString, int>> m_rawSubtypes;
    };

}

#endif // AUDIO_BUILTINFORMATENTRY_H
