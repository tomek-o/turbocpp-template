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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	AnsiString asConfigFile = ChangeFileExt( Application->ExeName, ".json" );
	appSettings.Read(asConfigFile);
	if (this->BorderStyle != bsSingle)
	{
		this->Width = appSettings.frmMain.iWidth;
		this->Height = appSettings.frmMain.iHeight;
	}
	this->Top = appSettings.frmMain.iPosY;
	this->Left = appSettings.frmMain.iPosX;
	if (appSettings.frmMain.bAlwaysOnTop)
		this->FormStyle = fsStayOnTop;
	else
		this->FormStyle = fsNormal;
	if (appSettings.frmMain.bWindowMaximized)
		this->WindowState = wsMaximized;
	if (appSettings.Logging.bLogToFile)
		CLog::Instance()->SetFile(ChangeFileExt(Application->ExeName, ".log").c_str());
	else
		CLog::Instance()->SetFile("");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	AnsiString asConfigFile = ChangeFileExt( Application->ExeName, ".json" );
	appSettings.frmMain.bWindowMaximized = (this->WindowState == wsMaximized);
	if (!appSettings.frmMain.bWindowMaximized)
	{
		// these values are meaningless is wnd is maximized
		appSettings.frmMain.iWidth = this->Width;
		appSettings.frmMain.iHeight = this->Height;
		appSettings.frmMain.iPosY = this->Top;
		appSettings.frmMain.iPosX = this->Left;
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
	if (appSettings.frmMain.bAlwaysOnTop)
		this->FormStyle = fsStayOnTop;
	else
		this->FormStyle = fsNormal;
	if (appSettings.Logging.bLogToFile)
		CLog::Instance()->SetFile(ChangeFileExt(Application->ExeName, ".log").c_str());
	else
		CLog::Instance()->SetFile("");
	frmLog->SetLogLinesLimit(appSettings.Logging.iMaxUiLogLines);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
    static bool once = false;
    if (!once)
    {
		once = true;
		frmLog->SetLogLinesLimit(appSettings.Logging.iMaxUiLogLines);		
		CLog::Instance()->SetLevel(E_LOG_TRACE);
		CLog::Instance()->callbackLog = frmLog->OnLog;
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

