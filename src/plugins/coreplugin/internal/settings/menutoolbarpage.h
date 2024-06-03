#ifndef MENUTOOLBARPAGE_H
#define MENUTOOLBARPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Core {

    class MenuToolbarPageWidget;

    class MenuToolbarPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit MenuToolbarPage(QObject *parent = nullptr);
        ~MenuToolbarPage() override;

        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<MenuToolbarPageWidget> m_widget;
    };

} // Core

#endif // MENUTOOLBARPAGE_H
