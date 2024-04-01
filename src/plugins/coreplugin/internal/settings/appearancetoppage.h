#ifndef CHORUSKIT_APPEARANCETOPPAGE_H
#define CHORUSKIT_APPEARANCETOPPAGE_H

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

    class AppearanceTopPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit AppearanceTopPage(QObject *parent = nullptr);
        ~AppearanceTopPage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QWidget *m_widget;
    };

}

#endif // CHORUSKIT_APPEARANCETOPPAGE_H
