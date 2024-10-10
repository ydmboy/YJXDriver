#include "pch.h"
#include "ListInfo.h"
#include "CommonHeader.h"


// ������� OBJECT_TYPE �ṹ�������
void ListInfo::PrintSingleObjectType(POBJECT_TYPE objectType)
{
	DbgPrint("Object Type: %wZ\n", &objectType->Name);
	DbgPrint("  Default Object: %p\n", objectType->DefaultObject);
	DbgPrint("  Index: %u\n", objectType->Index);
	DbgPrint("  Total Number of Objects: %u\n", objectType->TotalNumberOfObjects);
	DbgPrint("  Total Number of Handles: %u\n", objectType->TotalNumberOfHandles);
	DbgPrint("  High Watermark Objects: %u\n", objectType->HighWaterNumberOfObjects);
	DbgPrint("  High Watermark Handles: %u\n", objectType->HighWaterNumberOfHandles);
    // ������Ҫ��������ֶ�
}

// ���� TypeList �����ÿ�� OBJECT_TYPE ������
void ListInfo::PrintObjectTypeInfo()
{
    extern POBJECT_TYPE ObpTypeObjectType;  // �������ǿ��Է��� ObpTypeObjectType�����Ƕ������������ͷ
    PLIST_ENTRY head = &(ObpTypeObjectType->TypeList);  // ����ͷ
    PLIST_ENTRY current = head->Flink;  // ָ���һ����������

    // ���� TypeList ����
    while (current != head)
    {
        // ͨ�� CONTAINING_RECORD ���ȡ���� TypeList ����ڵ�� OBJECT_TYPE �ṹ��
        POBJECT_TYPE objectType = CONTAINING_RECORD(current, OBJECT_TYPE, TypeList);

        // ���������б���ֹ�����޸�
        ExAcquirePushLockExclusive(&objectType->TypeLock);

        // ����� OBJECT_TYPE ����Ϣ
        PrintSingleObjectType(objectType);

        // ���������б�
        ExReleasePushLockExclusive(&objectType->TypeLock);

        // �ƶ�����һ����������
        current = current->Flink;
    }
}
