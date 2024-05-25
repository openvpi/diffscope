#include "cantoneseconverter.h"

#include <QtWidgets/QWidget>

namespace LyricTool::Internal {

    class CantoneseConverterSettingPage : public QWidget {
    public:
        explicit CantoneseConverterSettingPage(CantoneseConverter *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        CantoneseConverter *d;
    };

    CantoneseConverter::CantoneseConverter(QObject *parent)
        : IG2pConverter(QStringLiteral("Cantonese"), parent) {
        setDisplayName(tr("Cantonese"));
        setDescription(tr("Using Cantonese Pinyin as the phonetic notation method."));
    }

    CantoneseConverter::~CantoneseConverter() = default;

    QWidget *CantoneseConverter::createSettingPage(QWidget *parent) {
        return new CantoneseConverterSettingPage(this, parent);
    }

    QList<LyricInfo> CantoneseConverter::convert(const QStringList &input,
                                                const QVariantMap &args) const {
        return {};
    }

}