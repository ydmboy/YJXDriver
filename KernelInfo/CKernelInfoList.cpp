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
        // 调整 CListCtrl 的大小，使其占据整个对话框
        // 设置边距，例如：上、左为 10 像素，右、下为 10 像素
        int margin = 10;
       this->MoveWindow(margin, margin, cx - 2 * margin, cy - 2 * margin);
    }
    for (int i = 0; i < colCount; ++i)
    {
        // 先根据内容调整列宽
        this->SetColumnWidth(i, LVSCW_AUTOSIZE);

        // 再次调用，根据列标题调整宽度（如果标题宽度大于内容宽度，则使用标题宽度）
        int headerWidth = this->GetColumnWidth(i);
        this->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        int contentWidth = this->GetColumnWidth(i);

        // 设置最终列宽为内容宽度和标题宽度中的较大者
        this->SetColumnWidth(i, max(headerWidth, contentWidth)+ LIST_COLOUM_WIDTH);
    }

}

void CKernelInfoList::OnSize(UINT nType, int cx, int cy)
{
    CListCtrl::OnSize(nType, cx, cy);
	// 确保控件存在

	if (this->GetSafeHwnd())
		AdjustColumns(nType,cx,cy);
}

void CKernelInfoList::SetDataByKernel()
{
	this->InsertColumn(0, _T("函数名"), LVCFMT_LEFT, 120);
	this->InsertColumn(1, _T("当前函数地址"), LVCFMT_LEFT, 120);
	this->InsertColumn(2, _T("Hook"), LVCFMT_LEFT, 100);
	this->InsertColumn(3, _T("原始函数地址"), LVCFMT_LEFT, 120);
	this->InsertColumn(4, _T("Object类型"), LVCFMT_LEFT, 120);
	this->InsertColumn(5, _T("Object地址"), LVCFMT_LEFT, 120);
	this->InsertColumn(6, _T("当前函数地址所在模块"), LVCFMT_LEFT, 180);

	// 插入数据
	this->InsertItem(0, _T("OpenProcedure"));
	this->SetItemText(0, 1, _T("0xFFFFF802EAD39100"));
	this->SetItemText(0, 2, _T("-"));
	this->SetItemText(0, 3, _T("-"));
	this->SetItemText(0, 4, _T("Composition"));
	this->SetItemText(0, 5, _T("0xFFFFF9E000309DC0"));
	this->SetItemText(0, 6, _T("C:\\Windows\\System32\\ntoskrnl.exe"));
}

BEGIN_MESSAGE_MAP(CKernelInfoList, CListCtrl)
	// 如果有其他消息需要处理，可以在这里添加
END_MESSAGE_MAP()
