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

#include <SVSCraftWidgets/expressionspinbox.h>

namespace Audio {

    class CustomizeThemeDialog : public QDialog {
    public:
        explicit CustomizeThemeDialog(QWidget *parent = nullptr) : QDialog(parent) {
            auto mainLayout = new QVBoxLayout;

            m_demoBackgroundMainItem = m_demoScene.addRect({0, 0, 400, 200}, Qt::NoPen, componentColor(BackgroundMain));
            m_demoBackgroundSidebarItem = m_demoScene.addRect({0, 150, 400, 50}, Qt::NoPen, componentColor(BackgroundSidebar));
            QPainterPath buttonPath;
            buttonPath.addRoundedRect({100, 50, 200, 50}, 6, 6);
            m_demoButtonItem = m_demoScene.addPath(buttonPath, componentColor(ForegroundBorder), componentColor(Foreground));
            m_demoButtonTextItem = m_demoScene.addSimpleText(QString("Show %1 Editor").arg(QApplication::applicationName()));
            m_demoButtonTextItem->setBrush(componentColor(ForegroundLabel));
            m_demoButtonTextItem->setPos({200 - m_demoButtonTextItem->boundingRect().width() / 2, 75 - m_demoButtonTextItem->boundingRect().height() / 2});
            m_demoStatusTextItem = m_demoScene.addSimpleText("Status: Connected");
            m_demoStatusTextItem->setBrush(componentColor(BackgroundMessage));
            m_demoStatusTextItem->setPos({8, 176});
            m_demoErrorTextItem = m_demoScene.addSimpleText("Error message");
            m_demoErrorTextItem->setBrush(componentColor(BackgroundError));
            m_demoErrorTextItem->setPos({8, 156});
            m_demoVersionTextItem = m_demoScene.addSimpleText("Version 0.0.0");
            m_demoVersionTextItem->setBrush(componentColor(BackgroundMessage));
            m_demoVersionTextItem->setPos({392 - m_demoVersionTextItem->boundingRect().width(), 176});

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
            m_demoView->setWhatsThis(tr("This view demonstrates what %1 Bridge's editor looks like. May differ from actual.").arg(QApplication::applicationName()));
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
            selectComponentComboBox->setWhatsThis(tr(
                "Use this field to select the target of Pick Color."
                "<ul>"
                "<li><b>Button background</b>: the background color of \"Show %1 Editor\" button</li>"
                "<li><b>Button border</b>: the border color of \"Show %1 Editor\" button</li>"
                "<li><b>Button text</b>: the color of \"Show %1 Editor\" text on the button</li>"
                "<li><b>Editor background</b>: the background color of the editor's main view</li>"
                "<li><b>Bottom bar background</b>: the background color of the editor's bottom bar where show the status, version and error message</li>"
                "<li><b>Message text</b>: the color of the status and version text</li>"
                "<li><b>Error text</b>: the color of the error message text</li>"
                "</ul>").arg(QApplication::applicationName()));
            editColorsLayout->addRow(tr("&Component"), selectComponentComboBox);
            auto pickColorButton = new QPushButton(tr("&Pick Color"));
            pickColorButton->setWhatsThis(tr("After pressing this button, you can set a custom color for the selected target in the pop-up dialog."));
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

    VSTModePage::VSTModePage(QObject *parent) : Core::ISettingPage("audio.PluginMode", parent) {
        setTitle([] { return tr("Plugin Mode"); });
        setDescription([] { return tr("Plug %1 to External Host Application.").arg(QApplication::applicationName()); });
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
        auto connectivityDescriptionLabel = new QLabel(tr("%1 communicates with %1 Bridge over a local TCP connection. (except for audio transmission)").arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityDescriptionLabel);
        m_editorPortSpinBox = new SVS::ExpressionSpinBox;
        m_editorPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(tr("%1 listening &port").arg(QApplication::applicationName()), m_editorPortSpinBox);
        m_pluginPortSpinBox = new SVS::ExpressionSpinBox;
        m_pluginPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(tr("%1 &Bridge listening port").arg(QApplication::applicationName()), m_pluginPortSpinBox);
        auto connectivityHintLabel = new QLabel(tr("Only after both %1 and %1 Bridge are restarted will these options take effect.").arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityHintLabel);
        connectivityGroupBox->setLayout(connectivityLayout);
        mainLayout->addWidget(connectivityGroupBox);

        auto editorGroupBox = new QGroupBox(tr("Editor"));
        auto editorLayout = new QFormLayout;
        m_closeEditorBehaviorComboBox = new QComboBox;
        m_closeEditorBehaviorComboBox->addItems({tr("Hide"), tr("Minimize")});
        editorLayout->addRow(tr("When plugin editor is &closed, make project window"), m_closeEditorBehaviorComboBox);
        m_alwaysOnTopCheckBox = new QCheckBox(tr("&Keep project windows always on top"));
        editorLayout->addRow(m_alwaysOnTopCheckBox);
        auto themeLayout = new QHBoxLayout;
        m_pluginEditorUsesCustomThemeCheckBox = new QCheckBox(tr("Plugin editor &uses custom theme"));
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
        statusGroupBox->setLayout(statusLayout);
        mainLayout->addWidget(statusGroupBox, 1);

        m_widget->setLayout(mainLayout);

        connect(m_pluginEditorUsesCustomThemeCheckBox, &QCheckBox::toggled, customizeThemeButton, &QPushButton::setEnabled);
        connect(customizeThemeButton, &QPushButton::clicked, m_customizeThemeDialog, &CustomizeThemeDialog::exec);

        return m_widget;
    }
    bool VSTModePage::accept() {
        // TODO
        return true;
    }
    void VSTModePage::finish() {
        m_widget.clear();
    }
} // Audio