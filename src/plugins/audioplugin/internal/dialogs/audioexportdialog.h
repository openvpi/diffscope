#ifndef AUDIO_AUDIOEXPORTDIALOG_H
#define AUDIO_AUDIOEXPORTDIALOG_H

#include <QDialog>

#include <audioplugin/audioexporter.h>

namespace Core {
    class IProjectWindow;
}

class QLabel;
class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;
class QSlider;
class QDoubleSpinBox;
class QListWidget;
class QCheckBox;
class QRadioButton;

namespace Audio::Internal {

    class AudioExportDialog : public QDialog {
        Q_OBJECT
    public:
        explicit AudioExportDialog(QWidget *parent = nullptr) : AudioExportDialog(nullptr, parent) {}
        explicit AudioExportDialog(Core::IProjectWindow *windowHandle, QWidget *parent = nullptr);
        ~AudioExportDialog() override;

    private:
        QComboBox *m_presetComboBox;
        QPushButton *m_presetDeleteButton;
        QLineEdit *m_fileDirectoryEdit;
        QLineEdit *m_fileNameEdit;
        QLabel *m_fileNamePreviewLabel;
        QComboBox *m_fileTypeComboBox;
        QComboBox *m_formatMonoComboBox;
        QComboBox *m_formatOptionComboBox;
        QSlider *m_vbrSlider;
        QComboBox *m_formatSampleRateComboBox;
        QComboBox *m_sourceComboBox;
        QListWidget *m_sourceListWidget;
        QComboBox *m_mixingOptionComboBox;
        QCheckBox *m_enableMuteSoloCheckBox;
        QRadioButton *m_rangeSelectAllRadio;
        QRadioButton *m_rangeLoopIntervalRadio;
        QCheckBox *m_keepOpenCheckBox;

        QPushButton *m_warningButton;

        QString m_warningText;

        AudioExporter *m_audioExporter;

        QStringList projectTrackList() const;

        void browseFile();
        void showWarnings();
        void showDryRunResult();
        void updateConfig();
        void updateView();

        bool skipUpdateFlag = false;

        void runExport();

        bool askWarningBeforeExport(AudioExporter::Warning warning, bool canIgnored = false);

        void saveTemporaryPreset();
        void restoreTemporaryPreset();
        bool hasTemporaryPreset() const;
    };

}

#endif // AUDIO_AUDIOEXPORTDIALOG_H
