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
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDevice.h>

#include <SVSCraftWidgets/expressiondoublespinbox.h>

#include <CoreApi/iloader.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/devicetesteraddon.h>

namespace Audio {

    static inline double sliderValueToGain(int sliderValue) {
        return std::pow(sliderValue / 52056.0, 6);
    }
    static inline int gainToSliderValue(double gain) {
        return static_cast<int>(std::pow(gain, 1.0 / 6.0) * 52056.0);
    }
    static inline double sliderValueToPan(int sliderValue) {
        return (sliderValue - 32768) / 32768.0;
    }
    static inline int panToSliderValue(double pan) {
        return static_cast<int>(pan * 32768.0) + 32768;
    }

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

        m_widget = new QWidget;
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
        m_deviceGainSlider = new QSlider(Qt::Horizontal);
        m_deviceGainSlider->setRange(0, 65535);
        deviceGainLayout->addWidget(m_deviceGainSlider);
        m_deviceGainSpinBox = new SVS::ExpressionDoubleSpinBox;
        m_deviceGainSpinBox->setRange(-96, 6);
        m_deviceGainSpinBox->setSpecialValueText("-INF");
        deviceGainLayout->addWidget(m_deviceGainSpinBox);
        deviceGainLayoutLabel->setBuddy(m_deviceGainSpinBox);
        audioOutputLayout->addRow(deviceGainLayoutLabel, deviceGainLayout);
        
        auto devicePanLayoutLabel = new QLabel(tr("Device &Pan"));
        auto devicePanLayout = new QHBoxLayout;
        m_devicePanSlider = new QSlider(Qt::Horizontal);
        m_devicePanSlider->setRange(0, 65536);
        devicePanLayout->addWidget(m_devicePanSlider);
        m_devicePanSpinBox = new SVS::ExpressionDoubleSpinBox;
        m_devicePanSpinBox->setRange(-1, 1);
        devicePanLayout->addWidget(m_devicePanSpinBox);
        devicePanLayoutLabel->setBuddy(m_devicePanSpinBox);
        audioOutputLayout->addRow(devicePanLayoutLabel, devicePanLayout);

        audioOutputGroupBox->setLayout(audioOutputLayout);
        mainLayout->addWidget(audioOutputGroupBox);

        auto playbackGroupBox = new QGroupBox(tr("Playback"));
        auto playbackLayout = new QFormLayout;
        m_playHeadBehaviorComboBox = new QComboBox;
        m_playHeadBehaviorComboBox->addItems({
            tr("Start position"),
            tr("End position"),
        });
        playbackLayout->addRow(tr("After playback is stopped, &move the play head to"),
                               m_playHeadBehaviorComboBox);
        m_closeDeviceOnPlaybackStopCheckBox =
            new QCheckBox(tr("&Close audio device when playback is stopped"));
        playbackLayout->addRow(m_closeDeviceOnPlaybackStopCheckBox);
        playbackGroupBox->setLayout(playbackLayout);
        mainLayout->addWidget(playbackGroupBox);

        auto fileGroupBox = new QGroupBox(tr("File Caching"));
        auto fileLayout = new QFormLayout;
        m_fileBufferSizeMsecSpinBox = new SVS::ExpressionDoubleSpinBox;
        m_fileBufferSizeMsecSpinBox->setRange(0.0, std::numeric_limits<double>::max());
        fileLayout->addRow(tr("&File reading buffer size (millisecond)"),
                           m_fileBufferSizeMsecSpinBox);
        fileGroupBox->setLayout(fileLayout);
        mainLayout->addWidget(fileGroupBox);
        mainLayout->addStretch();
        m_widget->setLayout(mainLayout);

        auto outputSys = AudioSystem::outputSystem();

        connect(testDeviceButton, &QPushButton::clicked, this, [=] {
            if (!outputSys->makeReady()) {
                QMessageBox msgBox(m_widget);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText(tr("Cannot start audio playback"));
                msgBox.setInformativeText(
                    tr("Please check the status of the audio driver and device."));
                msgBox.exec();
            } else {
                DeviceTesterAddOn::testDevice(false);
            }
        });

        connect(deviceControlPanelButton, &QPushButton::clicked, this, [=] {
            if (AudioSystem::outputSystem()->device())
                AudioSystem::outputSystem()->device()->openControlPanel();
        });

        updateDriverComboBox();

        m_hotPlugModeComboBox->setCurrentIndex(
            AudioSystem::outputSystem()->hotPlugNotificationMode());

        m_deviceGainSlider->setValue(gainToSliderValue(outputSys->gain()));
        connect(m_deviceGainSlider, &QSlider::valueChanged, this, [=](int value) {
            updateGain(sliderValueToGain(value));
        });
        m_deviceGainSpinBox->setValue(talcs::Decibels::gainToDecibels(outputSys->gain()));
        connect(m_deviceGainSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            updateGain(talcs::Decibels::decibelsToGain(static_cast<float>(value)));
        });
        m_devicePanSlider->setValue(panToSliderValue(outputSys->pan()));
        connect(m_devicePanSlider, &QSlider::valueChanged, this, [=](int value) {
            updatePan(sliderValueToPan(value));
        });
        m_devicePanSpinBox->setValue(outputSys->pan());
        connect(m_devicePanSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            updatePan(value);
        });

        return m_widget;
    }
    bool OutputPlaybackPage::accept() {
        if (!m_widget)
            return true;
        AudioSystem::outputSystem()->setHotPlugNotificationMode(
            static_cast<OutputSystem::HotPlugNotificationMode>(
                m_hotPlugModeComboBox->currentIndex()));
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["deviceGain"] = AudioSystem::outputSystem()->gain();
        obj["devicePan"] = AudioSystem::outputSystem()->pan();
        settings["Audio"] = obj;
        // TODO
        return true;
    }
    void OutputPlaybackPage::finish() {
    }

    void OutputPlaybackPage::updateDriverComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto driverList = outputSys->driverManager()->drivers();
        for (int i = 0; i < driverList.size(); i++) {
            m_driverComboBox->addItem(OutputSystem::driverDisplayName(driverList[i]),
                                      driverList[i]);
            if (outputSys->driver() && driverList[i] == outputSys->driver()->name())
                m_driverComboBox->setCurrentIndex(i);
        }
        if (!outputSys->driver()) {
            m_driverComboBox->addItem(tr("(Not working)"));
            m_driverComboBox->setCurrentIndex(m_driverComboBox->count() - 1);
        }

        updateDeviceComboBox();

        if (outputSys->driver()) {
            connect(outputSys->driver(), &talcs::AudioDriver::deviceChanged, this, [=] {
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
                            m_widget, {},
                            tr("Cannot initialize %1 driver")
                                .arg(OutputSystem::driverDisplayName(newDrvName)));
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
    void OutputPlaybackPage::updateDeviceComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto deviceList = outputSys->driver()->devices();
        for (int i = 0; i < deviceList.size(); i++) {
            m_deviceComboBox->addItem(deviceList[i], deviceList[i]);
            if (outputSys->device() && deviceList[i] == outputSys->device()->name())
                m_deviceComboBox->setCurrentIndex(i);
        }
        if (!outputSys->device()) {
            m_deviceComboBox->addItem(tr("(Not working)"));
            m_deviceComboBox->setCurrentIndex(m_deviceComboBox->count() - 1);
        }

        if (outputSys->device()) {
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
                        if ((!outputSys->device() && m_deviceComboBox->itemData(i).isNull()) ||
                            (outputSys->device() && outputSys->device()->name() ==
                                                        m_deviceComboBox->itemData(i).toString())) {
                            QSignalBlocker blocker(m_deviceComboBox);
                            m_deviceComboBox->setCurrentIndex(i);
                            break;
                        }
                    }
                    QMessageBox::warning(m_widget, {},
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
    void OutputPlaybackPage::updateBufferSizeAndSampleRateComboBox() {
        auto outputSys = AudioSystem::outputSystem();

        auto bufferSizeList = outputSys->device()->availableBufferSizes();
        for (int i = 0; i < bufferSizeList.size(); i++) {
            m_bufferSizeComboBox->addItem(QString::number(bufferSizeList[i]), bufferSizeList[i]);
            if (bufferSizeList[i] == outputSys->adoptedBufferSize())
                m_bufferSizeComboBox->setCurrentIndex(i);
        }
        connect(m_bufferSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                [=](int index) {
                    auto newBufferSize = m_bufferSizeComboBox->itemData(index).value<qint64>();
                    outputSys->setAdoptedBufferSize(newBufferSize);
                });
        auto sampleRateList = outputSys->device()->availableSampleRates();
        for (int i = 0; i < sampleRateList.size(); i++) {
            m_sampleRateComboBox->addItem(QString::number(sampleRateList[i]), sampleRateList[i]);
            if (sampleRateList[i] == outputSys->adoptedSampleRate())
                m_sampleRateComboBox->setCurrentIndex(i);
        }
        connect(m_sampleRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                [=](int index) {
                    auto newSampleRate = m_sampleRateComboBox->itemData(index).value<double>();
                    outputSys->setAdoptedSampleRate(newSampleRate);
                });
    }
    void OutputPlaybackPage::updateGain(double gain) {
        QSignalBlocker sliderBlocker(m_deviceGainSlider);
        QSignalBlocker spinBoxBlocker(m_deviceGainSpinBox);

        m_deviceGainSlider->setValue(gainToSliderValue(gain));
        m_deviceGainSpinBox->setValue(talcs::Decibels::gainToDecibels(static_cast<float>(gain)));
        AudioSystem::outputSystem()->setGainAndPan(static_cast<float>(gain), AudioSystem::outputSystem()->pan());
    }
    void OutputPlaybackPage::updatePan(double pan) {
        QSignalBlocker sliderBlocker(m_devicePanSlider);
        QSignalBlocker spinBoxBlocker(m_devicePanSpinBox);
        
        m_devicePanSlider->setValue(panToSliderValue(pan));
        m_devicePanSpinBox->setValue(pan);
        AudioSystem::outputSystem()->setGainAndPan(AudioSystem::outputSystem()->gain(), static_cast<float>(pan));
    }

}