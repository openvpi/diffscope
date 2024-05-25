#include "englishconverter.h"

#include <QtWidgets/QWidget>

namespace LyricTool::Internal {

    class EnglishConverterSettingPage : public QWidget {
    public:
        explicit EnglishConverterSettingPage(EnglishConverter *d, QWidget *parent = nullptr)
            : QWidget(parent), d(d) {
        }

        EnglishConverter *d;
    };

    EnglishConverter::EnglishConverter(QObject *parent)
        : IG2pConverter(QStringLiteral("English"), parent) {
        setDisplayName(tr("English"));
        setDescription(tr("Greedy matching of consecutive English letters."));
    }

    EnglishConverter::~EnglishConverter() = default;

    QWidget *EnglishConverter::createSettingPage(QWidget *parent) {
        return new EnglishConverterSettingPage(this, parent);
    }

    QList<LyricInfo> EnglishConverter::convert(const QStringList &input,
                                                const QVariantMap &args) const {
        return {};
    }

}