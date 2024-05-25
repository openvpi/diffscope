#ifndef ILANGUAGEANALYZER_H
#define ILANGUAGEANALYZER_H

#include <QtCore/QObject>

#include <QMCore/qmdisplaystring.h>

#include <lyrictool/lyricinfo.h>

namespace LyricTool {

    class ILanguageAnalyzerPrivate;

    class LYRICTOOL_EXPORT ILanguageAnalyzer : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ILanguageAnalyzer)
    public:
        ILanguageAnalyzer(const QString &id, QObject *parent = nullptr);
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

    public:
        QString category() const;
        void setCategory(const QString &category);

        bool enabled() const;
        void setEnabled(const bool &enable);

        bool discardResult() const;
        void setDiscardResult(const bool &discard);

    protected:
        ILanguageAnalyzer(ILanguageAnalyzerPrivate &d, const QString &id,
                          QObject *parent = nullptr);

        QScopedPointer<ILanguageAnalyzerPrivate> d_ptr;
    };

}

#endif // ILANGUAGEANALYZER_H
