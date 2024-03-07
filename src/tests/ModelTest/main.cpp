#include <QDebug>
#include <QApplication>

#include "MainWindow.h"

#include <dspxmodel/modelentity.h>
#include <substate/model.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <ostream>
#include <QDir>

int main(int argc, char *argv[]) {
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    // MainWindow w;
    // w.show();

    Substate::Model dbModel;

    auto fileModel = new QDspx::ModelEntity();
    QObject::connect(fileModel->metadata(), &QDspx::FileMetaEntity::projectNameChanged,
                     [](const QString &projectName) {
                         qDebug() << "project name changes:" << projectName; //
                     });

    dbModel.beginTransaction();
    dbModel.setRoot(fileModel->internalData());
    dbModel.commitTransaction({});

    qDebug() << fileModel->metadata()->projectName();

    dbModel.beginTransaction();
    fileModel->metadata()->setProjectName("22");
    dbModel.commitTransaction({});

    qDebug() << fileModel->metadata()->projectName();

    dbModel.undo();

    qDebug() << fileModel->metadata()->projectName();

    return 0;
}