#ifndef AUDIO_OUTPUTPLAYBACKPAGE_H
#define AUDIO_OUTPUTPLAYBACKPAGE_H

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;

namespace SVS {
    class ExpressionDoubleSpinBox;
};

namespace Audio {

    class OutputPlaybackPage : public Core::ISettingPage {
    public:
        explicit OutputPlaybackPage(QObject *parent = nullptr);
        ~OutputPlaybackPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QWidget *m_widget = nullptr;
        QComboBox *m_driverComboBox;
        QComboBox *m_deviceComboBox;
        QComboBox *m_bufferSizeComboBox;
        QComboBox *m_sampleRateComboBox;
        QComboBox *m_hotPlugModeComboBox;
        QComboBox *m_playHeadBehaviorComboBox;
        QCheckBox *m_closeDeviceOnPlaybackStopCheckBox;
        SVS::ExpressionDoubleSpinBox *m_fileBufferSizeMsecSpinBox;
    };

} // Audio

#endif // AUDIO_OUTPUTPLAYBACKPAGE_H
