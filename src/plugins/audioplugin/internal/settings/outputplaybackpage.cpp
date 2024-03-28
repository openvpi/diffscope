#include "outputplaybackpage.h"

#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

#include <SVSCraftWidgets/expressiondoublespinbox.h>

namespace Audio {
    OutputPlaybackPage::OutputPlaybackPage(QObject *parent) : Core::ISettingPage("audio.OutputPlayback", parent) {
        setTitle([] { return tr("Output & Playback"); });
        setDescription([] { return tr("Specify the Audio Output Device and Playback Behaviors."); });
    }
    OutputPlaybackPage::~OutputPlaybackPage() = default;
    QString OutputPlaybackPage::sortKeyword() const {
        return ISettingPage::sortKeyword();
    }
    QWidget *OutputPlaybackPage::widget() {
        m_widget = new QWidget;
        auto mainLayout = new QVBoxLayout;
        
        auto audioOutputGroupBox = new QGroupBox(tr("Audio Driver && Device"));
        auto audioOutputLayout = new QFormLayout;
        m_driverComboBox = new QComboBox;
        audioOutputLayout->addRow(tr("Audio D&river"), m_driverComboBox);
        auto deviceLayout = new QHBoxLayout;
        m_deviceComboBox = new QComboBox;
        deviceLayout->addWidget(m_deviceComboBox, 1);
        auto testDeviceButton = new QPushButton(tr("&Test"));
        deviceLayout->addWidget(testDeviceButton);
        auto deviceControlPanelButton = new QPushButton(tr("Control &Panel"));
        deviceLayout->addWidget(deviceControlPanelButton);
        auto deviceLayoutLabel = new QLabel(tr("Audio &Device"));
        deviceLayoutLabel->setBuddy(m_deviceComboBox);
        audioOutputLayout->addRow(deviceLayoutLabel, deviceLayout);
        m_bufferSizeComboBox = new QComboBox;
        audioOutputLayout->addRow(tr("&Buffer Size"), m_bufferSizeComboBox);
        m_sampleRateComboBox = new QComboBox;
        audioOutputLayout->addRow(tr("&Sample Rate"), m_sampleRateComboBox);
        m_hotPlugModeComboBox = new QComboBox;
        m_hotPlugModeComboBox->addItems({
            tr("Notify when any device added or removed"),
            tr("Notify when current device removed"),
            tr("Do not notify")});
        audioOutputLayout->addRow(tr("&Hot Plug Notification"), m_hotPlugModeComboBox);
        audioOutputGroupBox->setLayout(audioOutputLayout);
        mainLayout->addWidget(audioOutputGroupBox);

        auto playbackGroupBox = new QGroupBox(tr("Playback"));
        auto playbackLayout = new QFormLayout;
        m_playHeadBehaviorComboBox = new QComboBox;
        m_playHeadBehaviorComboBox->addItems({
            tr("Start position"),
            tr("End position"),
        });
        playbackLayout->addRow("After Playback is Stopped, &Move the Play Head to", m_playHeadBehaviorComboBox);
        m_closeDeviceOnPlaybackStopCheckBox = new QCheckBox(tr("&Close Audio Device When Playback is Stopped"));
        playbackLayout->addRow(m_closeDeviceOnPlaybackStopCheckBox);
        playbackGroupBox->setLayout(playbackLayout);
        mainLayout->addWidget(playbackGroupBox);

        auto fileGroupBox = new QGroupBox(tr("File Caching"));
        auto fileLayout = new QFormLayout;
        m_fileBufferSizeMsecSpinBox = new SVS::ExpressionDoubleSpinBox;
        m_fileBufferSizeMsecSpinBox->setRange(0.0, std::numeric_limits<double>::max());
        fileLayout->addRow(tr("&File Reading Buffer Size (Millisecond)"), m_fileBufferSizeMsecSpinBox);
        fileGroupBox->setLayout(fileLayout);
        mainLayout->addWidget(fileGroupBox);
        mainLayout->addStretch();
        m_widget->setLayout(mainLayout);
        return m_widget;
    }
    bool OutputPlaybackPage::accept() {
        return true;
    }
    void OutputPlaybackPage::finish() {
    }
} // Audio