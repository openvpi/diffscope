#ifndef SCRIPT_MANAGER_TESTMODELWRAPPER_H
#define SCRIPT_MANAGER_TESTMODELWRAPPER_H

#include <QObject>
#include <QJSEngine>
#include <QDebug>

#include <dspxmodel/trackentity.h>

#include <scriptmanager/internal/TrackList.h>

namespace ScriptManager::Internal {

    class TestModelWrapper : public QObject {
        Q_OBJECT
    private slots:
        void vectorEntityWrapper() {
            using namespace QDspx;
            QJSEngine engine;
            engine.installExtensions(QJSEngine::ConsoleExtension);
            TrackListEntity trackListEntity;
            TrackEntity trackEntity;
            trackListEntity.insert(0, {&trackEntity});
            auto wrappedObj = (new TrackList(&trackListEntity))->wrap(&engine);
            qDebug() << engine.evaluate("trackList => trackList.length").call({wrappedObj}).toString();
        }
    };

}

#endif // SCRIPT_MANAGER_TESTMODELWRAPPER_H
