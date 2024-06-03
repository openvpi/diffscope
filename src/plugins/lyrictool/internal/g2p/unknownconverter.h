#ifndef UNKNOWNCONVERTER_H
#define UNKNOWNCONVERTER_H

#include <QObject>

#include <lyrictool/ig2pconverter.h>

namespace LyricTool::Internal {

    class UnknownConverterSettingPage;

    class UnknownConverter : public IG2pConverter {
        Q_OBJECT
    public:
        explicit UnknownConverter(QObject *parent = nullptr);
        ~UnknownConverter() override;

        QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const override;

    public:
        QWidget *createSettingPage(QWidget *parent);

    private:
        // TODO

        friend class UnknownConverterSettingPage;
    };

}

#endif //UNKNOWNCONVERTER_H
