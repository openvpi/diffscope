#include "FileSelector.h"

#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QJSEngine>
#include <QFileDialog>

#include "../../Global/GlobalObject.h"
#include "../../Global/File.h"

FileSelector::FileSelector(QWidget *parent) : QWidget(parent) {
    m_mainLayout = new QVBoxLayout(this);

    m_singleWidget = new QWidget;
    auto singleLayout = new QHBoxLayout;
    m_singleFilePath = new QLineEdit;
    m_singleFilePath->setReadOnly(true);
    auto singleFileBrowse = new QPushButton("Browse");
    auto singleFileReset = new QPushButton("Reset");
    singleLayout->addWidget(m_singleFilePath);
    singleLayout->addWidget(singleFileBrowse);
    singleLayout->addWidget(singleFileReset);
    m_singleWidget->setLayout(singleLayout);
    m_mainLayout->addWidget(m_singleWidget);

    m_multipleWidget = new QWidget;
    auto multipleLayout = new QVBoxLayout;
    m_multipleFilePaths = new QPlainTextEdit;
    m_multipleFilePaths->setReadOnly(true);
    multipleLayout->addWidget(m_multipleFilePaths);
    auto multipleFileBrowse = new QPushButton("Browse");
    auto multipleFileReset = new QPushButton("Reset");
    auto multipleButtonLayout = new QHBoxLayout;
    multipleButtonLayout->addWidget(multipleFileBrowse);
    multipleButtonLayout->addWidget(multipleFileReset);
    multipleLayout->addLayout(multipleButtonLayout);
    m_multipleWidget->setLayout(multipleLayout);
    m_mainLayout->addWidget(m_multipleWidget);

    m_multipleWidget->hide();

    // TODO margin
    setLayout(m_mainLayout);

    connect(singleFileBrowse, &QPushButton::clicked, this, [=]() {
        auto func = m_isOpenFile ? &QFileDialog::getOpenFileName : &QFileDialog::getSaveFileName;
        auto s = func(this, m_title, {}, m_filter, &m_selectedFilter, {});
        if (!s.isEmpty()) {
            setFile(s);
        }
    });
    connect(singleFileReset, &QPushButton::clicked, this, &FileSelector::clearFiles);
    connect(multipleFileBrowse, &QPushButton::clicked, this, [=]() {
        auto s = QFileDialog::getOpenFileNames(this, m_title, {}, m_filter, &m_selectedFilter, {});
        if (!s.isEmpty()) {
            addFiles(s);
        }
    });
    connect(multipleFileReset, &QPushButton::clicked, this, &FileSelector::clearFiles);
}

FileSelector::~FileSelector() = default;

bool FileSelector::allowsMultipleFiles() const {
    return m_multipleWidget->isVisible();
}

void FileSelector::setAllowsMultipleFiles(bool allows) {
    m_multipleWidget->setVisible(allows);
    m_singleWidget->setVisible(!allows);
}

QString FileSelector::selectedFilter() const {
    return m_selectedFilter;
}

QJSValue FileSelector::files() const {
    return jsGlobal->engine()->toScriptValue(m_files);
}

void FileSelector::setFile(const QString &file) {
    m_singleFilePath->clear();
    m_multipleFilePaths->clear();
    m_files.clear();

    m_singleFilePath->setText(file);
    m_multipleFilePaths->setPlainText(file);
    m_files.append(JS_QOBJ(new File(file)));

    emit fileChanged();
}

void FileSelector::addFiles(const QStringList &filePaths) {
    for (const auto &s: filePaths) {
        m_singleFilePath->setText(s);
        m_multipleFilePaths->appendPlainText(s);
        m_files.append(JS_QOBJ(new File(s)));
    }
    emit fileChanged();
}

void FileSelector::clearFiles() {
    m_singleFilePath->clear();
    m_multipleFilePaths->clear();
    m_files.clear();
    emit fileChanged();
}
