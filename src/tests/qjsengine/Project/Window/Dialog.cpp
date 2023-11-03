#include "Dialog.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QJSEngine>
#include <QPushButton>

#include "../../Global/GlobalObject.h"

Dialog::Dialog(QWidget *win) : QObject(win), m_dlg(new QDialog(win)), m_dlgLayout(new QVBoxLayout) {
    m_dlg->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    m_dlg->setLayout(m_dlgLayout);
    auto okBtn = new QPushButton("OK");
    auto cancelBtn = new QPushButton("Cancel");
    auto btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    m_dlgLayout->addLayout(btnLayout);
    connect(okBtn, &QPushButton::clicked, m_dlg, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, m_dlg, &QDialog::reject);
}

Dialog::~Dialog() = default;

void Dialog::setContent(const QJSValue &jsWidget) {
    if (!m_content.isNull())
        m_dlgLayout->takeAt(0);
    auto *widget = qobject_cast<QWidget *>(jsWidget.property("_p").toQObject());
    if (widget) {
        m_content = jsWidget;
        m_dlgLayout->insertWidget(0, widget);
        return;
    }
    auto *layout = qobject_cast<QLayout *>(jsWidget.property("_p").toQObject());
    if (layout) {
        m_content = jsWidget;
        m_dlgLayout->insertLayout(0, layout);
        return;
    }
    m_content = QJSValue::NullValue;
    JS_THROW(QJSValue::TypeError, "Invalid type of dialog content");
}

QJSValue Dialog::content() const {
    return m_content;
}

bool Dialog::open() {
    return m_dlg->exec() == QDialog::Accepted;
}

void Dialog::close(bool accepted) {
    m_dlg->done(accepted ? QDialog::Accepted : QDialog::Rejected);
}
