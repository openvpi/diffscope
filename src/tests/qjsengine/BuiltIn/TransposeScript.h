#ifndef DIFFSCOPE_TRANSPOSESCRIPT_H
#define DIFFSCOPE_TRANSPOSESCRIPT_H

#include <QObject>
#include <QJSValue>

class ProjectObject;

class TransposeScript : public QObject {
    Q_OBJECT
    enum Entries {
        UpSemitone,
        DownSemitone,
        UpOctave,
        DownOctave,
        Customize,
    };
public:
    Q_INVOKABLE explicit TransposeScript(const QJSValue &project);
    static QJSValue manifest();
public slots:
    bool prepare(int index);
    void main();

private:
    ProjectObject *m_project;
    int m_value = 0;
};

#endif // DIFFSCOPE_TRANSPOSESCRIPT_H
