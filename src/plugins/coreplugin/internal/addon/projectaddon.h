#ifndef PROJECTADDON_H
#define PROJECTADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindow.h>
#include <CoreApi/actionitem.h>

namespace Core::Internal {

    class ProjectAddOn : public IWindowAddOn {
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
            ActionItem *NewFile;
            ActionItem *OpenFile;
            ActionItem *SaveFile;
            ActionItem *SaveAsFile;
            ActionItem *CloseFile;
            ActionItem *Settings;
            ActionItem *ColorThemes;
            ActionItem *Languages;
            ActionItem *Undo;
            ActionItem *Redo;
            ActionItem *Cut;
            ActionItem *Copy;
            ActionItem *Paste;
            ActionItem *Delete;
            ActionItem *SelectAll;
            ActionItem *Deselect;
            ActionItem *Find;
            ActionItem *Replace;
            ActionItem *MainMenuVisible;
            ActionItem *MainToolbarVisible;
            ActionItem *DockVisible;
            ActionItem *StatusBarVisible;
            ActionItem *Play;
            ActionItem *Stop;
            ActionItem *MoveToStart;
            ActionItem *MoveToEnd;
            ActionItem *Metronome;
            ActionItem *LoopPlay;
            ActionItem *ShowHome;
            ActionItem *FindAction;
            ActionItem *AboutPlugins;
            ActionItem *AboutApp;
            ActionItem *AboutQt;
            ActionItem *ShowRecentFiles;

            ActionItem *TimerLabel;
            ActionItem *QuantizeSelector;

            ActionItem *MainMenu;
            ActionItem *MainToolbar;
        };

        ActionItems ai;
    };

}

#endif // PROJECTADDON_H
