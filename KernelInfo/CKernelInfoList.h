#pragma once
class CKernelInfoList: public CListCtrl
{
public:
	CKernelInfoList();
	virtual ~CKernelInfoList();
	void SetDataByKernel();
	void AdjustColumns(UINT nType, int cx, int cy);
	void OnFileSave();
protected:
	void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
};

