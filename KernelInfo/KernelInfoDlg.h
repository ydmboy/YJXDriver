
// KernelInfoDlg.h: 头文件
//

#pragma once
#include "CKernelInfoList.h"

// CKernelInfoDlg 对话框
class CKernelInfoDlg : public CDialogEx
{
// 构造
public:
	CKernelInfoDlg(CWnd* pParent = nullptr);	// 标准构造函数
	void OnFileSave();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KERNELINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:
	CKernelInfoList m_HooksListCtrl;
	DriverComm m_dc;
	//CListCtrl m_HooksListCtrl;



};
