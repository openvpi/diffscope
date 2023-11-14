#ifndef DIFFSCOPE_FILESELECTOR_H
#define DIFFSCOPE_FILESELECTOR_H

#include <QWidget>

#include "../../ScriptObject.h"

class QVBoxLayout;
class QLineEdit;
class QPlainTextEdit;

class FileSelector : public QWidget, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(bool isOpenFile MEMBER m_isOpenFile)
    Q_PROPERTY(bool allowsMultipleFiles READ allowsMultipleFiles WRITE setAllowsMultipleFiles)
    Q_PROPERTY(QString filter MEMBER m_filter)
    Q_PROPERTY(QString title MEMBER m_title)
    Q_PROPERTY(QString selectedFilter READ selectedFilter)
    Q_PROPERTY(QJSValue files READ files)
public:
    QJSValue createScriptObject() override;

    explicit FileSelector(QWidget *parent = nullptr);
    ~FileSelector() override;

    bool allowsMultipleFiles() const;
    void setAllowsMultipleFiles(bool allows);

    QString selectedFilter() const;

    QJSValue files() const;

signals:
    void fileChanged();

private:
    bool m_isOpenFile = true;
    QString m_filter;
    QString m_title;
    QString m_selectedFilter;

    QWidget *m_singleWidget;
    QWidget *m_multipleWidget;
    QVBoxLayout *m_mainLayout;
    QLineEdit *m_singleFilePath;
    QPlainTextEdit *m_multipleFilePaths;

    QList<QJSValue> m_files;

    void setFile(const QString &file);
    void addFiles(const QStringList &filePaths);
    void clearFiles();

};

#endif // DIFFSCOPE_FILESELECTOR_H
