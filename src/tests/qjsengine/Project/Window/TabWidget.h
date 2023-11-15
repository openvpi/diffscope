#ifndef DIFFSCOPE_TABWIDGET_H
#define DIFFSCOPE_TABWIDGET_H

#include <QTabWidget>

#include "../../ScriptObject.h"

class TabWidget : public QTabWidget, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(int tabPosition READ tabPosition WRITE setTabPosition)
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                const QJSValue &children,
                                                const std::function<QJSValue(const QJSValue &)> &renderer) override;

    explicit TabWidget(QWidget *parent = nullptr);

    void setTabPosition(int position);

public slots:
    int addTab(const QJSValue &jsWidget, const QString &label);
    int insertTab(int index, const QJSValue &jsWidget, const QString &label);

    void setTabEnabled(int index, bool enabled);
    bool isTabEnabled(int index) const;

    void setTabText(int index, const QString &label);
    QString tabText(int index) const;

    void setTabToolTip(int index, const QString &tip);
    QString tabToolTip(int index) const;

    void setTabVisible(int index, bool visible);
    bool isTabVisible(int index);
};

#endif // DIFFSCOPE_TABWIDGET_H
