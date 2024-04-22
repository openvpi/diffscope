#ifndef PROJECTADDON_H
#define PROJECTADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindowaddon.h>
#include <CoreApi/actionitem.h>

namespace Core::Internal {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();

    protected:
        void initActions();

        struct ActionItems {
            Core::ActionItem *NewFile;
            Core::ActionItem *OpenFile;
            Core::ActionItem *SaveFile;
            Core::ActionItem *SaveAsFile;
            Core::ActionItem *CloseFile;
            Core::ActionItem *Settings;
            Core::ActionItem *ColorThemes;
            Core::ActionItem *Languages;
            Core::ActionItem *Undo;
            Core::ActionItem *Redo;
            Core::ActionItem *Cut;
            Core::ActionItem *Copy;
            Core::ActionItem *Paste;
            Core::ActionItem *Delete;
            Core::ActionItem *SelectAll;
            Core::ActionItem *Deselect;
            Core::ActionItem *Find;
            Core::ActionItem *Replace;
            Core::ActionItem *MainMenuVisible;
            Core::ActionItem *MainToolbarVisible;
            Core::ActionItem *DockVisible;
            Core::ActionItem *StatusBarVisible;
            Core::ActionItem *Play;
            Core::ActionItem *Stop;
            Core::ActionItem *MoveToStart;
            Core::ActionItem *MoveToEnd;
            Core::ActionItem *Metronome;
            Core::ActionItem *LoopPlay;
            Core::ActionItem *ShowHome;
            Core::ActionItem *FindAction;
            Core::ActionItem *AboutPlugins;
            Core::ActionItem *AboutApp;
            Core::ActionItem *AboutQt;
            Core::ActionItem *ShowRecentFiles;
            Core::ActionItem *OpenRecent;
        };

        struct WidgetItems {
            Core::ActionItem *TimerLabel;
            Core::ActionItem *QuantizeSelector;
        };

        struct TopLevelItems {
            Core::ActionItem *MainMenu;
            Core::ActionItem *MainToolBar;
        };

        struct ActionExtensionItems {
            ActionItems a;
            WidgetItems w;
            TopLevelItems t;
        };

        ActionExtensionItems aei;
    };

}

#endif // PROJECTADDON_H
