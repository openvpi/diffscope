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

    void ProjectAddOn::initActions() {
        auto iWin = windowHandle()->cast<IProjectWindow>();

        // Actions
        ai.NewFile = AppExtra::actionItem(QStringLiteral("NewFile"), this);
        ai.OpenFile = AppExtra::actionItem(QStringLiteral("OpenFile"), this);
        ai.SaveFile = AppExtra::actionItem(QStringLiteral("SaveFile"), this);
        ai.SaveAsFile = AppExtra::actionItem(QStringLiteral("SaveAsFile"), this);
        ai.CloseFile = AppExtra::actionItem(QStringLiteral("CloseFile"), this);
        ai.Settings = AppExtra::actionItem(QStringLiteral("Settings"), this);
        ai.ColorThemes = AppExtra::actionItem(QStringLiteral("ColorThemes"), this);
        ai.Languages = AppExtra::actionItem(QStringLiteral("Languages"), this);
        ai.Undo = AppExtra::actionItem(QStringLiteral("Undo"), this);
        ai.Redo = AppExtra::actionItem(QStringLiteral("Redo"), this);
        ai.Cut = AppExtra::actionItem(QStringLiteral("Cut"), this);
        ai.Copy = AppExtra::actionItem(QStringLiteral("Copy"), this);
        ai.Paste = AppExtra::actionItem(QStringLiteral("Paste"), this);
        ai.Delete = AppExtra::actionItem(QStringLiteral("Delete"), this);
        ai.SelectAll = AppExtra::actionItem(QStringLiteral("SelectAll"), this);
        ai.Deselect = AppExtra::actionItem(QStringLiteral("Deselect"), this);
        ai.Find = AppExtra::actionItem(QStringLiteral("Find"), this);
        ai.Replace = AppExtra::actionItem(QStringLiteral("Replace"), this);
        ai.MainMenuVisible = AppExtra::actionItem(QStringLiteral("MainMenuVisible"), this);
        ai.MainToolbarVisible = AppExtra::actionItem(QStringLiteral("MainToolbarVisible"), this);
        ai.DockVisible = AppExtra::actionItem(QStringLiteral("DockVisible"), this);
        ai.StatusBarVisible = AppExtra::actionItem(QStringLiteral("StatusBarVisible"), this);
        ai.Play = AppExtra::actionItem(QStringLiteral("Play"), this);
        ai.Stop = AppExtra::actionItem(QStringLiteral("Stop"), this);
        ai.MoveToStart = AppExtra::actionItem(QStringLiteral("MoveToStart"), this);
        ai.MoveToEnd = AppExtra::actionItem(QStringLiteral("MoveToEnd"), this);
        ai.Metronome = AppExtra::actionItem(QStringLiteral("Metronome"), this);
        ai.LoopPlay = AppExtra::actionItem(QStringLiteral("LoopPlay"), this);
        ai.ShowHome = AppExtra::actionItem(QStringLiteral("ShowHome"), this);
        ai.FindAction = AppExtra::actionItem(QStringLiteral("FindAction"), this);
        ai.AboutPlugins = AppExtra::actionItem(QStringLiteral("AboutPlugins"), this);
        ai.AboutApp = AppExtra::actionItem(QStringLiteral("AboutApp"), this);
        ai.AboutQt = AppExtra::actionItem(QStringLiteral("AboutQt"), this);
        ai.ShowRecentFiles = AppExtra::actionItem(QStringLiteral("ShowRecentFiles"), this);

        // Widgets
        ai.QuantizeSelector = new ActionItem(QStringLiteral("QuantizeSelector"),
                                                ActionItem::WidgetFactory([](QWidget *parent) {
                                                    return new QLabel("1/4", parent); //
                                                }),
                                                this);
        ai.TimerLabel = new ActionItem(QStringLiteral("TimerLabel"),
                                          ActionItem::WidgetFactory([](QWidget *parent) {
                                              return new QLabel("00:00:00", parent); //
                                          }),
                                          this);

        // TopLevels
        ai.MainMenu = new ActionItem(QStringLiteral("MainMenu"), iWin->menuBar(), this);
        ai.MainToolBar =
            new ActionItem(QStringLiteral("MainToolBar"), iWin->mainToolbar(), this);

        iWin->addActionItems({
            ai.NewFile,
            ai.OpenFile,
            ai.SaveFile,
            ai.SaveAsFile,
            ai.CloseFile,
            ai.Settings,
            ai.ColorThemes,
            ai.Languages,
            ai.Undo,
            ai.Redo,
            ai.Cut,
            ai.Copy,
            ai.Paste,
            ai.Delete,
            ai.SelectAll,
            ai.Deselect,
            ai.Find,
            ai.Replace,
            ai.MainMenuVisible,
            ai.MainToolbarVisible,
            ai.DockVisible,
            ai.StatusBarVisible,
            ai.Play,
            ai.Stop,
            ai.MoveToStart,
            ai.MoveToEnd,
            ai.Metronome,
            ai.LoopPlay,
            ai.ShowHome,
            ai.FindAction,
            ai.AboutPlugins,
            ai.AboutApp,
            ai.AboutQt,
            ai.ShowRecentFiles,

            ai.TimerLabel,
            ai.QuantizeSelector,

            ai.MainMenu,
            ai.MainToolBar,
        });
    }

}