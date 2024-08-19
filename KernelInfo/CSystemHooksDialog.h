#pragma once
#include "afxdialogex.h"



class CSystemHooksDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CSystemHooksDialog)

public:
    CSystemHooksDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CSystemHooksDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SYSTEM_HOOKS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	void OnSize(UINT nType, int cx, int cy);

    CListCtrl m_HooksListCtrl;
};

