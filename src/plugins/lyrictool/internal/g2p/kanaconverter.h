#ifndef KANACONVERTER_H
#define KANACONVERTER_H

#include <QObject>

#include <lyrictool/ig2pconverter.h>

namespace LyricTool::Internal {

    class KanaConverterSettingPage;

    class KanaConverter : public IG2pConverter {
        Q_OBJECT
    public:
        explicit KanaConverter(QObject *parent = nullptr);
        ~KanaConverter() override;

        QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const override;

    public:
        QWidget *createSettingPage(QWidget *parent) override;

    private:
        friend class KanaConverterSettingPage;
    };

}

#endif // KANACONVERTER_H
