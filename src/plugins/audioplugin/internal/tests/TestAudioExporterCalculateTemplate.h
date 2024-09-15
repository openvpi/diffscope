#ifndef AUDIO_TESTAUDIOEXPORTERCALCULATETEMPLATE_H
#define AUDIO_TESTAUDIOEXPORTERCALCULATETEMPLATE_H

#include <audioplugin/private/audioexporter_p.h>


#include <QtTest/QtTest>

namespace Audio::Internal {
    class TestAudioExporterCalculateTemplate : public QObject {
        Q_OBJECT
    public:
        explicit TestAudioExporterCalculateTemplate(QObject *parent = nullptr) : QObject(parent) {
        }
    private slots:
        void testCalculateTemplate() {
            AudioExporterPrivate d;
            QString s = "aaa${trackName}bbb${trackIndex}ccc";
            QVERIFY(d.calculateTemplate(s, "xxx", 1));
            QCOMPARE(s, "aaaxxxbbb1ccc");
            s = "aaa${trackName}bbb${trackIndex}ccc";
            QVERIFY(!d.calculateTemplate(s));
            QCOMPARE(s, "aaa${trackName}bbb${trackIndex}ccc");
        }
    };
}

#endif //AUDIO_TESTAUDIOEXPORTERCALCULATETEMPLATE_H
