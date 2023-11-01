#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QJSEngine>
#include <QMainWindow>
#include <QPushButton>

#include "ObjectWrapper.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QJSEngine engine;
    engine.installExtensions(QJSEngine::ConsoleExtension);

    auto btn = new QPushButton;
    auto obj = ObjectWrapper::wrap(btn, &engine);
    OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, clicked);
    engine.globalObject().setProperty("btn", obj);

    auto win = new QMainWindow;
    win->setCentralWidget(btn);

    btn->setText("0");

    QFile f(":/scripts/main.js");
    f.open(QFile::ReadOnly);
    auto ret = engine.evaluate(f.readAll(), ":/scripts/main.js");
    if (ret.isError()) {
        qDebug() << ret.toString();
    }

    qDebug() << engine.globalObject().property("Object").property("keys").call({obj}).toVariant().toStringList();
    win->show();

    return a.exec();
}