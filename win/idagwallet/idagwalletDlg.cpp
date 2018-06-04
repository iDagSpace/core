// cheatcoinwalletDlg.cpp : implementation file
//

#include "stdafx.h"
#include "idagwallet.h"
#include "idagwalletDlg.h"
#include "afxdialogex.h"
#include "PasswordDlg.h"
#include "../../client/init.h"
#include "../../client/commands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CidagWalletDlg *g_dlg = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CidagWalletDlg dialog

CidagWalletDlg::CidagWalletDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_idagWALLET_DIALOG, pParent)
	, _poolAddress(_T(""))
	, _miningThreadsCount(0)
	, _balance(_T(""))
	, _accountAddress(_T(""))
	, _transferAmount(_T(""))
	, _transferAddress(_T(""))
	, _state(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CidagWalletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_APPLY, _applyButton);
	DDX_Text(pDX, IDC_EDIT_POOL_ADDRESS, _poolAddress);
	DDV_MaxChars(pDX, _poolAddress, 64);
	DDX_Text(pDX, IDC_EDIT_MINING_THREADS, _miningThreadsCount);
	DDV_MinMaxInt(pDX, _miningThreadsCount, 0, 999);
	DDX_Control(pDX, IDC_EDIT_HASHRATE, _hashRateEdit);
	DDX_Control(pDX, IDC_EDIT_BALANCE, _balanceEdit);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT_ADDRESS, _accountAddressEdit);
	DDX_Text(pDX, IDC_EDIT_TRANSFER_AMOUNT, _transferAmount);
	DDV_MaxChars(pDX, _transferAmount, 32);
	DDX_Text(pDX, IDC_EDIT_TRANSFER_TO, _transferAddress);
	DDV_MaxChars(pDX, _transferAddress, 64);
	DDX_Control(pDX, IDC_STATIC_STATE, _stateControl);
	DDX_Control(pDX, IDC_EDIT_TRANSFER_AMOUNT, _transferAmountEdit);
	DDX_Control(pDX, IDC_EDIT_TRANSFER_TO, _transferAddressEdit);
	DDX_Control(pDX, IDC_BUTTON_XFER, _xferButton);
}

BEGIN_MESSAGE_MAP(CidagWalletDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CidagWalletDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_XFER, &CidagWalletDlg::OnClickedButtonXfer)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CidagWalletDlg::OnBnClickedButtonApply)
	ON_MESSAGE(WM_UPDATE_STATE, &CidagWalletDlg::OnUpdateState)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CidagWalletDlg message handlers

BOOL CidagWalletDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	_applyButton.EnableWindow(FALSE);
	_transferAmountEdit.EnableWindow(FALSE);
	_transferAddressEdit.EnableWindow(FALSE);
	_xferButton.EnableWindow(FALSE);

	_poolAddress = AfxGetApp()->GetProfileString(_T("Settings"), _T("PoolAddress"));
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CidagWalletDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CidagWalletDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CidagWalletDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CidagWalletDlg::InputPassword(const char *prompt, char *buf, unsigned size)
{
	CPasswordDlg dlgPwd;
	dlgPwd.SetPasswordPromt(CString(prompt));
	dlgPwd.DoModal();
	strncpy(buf, (char*)(LPCTSTR)dlgPwd.GetPassword(), size);
	return 0;
}

int CidagWalletDlg::ShowState(const char *state, const char *balance, const char *address)
{
	g_dlg->SetBalance(balance);
	g_dlg->SetAccountAddress(address);
	g_dlg->SetState(state);
	g_dlg->PostMessage(WM_UPDATE_STATE);
	return 0;
}

LRESULT CidagWalletDlg::OnUpdateState(WPARAM wParam, LPARAM lParam)
{
	CString currentValue;
	_balanceEdit.GetWindowText(currentValue);
	if (currentValue != _balance) {
		_balanceEdit.SetWindowText(_balance);
	}
	_accountAddressEdit.GetWindowText(currentValue);
	if (currentValue != _accountAddress) {
		_accountAddressEdit.SetWindowText(_accountAddress);
	}
	_stateControl.SetWindowText(_state);
	return 0;
}

void CidagWalletDlg::OnClickedButtonConnect()
{
	_accountAddressEdit.SendMessageA(WM_HIDE_TOOLTIP);

	UpdateData(true);
	if (_poolAddress.IsEmpty()) {
		MessageBox("Pool address must be set", "Dagger wallet", MB_OK | MB_ICONSTOP);
		return;
	}
	char buf[10];
	char *argv[] = { "idag.exe", "-m", _itoa(_miningThreadsCount, buf, 10), (char*)(LPCTSTR)_poolAddress };
	idag_set_password_callback(&InputPassword);
	g_idag_show_state = &ShowState;
	idag_init(4, argv, 1);

	_applyButton.EnableWindow(TRUE);
	_transferAmountEdit.EnableWindow(TRUE);
	_transferAddressEdit.EnableWindow(TRUE);
	_xferButton.EnableWindow(TRUE);
	SetTimer(ID_TIMER_HASHRATE, 5000, NULL);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("PoolAddress"), _poolAddress);
}

void CidagWalletDlg::OnClickedButtonXfer()
{
	UpdateData(TRUE);
	if (_transferAmount.IsEmpty()) {
		MessageBox("Transfer amount must be set", "Dagger wallet", MB_OK | MB_ICONSTOP);
		return;
	}
	if (_transferAddress.IsEmpty()) {
		MessageBox("Transfer address must be set", "Dagger wallet", MB_OK | MB_ICONSTOP);
		return;
	}
	idag_do_xfer(0, (char*)(LPCTSTR)_transferAmount, (char*)(LPCTSTR)_transferAddress, 1);
}

void CidagWalletDlg::OnBnClickedButtonApply()
{
	UpdateData(TRUE);
	idagSetCountMiningTread(_miningThreadsCount);
}

void CidagWalletDlg::OnTimer(WPARAM wParam)
{
	const double hashRate = idagGetHashRate();
	CString hashRateStr;
	hashRateStr.Format("%.2lf", hashRate);
	_hashRateEdit.SetWindowText(hashRateStr);
}

LRESULT CidagWalletDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_NCLBUTTONDOWN:
		_accountAddressEdit.PostMessageA(WM_HIDE_TOOLTIP);
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
