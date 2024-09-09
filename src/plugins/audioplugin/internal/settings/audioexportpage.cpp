#include "audioexportpage.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QThread>
#include <QLabel>

#include <SVSCraftWidgets/expressionspinbox.h>

namespace Audio::Internal {

    class AudioExportPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit AudioExportPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QFormLayout;
            auto enableClippingCheckCheckBox = new QCheckBox(tr("Enable &clipping check"));
            mainLayout->addRow(enableClippingCheckCheckBox);
            auto threadCountSpinBox = new SVS::ExpressionSpinBox;
            threadCountSpinBox->setRange(0, QThread::idealThreadCount());
            threadCountSpinBox->setSpecialValueText(tr("Auto"));
            mainLayout->addRow(tr("Number of &threads for exporting"), threadCountSpinBox);
            auto multiThreadLabel = new QLabel(tr("Audio exporting will be multi-threaded when the mixing option is \"separated\""));
            multiThreadLabel->setWordWrap(true);
            multiThreadLabel->setAlignment(Qt::AlignTop);
            mainLayout->addWidget(multiThreadLabel);
            setLayout(mainLayout);
        }

        void accept() {

        }
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