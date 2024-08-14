#include "pch.h"
#include "CSystemHooksDialog.h"
#include "Resource.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CSystemHooksDialog, CDialogEx)

CSystemHooksDialog::CSystemHooksDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SYSTEM_HOOKS_DIALOG, pParent)
{
}

CSystemHooksDialog::~CSystemHooksDialog()
{
}

void CSystemHooksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOOKS, m_HooksListCtrl);
}

BEGIN_MESSAGE_MAP(CSystemHooksDialog, CDialogEx)
	//ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CSystemHooksDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();




	// 初始化List Control
	m_HooksListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 添加列
	m_HooksListCtrl.InsertColumn(0, _T("函数名"), LVCFMT_LEFT, 120);
	m_HooksListCtrl.InsertColumn(1, _T("当前函数地址"), LVCFMT_LEFT, 120);
	m_HooksListCtrl.InsertColumn(2, _T("Hook"), LVCFMT_LEFT, 100);
	m_HooksListCtrl.InsertColumn(3, _T("原始函数地址"), LVCFMT_LEFT, 120);
	m_HooksListCtrl.InsertColumn(4, _T("Object类型"), LVCFMT_LEFT, 120);
	m_HooksListCtrl.InsertColumn(5, _T("Object地址"), LVCFMT_LEFT, 120);
	m_HooksListCtrl.InsertColumn(6, _T("当前函数地址所在模块"), LVCFMT_LEFT, 180);

	// 插入数据
	m_HooksListCtrl.InsertItem(0, _T("OpenProcedure"));
	m_HooksListCtrl.SetItemText(0, 1, _T("0xFFFFF802EAD39100"));
	m_HooksListCtrl.SetItemText(0, 2, _T("-"));
	m_HooksListCtrl.SetItemText(0, 3, _T("-"));
	m_HooksListCtrl.SetItemText(0, 4, _T("Composition"));
	m_HooksListCtrl.SetItemText(0, 5, _T("0xFFFFF9E000309DC0"));
	m_HooksListCtrl.SetItemText(0, 6, _T("C:\\Windows\\System32\\ntoskrnl.exe"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSystemHooksDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_HooksListCtrl.GetSafeHwnd())
    {
        m_HooksListCtrl.MoveWindow(10, 10, cx - 20, cy - 20);
    }
}
