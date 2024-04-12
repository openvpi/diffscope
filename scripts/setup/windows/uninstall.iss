[CustomMessages]
english.ButtonUninstall=Uninstall
english.WizardSelectTasks=Select Additional Tasks
english.SelectTasksDesc=Which additional tasks should be performed?
english.SelectTasksLabel2=Select the additional tasks you would like Uninstaller to perform, then click Uninstall.
english.RemoveUserDataDesc=Remove all user data and temporary files
english.RemovePluginsDesc=Remove all plugins

chinesesimplified.ButtonUninstall=卸载
chinesesimplified.WizardSelectTasks=选择附加任务
chinesesimplified.SelectTasksDesc=您想要卸载程序执行哪些附加任务？
chinesesimplified.SelectTasksLabel2=选择您想要卸载程序要执行的附加任务，然后点击“卸载”。
chinesesimplified.RemoveUserDataDesc=删除所有用户数据与临时文件
chinesesimplified.RemovePluginsDesc=删除所有插件

chinesetraditional.ButtonUninstall=解除安裝
chinesetraditional.WizardSelectTasks=選擇附加的工作
chinesetraditional.SelectTasksDesc=選擇要執行的附加工作。
chinesetraditional.SelectTasksLabel2=選擇解除安裝程式要執行的附加工作，然後按「解除安裝」。
chinesetraditional.RemoveUserDataDesc=刪除所有用戶數據與暫存檔案
chinesetraditional.RemovePluginsDesc=刪除所有外掛程式

japanese.ButtonUninstall=アンインストール
japanese.WizardSelectTasks=追加タスクの選択
japanese.SelectTasksDesc=実行する追加タスクを選択してください。
japanese.SelectTasksLabel2=アンインストール時に実行する追加タスクを選択して、「アンインストール」をクリックしてください。
japanese.RemoveUserDataDesc=すべてのユーザーデータと一時ファイルを削除する
japanese.RemovePluginsDesc=すべてのプラグインを削除する


[Code]

var
    DeleteUserDataCheckBox: TNewCheckBox;
    DeletePluginsCheckBox: TNewCheckBox;

function IsDirectoryEmpty(const Directory: string): Boolean;
var
    FindRec: TFindRec;
    IsEmpty: Boolean;
begin
    IsEmpty := True;
    if FindFirst(Directory + '\*.*', FindRec) then
    begin
        try
            repeat
                if (FindRec.Name <> '.') and (FindRec.Name <> '..') then
                begin
                    IsEmpty := False;
                    Break;
                end;
            until not FindNext(FindRec);
        finally
            FindClose(FindRec);
        end;
    end;
    Result := IsEmpty;
end;

procedure RemoveAppDirectory(Dir: String);
var
    OrgDataDir: String;
    DataDir: String;
begin
    OrgDataDir := Dir + '\OpenVPI'
    DataDir := OrgDataDir + ExpandConstant('\{#MyAppName}')

    if not DirExists(DataDir) then
    begin
        Exit;
    end;

    // Remove data directory
    DelTree(DataDir, True, True, True);

    // Remove org data directory if empty
    if (IsDirectoryEmpty(OrgDataDir)) then
    begin
        RemoveDir(OrgDataDir);
    end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
    InstallPluginsDir: String;
begin
    if CurUninstallStep = usPostUninstall then
    begin
        if DeleteUserDataCheckBox.Checked then
        begin
            RemoveAppDirectory(ExpandConstant('{userdocs}'));
            RemoveAppDirectory(ExpandConstant('{userappdata}'));
            RemoveAppDirectory(GetEnv('TMP'));
            RemoveAppDirectory(GetEnv('TEMP'));
        end;

        if DeletePluginsCheckBox.Checked then
        begin
            InstallPluginsDir := ExpandConstant('{app}\lib\{#MyAppName}\plugins')
            if (DirExists(InstallPluginsDir)) then
            begin
                DelTree(InstallPluginsDir, True, True, True);
            end;
        end;
    end;
end;

// https://stackoverflow.com/questions/7415457/custom-inno-setup-uninstall-page-not-msgbox

var
    UninstallFirstPage: TNewNotebookPage;
    UninstallButton: TNewButton;
    PageText: TNewStaticText;

procedure UninstallButtonClick(Sender: TObject);
begin
      UninstallButton.Visible := False;
end;

procedure AdjustButtonPositions(LeftButton: TNewButton; RightButton: TNewButton);
var
    HalfButtonWidth: Integer;
begin
    HalfButtonWidth := RightButton.Width div 4;
    RightButton.Left := RightButton.Left - 2 * HalfButtonWidth;
    RightButton.Width := RightButton.Width + 2 * HalfButtonWidth;

    LeftButton.Left := RightButton.Left - RightButton.Width - ScaleX(10);
    LeftButton.Top := RightButton.Top;
    LeftButton.Width := RightButton.Width;
    LeftButton.Height := RightButton.Height;
end;

procedure UpdateUninstallWizard;
begin
    UninstallProgressForm.PageNameLabel.Caption := ExpandConstant('{cm:WizardSelectTasks}');
    UninstallProgressForm.PageDescriptionLabel.Caption := ExpandConstant('{cm:SelectTasksDesc}');
    PageText.Caption := ExpandConstant('{cm:SelectTasksLabel2}');

    UninstallButton.Caption := ExpandConstant('{cm:ButtonUninstall}');

    // Make the "Uninstall" button break the ShowModal loop
    UninstallButton.ModalResult := mrOK;
end;

procedure InitializeUninstallProgressForm();
var
    PageNameLabel: string;
    PageDescriptionLabel: string;
    CancelButtonEnabled: Boolean;
    CancelButtonModalResult: Integer;

begin
    if not UninstallSilent then
    begin
        // First page
        UninstallFirstPage := TNewNotebookPage.Create(UninstallProgressForm);
        UninstallFirstPage.Notebook := UninstallProgressForm.InnerNotebook;
        UninstallFirstPage.Parent := UninstallProgressForm.InnerNotebook;
        UninstallFirstPage.Align := alClient;
    
        // Label
        PageText := TNewStaticText.Create(UninstallProgressForm);
        PageText.Parent := UninstallFirstPage;
        PageText.Top := UninstallProgressForm.StatusLabel.Top;
        PageText.Left := UninstallProgressForm.StatusLabel.Left;
        PageText.Width := UninstallProgressForm.StatusLabel.Width;
        PageText.Height := UninstallProgressForm.StatusLabel.Height;
        // PageText.AutoSize := False;
        PageText.WordWrap := True;
        PageText.ShowAccelChar := False;

        // First checkbox - Delete user data
        DeleteUserDataCheckBox := TNewCheckBox.Create(UninstallProgressForm);
        DeleteUserDataCheckBox.Parent := UninstallFirstPage;
        DeleteUserDataCheckBox.Top := PageText.Top + PageText.Height + ScaleY(30);
        DeleteUserDataCheckBox.Left := PageText.Left;
        DeleteUserDataCheckBox.Width := UninstallProgressForm.StatusLabel.Width;
        DeleteUserDataCheckBox.Height := PageText.Height;
        DeleteUserDataCheckBox.Caption := ExpandConstant('{cm:RemoveUserDataDesc}');
        DeleteUserDataCheckBox.Checked := True; // Default Checked

        // Second checkbox - Delete plugins
        DeletePluginsCheckBox := TNewCheckBox.Create(UninstallProgressForm);
        DeletePluginsCheckBox.Parent := UninstallFirstPage;
        DeletePluginsCheckBox.Top := DeleteUserDataCheckBox.Top + DeleteUserDataCheckBox.Height + ScaleY(10);
        DeletePluginsCheckBox.Left := DeleteUserDataCheckBox.Left;
        DeletePluginsCheckBox.Width := UninstallProgressForm.StatusLabel.Width;
        DeletePluginsCheckBox.Height := PageText.Height;
        DeletePluginsCheckBox.Caption := ExpandConstant('{cm:RemovePluginsDesc}');
        DeletePluginsCheckBox.Checked := True; // Default Checked

        // Uninstall button
        UninstallButton := TNewButton.Create(UninstallProgressForm);
        UninstallButton.Parent := UninstallProgressForm;
        UninstallButton.OnClick := @UninstallButtonClick;
        UninstallButton.TabOrder := UninstallProgressForm.CancelButton.TabOrder;

        UninstallProgressForm.CancelButton.TabOrder := UninstallButton.TabOrder + 1;

        // Adjust button positions
        AdjustButtonPositions(UninstallButton, UninstallProgressForm.CancelButton);
    
        // Make first page active
        UninstallProgressForm.InnerNotebook.ActivePage := UninstallFirstPage;

        PageNameLabel := UninstallProgressForm.PageNameLabel.Caption;
        PageDescriptionLabel := UninstallProgressForm.PageDescriptionLabel.Caption;

        // Run our wizard pages
        UpdateUninstallWizard;
        CancelButtonEnabled := UninstallProgressForm.CancelButton.Enabled
        UninstallProgressForm.CancelButton.Enabled := True;
        CancelButtonModalResult := UninstallProgressForm.CancelButton.ModalResult;
        UninstallProgressForm.CancelButton.ModalResult := mrCancel;

        if UninstallProgressForm.ShowModal = mrCancel then Abort;

        // Restore the standard page payout
        UninstallProgressForm.CancelButton.Enabled := CancelButtonEnabled;
        UninstallProgressForm.CancelButton.ModalResult := CancelButtonModalResult;

        UninstallProgressForm.PageNameLabel.Caption := PageNameLabel;
        UninstallProgressForm.PageDescriptionLabel.Caption := PageDescriptionLabel;

        UninstallProgressForm.InnerNotebook.ActivePage := UninstallProgressForm.InstallingPage;
    end;
end;
