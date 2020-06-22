//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <StdActns.hpp>

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *miFile;
	TMenuItem *miSettings;
	TMenuItem *miHelp;
	TMenuItem *miAbout;
	TMenuItem *miCommonSettings;
	TActionList *ActionList;
	TImageList *imglistActions;
	TAction *actShowAbout;
	TAction *actShowSettings;
	TStatusBar *StatusBar;
	TMenuItem *miView;
	TMenuItem *miViewLog;
	TAction *actShowLog;
	TFileExit *FileExit;
	TMenuItem *Exit1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall actShowAboutExecute(TObject *Sender);
	void __fastcall actShowSettingsExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall actShowLogExecute(TObject *Sender);
private:	// User declarations

public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
