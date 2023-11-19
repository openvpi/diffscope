#include "TransposeScript.h"

#include <QJSEngine>
#include "../Global/GlobalObject.h"
#include "../Project/ProjectObject.h"
#include "../Project/ProjectWindowObject.h"

TransposeScript::TransposeScript(const QJSValue &project) {
    m_project = qobject_cast<ProjectObject *>(project.toQObject());
    Q_ASSERT(m_project);
}

QJSValue TransposeScript::manifest() {
    auto ret = jsGlobal->engine()->evaluate(QString(R"(
        ({
            id: "transpose",
            name: "%1",
            isScriptSet: true,
            children: [{
                id: "up_semitone",
                name: "%2",
            }, {
                id: "down_semitone",
                name: "%3",
            }, {
                id: "up_octave",
                name: "%4",
            }, {
                id: "down_octave",
                name: "%5",
            }, {
                id: "customize",
                name: "%6"
            }],
        })
    )").arg(tr("Transpose"), tr("Shift Up a Semitone"),
            tr("Shift Down a Semitone"), tr("Shift Up an Octave"),
            tr("Shift Down an Octave"), tr("Customise")));
    Q_ASSERT(!ret.isError());
    return ret;
}

bool TransposeScript::prepare(int index) {
    switch (index) {
        case UpSemitone:
            m_value = 1;
            return true;
        case DownSemitone:
            m_value = -1;
            return true;
        case UpOctave:
            m_value = 12;
            return true;
        case DownOctave:
            m_value = -12;
            return true;
        case Customize:
            auto dlg = m_project->windowObject()->createDialog();
            auto formLayout = m_project->windowObject()->createElement("form-layout");
            auto spinBox = m_project->windowObject()->createElement("spin-box");
            spinBox.setProperty("minimum", -127);
            spinBox.setProperty("maximum", 127);
            formLayout.property("addRow").call({tr("Semitone(s)"), spinBox});
            dlg.setProperty("content", formLayout);
            dlg.property("openDialog").call();
            m_value = spinBox.property("value").toInt();
            return true;
    }
    return false;
}

void TransposeScript::main() {
    // TODO
    m_project->windowObject()->alert(QString::number(m_value));
}
