; -----------------------------------------------------------------------------
; Foreign Keyboard - Inno Setup Installer Script
; Compiler: Inno Setup 6.x  (https://jrsoftware.org/isinfo.php)
;
; Build the CHM first:
;   hhc.exe help\ForeignKeyboard.hhp
; then build Release configuration in Visual Studio:
;   x64\Release\ForeignKeyboard.exe
; then compile this script with Inno Setup Compiler.
; -----------------------------------------------------------------------------

#define AppName        "Foreign Keyboard"
#define AppVersion     "1.0"
#define AppPublisher   "Mika Huttunen"
#define AppCopyright   "Copyright (C) 2026 Mika Huttunen"
#define AppExeName     "ForeignKeyboard.exe"
#define AppHelpFile    "ForeignKeyboard.chm"
#define SourceDir      "x64\Release"
#define HelpSourceDir  "help"

[Setup]
; --- Application identity ---
AppId={{A3F7B2C1-4D5E-4F6A-8B9C-0D1E2F3A4B5C}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
AppCopyright={#AppCopyright}
AppVerName={#AppName} {#AppVersion}

; --- Installer appearance ---
WizardStyle=modern
SetupIconFile=res\ForeignKeyboard.ico
UninstallDisplayIcon={app}\{#AppExeName}

; --- Installation target ---
; ArchitecturesInstallIn64BitMode tells Inno Setup to install into the native
; 64-bit Program Files folder (C:\Program Files) instead of Program Files (x86).
ArchitecturesInstallIn64BitMode=x64compatible
DefaultDirName={autopf}\{#AppName}
DefaultGroupName={#AppName}
AllowNoIcons=no

; --- Output ---
OutputDir=Installer
OutputBaseFilename=ForeignKeyboard_Setup_{#AppVersion}
Compression=lzma2/ultra64
SolidCompression=yes

; --- Privileges ---
; Use "lowest" so the app can install without admin rights when desired,
; or change to "admin" if you want a per-machine install.
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

; --- Misc ---
VersionInfoVersion={#AppVersion}.0.0
VersionInfoCompany={#AppPublisher}
VersionInfoDescription={#AppName} Installer
VersionInfoCopyright={#AppCopyright}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon";   Description: "Create a &desktop shortcut";         GroupDescription: "Additional shortcuts:"; Flags: unchecked
Name: "quicklaunch";   Description: "Create a &Quick Launch shortcut";    GroupDescription: "Additional shortcuts:"; Flags: unchecked; OnlyBelowVersion: 6.1

[Files]
; Main executable
Source: "{#SourceDir}\{#AppExeName}";  DestDir: "{app}"; Flags: ignoreversion

; CHM help file (built from help\ sources using hhc.exe)
Source: "{#HelpSourceDir}\{#AppHelpFile}"; DestDir: "{app}"; Flags: ignoreversion

; Visual C++ runtime DLLs (if you use static CRT linking, comment these out)
; Source: "{#SourceDir}\msvcp140.dll";   DestDir: "{app}"; Flags: ignoreversion
; Source: "{#SourceDir}\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
; Start Menu
Name: "{group}\{#AppName}";           Filename: "{app}\{#AppExeName}"
Name: "{group}\{#AppName} Help";      Filename: "{app}\{#AppHelpFile}"
Name: "{group}\Uninstall {#AppName}"; Filename: "{uninstallexe}"

; Desktop (optional task)
Name: "{autodesktop}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

; Quick Launch (optional task, Windows XP / Vista)
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: quicklaunch

[Run]
; Offer to launch the application after installation
Filename: "{app}\{#AppExeName}"; Description: "Launch {#AppName}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Remove any files the application may have created in its folder
Type: filesandordirs; Name: "{app}"
