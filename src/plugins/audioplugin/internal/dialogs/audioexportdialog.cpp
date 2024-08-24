#include "audioexportdialog.h"

#include <limits>

#include <QBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QLineEdit>
#include <QMenu>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QListWidget>
#include <QLabel>

namespace Audio::Internal {
    AudioExportDialog::AudioExportDialog(QWidget *parent) : QDialog(parent) {
        setWindowTitle(tr("Export Audio"));
        auto mainLayout = new QVBoxLayout;
        
        auto presetLayout = new QFormLayout;
        auto presetOptionLayout = new QHBoxLayout;
        m_presetComboBox = new QComboBox;
        presetOptionLayout->addWidget(m_presetComboBox, 1);
        auto presetSaveAsButton = new QPushButton(tr("Save &As"));
        presetOptionLayout->addWidget(presetSaveAsButton);
        m_presetDeleteButton = new QPushButton(tr("&Delete"));
        presetOptionLayout->addWidget(m_presetDeleteButton);
        auto presetLabel = new QLabel(tr("&Preset"));
        presetLabel->setBuddy(m_presetComboBox);
        presetLayout->addRow(presetLabel, presetOptionLayout);
        mainLayout->addLayout(presetLayout);

        auto mainOptionsLayout = new QVBoxLayout;

        auto leftLayout = new QVBoxLayout;
        auto rightLayout = new QVBoxLayout;

        auto pathGroupBox = new QGroupBox(tr("File Path"));
        auto pathLayout = new QFormLayout;
        auto filePathBrowseButton = new QPushButton(tr("&Browse"));
        pathLayout->addRow(filePathBrowseButton);
        auto fileNameLayout = new QHBoxLayout;
        m_fileNameEdit = new QLineEdit;
        fileNameLayout->addWidget(m_fileNameEdit, 1);
        auto fileNameTemplateButton = new QPushButton;
        fileNameTemplateButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMenuButton));
        fileNameTemplateButton->setToolTip(tr("Template"));
        auto fileNameTemplateMenu = new QMenu(this);
        fileNameTemplateButton->setMenu(fileNameTemplateMenu);
        fileNameLayout->addWidget(fileNameTemplateButton);
        auto fileNameTemplateContextHelpButton = new QPushButton;
        fileNameTemplateContextHelpButton->setToolTip(tr("<p>You can specify the name (including extension name) of exported files.</p>"
                                                         "<p>Template tags in the file name will be replaced with the corresponding text. The following are the available template tags:</p>"
                                                         "<ul>"
                                                         "<li><b>${projectName}</b>: the base name of the project file (excluding extension name)</li>"
                                                         "<li><b>${sampleRate}</b>: the sample rate specified in export configuration</li>"
                                                         "<li><b>${today}</b>: today's date in \"yyyyMMdd\" format (e. g. \"19260817\" for August 17, 1926)</li>"
                                                         "<li><b>${$}</b>: a single \"$\" character</li>"
                                                         "</ul>"
                                                         "<p>In particular, the following template tags are available only when the mixing option is not \"mixed\":</p>"
                                                         "<ul>"
                                                         "<li><b>${trackName}</b>: the name of track</li>"
                                                         "<li><b>${trackIndex}</b>: the index of track (starting from 1)</li>"
                                                         "</ul>"
                                                         "<p>You can select the template tag in the pop-up menu. The selected template tag will be appended to the end of the file name.</p>"));
        fileNameTemplateContextHelpButton->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
        fileNameLayout->addWidget(fileNameTemplateContextHelpButton);
        auto nameLabel = new QLabel(tr("&Name"));
        nameLabel->setBuddy(m_fileNameEdit);
        pathLayout->addRow(nameLabel, fileNameLayout);
        m_fileDirectoryEdit = new QLineEdit;
        m_fileDirectoryEdit->setPlaceholderText(tr("(Project directory)"));
        pathLayout->addRow(tr("&Directory"), m_fileDirectoryEdit);
        m_fileTypeComboBox = new QComboBox;

        // TODO Audio formats

        pathLayout->addRow(tr("&Type"), m_fileTypeComboBox);
        pathGroupBox->setLayout(pathLayout);
        leftLayout->addWidget(pathGroupBox);

        auto formatGroupBox = new QGroupBox(tr("Format"));
        auto formatLayout = new QFormLayout;

        m_formatOptionComboBox = new QComboBox;
        formatLayout->addRow(tr("&Option"), m_formatOptionComboBox);
        m_vbrSlider = new QSlider(Qt::Horizontal);
        formatLayout->addRow(tr("&Quality"), m_vbrSlider);
        m_formatSampleRateComboBox = new QComboBox;
        m_formatSampleRateComboBox->setEditable(true);
        m_formatSampleRateComboBox->setValidator(new QDoubleValidator(0.01, std::numeric_limits<double>::max(), 2));
        m_formatSampleRateComboBox->addItems({"8000",   "11025",  "12000",  "16000",  "22050",  "24000",
                                              "32000",  "44100",  "48000",  "64000",  "88200",  "96000",
                                              "128000", "176400", "192000", "256000", "352800", "384000"});
        formatLayout->addRow(tr("&Sample rate"), m_formatSampleRateComboBox);
        formatGroupBox->setLayout(formatLayout);
        leftLayout->addWidget(formatGroupBox);

        auto mixingGroupBox = new QGroupBox(tr("Mixer"));
        auto mixingLayout = new QFormLayout;
        m_mixingOptionComboBox = new QComboBox;
        m_mixingOptionComboBox->addItems({
            tr("Mixed"),
            tr("Separated"),
            tr("Separated (through master track)"),
        });
        mixingLayout->addRow(tr("&Mixing Option"), m_mixingOptionComboBox);
        m_enableMuteSoloCheckBox = new QCheckBox(tr("Enable m&ute/solo"));
        m_enableMuteSoloCheckBox->setChecked(true);
        mixingLayout->addRow(m_enableMuteSoloCheckBox);
        m_sourceComboBox = new QComboBox;
        m_sourceComboBox->addItems({
            tr("All tracks"),
            tr("Selected tracks"),
            tr("Custom"),
        });
        mixingLayout->addRow(tr("&Source"), m_sourceComboBox);
        m_sourceListWidget = new QListWidget;
        // TODO add tracks

        mixingLayout->addRow(m_sourceListWidget);
        mixingGroupBox->setLayout(mixingLayout);
        rightLayout->addWidget(mixingGroupBox);

        auto timeRangeGroupBox = new QGroupBox(tr("Time Range"));
        auto timeRangeLayout = new QVBoxLayout;
        auto rangeOptionLayout = new QVBoxLayout;
        m_rangeSelectAllRadio = new QRadioButton(tr("A&ll"));
        m_rangeSelectAllRadio->setChecked(true);
        rangeOptionLayout->addWidget(m_rangeSelectAllRadio);
        m_rangeLoopIntervalRadio = new QRadioButton(tr("Loop s&ection"));
        rangeOptionLayout->addWidget(m_rangeLoopIntervalRadio);
        rangeOptionLayout->addStretch();
        timeRangeLayout->addLayout(rangeOptionLayout);
        timeRangeGroupBox->setLayout(timeRangeLayout);
        leftLayout->addWidget(timeRangeGroupBox);

        mainOptionsLayout->addLayout(leftLayout);
        mainOptionsLayout->addLayout(rightLayout);

        mainLayout->addLayout(mainOptionsLayout);

        m_keepOpenCheckBox = new QCheckBox(tr("&Keep this dialog open after successful export"));
        mainLayout->addWidget(m_keepOpenCheckBox);

        auto buttonLayout = new QHBoxLayout;
        auto dryRunButton = new QPushButton(tr("Dry &Run"));
        buttonLayout->addWidget(dryRunButton);
        auto dryRunContextHelpButton = new QPushButton;
        dryRunContextHelpButton->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
        dryRunContextHelpButton->setToolTip(tr("\"Dry Run\" shows the paths of files to export. No files will actually be exported."));
        buttonLayout->addWidget(dryRunContextHelpButton);
        buttonLayout->addStretch();
        m_warningButton = new QPushButton;
        m_warningButton->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
        m_warningButton->setToolTip(tr("Warning"));
        buttonLayout->addWidget(m_warningButton);
        auto exportButton = new QPushButton(tr("Export"));
        // exportButton->setPrimary(true);
        exportButton->setDefault(true);
        buttonLayout->addWidget(exportButton);
        auto cancelButton = new QPushButton(tr("Cancel"));
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        setLayout(mainLayout);
    }
    AudioExportDialog::~AudioExportDialog() {
    }
}