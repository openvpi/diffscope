#include "appearancepage.h"

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QFontDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QListView>
#include <QCheckBox>

#include <QMWidgets/ccombobox.h>
#include <QMWidgets/clineedit.h>
#include <QMWidgets/qmdecoratorv2.h>

namespace Core::Internal {

    static const int zoomRatioList[] = {
        100, 110, 125, 150, 175, 200,
    };

    class AppearancePageWidget : public QWidget {
    public:
        explicit AppearancePageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto displayGroup = new QGroupBox(AppearancePage::tr("Display"));
            auto displayLayout = new QFormLayout();

            themeComboBox = new CComboBox();
            themeComboBox->addItems(qIDec->themes());
            themeComboBox->setCurrentText(qIDec->theme());

            zoomComboBox = new CComboBox();
            for (const auto &item : std::as_const(zoomRatioList)) {
                zoomComboBox->addItem(QString::asprintf("%d%%", item));
            }
            zoomComboBox->setCurrentIndex(0);

            fontText = new CLineEdit();
            fontText->setReadOnly(true);
            fontText->setText(QApplication::font().family());

            selectFontButton = new QPushButton(AppearancePage::tr("Select"));
            connect(selectFontButton, &QPushButton::clicked, this,
                    &AppearancePageWidget::showFontDialog);
            useSystemFontCheckBox = new QCheckBox(AppearancePage::tr("Use system font"));
            connect(useSystemFontCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
                if (checked) {
                    fontText->setText(QApplication::font().family());
                }
                fontText->setDisabled(checked);
                selectFontButton->setDisabled(checked);
            });

            auto fontLayout = new QHBoxLayout();
            fontLayout->addWidget(fontText);
            fontLayout->addWidget(selectFontButton);
            fontLayout->addWidget(useSystemFontCheckBox);

            displayLayout->addRow(AppearancePage::tr("Theme"), themeComboBox);
            displayLayout->addRow(AppearancePage::tr("Zoom"), zoomComboBox);
            displayLayout->addRow(AppearancePage::tr("Font"), fontLayout);

            displayGroup->setLayout(displayLayout);

            auto mainLayout = new QVBoxLayout();
            mainLayout->addWidget(displayGroup);
            mainLayout->addStretch();
            setLayout(mainLayout);
        }

        QComboBox *themeComboBox;
        QComboBox *zoomComboBox;
        QLineEdit *fontText;
        QPushButton *selectFontButton;
        QCheckBox *useSystemFontCheckBox;

    private:
        static QFont getFont(bool *ok, const QFont &initial, QWidget *parent,
                             const QString &title = {},
                             QFontDialog::FontDialogOptions options = {}) {
            QFontDialog dlg(parent);
            dlg.setOptions(options);
            dlg.setCurrentFont(initial);
            dlg.resize(640, 640);
            if (!title.isEmpty())
                dlg.setWindowTitle(title);

            auto combos = dlg.findChildren<QComboBox *>();
            for (auto combo : qAsConst(combos)) {
                qDebug() << combo;
                combo->setItemDelegate(new QStyledItemDelegate());
            }

            int ret = (dlg.exec() || (options & QFontDialog::NoButtons));
            if (ok)
                *ok = !!ret;
            if (ret) {
                return dlg.selectedFont();
            } else {
                return initial;
            }
        };

        void showFontDialog() {
            bool ok = false;
            auto font =
                getFont(&ok, QFont(fontText->text()), this, AppearancePage::tr("Select Font"));
            if (ok) {
                fontText->setText(font.family());
            }
        }
    };

    AppearancePage::AppearancePage(QObject *parent)
        : ISettingPage(QStringLiteral("core.Appearance"), parent) {
        setTitle([]() { return tr("Appearance"); });
        setDescription([]() { return tr("Appearance"); });
    }

    AppearancePage::~AppearancePage() = default;

    QString AppearancePage::sortKeyword() const {
        return QStringLiteral("Appearance");
    }

    bool AppearancePage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *AppearancePage::widget() {
        if (!m_widget) {
            m_widget = new AppearancePageWidget();
        }
        return m_widget;
    }

    bool AppearancePage::accept() {
        return true;
    }

    void AppearancePage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}