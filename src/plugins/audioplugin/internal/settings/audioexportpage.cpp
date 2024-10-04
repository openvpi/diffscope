#include "audioexportpage.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QThread>
#include <QLabel>
#include <QPushButton>

#include <audioplugin/internal/audiosettings.h>

namespace Audio::Internal {

    class AudioExportPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit AudioExportPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QFormLayout;
            auto enableClippingCheckCheckBox = new QCheckBox(tr("Enable &clipping check"));
            mainLayout->addRow(enableClippingCheckCheckBox);
            auto resetWarningDialogsLayout = new QHBoxLayout;
            auto resetWarningDialogsButton = new QPushButton(tr("Reset &Warning Dialogs"));
            resetWarningDialogsLayout->addWidget(resetWarningDialogsButton);
            auto resetLabel = new QLabel;
            resetWarningDialogsLayout->addWidget(resetLabel);
            resetWarningDialogsLayout->addStretch();
            mainLayout->addRow(resetWarningDialogsLayout);
            setLayout(mainLayout);

            enableClippingCheckCheckBox->setChecked(m_clippingCheckEnabled = AudioSettings::audioExporterClippingCheckEnabled());
            connect(enableClippingCheckCheckBox, &QAbstractButton::clicked, this, [=](bool checked) {
                m_clippingCheckEnabled = checked;
            });
            connect(resetWarningDialogsButton, &QAbstractButton::clicked, this, [=] {
                AudioSettings::setAudioExporterIgnoredWarningFlag(0);
                resetWarningDialogsButton->setDisabled(true);
                resetLabel->setText(tr("Warning dialogs have been reset"));
            });
        }

        void accept() {
            AudioSettings::setAudioExporterClippingCheckEnabled(m_clippingCheckEnabled);
        }

    private:
        bool m_clippingCheckEnabled;
    };

    AudioExportPage::AudioExportPage(QObject *parent) : Core::ISettingPage("audio.AudioExport", parent) {
        setTitle([] { return tr("Audio Export"); });
        setDescription([] { return tr("Configure audio export preferences."); });
    }

    AudioExportPage::~AudioExportPage() = default;

    QString AudioExportPage::sortKeyword() const {
        return QStringLiteral("AudioExport");
    }

    QWidget *AudioExportPage::widget() {
        if (m_widget)
            return m_widget;
        return m_widget = new AudioExportPageWidget;
    }
    bool AudioExportPage::accept() {
        if (m_widget)
            m_widget->accept();
        return true;
    }
    void AudioExportPage::finish() {
    }
}

#include "audioexportpage.moc"