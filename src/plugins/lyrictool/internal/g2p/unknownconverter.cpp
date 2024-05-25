#include "unknownconverter.h"

#include <QtWidgets/QWidget>

namespace LyricTool::Internal {

    class UnknownConverterSettingPage : public QWidget {
    public:
        explicit UnknownConverterSettingPage(UnknownConverter *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        UnknownConverter *d;
    };

    UnknownConverter::UnknownConverter(QObject *parent)
        : IG2pConverter(QStringLiteral("Unknown"), parent) {
        setDisplayName(tr("Unknown"));
        setDescription(tr("Unknown language, no conversion required."));
    }

    UnknownConverter::~UnknownConverter() = default;

    QWidget *UnknownConverter::createSettingPage(QWidget *parent) {
        return new UnknownConverterSettingPage(this, parent);
    }

    QList<LyricInfo> UnknownConverter::convert(const QStringList &input,
                                                const QVariantMap &args) const {
        return {};
    }

}