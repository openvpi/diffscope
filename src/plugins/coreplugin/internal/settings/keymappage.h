#ifndef KEYMAPPAGE_H
#define KEYMAPPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

    class KeymapPageWidget;

    class KeymapPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit KeymapPage(QObject *parent = nullptr);
        ~KeymapPage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<KeymapPageWidget> m_widget;
    };

}

#endif // KEYMAPPAGE_H
