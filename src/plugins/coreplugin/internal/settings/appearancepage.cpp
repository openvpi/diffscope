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

#include <extensionsystem/pluginmanager.h>

#include <QMWidgets/ccombobox.h>
#include <QMWidgets/clineedit.h>
#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmappextension.h>

// Setting Page Roles
// 1. Theme:
//      - Object:   QMDecoratorV2::instance()
//      - Property: theme
//      - Type:     QString
// 2. Zoom Ratio
//      - Object:   QMDecoratorV2::instance()
//      - Property: zoomRatio
//      - Type:     double
// 3. Font
//      - Object:   QGuiApplication::instance()
//      - Property: font
//      - Type:     QFont
//    Font Ratio
//      - Object:   QMDecoratorV2::instance()
//      - Property: fontRatio
//      - Type:     double
//    Use System Font
//      - Object:   QApplication::instance()
//      - Property: useSystemFont (meta)
//      - Type:     bool

namespace Core::Internal {

    static const int zoomRatioList[] = {
        100, 125, 150, 175, 200, 225, 250,
    };

    class AppearancePageWidget : public QWidget {
    public:
        explicit AppearancePageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto displayGroup = new QGroupBox(AppearancePage::tr("Display"));
            auto displayLayout = new QFormLayout();

            themeComboBox = new CComboBox();
            themeComboBox->addItems(qIDec->themes());

            zoomComboBox = new CComboBox();
            for (const auto &item : std::as_const(zoomRatioList)) {
                zoomComboBox->addItem(QString::asprintf("%d%%", item));
            }

            fontText = new CLineEdit();
            fontText->setReadOnly(true);

            selectFontButton = new QPushButton(AppearancePage::tr("Select"));
            connect(selectFontButton, &QPushButton::clicked, this,
                    &AppearancePageWidget::showFontDialog);
            useSystemFontCheckBox = new QCheckBox(AppearancePage::tr("Use system font"));
            connect(useSystemFontCheckBox, &QCheckBox::toggled, this,
                    &AppearancePageWidget::setUseSystemFont);

            auto fontLayout = new QHBoxLayout();
            fontLayout->addWidget(fontText);
            fontLayout->addWidget(selectFontButton);
            fontLayout->addWidget(useSystemFontCheckBox);

            displayLayout->addRow(AppearancePage::tr("Theme"), themeComboBox);
            displayLayout->addRow(AppearancePage::tr("Zoom"), zoomComboBox);
            displayLayout->addRow(AppearancePage::tr("Font"), fontLayout);
            displayLayout->addRow(new QLabel(
                AppearancePage::tr("The font change takes effect only after the restart.")));

            displayGroup->setLayout(displayLayout);

            auto mainLayout = new QVBoxLayout();
            mainLayout->addWidget(displayGroup);
            mainLayout->addStretch();
            setLayout(mainLayout);

            currentFont = QGuiApplication::font();
            updateFontText();
            if (qApp->property("useSystemFont").toBool()) {
                useSystemFontCheckBox->setChecked(true);
            }
            themeComboBox->setCurrentText(qIDec->theme());
            {
                auto idx = int((qIDec->zoomRatio() - 1) * 4);
                if (idx >= 0 && idx < sizeof(zoomRatioList)) {
                    zoomComboBox->setCurrentIndex(idx);
                } else {
                    zoomComboBox->setCurrentIndex(0);
                }
            }
        }

        QComboBox *themeComboBox;
        QComboBox *zoomComboBox;
        QLineEdit *fontText;
        QPushButton *selectFontButton;
        QCheckBox *useSystemFontCheckBox;

        QFont currentFont;

        static QFont systemFont() {
            auto font = qAppExt->systemDefaultFont();
            font.setPixelSize(12);
            return font;
        }

    private:
        void setUseSystemFont(bool on) {
            if (on) {
                currentFont = systemFont();
                updateFontText();
            }
            fontText->setDisabled(on);
            selectFontButton->setDisabled(on);
        }

        void updateFontText() const {
            if (auto style = currentFont.styleName(); style.isEmpty()) {
                fontText->setText(QStringLiteral("%1, %3").arg(
                    currentFont.family(), QString::number(currentFont.pixelSize())));
            } else {
                fontText->setText(QStringLiteral("%1, %2, %3")
                                      .arg(currentFont.family(), currentFont.styleName(),
                                           QString::number(currentFont.pixelSize())));
            }
        }

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
                currentFont = font;
                currentFont.setPixelSize(font.pointSize());
                updateFontText();
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
        if (!m_widget)
            return true;

        QString theme = m_widget->themeComboBox->currentText();
        int zoomPercent = zoomRatioList[m_widget->zoomComboBox->currentIndex()];
        QFont currentFont = m_widget->currentFont;
        bool useSystemFont = m_widget->useSystemFontCheckBox->isChecked();

        // Save Settings
        {
            auto settings = ExtensionSystem::PluginManager::settings();
            settings->beginGroup("Preferences");
            settings->setValue(QStringLiteral("Theme"), theme);
            settings->setValue(QStringLiteral("Zoom"), zoomPercent);
            settings->setValue(QStringLiteral("Font"), currentFont.toString());
            settings->setValue(QStringLiteral("UseSystemFont"), useSystemFont);
            settings->endGroup();
        }

        qIDec->setTheme(theme);
        qIDec->setZoomRatio(double(zoomPercent) / 100);
        QGuiApplication::setFont(currentFont);
        qApp->setProperty("useSystemFont", useSystemFont);
        return true;
    }

    void AppearancePage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}