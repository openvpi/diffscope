#ifndef MANDARIN_H
#define MANDARIN_H

#include <QObject>

#include <lyrictool/ig2pconverter.h>

namespace LyricTool::Internal {

    class MandarinConverterSettingPage;

    class MandarinConverter : public IG2pConverter {
        Q_OBJECT
    public:
        explicit MandarinConverter(QObject *parent = nullptr);
        ~MandarinConverter() override;

        QList<LyricInfo> convert(const QStringList &input, const QVariantMap &args) const = 0;

    public:
        QWidget *createSettingPage(QWidget *parent);

    private:
        // TODO

        friend class MandarinConverterSettingPage;
    };

}

#endif // MANDARIN_H
