
// KernelInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "KernelInfo.h"
#include "KernelInfoDlg.h"
#include "afxdialogex.h"
#include "CSystemHooksDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CKernelInfoDlg 对话框



CKernelInfoDlg::CKernelInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KERNELINFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKernelInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOOKS, m_HooksListCtrl);

}

BEGIN_MESSAGE_MAP(CKernelInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKernelInfoDlg 消息处理程序

BOOL CKernelInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	m_HooksListCtrl.SetDataByKernel();
	//ShowWindow(SW_MINIMIZE);

// Set Max window  ON

		// 获取当前窗口的样式
    //LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);

    //// 确保窗口可以最大化
    //lStyle |= WS_MAXIMIZEBOX | WS_THICKFRAME;

    //// 设置窗口样式
    //SetWindowLong(m_hWnd, GWL_STYLE, lStyle);

    //// 将对话框设置为最大化
    //ShowWindow(SW_SHOWMAXIMIZED);

// Set Max window  OFF


// 
	//CSystemHooksDialog dlg;
 //   dlg.DoModal();


	// 绑定 CListCtrl 控件
    //m_HooksListCtrl.SubclassDlgItem(IDC_LIST_HOOKS, this);

    //// 设置 CListCtrl 的扩展样式和列
 //   m_HooksListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		// 添加列



	//CRect rect;
 //   GetClientRect(&rect);
 //   int cx = rect.Width();
 //   int cy = rect.Height();

	//    if (m_HooksListCtrl.GetSafeHwnd())
 //   {
 //       // 调整 CListCtrl 的大小，使其占据整个对话框
 //       // 设置边距，例如：上、左为 10 像素，右、下为 10 像素
 //       int margin = 10;
 //       m_HooksListCtrl.MoveWindow(margin, margin, cx - 2 * margin, cy - 2 * margin);
 //   }
	//if (m_HooksListCtrl.GetSafeHwnd())
 //   {
 //       int margin = 10;
 //       m_HooksListCtrl.MoveWindow(margin, margin, cx - 2 * margin, cy - 2 * margin);

 //       // 动态调整列宽（这里假设所有列等宽）
 //       int colCount = m_HooksListCtrl.GetHeaderCtrl()->GetItemCount();
	//	int colWidth = 0;
	//	if (colCount)
	//		colWidth = (cx - 2 * margin) / colCount;
 //       for (int i = 0; i < colCount; ++i)
 //       {
 //           m_HooksListCtrl.SetColumnWidth(i, colWidth);
 //       }
 //   }



	



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}







void CKernelInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKernelInfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKernelInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

