#include "Dialog.h"
#include "Dialog_p.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QJSEngine>

#include "../../Global/GlobalObject.h"

Dialog::Dialog(QWidget *win) : QObject(win), d(new DialogPrivate) {
    d->dlg = new QDialog(win);
    d->dlgLayout = new QVBoxLayout;
    d->dlg->setLayout(d->dlgLayout);
    auto okBtn = new QPushButton("OK");
    auto cancelBtn = new QPushButton("Cancel");
    auto btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    d->dlgLayout->addLayout(btnLayout);
    connect(okBtn, &QPushButton::clicked, d->dlg, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, d->dlg, &QDialog::reject);
}

Dialog::~Dialog() = default;

void Dialog::setContent(const QJSValue &jsWidget) {
    if (!d->content.isNull())
        d->dlgLayout->takeAt(0);
    auto *widget = qobject_cast<QWidget *>(jsWidget.property("_p").toQObject());
    if (widget) {
        d->content = jsWidget;
        d->dlgLayout->insertWidget(0, widget);
        return;
    }
    auto *layout = qobject_cast<QLayout *>(jsWidget.property("_p").toQObject());
    if (layout) {
        d->content = jsWidget;
        d->dlgLayout->insertLayout(0, layout);
        return;
    }
    d->content = QJSValue::NullValue;
    jsGlobal->engine()->throwError(QJSValue::TypeError, "Invalid type of dialog content");
}

QJSValue Dialog::content() const {
    return d->content;
}

bool Dialog::open() {
    return d->dlg->exec() == QDialog::Accepted;
}

void Dialog::close(bool accepted) {
    d->dlg->done(accepted ? QDialog::Accepted : QDialog::Rejected);
}
