#ifndef ENGLISHCONVERTER_H
#define ENGLISHCONVERTER_H

#include <QObject>

#include <lyrictool/ig2pconverter.h>

namespace LyricTool::Internal {
    class EnglishConverterSettingPage;

    class EnglishConverter : public IG2pConverter {
        Q_OBJECT
    public:
        explicit EnglishConverter(QObject *parent = nullptr);
        ~EnglishConverter() override;

        QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const override;

    public:
        QWidget *createSettingPage(QWidget *parent);

    private:
        // TODO

        friend class EnglishConverterSettingPage;
    };
}

#endif // ENGLISHCONVERTER_H
