#ifndef CANTONESECONVERTER_H
#define CANTONESECONVERTER_H

#include <QObject>

#include <lyrictool/ig2pconverter.h>

namespace LyricTool::Internal {

    class CantoneseConverterSettingPage;

    class CantoneseConverter : public IG2pConverter {
        Q_OBJECT
    public:
        explicit CantoneseConverter(QObject *parent = nullptr);
        ~CantoneseConverter() override;

        QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const override;

    public:
        QWidget *createSettingPage(QWidget *parent) override;

    private:
        // TODO

        friend class CantoneseConverterSettingPage;
    };

}


#endif // CANTONESECONVERTER_H
