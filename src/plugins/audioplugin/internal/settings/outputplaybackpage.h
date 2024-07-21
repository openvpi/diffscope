#ifndef AUDIO_OUTPUTPLAYBACKPAGE_H
#define AUDIO_OUTPUTPLAYBACKPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;

namespace SVS {
    class ExpressionDoubleSpinBox;
    class ExpressionSpinBox;
    class SeekBar;
};

namespace Audio::Internal {

    class OutputPlaybackPageWidget;

    class OutputPlaybackPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit OutputPlaybackPage(QObject *parent = nullptr);
        ~OutputPlaybackPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<OutputPlaybackPageWidget> m_widget;

    };

}

#endif // AUDIO_OUTPUTPLAYBACKPAGE_H
