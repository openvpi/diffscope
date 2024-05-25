#include "mandarinconverter.h"

#include <QtWidgets/QWidget>

namespace LyricTool::Internal {

    class MandarinConverterSettingPage : public QWidget {
    public:
        explicit MandarinConverterSettingPage(MandarinConverter *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        MandarinConverter *d;
    };

    MandarinConverter::MandarinConverter(QObject *parent)
        : IG2pConverter(QStringLiteral("Mandarin"), parent) {
        setDisplayName(tr("Mandarin"));
        setDescription(tr("Using Pinyin as the phonetic notation method."));
    }

    MandarinConverter::~MandarinConverter() = default;

    QWidget *MandarinConverter::createSettingPage(QWidget *parent) {
        return new MandarinConverterSettingPage(this, parent);
    }

    QList<LyricInfo> MandarinConverter::convert(const QStringList &input,
                                                const QVariantMap &args) const {
        return {};
    }

}