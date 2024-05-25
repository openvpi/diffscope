#ifndef IG2PCONVERTER_H
#define IG2PCONVERTER_H

#include <QtCore/QObject>
#include <QtCore/QJsonObject>

#include <QMCore/qmdisplaystring.h>

#include <lyrictool/lyricinfo.h>

namespace LyricTool {

    class IG2pConverterPrivate;

    class LYRICTOOL_EXPORT IG2pConverter : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IG2pConverter)
    public:
        IG2pConverter(const QString &id, QObject *parent = nullptr);
        ~IG2pConverter();

    public:
        QString id() const;

        QString displayName() const;
        void setDisplayName(const QMDisplayString &displayName);

        QString description() const;
        void setDescription(const QMDisplayString &description);

    public:
        inline LyricInfo convert(const QString &input, const QVariantMap &args) const;
        virtual QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const;

        virtual QWidget *createSettingPage(QWidget *parent) = 0;

    protected:
        IG2pConverter(IG2pConverterPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IG2pConverterPrivate> d_ptr;
    };

    inline LyricInfo IG2pConverter::convert(const QString &input, const QVariantMap &args) const {
        return convert(QStringList{input}, args).front();
    }

}

#endif // IG2PCONVERTER_H