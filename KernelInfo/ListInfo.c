#include "pch.h"
#include "ListInfo.h"
#include "CommonHeader.h"


// 输出单个 OBJECT_TYPE 结构体的内容
void ListInfo::PrintSingleObjectType(POBJECT_TYPE objectType)
{
	DbgPrint("Object Type: %wZ\n", &objectType->Name);
	DbgPrint("  Default Object: %p\n", objectType->DefaultObject);
	DbgPrint("  Index: %u\n", objectType->Index);
	DbgPrint("  Total Number of Objects: %u\n", objectType->TotalNumberOfObjects);
	DbgPrint("  Total Number of Handles: %u\n", objectType->TotalNumberOfHandles);
	DbgPrint("  High Watermark Objects: %u\n", objectType->HighWaterNumberOfObjects);
	DbgPrint("  High Watermark Handles: %u\n", objectType->HighWaterNumberOfHandles);
    // 根据需要输出更多字段
}

// 遍历 TypeList 并输出每个 OBJECT_TYPE 的内容
void ListInfo::PrintObjectTypeInfo()
{
    extern POBJECT_TYPE ObpTypeObjectType;  // 假设我们可以访问 ObpTypeObjectType，它是对象类型链表的头
    PLIST_ENTRY head = &(ObpTypeObjectType->TypeList);  // 链表头
    PLIST_ENTRY current = head->Flink;  // 指向第一个对象类型

    // 遍历 TypeList 链表
    while (current != head)
    {
        // 通过 CONTAINING_RECORD 宏获取包含 TypeList 链表节点的 OBJECT_TYPE 结构体
        POBJECT_TYPE objectType = CONTAINING_RECORD(current, OBJECT_TYPE, TypeList);

        // 锁定类型列表，防止并发修改
        ExAcquirePushLockExclusive(&objectType->TypeLock);

        // 输出该 OBJECT_TYPE 的信息
        PrintSingleObjectType(objectType);

        // 解锁类型列表
        ExReleasePushLockExclusive(&objectType->TypeLock);

        // 移动到下一个对象类型
        current = current->Flink;
    }
}
