#pragma once
#include "afxdialogex.h"



class CSystemHooksDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CSystemHooksDialog)

public:
    CSystemHooksDialog(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CSystemHooksDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SYSTEM_HOOKS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	void OnSize(UINT nType, int cx, int cy);

    CListCtrl m_HooksListCtrl;
};

