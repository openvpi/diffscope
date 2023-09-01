#include <QCoreApplication>
#include <QDebug>

#include "device/SDLAudioDriver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    SDLAudioDriver driver;
    driver.initialize();
    qDebug() << driver.devices();

    return a.exec();
}