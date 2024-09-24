#include "audiotroubleshootingdialog.h"

#include <cstdlib>

#include <QApplication>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QCommandLinkButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QPlainTextEdit>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/devicetester.h>

namespace Audio::Internal {

    enum Pages {
        Page_Welcome,
        Page_Test,
        Page_Configure,
        Page_ExternalCheck,
        Page_OK,
        Page_Fail,
    };

    class WelcomePage : public QWizardPage {
        Q_OBJECT
    public:
        explicit WelcomePage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle(tr("<h3>Audio Troubleshooting Wizard</h3>"));
            setSubTitle(tr("<p>This wizard will help you troubleshoot and resolve problems with audio output</p>"));


            auto layout = new QVBoxLayout;
            auto continueButton = new QCommandLinkButton(tr("&Continue"));
            layout->addWidget(continueButton);

            setLayout(layout);

            connect(continueButton, &QAbstractButton::clicked, parent, &QWizard::next);
        }

        int nextId() const override {
            if (AudioSystem::outputSystem()->isReady()) {
                return Page_Test;
            } else {
                return Page_Configure;
            }
        }
    };

    class TestPage : public QWizardPage {
        Q_OBJECT
    public:
        explicit TestPage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle(tr("<h3>Test Audio Device</h3>"));
            setSubTitle(tr("The audio device seems to be working. Please click the \"Test\" button "
                           "to check whether the audio device can play any sound"));
            auto layout = new QVBoxLayout;
            auto testButtonLayout = new QHBoxLayout;
            auto testButton = new QPushButton(tr("&Test"));
            testButtonLayout->addWidget(testButton);
            testButtonLayout->addStretch();
            layout->addLayout(testButtonLayout);
            auto noteLabel = new QLabel(tr("<p>Before testing, please ensure that the device you are monitoring is the one you selected "
                                           "if your computer has multiple audio devices (e.g. speaker and headphone)</p>"
                                           "<p>Did you muted the device or set the volume to zero? This may cause it to not play sound</p>"));
            noteLabel->setWordWrap(true);
            layout->addWidget(noteLabel);
            auto line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            layout->addSpacing(16);
            layout->addWidget(line);
            layout->addSpacing(16);
            auto questionLabel = new QLabel(tr("Is any sound played after clicking the \"Test\" button?"));
            questionLabel->setWordWrap(true);
            layout->addWidget(questionLabel);
            auto yesButton = new QCommandLinkButton(
                tr("&Yes"), tr("I heard the test sound played by the audio device"));
            layout->addWidget(yesButton);
            auto noButton = new QCommandLinkButton(
                tr("&No"), tr("I couldn't hear any sound played by the audio device"));
            layout->addWidget(noButton);

            setLayout(layout);

            connect(testButton, &QAbstractButton::clicked, this, [=] {
                DeviceTester::playTestSound(false);
            });

            connect(yesButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_OK;
                parent->next();
            });
            connect(noButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_Configure;
                parent->next();
            });
        }

        int nextId() const override {
            return m_nextId;
        }

    private:
        int m_nextId = Page_OK;
    };

    class ResultOkPage : public QWizardPage {
        Q_OBJECT
    public:
        explicit ResultOkPage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle(tr("<h3>Everything is OK</h3>"));
            setSubTitle(tr("<p>There is no problem with audio output now</p>"
                           "<p>If you still cannot hear any sound while playing-back your project, "
                           "please check whether there are any problems with the project "
                           "(e.g. tracks are muted or the gain has been set to negative infinity)</p>"));
            setLayout(new QVBoxLayout);
        }

        int nextId() const override {
            return -1;
        }
    };

    class ConfigurePage : public QWizardPage {
        Q_OBJECT
    public:
        explicit ConfigurePage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle(tr("<h3>Configure Audio Device</h3>"));
            setSubTitle(tr("<p>Select a working audio driver and audio device</p>"
                           "<p>After selecting, please click the \"Test\" button to check whether "
                           "the audio device can play any sound</p>"
                           "<p>The default buffer size and sample rate will be used and the device "
                           "gain and pan will be reset. If you want to use a custom specification, "
                           "you can configure it later in Settings > Audio > Output and Playback</p>"
                           "<p>Note: Some audio devices on your computer could be virtual devices. "
                           "They may not output any sound to the physical audio device</p>"));
            auto layout = new QVBoxLayout;
            auto deviceLayout = new QFormLayout;
            auto driverComboBox = new QComboBox;
            deviceLayout->addRow(tr("Audio d&river"), driverComboBox);
            auto deviceComboBox = new QComboBox;
            deviceLayout->addRow(tr("Audio &device"), deviceComboBox);
            layout->addLayout(deviceLayout);
            auto testButtonLayout = new QHBoxLayout;
            auto testButton = new QPushButton(tr("&Test"));
            testButtonLayout->addWidget(testButton);
            testButtonLayout->addStretch();
            layout->addLayout(testButtonLayout);
            auto noteLabel = new QLabel(tr("<p>Before testing, please ensure that the device you are monitoring is the one you selected "
                                           "if your computer has multiple audio devices (e.g. speaker and headphone)</p>"
                                           "<p>Did you muted the device or set the volume to zero? This may cause it to not play sound</p>"));
            noteLabel->setWordWrap(true);
            layout->addWidget(noteLabel);
            auto line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            layout->addSpacing(16);
            layout->addWidget(line);
            layout->addSpacing(16);
            auto questionLabel = new QLabel(tr("Have you selected a working audio device which plays sound after clicking the \"Test\" button?"));
            questionLabel->setWordWrap(true);
            layout->addWidget(questionLabel);
            auto yesButton = new QCommandLinkButton(
                tr("&Yes"), tr("I have selected a working audio device"));
            layout->addWidget(yesButton);
            auto noButton = new QCommandLinkButton(
                tr("&No"), tr("The device I want to use is either not available or not able to play sound"));
            layout->addWidget(noButton);
            setLayout(layout);

            connect(testButton, &QAbstractButton::clicked, this, [=] {
                DeviceTester::playTestSound(false);
            });

            connect(yesButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_OK;
                parent->next();
            });
            connect(noButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_ExternalCheck;
                parent->next();
            });

        }

        int nextId() const override {
            return m_nextId;
        }

    private:
        int m_nextId = Page_OK;
    };

    class ExternalCheckPage : public QWizardPage {
        Q_OBJECT
    public:
        explicit ExternalCheckPage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle("<h3>Check Your System Audio Settings</h3>");
            setSubTitle(tr("Please ensure the audio devices have been connected to your computer and configured properly in the settings of your system"));
            auto layout = new QVBoxLayout;
            auto goToSettingsButtonLayout = new QHBoxLayout;
            auto goToSettingsButton = new QPushButton(
#ifdef Q_OS_MACOS
                tr("&Go to System Settings")
#else
                tr("&Go to Control Panel")
#endif
            );
            goToSettingsButtonLayout->addWidget(goToSettingsButton);
            goToSettingsButtonLayout->addStretch();
            layout->addLayout(goToSettingsButtonLayout);
            auto hintLabel = new QLabel(tr("<p>Also, please check whether other applications are able to play any sound.</p>"
                                           "<p>Note: If you are running another audio application (e.g. DAW) at the same time "
                                           "but it can play sound normally, please go to that application's settings "
                                           "to check whether it is using some kind of \"exclusive mode\", "
                                           "in which case the audio device can only be used by that one program.</p>"));
            hintLabel->setWordWrap(true);
            layout->addWidget(hintLabel);
            auto line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            layout->addSpacing(16);
            layout->addWidget(line);
            layout->addSpacing(16);
            auto questionLabel = new QLabel(tr("Are audio devices configured properly and able to be played by other applications?"));
            questionLabel->setWordWrap(true);
            layout->addWidget(questionLabel);
            auto yesButton = new QCommandLinkButton(
                tr("&Yes"), tr("The audio devices work well on my system. I want to retry to configure it in %1").arg(QApplication::applicationName()));
            layout->addWidget(yesButton);
            auto abortButton = new QCommandLinkButton(
                tr("Yes, &but..."), tr("Even though other applications can play sound, I failed to configure it in %1").arg(QApplication::applicationName()));
            layout->addWidget(abortButton);
            auto noButton = new QCommandLinkButton(
                tr("&No"), tr("Other applications also cannot play any sound"));
            layout->addWidget(noButton);
            auto idkButton = new QCommandLinkButton(tr("&I don't know"));
            layout->addWidget(idkButton);
            setLayout(layout);

            connect(goToSettingsButton, &QAbstractButton::clicked, this, [=] {
#ifdef Q_OS_WINDOWS
                std::system("mmsys.cpl");
                return;
#elif defined(Q_OS_MACOS)
                if (std::system("open 'x-apple.systempreferences:com.apple.preference.sound'") == 0)
                    return;
#endif
                QMessageBox::information(this, {}, tr("%1 cannot open the control panel on your system. Please open it manually").arg(QApplication::applicationName()));
            });

            connect(yesButton, &QAbstractButton::clicked, this, [=] {
                parent->back();
            });
            connect(abortButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_Fail;
                parent->next();
            });
            connect(noButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_Fail;
                parent->next();
            });
            connect(idkButton, &QAbstractButton::clicked, this, [=] {
                m_nextId = Page_Fail;
                parent->next();
            });

        }

        int nextId() const override {
            return m_nextId;
        }

    private:
        int m_nextId = Page_Fail;
    };

    class ResultFailPage : public QWizardPage {
        Q_OBJECT
    public:
        explicit ResultFailPage(AudioTroubleshootingDialog *parent = nullptr) : QWizardPage(parent) {
            setTitle("<h3>Cannot Resolve the Problem with Audio Output</h3>");
            setSubTitle(tr("You can send us feedback and attach the message below"));
            auto layout = new QVBoxLayout;
            auto textEdit = new QPlainTextEdit;
            textEdit->setReadOnly(true);
            layout->addWidget(textEdit);
            auto noteLabel = new QLabel(tr("The message contains the names of audio devices on your computer "
                                           "and does not contain sensitive information (e.g. your username)"));
            noteLabel->setWordWrap(true);
            layout->addWidget(noteLabel);
            setLayout(layout);
        }

        int nextId() const override {
            return -1;
        }
    };


    AudioTroubleshootingDialog::AudioTroubleshootingDialog(QWidget *parent) : QWizard(parent) {
        setWindowTitle(tr("Audio Troubleshooting Wizard"));
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setPage(Page_Welcome, new WelcomePage(this));
        setPage(Page_Test, new TestPage(this));
        setPage(Page_OK, new ResultOkPage(this));
        setPage(Page_Configure, new ConfigurePage(this));
        setPage(Page_ExternalCheck, new ExternalCheckPage(this));
        setPage(Page_Fail, new ResultFailPage(this));

        setButtonLayout({QWizard::Stretch, QWizard::BackButton, QWizard::CancelButton, QWizard::FinishButton});

        resize(800, 900);
    }
    AudioTroubleshootingDialog::~AudioTroubleshootingDialog() = default;

}

#include "audiotroubleshootingdialog.moc"