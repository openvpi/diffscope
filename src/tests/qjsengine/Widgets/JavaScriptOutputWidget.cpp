#include "JavaScriptOutputWidget.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontDialog>
#include <QMenuBar>
#include <QPainter>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTextTable>
#include <QTime>
#include <QVBoxLayout>
#include <QtSvg/QSvgRenderer>

#include "QMSystem.h"

#include "../Global/Console.h"
#include "../Global/GlobalObject.h"

JavaScriptOutputWidget::JavaScriptOutputWidget(QWidget *parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto menuBar = new QMenuBar;

    auto fileMenu = menuBar->addMenu(tr("&File"));

    auto exportAction = fileMenu->addAction(tr("E&xport"));
    connect(exportAction, &QAction::triggered, this, [=]() {
        auto initialFileName = "javascript-output-" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".log";
        auto filePath = QFileDialog::getSaveFileName(this, {}, initialFileName, QString("%1 (*.txt *.log)").arg(tr("Text files")));
        if (filePath.isEmpty())
            return;
        QFile f(filePath);
        f.open(QIODevice::WriteOnly);
        f.write(m_cachedMessages.toUtf8());
    });

    auto editMenu = menuBar->addMenu(tr("&Edit"));

    auto copyAction = editMenu->addAction(tr("&Copy"));
    copyAction->setShortcut(QKeySequence::Copy);

    editMenu->addSeparator();

    auto findAction = editMenu->addAction(tr("&Find"));
    findAction->setShortcut(QKeySequence::Find);

    auto findNextAction = editMenu->addAction(tr("Find &Next"));
    findNextAction->setShortcut(QKeySequence::FindNext);

    auto findPreviousAction = editMenu->addAction(tr("Find &Previous"));
    findPreviousAction->setShortcut(QKeySequence::FindPrevious);

    editMenu->addSeparator();

    auto selectAllAction = editMenu->addAction(tr("Select &All"));
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, this, [=]() {
        m_textEdit->selectAll();
    });

    editMenu->addSeparator();

    auto clearAction = editMenu->addAction(tr("C&lear"));
    connect(clearAction, &QAction::triggered, this, &JavaScriptOutputWidget::clear);

    auto viewMenu = menuBar->addMenu(tr("&View"));
    auto errorAction = viewMenu->addAction(tr("&Error"));
    errorAction->setCheckable(true);
    errorAction->setChecked(true);
    connect(errorAction, &QAction::triggered, this, [=](bool checked) {
        setLevelFlag(Console::Error, checked);
    });

    auto warningAction = viewMenu->addAction(tr("&Warning"));
    warningAction->setCheckable(true);
    warningAction->setChecked(true);
    connect(warningAction, &QAction::triggered, this, [=](bool checked) {
        setLevelFlag(Console::Warning, checked);
    });

    auto logAction = viewMenu->addAction(tr("&Log"));
    logAction->setCheckable(true);
    logAction->setChecked(true);
    connect(logAction, &QAction::triggered, this, [=](bool checked) {
        setLevelFlag(Console::Log, checked);
    });

    auto infoAction = viewMenu->addAction(tr("&Info"));
    infoAction->setCheckable(true);
    infoAction->setChecked(true);
    connect(infoAction, &QAction::triggered, this, [=](bool checked) {
        setLevelFlag(Console::Info, checked);
    });

    auto debugAction = viewMenu->addAction(tr("&Debug"));
    debugAction->setCheckable(true);
    debugAction->setChecked(true);
    connect(debugAction, &QAction::triggered, this, [=](bool checked) {
        setLevelFlag(Console::Debug, checked);
    });

    viewMenu->addSeparator();

    auto showFileTraceAction = viewMenu->addAction(tr("Show &File Trace"));
    showFileTraceAction->setCheckable(true);
    showFileTraceAction->setChecked(true);
    connect(showFileTraceAction, &QAction::triggered, this, [=](bool checked) {
        m_showFileTrace = checked;
    });

    auto showTimeAction = viewMenu->addAction(tr("Show &Time"));
    showTimeAction->setCheckable(true);
    showTimeAction->setChecked(false);
    connect(showTimeAction, &QAction::triggered, this, [=](bool checked) {
        m_showTime = checked;
    });

    viewMenu->addSeparator();

    auto fontAction = viewMenu->addAction(tr("Fo&nt"));
    connect(fontAction, &QAction::triggered, this, [=]() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, m_textEdit->font(), this);
        if (ok) {
            m_textEdit->setFont(font);
            // TODO save font
        }
    });

    auto wordWrapAction = viewMenu->addAction(tr("W&ord Wrap"));
    wordWrapAction->setCheckable(true);
    wordWrapAction->setChecked(true);
    connect(wordWrapAction, &QAction::triggered, this, [=](bool checked) {
        if (checked)
            m_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        else
            m_textEdit->setLineWrapMode(QTextEdit::NoWrap);
    });

    mainLayout->addWidget(menuBar);

    m_textEdit = new QTextBrowser;
    m_textEdit->setOpenLinks(false);
    QFont font("Monospace", 10);
    font.setStyleHint(QFont::TypeWriter);
    m_textEdit->setFont(font);

    connect(m_textEdit, &QTextBrowser::anchorClicked, this, [=](const QUrl &url) {
        QDesktopServices::openUrl(url);
    });
    mainLayout->addWidget(m_textEdit);

    setMinimumSize(600, 400);
    setWindowTitle(tr("JavaScript Output"));

    connect(jsGlobal->console(), &Console::messagePrinted, this, &JavaScriptOutputWidget::receiveMessage);
    connect(jsGlobal->console(), &Console::clearRequested, this, &JavaScriptOutputWidget::clear);

}

void JavaScriptOutputWidget::receiveMessage(int level, const QString &message, const QString &fileTrace) {
    QString levelText = QStringList{"[Debug]", "[Info]", "[Log]", "[Warning]", "[Error]"}[level];
    auto timeText = QString("[%1]").arg(QTime::currentTime().toString());
    for (const auto &s: message.split('\n'))
        m_cachedMessages += (levelText + "[" + fileTrace + "]" + timeText + " " + s + "\n");

    if (!(m_levelFlags & (1 << level)))
        return;
    QColor levelColor = QList<QColor>{{0x16, 0xa0, 0x85}, {0x40, 0x73, 0x9e}, {}, {0xe6, 0x7e, 0x22}, {0xc0, 0x39, 0x2b}}[level];
    QColor levelBackground = QList<QColor>{{}, {0xa5, 0xd8, 0xff}, {}, {0xff, 0xec, 0x99}, {0xff, 0xc9, 0xc9}}[level];
    QString levelImage = QStringList{{}, ":/icons/info.svg", {}, ":/icons/warning.svg", ":/icons/error.svg"}[level];
    QTextCursor cur(m_textEdit->document());
    cur.movePosition(QTextCursor::End);
    cur.movePosition(QTextCursor::PreviousBlock);
    QTextTable *table = cur.currentTable();
    if (!table) {
        QTextTableFormat tableFmt;
        tableFmt.setWidth(QTextLength(QTextLength::PercentageLength, 100));
        tableFmt.setCellSpacing(0);
        tableFmt.setCellPadding(4);
        QBrush bs(Qt::BrushStyle::SolidPattern);
        bs.setColor({0xbf, 0xbf, 0xbf});
        tableFmt.setBorderBrush(bs);
        table = cur.insertTable(1, 1, tableFmt);
    } else {
        table->appendRows(1);
    }
    QTextCharFormat cellFmt;
    if (levelBackground.isValid())
        cellFmt.setBackground(QBrush{levelBackground});
    table->cellAt(table->rows() - 1, 0).setFormat(cellFmt);
    cur = table->cellAt(table->rows() - 1, 0).firstCursorPosition();

    QTextCharFormat fmt;
    fmt.setForeground(QBrush(levelColor));

    if (!levelImage.isEmpty()) {
        QTextTableFormat iconTableFmt;
        iconTableFmt.setBorder(0);
        iconTableFmt.setMargin(0);
        auto iconTable = cur.insertTable(1, 2, iconTableFmt);
        cur = iconTable->cellAt(0, 0).firstCursorPosition();
        double ratio = qApp->primaryScreen()->logicalDotsPerInch() / QMOs::unitDpi();
        QImage img(12 * ratio, 12 * ratio, QImage::Format_ARGB32);
        img.fill(QColor(0, 0, 0, 0));
        QSvgRenderer renderer(levelImage);
        QPainter painter(&img);
        renderer.render(&painter);
        cur.insertImage(img);
        cur = iconTable->cellAt(0, 1).firstCursorPosition();
    }
    QRegExp rx(R"(file://[A-Za-z0-9\$\-_\.\+!\*'\(\)\/&\?=:%]*)");
    int pos = 0;
    int offsetPos = 0;
    while ((pos = rx.indexIn(message, offsetPos)) != -1) {
        cur.insertText(message.mid(offsetPos, pos - offsetPos), fmt);
        fmt.setAnchor(true);
        auto href = rx.cap();
        href.replace(QRegExp("(:[0-9]+)+$"), "");
        fmt.setAnchorHref(href);
        fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        cur.insertText(rx.cap(), fmt);
        fmt.setAnchor(false);
        fmt.setAnchorHref({});
        fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);
        offsetPos = pos + rx.matchedLength();
    }
    cur.insertText(message.mid(offsetPos), fmt);
    fmt.setForeground(QBrush(QColor{0x7f, 0x7f, 0x7f}));
    cur.insertText(" ", fmt);


    // File Trace
    if (m_showFileTrace && !fileTrace.isEmpty()) {
        cur.insertText("[", fmt);
        QUrl fileUrl(fileTrace);
        if (fileUrl.isLocalFile()) {
            fmt.setAnchor(true);
            auto href = fileTrace;
            href.replace(QRegExp("(:[0-9]+)+$"), "");
            fmt.setAnchorHref(href);
            fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        }
        cur.insertText(fileUrl.fileName(), fmt);
        fmt.setAnchor(false);
        fmt.setAnchorHref({});
        fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);
        cur.insertText("]", fmt);
    }

    // Time
    if (m_showTime) {
        cur.insertText(timeText, fmt);
    }

}

void JavaScriptOutputWidget::clear() {
    m_textEdit->clear();
    m_cachedMessages.clear();
}

void JavaScriptOutputWidget::setLevelFlag(int flag, bool enabled) {
    if (enabled)
        m_levelFlags |= (1 << flag);
    else
        m_levelFlags &= ~(1 << flag);
}
