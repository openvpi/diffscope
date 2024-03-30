#ifndef AUDIO_VSTMODEPAGE_H
#define AUDIO_VSTMODEPAGE_H

#include <CoreApi/isettingpage.h>

namespace SVS {
    class ExpressionSpinBox;
}

class QComboBox;
class QTreeWidget;

namespace Audio {

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
        QWidget *m_widget = nullptr;
        SVS::ExpressionSpinBox *m_editorPortSpinBox;
        SVS::ExpressionSpinBox *m_pluginPortSpinBox;
        QComboBox *m_closeEditorBehaviorComboBox;
        QTreeWidget *m_statusTreeWidget;
    };

} // Audio

#endif // AUDIO_VSTMODEPAGE_H
