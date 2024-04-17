#ifndef KEYMAPPAGE_H
#define KEYMAPPAGE_H

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

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
        QWidget *m_widget;
    };

}

#endif // KEYMAPPAGE_H
