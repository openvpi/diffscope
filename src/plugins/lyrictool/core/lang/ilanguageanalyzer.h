#ifndef ILANGUAGEANALYZER_H
#define ILANGUAGEANALYZER_H

#include <QtCore/QObject>
#include <QtCore/QJsonObject>

#include <QMCore/qmdisplaystring.h>

#include <lyrictool/lyricinfo.h>

namespace LyricTool {

    class ILanguageAnalyzerPrivate;

    class LYRICTOOL_EXPORT ILanguageAnalyzer : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ILanguageAnalyzer)
    public:
        ~ILanguageAnalyzer();

    public:
        virtual bool contains(QChar c) const;
        virtual bool contains(const QString &input) const;

        virtual QString randString() const;

        virtual QList<LyricInfo> split(const QString &input) const;
        QList<LyricInfo> split(const QList<LyricInfo> &input) const;
        QString analyze(const QString &input) const;
        void correct(const QList<LyricInfo> &input) const;

        virtual QWidget *createSettingPage() = 0;

    public:
        QString id() const;

        QString displayName() const;
        void setDisplayName(const QMDisplayString &displayName);

        QString description() const;
        void setDescription(const QMDisplayString &description);

    protected:
        ILanguageAnalyzer(ILanguageAnalyzerPrivate &d, const QString &id,
                          QObject *parent = nullptr);

        QScopedPointer<ILanguageAnalyzerPrivate> d_ptr;
    };

}

#endif // ILANGUAGEANALYZER_H
