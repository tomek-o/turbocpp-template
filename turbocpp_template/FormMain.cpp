//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormAbout.h"
#include "FormSettings.h"
#include "Settings.h"
#include "LogUnit.h"
#include "Log.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
#ifdef ACCEPT_WM_DROPFILES
	// inform OS that we accepting dropping files
	DragAcceptFiles(Handle, True);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	AnsiString asConfigFile = ChangeFileExt( Application->ExeName, ".json" );
	appSettings.Read(asConfigFile);
	if (this->BorderStyle != bsSingle)
	{
		this->Width = appSettings.frmMain.width;
		this->Height = appSettings.frmMain.height;
	}
	this->Top = appSettings.frmMain.posY;
	this->Left = appSettings.frmMain.posX;
	if (appSettings.frmMain.alwaysOnTop)
		this->FormStyle = fsStayOnTop;
	else
		this->FormStyle = fsNormal;
	if (appSettings.frmMain.windowMaximized)
		this->WindowState = wsMaximized;
	if (appSettings.logging.logToFile)
		CLog::Instance()->SetFile(ChangeFileExt(Application->ExeName, ".log").c_str());
	else
		CLog::Instance()->SetFile("");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	AnsiString asConfigFile = ChangeFileExt( Application->ExeName, ".json" );
	appSettings.frmMain.windowMaximized = (this->WindowState == wsMaximized);
	if (!appSettings.frmMain.windowMaximized)
	{
		// these values are meaningless is wnd is maximized
		appSettings.frmMain.width = this->Width;
		appSettings.frmMain.height = this->Height;
		appSettings.frmMain.posY = this->Top;
		appSettings.frmMain.posX = this->Left;
	}
	appSettings.Write(asConfigFile);

	CanClose = true;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actShowAboutExecute(TObject *Sender)
{
	frmAbout->ShowModal();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actShowSettingsExecute(TObject *Sender)
{
	frmSettings->appSettings = &appSettings;
	frmSettings->ShowModal();
	if (appSettings.frmMain.alwaysOnTop)
		this->FormStyle = fsStayOnTop;
	else
		this->FormStyle = fsNormal;
	if (appSettings.logging.logToFile)
		CLog::Instance()->SetFile(ChangeFileExt(Application->ExeName, ".log").c_str());
	else
		CLog::Instance()->SetFile("");
	frmLog->SetLogLinesLimit(appSettings.logging.maxUiLogLines);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
    static bool once = false;
    if (!once)
    {
		once = true;
		frmLog->SetLogLinesLimit(appSettings.logging.maxUiLogLines);		
		CLog::Instance()->SetLevel(E_LOG_TRACE);
		CLog::Instance()->callbackLog = frmLog->OnLog;
		// make sure window position is not outside of available monitors
		OnRestore(NULL);
	}
	LOG("Application started");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	CLog::Instance()->Destroy();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actShowLogExecute(TObject *Sender)
{
	if (!frmLog->Visible)
		frmLog->Show();
	frmLog->BringToFront();
}
//---------------------------------------------------------------------------

#ifdef ACCEPT_WM_DROPFILES
// fires an event when a file, or files are dropped onto the application.
void __fastcall TfrmMain::WMDropFiles(TWMDropFiles &message)
{
    AnsiString FileName;
    FileName.SetLength(MAX_PATH);

	int Count = DragQueryFile((HDROP)message.Drop, 0xFFFFFFFF, NULL, MAX_PATH);

	// ignore all files but first one
	if (Count > 1)
		Count = 1;

	// index through the files and query the OS for each file name...
	for (int index = 0; index < Count; ++index)
	{
		// the following code gets the FileName of the dropped file. it
		// looks cryptic but that's only because it is. Hey, Why do you think
		// Delphi and C++Builder are so popular anyway? Look up DragQueryFile
		// the Win32.hlp Windows API help file.
		FileName.SetLength(DragQueryFile((HDROP)message.Drop, index,FileName.c_str(), MAX_PATH));
//		appSettings.Editor.asDefaultDir = ExtractFileDir(FileName);
		AppOpenFile(FileName);
	}

	// tell the OS that you're finished...
	DragFinish((HDROP) message.Drop);
}
#endif

void __fastcall TfrmMain::OnRestore(TObject *Sender)
{
	bool monitorFound = false;
	for (int i=0; i<Screen->MonitorCount; i++)
	{
		TMonitor *monitor = Screen->Monitors[i];
		enum { MARGIN = 30 };
		if (
			(Left + Width + MARGIN >= monitor->Left) &&
			(Left - MARGIN <= monitor->Left + monitor->Width) &&
			(Top + 5 >= monitor->Top) &&
			(Top - MARGIN <= monitor->Top + monitor->Height)
			)
		{
			monitorFound = true;
			break;
		}
	}
	if (!monitorFound)
	{
		if (Screen->MonitorCount > 0)
		{
			LOG("Moving main window to first monitor\n");
			TMonitor *monitor = Screen->Monitors[0];
			Left = monitor->Left + 50;
			Top = monitor->Top + 50;
			if (monitor)
			{
				LOG("Moving main window to first monitor\n");
				Left = monitor->Left + 50;
				Top = monitor->Top + 50;
			}
		}
	}
}
