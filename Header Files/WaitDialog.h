
#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <process.h>
#include "Utility.h"
#include <commctrl.h>



//--------------------------------------------------------------------------------------
class WaitDialog
{
private:
	HWND	m_hDialogWnd;
	HANDLE	m_hThread;
	HWND	m_hProgressWnd;
	int		m_iProgress;
	BOOL	m_bDone;
	RECT	m_AppRect;
	WCHAR	m_szText[MAX_PATH];


	HINSTANCE	m_hAppInstanceForeign;
	HWND	m_hDialogWndForeign;

public:
	WaitDialog(HINSTANCE m_hAppInstanceForeign, HWND m_hDialogWndForeign);
	~WaitDialog();

	bool IsRunning();

	void UpdateProgressBar();

	bool GetDialogControls();

	bool ShowDialog(WCHAR* pszInputText);

	void DestroyDialog();
};


#endif; //WAITDIALOG