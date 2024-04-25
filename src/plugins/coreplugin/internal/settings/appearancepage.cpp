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
#include <QPushButton>
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
        AppearancePageWidget(QWidget *parent = nullptr) : QWidget(parent) {
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

        QFont resultFont;
        if (!loadFontSettings(resultFont))
            resultFont = QApplication::font();
        font = resultFont;
    }

    AppearancePage::~AppearancePage() {
    }

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
        if (!saveFontSettings(font))
            return false;
        return true;
    }

    void AppearancePage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

    bool AppearancePage::loadFontSettings(QFont &font) {
        auto appPath = QCoreApplication::applicationDirPath();
        auto jsonPath =
            QDir::cleanPath(appPath + QDir::separator() + QString("qtmediate.user.json"));
        if (!QFile::exists(jsonPath)) {
            // qDebug() << "\"qtmediate.user.json\" does not exist.";
            return false;
        }

        QJsonObject jsonObj;
        if (!loadJsonFile(jsonPath, &jsonObj))
            return false;

        if (!jsonObj.contains("AppFont")) {
            // qDebug() << "\"AppFont\" does not exist";
            return false;
        }

        auto objAppFont = jsonObj.value("AppFont").toObject();
        if (!objAppFont.contains("Family") || objAppFont.value("Family").toString().isEmpty())
            return false;

        if (!objAppFont.contains("Size") || objAppFont.value("Size").toInt() == 0)
            return false;

        font.setFamily(objAppFont.value("Family").toString());
        font.setPointSize(objAppFont.value("Size").toInt());
        if (objAppFont.contains("Weight"))
            font.setWeight(objAppFont.value("Weight").toInt());
        if (objAppFont.contains("Italic"))
            font.setItalic(objAppFont.value("Italic").toBool());

        return true;
    }

    bool AppearancePage::saveFontSettings(const QFont &font) {
        // Create qtmediate.user.json if not exist.
        auto appPath = QCoreApplication::applicationDirPath();
        auto jsonPath =
            QDir::cleanPath(appPath + QDir::separator() + QString("qtmediate.user.json"));
        if (!QFile::exists(jsonPath)) {
            // qDebug() << "\"qtmediate.user.json\" does not exist.";
            QFile file(jsonPath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&file);
            out << "{}";
            file.close();
        }

        QJsonObject jsonObj;
        if (!loadJsonFile(jsonPath, &jsonObj))
            return false;

        if (jsonObj.contains("AppFont")) {
            auto objAppFont = jsonObj.value("AppFont").toObject();
            objAppFont["Family"] = font.family();
            objAppFont["Size"] = font.pointSize();
            objAppFont["Weight"] = font.weight();
            objAppFont["Italic"] = font.italic();
            jsonObj["AppFont"] = objAppFont;
        } else {
            QJsonObject objAppFont;
            objAppFont.insert("Family", font.family());
            objAppFont.insert("Size", font.pointSize());
            objAppFont.insert("Weight", font.weight());
            objAppFont.insert("Italic", font.italic());
            jsonObj.insert("AppFont", objAppFont);
        }

        saveJsonFile(jsonPath, jsonObj);

        return true;
    }

    bool AppearancePage::loadJsonFile(const QString &filename, QJsonObject *jsonObj) {
        // Deserialize json
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            //                qDebug() << "Failed to open \"qtmediate.user.json\"";
            return false;
        }
        QByteArray allData = loadFile.readAll();
        loadFile.close();
        QJsonParseError err;
        QJsonDocument json = QJsonDocument::fromJson(allData, &err);
        if (err.error != QJsonParseError::NoError) {
            //                qDebug() << "Failed to deserialize \"qtmediate.user.json\"" <<
            //                err.error;
            return false;
        }
        if (json.isObject()) {
            *jsonObj = json.object();
        }
        return true;
    }

    bool AppearancePage::saveJsonFile(const QString &filename, QJsonObject &jsonObj) {
        QJsonDocument document;
        document.setObject(jsonObj);

        auto byteArray = document.toJson(QJsonDocument::Compact);
        QString jsonStr(byteArray);
        QFile file(filename);
        file.remove();

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to write " + filename;
            return false;
        }
        //            QTextStream in(&file);
        //            in << jsonStr;
        file.write(jsonStr.toUtf8());

        file.close();
        return true;
    }

    QString AppearancePage::getFontWeightStr(const QFont::Weight &weight) {
        switch (weight) {
            case QFont::Thin:
                // TODO: translation.
                return "Thin";
            case QFont::ExtraLight:
                return "ExtraLight";
            case QFont::Light:
                return "Light";
            case QFont::Normal:
                return "Normal";
            case QFont::Medium:
                return "Medium";
            case QFont::DemiBold:
                return "DemiBold";
            case QFont::Bold:
                return "Bold";
            case QFont::ExtraBold:
                return "ExtraBold";
            case QFont::Black:
                return "Black";
            default:
                return "";
        }
    }

}