#include "pch.h"
#include "CKernelInfoList.h"

CKernelInfoList::CKernelInfoList():CListCtrl()
{
}
CKernelInfoList::~CKernelInfoList()
{

}

void CKernelInfoList::AdjustColumns(UINT nType, int cx, int cy)
{
	int colCount = this->GetHeaderCtrl()->GetItemCount();

	if (this->GetSafeHwnd())
    {
        // ���� CListCtrl �Ĵ�С��ʹ��ռ�������Ի���
        // ���ñ߾࣬���磺�ϡ���Ϊ 10 ���أ��ҡ���Ϊ 10 ����
        int margin = 10;
       this->MoveWindow(margin, margin, cx - 2 * margin, cy - 2 * margin);
    }
    for (int i = 0; i < colCount; ++i)
    {
        // �ȸ������ݵ����п�
        this->SetColumnWidth(i, LVSCW_AUTOSIZE);

        // �ٴε��ã������б��������ȣ���������ȴ������ݿ�ȣ���ʹ�ñ����ȣ�
        int headerWidth = this->GetColumnWidth(i);
        this->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        int contentWidth = this->GetColumnWidth(i);

        // ���������п�Ϊ���ݿ�Ⱥͱ������еĽϴ���
        this->SetColumnWidth(i, max(headerWidth, contentWidth)+ LIST_COLOUM_WIDTH);
    }

}

void CKernelInfoList::OnSize(UINT nType, int cx, int cy)
{
    CListCtrl::OnSize(nType, cx, cy);
	// ȷ���ؼ�����

	if (this->GetSafeHwnd())
		AdjustColumns(nType,cx,cy);
}

void CKernelInfoList::SetDataByKernel()
{
	this->InsertColumn(0, _T("������"), LVCFMT_LEFT, 120);
	this->InsertColumn(1, _T("��ǰ������ַ"), LVCFMT_LEFT, 120);
	this->InsertColumn(2, _T("Hook"), LVCFMT_LEFT, 100);
	this->InsertColumn(3, _T("ԭʼ������ַ"), LVCFMT_LEFT, 120);
	this->InsertColumn(4, _T("Object����"), LVCFMT_LEFT, 120);
	this->InsertColumn(5, _T("Object��ַ"), LVCFMT_LEFT, 120);
	this->InsertColumn(6, _T("��ǰ������ַ����ģ��"), LVCFMT_LEFT, 180);

	// ��������
	this->InsertItem(0, _T("OpenProcedure"));
	this->SetItemText(0, 1, _T("0xFFFFF802EAD39100"));
	this->SetItemText(0, 2, _T("-"));
	this->SetItemText(0, 3, _T("-"));
	this->SetItemText(0, 4, _T("Composition"));
	this->SetItemText(0, 5, _T("0xFFFFF9E000309DC0"));
	this->SetItemText(0, 6, _T("C:\\Windows\\System32\\ntoskrnl.exe"));
}

BEGIN_MESSAGE_MAP(CKernelInfoList, CListCtrl)
	// �����������Ϣ��Ҫ�����������������
END_MESSAGE_MAP()
