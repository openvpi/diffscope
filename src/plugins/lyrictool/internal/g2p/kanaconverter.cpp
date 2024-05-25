#include "kanaconverter.h"

#include <QtWidgets/QWidget>

namespace LyricTool::Internal {

    class KanaConverterSettingPage : public QWidget {
    public:
        explicit KanaConverterSettingPage(KanaConverter *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        KanaConverter *d;
    };

    KanaConverter::KanaConverter(QObject *parent)
        : IG2pConverter(QStringLiteral("Kana"), parent) {
        setDisplayName(tr("Kana"));
        setDescription(tr("Kana to Romanization converter."));
    }

    KanaConverter::~KanaConverter() = default;

    QWidget *KanaConverter::createSettingPage(QWidget *parent) {
        return new KanaConverterSettingPage(this, parent);
    }

    QList<LyricInfo> KanaConverter::convert(const QStringList &input,
                                                const QVariantMap &args) const {
        return {};
    }

}