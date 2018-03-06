#include "../Header Files/WaitDialog.h"

//--------------------------------------------------------------------------------------
INT_PTR CALLBACK WaitDialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int __stdcall WaitThread2(void* pArg);




WaitDialog::WaitDialog(HINSTANCE m_hAppInstanceForeign, HWND m_hDialogWndForeign) :
m_hDialogWnd(NULL),
m_hThread(NULL),
m_hProgressWnd(NULL),
m_iProgress(0),
m_bDone(FALSE)
{

	this->m_hAppInstanceForeign = m_hAppInstanceForeign;
	this->m_hDialogWndForeign = m_hDialogWndForeign;
}


WaitDialog::~WaitDialog() 
{ 
	DestroyDialog(); 
}


bool WaitDialog::IsRunning() 
{
	return !m_bDone; 
}

void WaitDialog::UpdateProgressBar()
{
	m_iProgress++;
	if (m_iProgress > 110)
		m_iProgress = 0;


	//SendMessage(m_hProgressWnd, PBS_SMOOTH, 1, 0);
	SendMessage(m_hProgressWnd, PBM_SETPOS, m_iProgress, 0);
	InvalidateRect(m_hDialogWnd, NULL, FALSE);
	UpdateWindow(m_hDialogWnd);
}


//--------------------------------------------------------------------------------------
bool WaitDialog::GetDialogControls()
{
	/*m_bDone = FALSE;

	m_hDialogWnd = CreateDialog(m_hAppInstanceForeign, MAKEINTRESOURCE(IDD_COMPILINGSHADERS), NULL, WaitDialogProc2);
	if (!m_hDialogWnd)
		return false;

	SetWindowLongPtr(m_hDialogWnd, GWLP_USERDATA , (LONG_PTR)this);

	// Set the position
	int left = ((m_AppRect.left + m_AppRect.right) / 2) - 150;
	int up = ((m_AppRect.top + m_AppRect.bottom) / 2) - 100;

	SetWindowPos(m_hDialogWnd, NULL, left, up, 0, 0, SWP_NOSIZE);
	ShowWindow(m_hDialogWnd, SW_SHOW);

	// Get the progress bar
	m_hProgressWnd = GetDlgItem(m_hDialogWnd, IDC_PROGRESSBAR);
	SendMessage(m_hProgressWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 32));

	// Update the static text
	HWND hMessage = GetDlgItem(m_hDialogWnd, IDC_MESSAGE);
	SetWindowText(hMessage, m_szText);
	*/
	return true;
}
//--------------------------------------------------------------------------------------
bool WaitDialog::ShowDialog(WCHAR* pszInputText)
{
	// Get the window rect
	GetWindowRect(m_hDialogWndForeign, &m_AppRect);
	wcscpy_s(m_szText, MAX_PATH, pszInputText);

	// spawn a thread that does nothing but update the progress bar
	unsigned int threadAddr;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, WaitThread2, this, 0, &threadAddr);
	return true;
}

//--------------------------------------------------------------------------------------
void WaitDialog::DestroyDialog()
{
	m_bDone = TRUE;
	WaitForSingleObject(m_hThread, INFINITE);

	if (m_hDialogWnd)
		DestroyWindow(m_hDialogWnd);
	m_hDialogWnd = NULL;
}



//--------------------------------------------------------------------------------------
INT_PTR CALLBACK WaitDialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WaitDialog* pThisDialog = (WaitDialog*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_CLOSE:
		pThisDialog->DestroyDialog();
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------
unsigned int __stdcall WaitThread2(void* pArg)
{
	WaitDialog* pThisDialog = (WaitDialog*)pArg;

	// We create the dialog in this thread, so we can call SendMessage without blocking on the
	// main thread's message pump
	pThisDialog->GetDialogControls();

	while (pThisDialog->IsRunning())
	{
		pThisDialog->UpdateProgressBar();
		Sleep(100);
	}

	return 0;
}