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
#include <QToolTip>

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
//      - Property: userFont
//      - Type:     QFont
//    Font Ratio
//      - Object:   QMDecoratorV2::instance()
//      - Property: userFont
//      - Type:     double
//    Use System Font
//      - Object:   QApplication::instance()
//      - Property: useSystemFont (meta)
//      - Type:     bool

namespace Core::Internal {

    static const int zoomRatioList[] = {
        50, 75, 100, 125, 150, 175, 200, 225, 250,
    };

    static inline QFont systemFont() {
        auto font = qAppExt->systemDefaultFont();
        font.setPixelSize(12);
        return font;
    }

    static inline void syncAppFontSize() {
        auto font = qApp->property("userFontInitial").value<QFont>();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        font.setPointSize(font.pixelSize() * 0.75 * qIDec->zoomRatio());
#else
        font.setPixelSize(font.pixelSize() * qIDec->zoomRatio());
#endif
        // Make the default font update with system dpi
        QGuiApplication::setFont(font);
        QToolTip::setFont(font);
    }

    class AppearancePageWidget : public QWidget {
        Q_DECLARE_TR_FUNCTIONS(AppearancePageWidget)
    public:
        explicit AppearancePageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            recordCurrentArguments();

            auto displayGroup = new QGroupBox(tr("Display"));
            auto displayLayout = new QFormLayout();

            themeComboBox = new CComboBox();
            themeComboBox->addItems(qIDec->themes());

            zoomComboBox = new CComboBox();
            for (const auto &item : std::as_const(zoomRatioList)) {
                zoomComboBox->addItem(QString::asprintf("%d%%", item));
            }

            fontText = new CLineEdit();
            fontText->setReadOnly(true);

            selectFontButton = new QPushButton(tr("Select"));
            useSystemFontCheckBox = new QCheckBox(tr("Use system font"));

            auto fontLayout = new QHBoxLayout();
            fontLayout->addWidget(fontText);
            fontLayout->addWidget(selectFontButton);
            fontLayout->addWidget(useSystemFontCheckBox);

            displayLayout->addRow(tr("Theme"), themeComboBox);
            displayLayout->addRow(tr("Zoom"), zoomComboBox);
            displayLayout->addRow(tr("Font"), fontLayout);
            displayLayout->addRow(
                new QLabel(tr("The font change takes effect only after the restart.")));

            displayGroup->setLayout(displayLayout);

            auto mainLayout = new QVBoxLayout();
            mainLayout->addWidget(displayGroup);
            mainLayout->addStretch();
            setLayout(mainLayout);

            currentFont = originalFont;
            updateFontText();
            useSystemFontCheckBox->setChecked(originalUseSystemFont);
            fontText->setDisabled(originalUseSystemFont);
            selectFontButton->setDisabled(originalUseSystemFont);
            themeComboBox->setCurrentText(originalTheme);
            {
                auto idx = int((originalZoomRatio - 0.5) * 4);
                if (idx >= 0 && idx < sizeof(zoomRatioList)) {
                    zoomComboBox->setCurrentIndex(idx);
                } else {
                    zoomComboBox->setCurrentIndex(0);
                }
            }

            connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                    &AppearancePageWidget::_q_themeComboBoxIndexChanged);
            connect(zoomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                    &AppearancePageWidget::_q_zoomComboBoxIndexChanged);
            connect(selectFontButton, &QPushButton::clicked, this,
                    &AppearancePageWidget::showFontDialog);
            connect(useSystemFontCheckBox, &QCheckBox::toggled, this,
                    &AppearancePageWidget::_q_useSystemFontButtonToggled);
        }

        void recordCurrentArguments() {
            originalTheme = qIDec->theme();
            originalZoomRatio = qIDec->zoomRatio();
            originalFont = qApp->property("userFont").value<QFont>();
            originalUseSystemFont = qApp->property("useSystemFont").toBool();
        }

        QComboBox *themeComboBox;
        QComboBox *zoomComboBox;
        QLineEdit *fontText;
        QPushButton *selectFontButton;
        QCheckBox *useSystemFontCheckBox;

        QFont currentFont;

        QString originalTheme;
        double originalZoomRatio;
        QFont originalFont;
        bool originalUseSystemFont;

    private:
        void _q_useSystemFontButtonToggled(bool checked) {
            if (checked) {
                currentFont = systemFont();
                updateFontText();
                qApp->setProperty("userFont", currentFont);
            }
            fontText->setDisabled(checked);
            selectFontButton->setDisabled(checked);
            qApp->setProperty("useSystemFont", checked);
        }

        void _q_themeComboBoxIndexChanged(int index) {
            Q_UNUSED(this)
            qIDec->setTheme(themeComboBox->itemText(index));
        }

        void _q_zoomComboBoxIndexChanged(int index) {
            Q_UNUSED(this)
            qIDec->setZoomRatio(double(zoomRatioList[index]) / 100);
            syncAppFontSize();
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
            auto initialFont = currentFont;
            initialFont.setPointSize(initialFont.pixelSize());
            auto font = getFont(&ok, initialFont, this, tr("Select Font"));
            if (ok) {
                currentFont = font;
                currentFont.setPixelSize(font.pointSize());
                updateFontText();
                qApp->setProperty("userFont", currentFont);
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

        // Record arguments
        m_widget->recordCurrentArguments();
        return true;
    }

    void AppearancePage::finish() {
        if (m_widget) {
            // Maybe canceled
            qIDec->setTheme(m_widget->originalTheme);
            qIDec->setZoomRatio(m_widget->originalZoomRatio);
            qApp->setProperty("userFont", m_widget->originalFont);
            qApp->setProperty("useSystemFont", m_widget->originalUseSystemFont);

            syncAppFontSize();

            // Save recorded arguments
            auto settings = ExtensionSystem::PluginManager::settings();
            settings->beginGroup("Preferences");
            settings->setValue(QStringLiteral("Theme"), m_widget->originalTheme);
            settings->setValue(QStringLiteral("Zoom"), int(m_widget->originalZoomRatio * 100));
            settings->setValue(QStringLiteral("Font"), m_widget->originalFont.toString());
            settings->setValue(QStringLiteral("UseSystemFont"), m_widget->originalUseSystemFont);
            settings->endGroup();

            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}