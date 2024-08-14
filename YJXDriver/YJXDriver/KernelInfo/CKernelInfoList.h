#pragma once
class CKernelInfoList: public CListCtrl
{
public:
	CKernelInfoList();
	virtual ~CKernelInfoList();
	void SetDataByKernel();
protected:
	void OnSize(UINT nType, int cx, int cy);
	void AdjustColumns();
    DECLARE_MESSAGE_MAP()
};

