#include "Dialog.h"

#include <QDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QJSEngine>
#include <QPushButton>

#include "../../Global/GlobalObject.h"
#include "../ProjectWindowObject.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), m_dlgLayout(new QVBoxLayout) {
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    auto okBtn = new QPushButton("OK");
    auto cancelBtn = new QPushButton("Cancel");
    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(m_dlgLayout);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

Dialog::~Dialog() = default;

void Dialog::setContent(const QJSValue &jsWidget) {
    if (!m_content.isNull()) {
        JS_THROW("Dialog content is already set");
        return;
    }
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        m_content = jsWidget;
        m_dlgLayout->insertWidget(0, widget);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
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

bool Dialog::openDialog() {
    return exec() == QDialog::Accepted;
}

void Dialog::closeDialog(bool accepted) {
    done(accepted ? QDialog::Accepted : QDialog::Rejected);
}
