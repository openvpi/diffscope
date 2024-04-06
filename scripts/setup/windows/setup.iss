#define MyAppName GetEnv("SETUP_APP_NAME")
#define MyAppVersion GetEnv("SETUP_APP_VERSION")
#define MyAppPublisher "Team OpenVPI"
#define MyAppURL "https://www.example.com/"

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
DefaultDirName={autopf}\OpenVPI\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
LicenseFile={#MyAppInstalledDir}\share\doc\DiffScope\license-en.txt
InfoBeforeFile={#GetEnv("GPL3_LICENSE_PATH")}
OutputBaseFilename=diffscope_setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "{#GetEnv('GITHUB_WORKSPACE')}\ChineseSimplified.isl"
Name: "chinesetraditional"; MessagesFile: "{#GetEnv('GITHUB_WORKSPACE')}\ChineseTraditional.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Code]
function FullInstallationMessage(Param: String): String; begin Result := SetupMessage(msgFullInstallation); end;
function CompactInstallationMessage(Param: String): String; begin Result := SetupMessage(msgCompactInstallation); end;
function CustomInstallationMessage(Param: String): String; begin Result := SetupMessage(msgCustomInstallation); end;

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

