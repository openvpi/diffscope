#include "debugpage.h"

#include <QApplication>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QBoxLayout>
#include <QMetaMethod>
#include <QMessageBox>
#include <QTest>

#include <CoreApi/iloader.h>


namespace Debug::Internal {

    class DebugPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit DebugPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;
            auto tabWidget = new QTabWidget;

            auto interactiveDebugPage = new QWidget;
            auto interactiveDebugPageLayout = new QHBoxLayout;

            auto objectsGroupBox = new QGroupBox(tr("Objects"));
            auto objectsGroupBoxLayout = new QVBoxLayout;
            auto objectsListWidget = new QListWidget;
            objectsGroupBoxLayout->addWidget(objectsListWidget);
            objectsGroupBox->setLayout(objectsGroupBoxLayout);
            interactiveDebugPageLayout->addWidget(objectsGroupBox);

            auto slotsGroupBox = new QGroupBox(tr("Slots"));
            auto slotsGroupBoxLayout = new QVBoxLayout;
            auto slotsListWidget = new QListWidget;
            slotsGroupBoxLayout->addWidget(slotsListWidget);
            slotsGroupBox->setLayout(slotsGroupBoxLayout);
            interactiveDebugPageLayout->addWidget(slotsGroupBox);

            interactiveDebugPage->setLayout(interactiveDebugPageLayout);
            tabWidget->addTab(interactiveDebugPage, tr("Interactive Debug"));

            auto unitTestPage = new QWidget;
            auto unitTestPageLayout = new QVBoxLayout;
            auto unitTestObjectsListWidget = new QListWidget;
            unitTestObjectsListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
            unitTestPageLayout->addWidget(unitTestObjectsListWidget);
            auto runUnitTestButton = new QPushButton(tr("Run Unit Test"));
            runUnitTestButton->setDisabled(true);
            unitTestPageLayout->addWidget(runUnitTestButton);
            unitTestPage->setLayout(unitTestPageLayout);
            tabWidget->addTab(unitTestPage, tr("Unit Test"));

            mainLayout->addWidget(tabWidget);

            setLayout(mainLayout);

            auto iLoader = Core::ILoader::instance();
            auto interactiveDebugObjects = iLoader->getObjects("debug.InteractiveDebug");
            for (auto object : qAsConst(interactiveDebugObjects)) {
                auto item = new QListWidgetItem;
                item->setText(object->metaObject()->className());
                item->setData(Qt::UserRole, QVariant::fromValue(object));
                objectsListWidget->addItem(item);
            }
            objectsListWidget->sortItems();

            auto unitTestObjects = iLoader->getObjects("debug.UnitTest");
            for (auto object : qAsConst(unitTestObjects)) {
                auto item = new QListWidgetItem;
                item->setText(object->metaObject()->className());
                item->setData(Qt::UserRole, QVariant::fromValue(object));
                unitTestObjectsListWidget->addItem(item);
            }
            unitTestObjectsListWidget->sortItems();

            connect(objectsListWidget, &QListWidget::currentItemChanged, this, [=](QListWidgetItem *item) {
                auto metaObject = item->data(Qt::UserRole).value<QObject *>()->metaObject();
                for (int i = metaObject->methodOffset(); i < metaObject->methodCount(); i++) {
                    auto method = metaObject->method(i);
                    if (method.parameterCount())
                        continue;
                    auto methodItem = new QListWidgetItem;
                    methodItem->setText(method.name());
                    methodItem->setData(Qt::UserRole, i);
                    slotsListWidget->addItem(methodItem);
                }
            });

            connect(slotsListWidget, &QListWidget::itemActivated, this, [=](QListWidgetItem *item) {
                auto methodIndex = item->data(Qt::UserRole).toInt();
                auto object = objectsListWidget->currentItem()->data(Qt::UserRole).value<QObject *>();
                object->metaObject()->method(methodIndex).invoke(object, Qt::DirectConnection);
            });

            connect(unitTestObjectsListWidget, &QListWidget::itemSelectionChanged, this, [=] {
                auto count = unitTestObjectsListWidget->selectedItems().size();
                if (count) {
                    runUnitTestButton->setText(tr("Run %1 Unit Test(s)", nullptr, count).arg(QLocale().toString(count)));
                    runUnitTestButton->setDisabled(false);
                } else {
                    runUnitTestButton->setText(tr("Run Unit Test"));
                    runUnitTestButton->setDisabled(true);
                }
            });

            connect(runUnitTestButton, &QAbstractButton::clicked, this, [=] {
                int passCount = 0;
                int failCount = 0;
                for (auto item : unitTestObjectsListWidget->selectedItems()) {
                    auto object = item->data(Qt::UserRole).value<QObject *>();
                    if (QTest::qExec(object) == 0)
                        passCount++;
                    else
                        failCount++;
                }
                QMessageBox::information(this, tr("Unit Testing Completed"), tr("Unit Test Completed. %1 Passed. %2 Failed.").arg(QLocale().toString(passCount), QLocale().toString(failCount)));
            });

        }

        ~DebugPageWidget() override = default;
    };

    DebugPage::DebugPage(QObject *parent) : Core::ISettingPage("debug.Debug", parent) {
        setTitle([] { return tr("Debug and Test (Advanced)"); });
        setDescription([] { return tr("Carry out debugging and testing on %1.").arg(QApplication::applicationName()); });
    }
    DebugPage::~DebugPage() = default;
    QString DebugPage::sortKeyword() const {
        return QStringLiteral("Debug");
    }
    bool DebugPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }
    QWidget *DebugPage::widget() {
        if (m_widget)
            return m_widget;
        m_widget = new DebugPageWidget;
        return m_widget;
    }
    bool DebugPage::accept() {
        return true;
    }
    void DebugPage::finish() {
    }
} // Internal

#include "debugpage.moc"