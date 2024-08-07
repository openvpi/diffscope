#include "vstmodepage.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QTreeWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColorDialog>
#include <QMessageBox>
#include <QJsonObject>

#include <SVSCraftWidgets/expressionspinbox.h>

#include <TalcsDevice/AudioDevice.h>
#include <TalcsRemote/RemoteOutputContext.h>
#include <TalcsRemote/RemoteSocket.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>
#include <audioplugin/internal/devicetester.h>
#include <audioplugin/internal/audiosettings.h>

namespace Audio::Internal {

    class CustomizeThemeDialog : public QDialog {
        Q_OBJECT
    public:
        explicit CustomizeThemeDialog(QWidget *parent = nullptr) : QDialog(parent) {
            auto mainLayout = new QVBoxLayout;

            m_demoBackgroundMainItem =
                m_demoScene.addRect({0, 0, 400, 200}, Qt::NoPen, componentColor(BackgroundMain));
            m_demoBackgroundSidebarItem = m_demoScene.addRect({0, 150, 400, 50}, Qt::NoPen,
                                                              componentColor(BackgroundSidebar));
            QPainterPath buttonPath;
            buttonPath.addRoundedRect({100, 50, 200, 50}, 6, 6);
            m_demoButtonItem = m_demoScene.addPath(buttonPath, componentColor(ForegroundBorder),
                                                   componentColor(Foreground));
            m_demoButtonTextItem = m_demoScene.addSimpleText(
                QString("Show %1 Editor").arg(QApplication::applicationName()));
            m_demoButtonTextItem->setBrush(componentColor(ForegroundLabel));
            m_demoButtonTextItem->setPos({200 - m_demoButtonTextItem->boundingRect().width() / 2,
                                          75 - m_demoButtonTextItem->boundingRect().height() / 2});
            m_demoStatusTextItem = m_demoScene.addSimpleText("Status: Connected");
            m_demoStatusTextItem->setBrush(componentColor(BackgroundMessage));
            m_demoStatusTextItem->setPos({8, 176});
            m_demoErrorTextItem = m_demoScene.addSimpleText("Error message");
            m_demoErrorTextItem->setBrush(componentColor(BackgroundError));
            m_demoErrorTextItem->setPos({8, 156});
            m_demoVersionTextItem = m_demoScene.addSimpleText("Version 0.0.0");
            m_demoVersionTextItem->setBrush(componentColor(BackgroundMessage));
            m_demoVersionTextItem->setPos(
                {392 - m_demoVersionTextItem->boundingRect().width(), 176});

            setComponentColor(Foreground, QColor(0x5566FF));
            setComponentColor(ForegroundBorder, QColor(0x404144));
            setComponentColor(ForegroundLabel, QColor(0xFFFFFF));
            setComponentColor(BackgroundMain, QColor(0x212124));
            setComponentColor(BackgroundSidebar, QColor(0x252629));
            setComponentColor(BackgroundMessage, QColor(0xFFFFFF));
            setComponentColor(BackgroundError, QColor(0xFF0000));

            auto demoGroupBox = new QGroupBox("Preview");
            auto demoLayout = new QVBoxLayout;
            m_demoView = new QGraphicsView(&m_demoScene);
            m_demoView->setRenderHint(QPainter::Antialiasing, true);
            m_demoView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            m_demoView->setWhatsThis(tr("This view demonstrates what %1 Bridge's editor looks "
                                        "like. May differ from actual.")
                                         .arg(QApplication::applicationName()));
            demoLayout->addWidget(m_demoView);
            demoGroupBox->setLayout(demoLayout);
            mainLayout->addWidget(demoGroupBox);

            auto editColorsGroupBox = new QGroupBox("Edit Colors");
            auto editColorsLayout = new QFormLayout;
            auto selectComponentComboBox = new QComboBox;
            selectComponentComboBox->addItem(tr("Button background"), Foreground);
            selectComponentComboBox->addItem(tr("Button border"), ForegroundBorder);
            selectComponentComboBox->addItem(tr("Button text"), ForegroundLabel);
            selectComponentComboBox->addItem(tr("Editor background"), BackgroundMain);
            selectComponentComboBox->addItem(tr("Bottom bar background"), BackgroundSidebar);
            selectComponentComboBox->addItem(tr("Message text"), BackgroundMessage);
            selectComponentComboBox->addItem(tr("Error text"), BackgroundError);
            selectComponentComboBox->setWhatsThis(
                tr("Use this field to select the target of Pick Color."
                   "<ul>"
                   "<li><b>Button background</b>: the background color of \"Show %1 Editor\" "
                   "button</li>"
                   "<li><b>Button border</b>: the border color of \"Show %1 Editor\" button</li>"
                   "<li><b>Button text</b>: the color of \"Show %1 Editor\" text on the button</li>"
                   "<li><b>Editor background</b>: the background color of the editor's main "
                   "view</li>"
                   "<li><b>Bottom bar background</b>: the background color of the editor's bottom "
                   "bar where show the status, version and error message</li>"
                   "<li><b>Message text</b>: the color of the status and version text</li>"
                   "<li><b>Error text</b>: the color of the error message text</li>"
                   "</ul>")
                    .arg(QApplication::applicationName()));
            editColorsLayout->addRow(tr("&Component"), selectComponentComboBox);
            auto pickColorButton = new QPushButton(tr("&Pick Color"));
            pickColorButton->setWhatsThis(
                tr("After pressing this button, you can set a custom color for the selected target "
                   "in the pop-up dialog."));
            editColorsLayout->addRow(pickColorButton);
            editColorsGroupBox->setLayout(editColorsLayout);
            mainLayout->addWidget(editColorsGroupBox);

            mainLayout->addStretch();

            setLayout(mainLayout);
            setWindowTitle(tr("Customize Theme"));

            connect(pickColorButton, &QPushButton::clicked, this, [=] {
                auto component = selectComponentComboBox->currentData().value<Component>();
                auto color = QColorDialog::getColor(componentColor(component), this);
                if (color.isValid())
                    setComponentColor(component, color);
            });
        }
        ~CustomizeThemeDialog() override = default;

        enum Component {
            Foreground,
            ForegroundBorder,
            ForegroundLabel,
            BackgroundMain,
            BackgroundSidebar,
            BackgroundMessage,
            BackgroundError,
        };
        Q_ENUM(Component)
        void setComponentColor(Component component, const QColor &color) {
            switch (component) {
                case Foreground:
                    m_demoButtonItem->setBrush(color);
                    break;
                case ForegroundBorder:
                    m_demoButtonItem->setPen(color);
                    break;
                case ForegroundLabel:
                    m_demoButtonTextItem->setBrush(color);
                    break;
                case BackgroundMain:
                    m_demoBackgroundMainItem->setBrush(color);
                    break;
                case BackgroundSidebar:
                    m_demoBackgroundSidebarItem->setBrush(color);
                    break;
                case BackgroundMessage:
                    m_demoStatusTextItem->setBrush(color);
                    m_demoVersionTextItem->setBrush(color);
                    break;
                case BackgroundError:
                    m_demoErrorTextItem->setBrush(color);
                    break;
            }
            m_colorMap.insert(component, color);
        }
        QColor componentColor(Component component) {
            return m_colorMap.value(component);
        }

    private:
        QGraphicsPathItem *m_demoButtonItem;
        QGraphicsRectItem *m_demoBackgroundMainItem;
        QGraphicsRectItem *m_demoBackgroundSidebarItem;
        QGraphicsSimpleTextItem *m_demoButtonTextItem;
        QGraphicsSimpleTextItem *m_demoStatusTextItem;
        QGraphicsSimpleTextItem *m_demoVersionTextItem;
        QGraphicsSimpleTextItem *m_demoErrorTextItem;

        QGraphicsScene m_demoScene;
        QGraphicsView *m_demoView;

        QHash<Component, QColor> m_colorMap;
    };

    VSTModePage::VSTModePage(QObject *parent)
        : Core::ISettingPage(QStringLiteral("audio.PluginMode"), parent) {
        setTitle([] { return tr("Plugin Mode"); });
        setDescription([] {
            return tr("Plug %1 to External Host Application.").arg(QApplication::applicationName());
        });
    }

    VSTModePage::~VSTModePage() = default;

    QString VSTModePage::sortKeyword() const {
        return QStringLiteral("PluginMode");
    }
    QWidget *VSTModePage::widget() {
        if (m_widget)
            return m_widget;

        m_widget = new QWidget;
        auto mainLayout = new QVBoxLayout;

        auto connectivityGroupBox = new QGroupBox(tr("Connectivity"));
        auto connectivityLayout = new QFormLayout;
        auto connectivityDescriptionLabel =
            new QLabel(tr("%1 communicates with %1 Bridge over a local TCP connection. (except for "
                          "audio transmission)")
                           .arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityDescriptionLabel);
        m_editorPortSpinBox = new SVS::ExpressionSpinBox;
        m_editorPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(tr("%1 listening &port").arg(QApplication::applicationName()),
                                   m_editorPortSpinBox);
        m_pluginPortSpinBox = new SVS::ExpressionSpinBox;
        m_pluginPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(
            tr("%1 &Bridge listening port").arg(QApplication::applicationName()),
            m_pluginPortSpinBox);
        auto connectivityHintLabel = new QLabel(
            tr("Only after both %1 and %1 Bridge are restarted will these options take effect.")
                .arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityHintLabel);
        connectivityGroupBox->setLayout(connectivityLayout);
        mainLayout->addWidget(connectivityGroupBox);

        auto editorGroupBox = new QGroupBox(tr("Editor"));
        auto editorLayout = new QFormLayout;
        m_closeEditorBehaviorComboBox = new QComboBox;
        m_closeEditorBehaviorComboBox->addItems({tr("Hide"), tr("Minimize")});
        editorLayout->addRow(tr("When plugin editor is &closed, make project window"),
                             m_closeEditorBehaviorComboBox);
        m_alwaysOnTopCheckBox = new QCheckBox(tr("&Keep project windows always on top"));
        editorLayout->addRow(m_alwaysOnTopCheckBox);
        auto themeLayout = new QHBoxLayout;
        m_pluginEditorUsesCustomThemeCheckBox =
            new QCheckBox(tr("Plugin editor &uses custom theme"));
        themeLayout->addWidget(m_pluginEditorUsesCustomThemeCheckBox);
        auto customizeThemeButton = new QPushButton(tr("Customize &Theme"));
        customizeThemeButton->setDisabled(true);
        m_customizeThemeDialog = new CustomizeThemeDialog(m_widget);
        themeLayout->addWidget(customizeThemeButton);
        themeLayout->addStretch();
        editorLayout->addRow(themeLayout);
        editorGroupBox->setLayout(editorLayout);

        mainLayout->addWidget(editorGroupBox);

        auto statusGroupBox = new QGroupBox(tr("Status"));
        auto statusLayout = new QVBoxLayout;
        m_statusTreeWidget = new QTreeWidget;
        // TODO
        statusLayout->addWidget(m_statusTreeWidget);
        auto testAudioButton = new QPushButton(tr("Test Audio"));
        statusLayout->addWidget(testAudioButton);
        statusGroupBox->setLayout(statusLayout);
        mainLayout->addWidget(statusGroupBox, 1);

        m_widget->setLayout(mainLayout);

        connect(m_pluginEditorUsesCustomThemeCheckBox, &QCheckBox::toggled, customizeThemeButton,
                &QPushButton::setEnabled);
        connect(customizeThemeButton, &QPushButton::clicked, m_customizeThemeDialog,
                &CustomizeThemeDialog::exec);

        m_editorPortSpinBox->setValue(AudioSettings::vstEditorPort());
        m_pluginPortSpinBox->setValue(AudioSettings::vstPluginPort());
        // TODO
        m_pluginEditorUsesCustomThemeCheckBox->setChecked(AudioSettings::vstPluginEditorUsesCustomTheme());
        auto vstTheme = AudioSettings::vstTheme().toObject();
        if (vstTheme.contains("foreground"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::Foreground, QColor(static_cast<QRgb>(vstTheme["foreground"].toInt())));
        if (vstTheme.contains("foregroundBorder"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::ForegroundBorder, QColor(static_cast<QRgb>(vstTheme["foregroundBorder"].toInt())));
        if (vstTheme.contains("foregroundLabel"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::ForegroundLabel, QColor(static_cast<QRgb>(vstTheme["foregroundLabel"].toInt())));
        if (vstTheme.contains("backgroundMain"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::BackgroundMain, QColor(static_cast<QRgb>(vstTheme["backgroundMain"].toInt())));
        if (vstTheme.contains("backgroundSidebar"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::BackgroundSidebar, QColor(static_cast<QRgb>(vstTheme["backgroundSidebar"].toInt())));
        if (vstTheme.contains("backgroundMessage"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::BackgroundMessage, QColor(static_cast<QRgb>(vstTheme["backgroundMessage"].toInt())));
        if (vstTheme.contains("backgroundError"))
            m_customizeThemeDialog->setComponentColor(CustomizeThemeDialog::BackgroundError, QColor(static_cast<QRgb>(vstTheme["backgroundError"].toInt())));

        auto vstSys = AudioSystem::vstConnectionSystem();
        
        auto hostStatusItem = new QTreeWidgetItem({tr("Host")});
        auto pluginFormatStatusItem = new QTreeWidgetItem({tr("Plugin Format")});
        auto hostSpecsStatusChangeHandler = [=](const QString &hostExecutable, const QString &pluginFormat) {
            hostStatusItem->setText(1, hostExecutable.isEmpty() ? tr("N/A") : hostExecutable);
            pluginFormatStatusItem->setText(1, pluginFormat.isEmpty() ? tr("N/A") : pluginFormat);
        };
        connect(vstSys, &VSTConnectionSystem::hostSpecsChanged, m_widget, hostSpecsStatusChangeHandler);
        hostSpecsStatusChangeHandler(vstSys->hostSpecs().first, vstSys->hostSpecs().second);

        auto bufferSizeStatusItem = new QTreeWidgetItem({tr("Buffer Size")});
        auto bufferSizeStatusChangeHandler = [=](qint64 bufferSize) {
            bufferSizeStatusItem->setText(1, bufferSize ? QString::number(bufferSize) : tr("N/A"));
        };
        connect(vstSys->context(), &talcs::AbstractOutputContext::bufferSizeChanged, m_widget, bufferSizeStatusChangeHandler);
        bufferSizeStatusChangeHandler(vstSys->context()->device() ? vstSys->context()->device()->bufferSize() : 0);

        auto sampleRateStatusItem = new QTreeWidgetItem({tr("Sample Rate")});
        auto sampleRateStatusChangeHandler = [=](double sampleRate) {
            sampleRateStatusItem->setText(1, !qFuzzyIsNull(sampleRate) ? QString::number(sampleRate) : tr("N/A"));
        };
        connect(vstSys->context(), &talcs::AbstractOutputContext::sampleRateChanged, m_widget, sampleRateStatusChangeHandler);
        sampleRateStatusChangeHandler(vstSys->context()->device() ? vstSys->context()->device()->sampleRate() : 0);

        auto connectionStatusItem = new QTreeWidgetItem({tr("Connected")});
        auto connectionStatusChangeHandler = [=](talcs::RemoteSocket::Status status) {
            if (status == talcs::RemoteSocket::Connected) {
                connectionStatusItem->setText(1, tr("Yes"));
            } else {
                connectionStatusItem->setText(1, tr("No"));
                hostStatusItem->setText(1, tr("N/A"));
                pluginFormatStatusItem->setText(1, tr("N/A"));
                bufferSizeStatusItem->setText(1, tr("N/A"));
                sampleRateStatusItem->setText(1, tr("N/A"));
            }
        };
        connect(vstSys->remoteOutputContext()->socket(), &talcs::RemoteSocket::socketStatusChanged, m_widget, [=](int status) {
            connectionStatusChangeHandler(talcs::RemoteSocket::Status(status));
        });
        connectionStatusChangeHandler(vstSys->remoteOutputContext()->socket()->status());

        m_statusTreeWidget->setColumnCount(2);
        m_statusTreeWidget->setHeaderLabels({tr("Property"), tr("Value")});
        m_statusTreeWidget->addTopLevelItems({connectionStatusItem, hostStatusItem, pluginFormatStatusItem, bufferSizeStatusItem, sampleRateStatusItem});
        m_statusTreeWidget->resizeColumnToContents(0);

        connect(testAudioButton, &QPushButton::clicked, this, [=] {
            if (!vstSys->isReady()) {
                QMessageBox msgBox(m_widget);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText(tr("Cannot start audio playback"));
                msgBox.setInformativeText(tr("Please check the status of %1 Bridge and the host application.").arg(QApplication::applicationName()));
                msgBox.exec();
            } else {
                DeviceTester::playTestSound(true);
            }
        });

        return m_widget;
    }
    bool VSTModePage::accept() {
        if (!m_widget)
            return true;
        AudioSettings::setVstEditorPort(m_editorPortSpinBox->value());
        AudioSettings::setVstPluginPort(m_pluginPortSpinBox->value());
        AudioSettings::setVstTheme(QJsonObject{
            {"foreground",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::Foreground).rgba())},
            {"foregroundBorder",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::ForegroundBorder)
                     .rgba())                                                                    },
            {"foregroundLabel",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::ForegroundLabel)
                     .rgba())                                                                    },
            {"backgroundMain",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::BackgroundMain)
                     .rgba())                                                                    },
            {"backgroundSidebar",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::BackgroundSidebar)
                     .rgba())                                                                    },
            {"backgroundMessage",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::BackgroundMessage)
                     .rgba())                                                                    },
            {"backgroundError",
             static_cast<int>(
                 m_customizeThemeDialog->componentColor(CustomizeThemeDialog::BackgroundError)
                     .rgba())                                                                    },
        });
        AudioSettings::setVstPluginEditorUsesCustomTheme(m_pluginEditorUsesCustomThemeCheckBox->isChecked());
        VSTConnectionSystem::createVSTConfig();
        // TODO
        return true;
    }
    void VSTModePage::finish() {
    }

}

#include "vstmodepage.moc"