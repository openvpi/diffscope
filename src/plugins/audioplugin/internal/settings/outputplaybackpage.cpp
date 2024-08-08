#include "outputplaybackpage.h"

#include <cmath>

#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>

#include <TalcsCore/Decibels.h>
#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDevice/SDLAudioDriverDisplayNameHelper.h>
#include <TalcsDevice/OutputContext.h>

#include <SVSCraftWidgets/expressiondoublespinbox.h>
#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/seekbar.h>
#include <SVSCraftCore/decibellinearizer.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/devicetester.h>
#include <audioplugin/internal/vstconnectionsystem.h>
#include <audioplugin/internal/audiosettings.h>

namespace Audio::Internal {

    static inline double sliderValueToGain(double sliderValue) {
        return talcs::Decibels::decibelsToGain(SVS::DecibelLinearizer::linearValueToDecibel(sliderValue));
    }
    static inline double gainToSliderValue(float gain) {
        return SVS::DecibelLinearizer::decibelToLinearValue(talcs::Decibels::gainToDecibels(gain));
    }
    static inline double sliderValueToPan(int sliderValue) {
        return sliderValue / 100.0;
    }
    static inline int panToSliderValue(double pan) {
        return static_cast<int>(round(pan * 100.0));
    }

    class OutputPlaybackPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit OutputPlaybackPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;

            auto audioOutputGroupBox = new QGroupBox(tr("Audio Driver and Device"));
            auto audioOutputLayout = new QFormLayout;
            m_driverComboBox = new QComboBox;
            audioOutputLayout->addRow(tr("Audio d&river"), m_driverComboBox);
            auto deviceLayout = new QHBoxLayout;
            m_deviceComboBox = new QComboBox;
            deviceLayout->addWidget(m_deviceComboBox, 1);
            auto testDeviceButton = new QPushButton(tr("&Test"));
            deviceLayout->addWidget(testDeviceButton);
            auto deviceControlPanelButton = new QPushButton(tr("Control &Panel"));
            deviceLayout->addWidget(deviceControlPanelButton);
            auto deviceLayoutLabel = new QLabel(tr("Audio &device"));
            deviceLayoutLabel->setBuddy(m_deviceComboBox);
            audioOutputLayout->addRow(deviceLayoutLabel, deviceLayout);
            m_bufferSizeComboBox = new QComboBox;
            audioOutputLayout->addRow(tr("&Buffer size"), m_bufferSizeComboBox);
            m_sampleRateComboBox = new QComboBox;
            audioOutputLayout->addRow(tr("&Sample rate"), m_sampleRateComboBox);
            m_hotPlugModeComboBox = new QComboBox;
            m_hotPlugModeComboBox->addItems({tr("Notify when any device added or removed"),
                                             tr("Notify when current device removed"),
                                             tr("Do not notify")});
            audioOutputLayout->addRow(tr("&Hot plug notification"), m_hotPlugModeComboBox);

            auto deviceGainLayoutLabel = new QLabel(tr("Device &Gain (dB)"));
            auto deviceGainLayout = new QHBoxLayout;
            m_deviceGainSlider = new SVS::SeekBar;
            m_deviceGainSlider->setRange(SVS::DecibelLinearizer::decibelToLinearValue(-96), SVS::DecibelLinearizer::decibelToLinearValue(6));
            m_deviceGainSlider->setDisplayValueConverter([](double v) { return SVS::DecibelLinearizer::linearValueToDecibel(v); });
            deviceGainLayout->addWidget(m_deviceGainSlider);
            m_deviceGainSpinBox = new SVS::ExpressionDoubleSpinBox;
            m_deviceGainSpinBox->setDecimals(1);
            m_deviceGainSpinBox->setRange(-96, 6);
            m_deviceGainSpinBox->setSpecialValueText("-INF");
            deviceGainLayout->addWidget(m_deviceGainSpinBox);
            deviceGainLayoutLabel->setBuddy(m_deviceGainSpinBox);
            audioOutputLayout->addRow(deviceGainLayoutLabel, deviceGainLayout);

            auto devicePanLayoutLabel = new QLabel(tr("Device &Pan"));
            auto devicePanLayout = new QHBoxLayout;
            m_devicePanSlider = new SVS::SeekBar;
            m_devicePanSlider->setRange(-100, 100);
            m_devicePanSlider->setInterval(1);
            devicePanLayout->addWidget(m_devicePanSlider);
            m_devicePanSpinBox = new SVS::ExpressionSpinBox;
            m_devicePanSpinBox->setRange(-100, 100);
            devicePanLayout->addWidget(m_devicePanSpinBox);
            devicePanLayoutLabel->setBuddy(m_devicePanSpinBox);
            audioOutputLayout->addRow(devicePanLayoutLabel, devicePanLayout);

            audioOutputGroupBox->setLayout(audioOutputLayout);
            mainLayout->addWidget(audioOutputGroupBox);

            auto playbackGroupBox = new QGroupBox(tr("Playback"));
            auto playbackLayout = new QFormLayout;
            m_playHeadBehaviorComboBox = new QComboBox;
            m_playHeadBehaviorComboBox->addItems({
                tr("Return to the start position after stopped"),
                tr("Keep at current position after stopped, and play from current position next time"),
                tr("Keep at current position after stopped, but play from the start position next time")
            });
            playbackLayout->addRow(tr("Playhead behavior"),
                                   m_playHeadBehaviorComboBox);
            m_closeDeviceOnPlaybackStopCheckBox =
                new QCheckBox(tr("&Close audio device when playback is stopped"));
            playbackLayout->addRow(m_closeDeviceOnPlaybackStopCheckBox);
            playbackGroupBox->setLayout(playbackLayout);
            mainLayout->addWidget(playbackGroupBox);

            auto fileGroupBox = new QGroupBox(tr("File Caching"));
            auto fileLayout = new QFormLayout;
            m_fileBufferingReadAheadSizeSpinBox = new SVS::ExpressionSpinBox;
            m_fileBufferingReadAheadSizeSpinBox->setRange(0, std::numeric_limits<int>::max());
            fileLayout->addRow(tr("&File reading buffer size (samples)"), m_fileBufferingReadAheadSizeSpinBox);
            fileGroupBox->setLayout(fileLayout);
            mainLayout->addWidget(fileGroupBox);
            mainLayout->addStretch();
            setLayout(mainLayout);

            auto outputSys = AudioSystem::outputSystem();

            connect(testDeviceButton, &QPushButton::clicked, this, [=] {
                if (!outputSys->isReady()) {
                    QMessageBox msgBox(this);
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.setText(tr("Cannot start audio playback"));
                    msgBox.setInformativeText(
                        tr("Please check the status of the audio driver and device."));
                    msgBox.exec();
                } else {
                    DeviceTester::playTestSound(false);
                }
            });

            connect(deviceControlPanelButton, &QPushButton::clicked, this, [=] {
                if (AudioSystem::outputSystem()->outputContext()->device())
                    AudioSystem::outputSystem()->outputContext()->device()->openControlPanel();
            });

            updateDriverComboBox();

            m_hotPlugModeComboBox->setCurrentIndex(
                AudioSystem::outputSystem()->outputContext()->hotPlugNotificationMode());

            m_deviceGainSlider->setValue(gainToSliderValue(outputSys->outputContext()->controlMixer()->gain()));
            connect(m_deviceGainSlider, &SVS::SeekBar::valueChanged, this, [=](double value) {
                updateGain(sliderValueToGain(value));
            });
            m_deviceGainSpinBox->setValue(talcs::Decibels::gainToDecibels(outputSys->outputContext()->controlMixer()->gain()));
            connect(m_deviceGainSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
                updateGain(talcs::Decibels::decibelsToGain(static_cast<float>(value)));
            });
            m_devicePanSlider->setValue(panToSliderValue(outputSys->outputContext()->controlMixer()->pan()));
            connect(m_devicePanSlider, &SVS::SeekBar::valueChanged, this, [=](int value) {
                updatePan(sliderValueToPan(value));
            });
            m_devicePanSpinBox->setValue(panToSliderValue(outputSys->outputContext()->controlMixer()->pan()));
            connect(m_devicePanSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
                updatePan(sliderValueToPan(value));
            });

            m_playHeadBehaviorComboBox->setCurrentIndex(AudioSettings::playheadBehavior());

            m_fileBufferingReadAheadSizeSpinBox->setValue(AudioSettings::fileBufferingReadAheadSize());
        }

        void accept() const {
            AudioSystem::outputSystem()->setHotPlugNotificationMode(
                static_cast<talcs::OutputContext::HotPlugNotificationMode>(
                    m_hotPlugModeComboBox->currentIndex()));
            AudioSystem::outputSystem()->setFileBufferingReadAheadSize(m_fileBufferingReadAheadSizeSpinBox->value());
            AudioSystem::vstConnectionSystem()->setFileBufferingReadAheadSize(m_fileBufferingReadAheadSizeSpinBox->value());
            AudioSettings::setDeviceGain(AudioSystem::outputSystem()->outputContext()->controlMixer()->gain());
            AudioSettings::setDevicePan(AudioSystem::outputSystem()->outputContext()->controlMixer()->pan());
            AudioSettings::setFileBufferingReadAheadSize(m_fileBufferingReadAheadSizeSpinBox->value());
            AudioSettings::setPlayheadBehavior(m_playHeadBehaviorComboBox->currentIndex());
            // TODO
        }

        QComboBox *m_driverComboBox = nullptr;
        QComboBox *m_deviceComboBox = nullptr;
        QComboBox *m_bufferSizeComboBox = nullptr;
        QComboBox *m_sampleRateComboBox = nullptr;
        QComboBox *m_hotPlugModeComboBox = nullptr;
        SVS::SeekBar *m_deviceGainSlider = nullptr;
        SVS::ExpressionDoubleSpinBox *m_deviceGainSpinBox = nullptr;
        SVS::SeekBar *m_devicePanSlider = nullptr;
        SVS::ExpressionSpinBox *m_devicePanSpinBox = nullptr;
        QComboBox *m_playHeadBehaviorComboBox = nullptr;
        QCheckBox *m_closeDeviceOnPlaybackStopCheckBox = nullptr;
        SVS::ExpressionSpinBox *m_fileBufferingReadAheadSizeSpinBox = nullptr;

        void updateDriverComboBox();
        void updateDeviceComboBox();
        void updateBufferSizeAndSampleRateComboBox();

        void updateGain(double gain);
        void updatePan(double pan);
    };

    OutputPlaybackPage::OutputPlaybackPage(QObject *parent)
        : Core::ISettingPage(QStringLiteral("audio.OutputPlayback"), parent) {
        setTitle([] { return tr("Output and Playback"); });
        setDescription(
            [] { return tr("Specify the audio output device and playback behaviors."); });
    }
    OutputPlaybackPage::~OutputPlaybackPage() = default;
    QString OutputPlaybackPage::sortKeyword() const {
        return QStringLiteral("OutputPlayback");
    }
    QWidget *OutputPlaybackPage::widget() {
        if (m_widget)
            return m_widget;
        m_widget = new OutputPlaybackPageWidget;
        return m_widget;
    }
    bool OutputPlaybackPage::accept() {
        if (!m_widget)
            return true;
        m_widget->accept();
        return true;
    }
    void OutputPlaybackPage::finish() {
    }

    void OutputPlaybackPageWidget::updateDriverComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto driverList = outputSys->outputContext()->driverManager()->drivers();
        for (int i = 0; i < driverList.size(); i++) {
            m_driverComboBox->addItem(talcs::SDLAudioDriverDisplayNameHelper::getDisplayName(driverList[i]),
                                      driverList[i]);
            if (outputSys->outputContext()->driver() && driverList[i] == outputSys->outputContext()->driver()->name())
                m_driverComboBox->setCurrentIndex(i);
        }
        if (!outputSys->outputContext()->driver()) {
            m_driverComboBox->addItem(tr("(Not working)"));
            m_driverComboBox->setCurrentIndex(m_driverComboBox->count() - 1);
        }

        updateDeviceComboBox();

        if (outputSys->outputContext()->driver()) {
            connect(outputSys->outputContext()->driver(), &talcs::AudioDriver::deviceChanged, this, [=] {
                disconnect(m_deviceComboBox, nullptr, this, nullptr);
                m_deviceComboBox->clear();
                disconnect(m_bufferSizeComboBox, nullptr, this, nullptr);
                m_bufferSizeComboBox->clear();
                disconnect(m_sampleRateComboBox, nullptr, this, nullptr);
                m_sampleRateComboBox->clear();
                updateDeviceComboBox();
            });
        }

        connect(m_driverComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                [=](int index) {
                    auto newDrvName = m_driverComboBox->itemData(index).toString();
                    if (newDrvName.isEmpty())
                        return;
                    disconnect(m_deviceComboBox, nullptr, this, nullptr);
                    m_deviceComboBox->clear();
                    disconnect(m_bufferSizeComboBox, nullptr, this, nullptr);
                    m_bufferSizeComboBox->clear();
                    disconnect(m_sampleRateComboBox, nullptr, this, nullptr);
                    m_sampleRateComboBox->clear();
                    if (!outputSys->setDriver(newDrvName)) {
                        QMessageBox::warning(
                            this, {},
                            tr("Cannot initialize %1 driver")
                                .arg(talcs::SDLAudioDriverDisplayNameHelper::getDisplayName(newDrvName)));
                        if (m_driverComboBox->itemData(m_driverComboBox->count() - 1).isNull()) {
                            m_driverComboBox->setCurrentIndex(m_driverComboBox->count() - 1);
                        } else {
                            m_driverComboBox->addItem(tr("(Not working)"));
                            m_driverComboBox->setCurrentIndex(m_driverComboBox->count() - 1);
                        }
                    } else {
                        if (m_driverComboBox->itemData(m_driverComboBox->count() - 1).isNull()) {
                            m_driverComboBox->removeItem(m_driverComboBox->count() - 1);
                        }
                        updateDeviceComboBox();
                    }
                });
    }
    void OutputPlaybackPageWidget::updateDeviceComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto deviceList = outputSys->outputContext()->driver()->devices();
        for (int i = 0; i < deviceList.size(); i++) {
            m_deviceComboBox->addItem(deviceList[i], deviceList[i]);
            if (outputSys->outputContext()->device() && deviceList[i] == outputSys->outputContext()->device()->name())
                m_deviceComboBox->setCurrentIndex(i);
        }
        if (!outputSys->outputContext()->device()) {
            m_deviceComboBox->addItem(tr("(Not working)"));
            m_deviceComboBox->setCurrentIndex(m_deviceComboBox->count() - 1);
        }

        if (outputSys->outputContext()->device()) {
            updateBufferSizeAndSampleRateComboBox();
        }

        connect(
            m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [=](int index) {
                auto newDevName = m_deviceComboBox->itemData(index).toString();
                if (newDevName.isNull())
                    return;
                if (!outputSys->setDevice(newDevName)) {
                    for (int i = 0; i < m_deviceComboBox->count(); i++) {
                        if ((!outputSys->outputContext()->device() && m_deviceComboBox->itemData(i).isNull()) ||
                            (outputSys->outputContext()->device() && outputSys->outputContext()->device()->name() ==
                                                        m_deviceComboBox->itemData(i).toString())) {
                            QSignalBlocker blocker(m_deviceComboBox);
                            m_deviceComboBox->setCurrentIndex(i);
                            break;
                        }
                    }
                    QMessageBox::warning(this, {},
                                         tr("Audio device %1 is not available").arg(newDevName));
                } else {
                    if (m_deviceComboBox->itemData(m_deviceComboBox->count() - 1).isNull()) {
                        m_deviceComboBox->removeItem(m_deviceComboBox->count() - 1);
                    }
                    disconnect(m_bufferSizeComboBox, nullptr, this, nullptr);
                    m_bufferSizeComboBox->clear();
                    disconnect(m_sampleRateComboBox, nullptr, this, nullptr);
                    m_sampleRateComboBox->clear();
                    updateBufferSizeAndSampleRateComboBox();
                }
            });
    }
    void OutputPlaybackPageWidget::updateBufferSizeAndSampleRateComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto bufferSizeList = outputSys->outputContext()->device()->availableBufferSizes();
        for (int i = 0; i < bufferSizeList.size(); i++) {
            m_bufferSizeComboBox->addItem(QString::number(bufferSizeList[i]), bufferSizeList[i]);
            if (bufferSizeList[i] == outputSys->outputContext()->adoptedBufferSize())
                m_bufferSizeComboBox->setCurrentIndex(i);
        }
        connect(m_bufferSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                [=](int index) {
                    auto newBufferSize = m_bufferSizeComboBox->itemData(index).value<qint64>();
                    outputSys->setAdoptedBufferSize(newBufferSize);
                });
        auto sampleRateList = outputSys->outputContext()->device()->availableSampleRates();
        for (int i = 0; i < sampleRateList.size(); i++) {
            m_sampleRateComboBox->addItem(QString::number(sampleRateList[i]), sampleRateList[i]);
            if (sampleRateList[i] == outputSys->outputContext()->adoptedSampleRate())
                m_sampleRateComboBox->setCurrentIndex(i);
        }
        connect(m_sampleRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                [=](int index) {
                    auto newSampleRate = m_sampleRateComboBox->itemData(index).value<double>();
                    outputSys->setAdoptedSampleRate(newSampleRate);
                });
    }
    void OutputPlaybackPageWidget::updateGain(double gain) {
        QSignalBlocker sliderBlocker(m_deviceGainSlider);
        QSignalBlocker spinBoxBlocker(m_deviceGainSpinBox);

        m_deviceGainSlider->setValue(gainToSliderValue(static_cast<float>(gain)));
        m_deviceGainSpinBox->setValue(talcs::Decibels::gainToDecibels(static_cast<float>(gain)));
        AudioSystem::outputSystem()->outputContext()->controlMixer()->setGain(static_cast<float>(gain));
    }
    void OutputPlaybackPageWidget::updatePan(double pan) {
        QSignalBlocker sliderBlocker(m_devicePanSlider);
        QSignalBlocker spinBoxBlocker(m_devicePanSpinBox);
        
        m_devicePanSlider->setValue(panToSliderValue(pan));
        m_devicePanSpinBox->setValue(panToSliderValue(pan));
        AudioSystem::outputSystem()->outputContext()->controlMixer()->setPan(static_cast<float>(pan));
    }

}

#include "outputplaybackpage.moc"