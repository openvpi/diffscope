#include "projectaddon.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>

#include "iprojectwindow.h"
#include "icore.h"
#include "appextra.h"

namespace Core::Internal {

    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    ProjectAddOn::~ProjectAddOn() {
    }
    void ProjectAddOn::initialize() {
        initActions();
    }

    void ProjectAddOn::extensionsInitialized() {
    }

    void ProjectAddOn::reloadStrings() {
    }

    // Core::ActionItem \*(.+);
    // aei.a.$1 = new ActionItem(QStringLiteral("$1"), new QAction(), this);

    // Core::ActionItem \*(.+);
    // aei.m.$1 = AppExtra::menuItem(QStringLiteral("$1"), this);
    void ProjectAddOn::initActions() {
        auto iWin = windowHandle()->cast<IProjectWindow>();

        // Actions
        aei.a.NewFile = AppExtra::actionItem(QStringLiteral("NewFile"), this);
        aei.a.OpenFile = AppExtra::actionItem(QStringLiteral("OpenFile"), this);
        aei.a.SaveFile = AppExtra::actionItem(QStringLiteral("SaveFile"), this);
        aei.a.SaveAsFile = AppExtra::actionItem(QStringLiteral("SaveAsFile"), this);
        aei.a.CloseFile = AppExtra::actionItem(QStringLiteral("CloseFile"), this);
        aei.a.Settings = AppExtra::actionItem(QStringLiteral("Settings"), this);
        aei.a.ColorThemes = AppExtra::actionItem(QStringLiteral("ColorThemes"), this);
        aei.a.Languages = AppExtra::actionItem(QStringLiteral("Languages"), this);
        aei.a.Undo = AppExtra::actionItem(QStringLiteral("Undo"), this);
        aei.a.Redo = AppExtra::actionItem(QStringLiteral("Redo"), this);
        aei.a.Cut = AppExtra::actionItem(QStringLiteral("Cut"), this);
        aei.a.Copy = AppExtra::actionItem(QStringLiteral("Copy"), this);
        aei.a.Paste = AppExtra::actionItem(QStringLiteral("Paste"), this);
        aei.a.Delete = AppExtra::actionItem(QStringLiteral("Delete"), this);
        aei.a.SelectAll = AppExtra::actionItem(QStringLiteral("SelectAll"), this);
        aei.a.Deselect = AppExtra::actionItem(QStringLiteral("Deselect"), this);
        aei.a.Find = AppExtra::actionItem(QStringLiteral("Find"), this);
        aei.a.Replace = AppExtra::actionItem(QStringLiteral("Replace"), this);
        aei.a.MainMenuVisible = AppExtra::actionItem(QStringLiteral("MainMenuVisible"), this);
        aei.a.MainToolbarVisible = AppExtra::actionItem(QStringLiteral("MainToolbarVisible"), this);
        aei.a.DockVisible = AppExtra::actionItem(QStringLiteral("DockVisible"), this);
        aei.a.StatusBarVisible = AppExtra::actionItem(QStringLiteral("StatusBarVisible"), this);
        aei.a.Play = AppExtra::actionItem(QStringLiteral("Play"), this);
        aei.a.Stop = AppExtra::actionItem(QStringLiteral("Stop"), this);
        aei.a.MoveToStart = AppExtra::actionItem(QStringLiteral("MoveToStart"), this);
        aei.a.MoveToEnd = AppExtra::actionItem(QStringLiteral("MoveToEnd"), this);
        aei.a.Metronome = AppExtra::actionItem(QStringLiteral("Metronome"), this);
        aei.a.LoopPlay = AppExtra::actionItem(QStringLiteral("LoopPlay"), this);
        aei.a.ShowHome = AppExtra::actionItem(QStringLiteral("ShowHome"), this);
        aei.a.FindAction = AppExtra::actionItem(QStringLiteral("FindAction"), this);
        aei.a.AboutPlugins = AppExtra::actionItem(QStringLiteral("AboutPlugins"), this);
        aei.a.AboutApp = AppExtra::actionItem(QStringLiteral("AboutApp"), this);
        aei.a.AboutQt = AppExtra::actionItem(QStringLiteral("AboutQt"), this);
        aei.a.ShowRecentFiles = AppExtra::actionItem(QStringLiteral("ShowRecentFiles"), this);
        aei.a.OpenRecent = AppExtra::actionItem(QStringLiteral("OpenRecent"), this);

        // Widgets
        aei.w.QuantizeSelector = new ActionItem(QStringLiteral("QuantizeSelector"),
                                                ActionItem::WidgetFactory([](QWidget *parent) {
                                                    return new QLabel("1/4", parent); //
                                                }),
                                                this);
        aei.w.TimerLabel = new ActionItem(QStringLiteral("TimerLabel"),
                                          ActionItem::WidgetFactory([](QWidget *parent) {
                                              return new QLabel("00:00:00", parent); //
                                          }),
                                          this);

        // TopLevels
        aei.t.MainMenu = new ActionItem(QStringLiteral("MainMenu"), iWin->menuBar(), this);
        aei.t.MainToolBar =
            new ActionItem(QStringLiteral("MainToolBar"), iWin->mainToolbar(), this);

        iWin->addActionItems({
            aei.a.NewFile,
            aei.a.OpenFile,
            aei.a.SaveFile,
            aei.a.SaveAsFile,
            aei.a.CloseFile,
            aei.a.Settings,
            aei.a.ColorThemes,
            aei.a.Languages,
            aei.a.Undo,
            aei.a.Redo,
            aei.a.Cut,
            aei.a.Copy,
            aei.a.Paste,
            aei.a.Delete,
            aei.a.SelectAll,
            aei.a.Deselect,
            aei.a.Find,
            aei.a.Replace,
            aei.a.MainMenuVisible,
            aei.a.MainToolbarVisible,
            aei.a.DockVisible,
            aei.a.StatusBarVisible,
            aei.a.Play,
            aei.a.Stop,
            aei.a.MoveToStart,
            aei.a.MoveToEnd,
            aei.a.Metronome,
            aei.a.LoopPlay,
            aei.a.ShowHome,
            aei.a.FindAction,
            aei.a.AboutPlugins,
            aei.a.AboutApp,
            aei.a.AboutQt,
            aei.a.ShowRecentFiles,
            aei.a.OpenRecent,

            aei.w.TimerLabel,
            aei.w.QuantizeSelector,

            aei.t.MainMenu,
            aei.t.MainToolBar,
        });
    }

}