#ifndef APPEARANCEPAGE_H
#define APPEARANCEPAGE_H

#include <QFont>
#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

    class AppearancePageWidget;

    class AppearancePage : public ISettingPage {
        Q_OBJECT
    public:
        explicit AppearancePage(QObject *parent = nullptr);
        ~AppearancePage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<AppearancePageWidget> m_widget;
        QFont font;
        QString getFontWeightStr(const QFont::Weight &weight);
        bool loadJsonFile(const QString &filename, QJsonObject *jsonObj);
        bool saveJsonFile(const QString &filename, QJsonObject &jsonObj);
        bool loadFontSettings(QFont &font);
        bool saveFontSettings(const QFont &font);
    };

}

#endif // APPEARANCEPAGE_H
