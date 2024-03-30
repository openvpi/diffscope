#include "vstmodepage.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QTreeWidget>
#include <QGroupBox>
#include <QLabel>

#include <SVSCraftWidgets/expressionspinbox.h>

namespace Audio {
    VSTModePage::VSTModePage(QObject *parent) : Core::ISettingPage("audio.PluginMode", parent) {
        setTitle([] { return tr("Plugin Mode"); });
        setDescription([] { return tr("Plug %1 to External Host Application.").arg(QApplication::applicationName()); });
    }

    VSTModePage::~VSTModePage() = default;

    QString VSTModePage::sortKeyword() const {
        return QStringLiteral("PluginMode");
    }
    QWidget *VSTModePage::widget() {
        m_widget = new QWidget;
        auto mainLayout = new QVBoxLayout;

        auto connectivityGroupBox = new QGroupBox(tr("Connectivity"));
        auto connectivityLayout = new QFormLayout;
        auto connectivityDescriptionLabel = new QLabel(tr("%1 communicates with %1 Bridge over a local TCP connection. (except for audio transmission)").arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityDescriptionLabel);
        m_editorPortSpinBox = new SVS::ExpressionSpinBox;
        m_editorPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(tr("%1 Listening &Port").arg(QApplication::applicationName()), m_editorPortSpinBox);
        m_pluginPortSpinBox = new SVS::ExpressionSpinBox;
        m_pluginPortSpinBox->setRange(0, 65535);
        connectivityLayout->addRow(tr("%1 &Bridge Listening Port").arg(QApplication::applicationName()), m_pluginPortSpinBox);
        auto connectivityHintLabel = new QLabel(tr("Only after both %1 and %1 Bridge are restarted will these options take effect.").arg(QApplication::applicationName()));
        connectivityLayout->addRow(connectivityHintLabel);
        connectivityGroupBox->setLayout(connectivityLayout);
        mainLayout->addWidget(connectivityGroupBox);

        auto editorGroupBox = new QGroupBox(tr("Editor"));
        auto editorLayout = new QFormLayout;
        m_closeEditorBehaviorComboBox = new QComboBox;
        m_closeEditorBehaviorComboBox->addItems({tr("Hide"), tr("Minimize")});
        editorLayout->addRow(tr("When Plugin Editor is &Closed, Make Project Window"), m_closeEditorBehaviorComboBox);
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

        return m_widget;
    }
    bool VSTModePage::accept() {
        return false;
    }
    void VSTModePage::finish() {
    }
} // Audio