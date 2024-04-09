; Pre-defined Environments:
;   SETUP_APP_NAME
;   SETUP_APP_VERSION
;   SETUP_APP_INSTALLED_DIR
;   SETUP_APP_BRIDGE_ARTEFACTS_DIR
;   SETUP_MESSAGE_FILES_DIR
;   SETUP_OUTPUT_DIR
;   SETUP_OUTPUT_FILE_BASE
;   GPL3_LICENSE_PATH

#define MyAppName GetEnv("SETUP_APP_NAME")
#define MyAppVersion GetEnv("SETUP_APP_VERSION")
#define MyAppPublisher "Team OpenVPI"
#define MyAppURL "https://www.example.com/"
#define MyAppCopyright "Copyright 2019-2024 Team OpenVPI"

#define MyAppExeName "bin\" + MyAppName + ".exe"

#define MyAppProjectAssocName MyAppName + " Project File"
#define MyAppProjectAssocExt ".dspx"
#define MyAppProjectAssocKey StringChange(MyAppProjectAssocName, " ", "") + MyAppProjectAssocExt

#define MyAppSoundBankAssocName MyAppName + " Sound Bank"
#define MyAppSoundBankAssocExt ".aaa"
#define MyAppSoundBankAssocKey StringChange(MyAppProjectAssocName, " ", "") + MyAppProjectAssocExt

#define MyAppInstalledDir GetEnv("SETUP_APP_INSTALLED_DIR")

#define MyAppBridgeArtefactsDir GetEnv("SETUP_APP_BRIDGE_ARTEFACTS_DIR")

[Setup]
AppId={{E777B852-937C-42A1-930F-C1CD1FF5C07E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppCopyright={#MyAppCopyright}
DefaultDirName={autopf}\OpenVPI\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
LicenseFile={#MyAppInstalledDir}\share\doc\DiffScope\LICENSE
InfoBeforeFile={#GetEnv("GPL3_LICENSE_PATH")}
OutputDir={#GetEnv("SETUP_OUTPUT_DIR")}
OutputBaseFilename={#GetEnv("SETUP_OUTPUT_FILE_BASE")}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64
VersionInfoDescription={#MyAppName} Installer

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "{#GetEnv('SETUP_MESSAGE_FILES_DIR')}\ChineseSimplified.isl"
Name: "chinesetraditional"; MessagesFile: "{#GetEnv('SETUP_MESSAGE_FILES_DIR')}\ChineseTraditional.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[CustomMessages]
english.RemoveUserDataDesc=Do you want to remove all user data and temporary files?
chinesesimplified.RemoveUserDataDesc=是否删除所有用户数据与临时文件？
chinesetraditional.RemoveUserDataDesc=是否刪除所有用戶數據與暫存檔案？
japanese.RemoveUserDataDesc=すべてのユーザーデータと一時ファイルを削除しますか？

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Code]
var
    DeleteUserData: Boolean;

function FullInstallationMessage(Param: String): String;
begin
    Result := SetupMessage(msgFullInstallation);
end;

function CompactInstallationMessage(Param: String): String;
begin
    Result := SetupMessage(msgCompactInstallation);
end;

function CustomInstallationMessage(Param: String): String;
begin
    Result := SetupMessage(msgCustomInstallation);
end;

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
begin
    if CurUninstallStep = usUninstall then
    begin
        DeleteUserData := False;
        if MsgBox(ExpandConstant('{cm:RemoveUserDataDesc}'), mbConfirmation, MB_YESNO) = IDYES then
            DeleteUserData := True
        else
            DeleteUserData := False;
    end
    else if CurUninstallStep = usPostUninstall then
    begin
        if DeleteUserData then
        begin
            RemoveAppDirectory(ExpandConstant('{userdocs}'));
            RemoveAppDirectory(ExpandConstant('{userappdata}'));
            RemoveAppDirectory(GetEnv('TMP'));
            RemoveAppDirectory(GetEnv('TEMP'));
        end;
    end;
end;

[Types]
Name: "full"; Description: "{code:FullInstallationMessage}"
Name: "compact"; Description: "{code:CompactInstallationMessage}"
Name: "custom"; Description: "{code:CustomInstallationMessage}"; Flags: iscustom

[Components]
Name: "program"; Description: "{#MyAppName}"; Types: full compact custom; Flags: fixed
Name: "vst"; Description: "{#MyAppName} Bridge (VST3)"; Types: full
Name: "lv2"; Description: "{#MyAppName} Bridge (LV2)"; Types: full

[Files]
Source: "{#MyAppInstalledDir}\*"; DestDir: "{app}"; Components: program; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyAppBridgeArtefactsDir}\VST3\*.vst3"; DestDir: "{autocf}\VST3"; Components: vst; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyAppBridgeArtefactsDir}\LV2\*.lv2"; DestDir: "{autocf}\LV2"; Components: lv2; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppProjectAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppProjectAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppProjectAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppProjectAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppProjectAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},1"
Root: HKA; Subkey: "Software\Classes\{#MyAppProjectAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: "{#MyAppProjectAssocExt}"; ValueData: ""

Root: HKA; Subkey: "Software\Classes\{#MyAppSoundBankAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppSoundBankAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppSoundBankAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppSoundBankAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppSoundBankAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},2"
Root: HKA; Subkey: "Software\Classes\{#MyAppSoundBankAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: "{#MyAppSoundBankAssocExt}"; ValueData: ""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

