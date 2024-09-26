#include <ntddk.h>
#include <wdm.h>

void DriverUnload(PDRIVER_OBJECT pdo)
{

}


RTL_AVL_TABLE avlTable;
RTL_GENERIC_COMPARE_RESULTS NTAPI Avlcompare(PRTL_AVL_TABLE tree,PVOID fs,PVOID ss)
{

	int infs = *(int*)fs;
	int inss = *(int*)ss;
	
	return (infs < inss) ? GenericLessThan : (infs > inss) ? GenericGreaterThan : GenericEqual;

}
PVOID NTAPI AvlAllocate(PRTL_AVL_TABLE Table,CLONG ByteSize)
{
	UNREFERENCED_PARAMETER(Table);
	return ExAllocatePoolWithTag(NonPagedPool, ByteSize, 'Av1T');
}
PVOID NTAPI AvlFree(PRTL_AVL_TABLE Table, PVOID buffer)
{
	UNREFERENCED_PARAMETER(Table);
	ExFreePoolWithTag(buffer, 'AvlT');
}
BOOLEAN InsertElement(int value)
{
    BOOLEAN newElement = FALSE;
    RtlInsertElementGenericTableAvl(&avlTable, &value, sizeof(int), &newElement);
    return newElement;
}
void TraverseAvlTree()
{
	PVOID restartKey = NULL;
	PVOID element = RtlEnumerateGenericTableWithoutSplayingAvl(&avlTable, &restartKey);
	while (element != NULL)
	{
		int value = *(int*)element;
		DbgPrint("Avl Tree Element:%d\n",value);
		element = RtlEnumerateGenericTableWithoutSplayingAvl(&avlTable, &restartKey);
	}
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pobj, PUNICODE_STRING rgs)
{

	//PROCESS_CONTEXT asd;
	NTSTATUS nt = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(rgs);
	pobj->DriverUnload = DriverUnload;
	RtlInitializeGenericTableAvl(&avlTable, Avlcompare, AvlAllocate, AvlFree, NULL);

	  // 插入元素
	if (InsertElement(10))
		DbgPrint("Insert successfully\n");
	else
		DbgPrint("Insert failed\n");
	if(InsertElement(10))
		DbgPrint("Insert successfully");
	else
		DbgPrint("Insert failed\n");
    if(InsertElement(20))
		DbgPrint("Insert successfully");
	else
		DbgPrint("Insert failed\n");
    if(InsertElement(15))
		DbgPrint("Insert successfully");
	else
		DbgPrint("Insert failed\n");


    // 遍历并打印 AVL 树
    TraverseAvlTree();

    // 返回成功状态
    return STATUS_SUCCESS;

}