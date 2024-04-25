#ifndef AUDIO_VSTMODEPAGE_H
#define AUDIO_VSTMODEPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace SVS {
    class ExpressionSpinBox;
}

class QComboBox;
class QTreeWidget;
class QCheckBox;

namespace Audio {

    class CustomizeThemeDialog;

    class VSTModePage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit VSTModePage(QObject *parent = nullptr);
        ~VSTModePage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<QWidget> m_widget;
        SVS::ExpressionSpinBox *m_editorPortSpinBox;
        SVS::ExpressionSpinBox *m_pluginPortSpinBox;
        QComboBox *m_closeEditorBehaviorComboBox;
        QCheckBox *m_alwaysOnTopCheckBox;
        QCheckBox *m_pluginEditorUsesCustomThemeCheckBox;
        CustomizeThemeDialog *m_customizeThemeDialog;
        QTreeWidget *m_statusTreeWidget;
    };

}

#endif // AUDIO_VSTMODEPAGE_H
