#include "ObjectManager.h"

void UObjectManager::RegisterObject(UObject* Object)
{
	if (!Object) return;

	Object->UUID = UEngineStatics::GenUUID(); // Manager���� UUID ����
	Object->InternalIndex = GUObjectArray.size(); // ���� �迭 ũ�⸦ ������� Index �Ҵ�

	GUObjectArray.push_back(Object);

	FDebugConsole::DebugPrint("[UObjectManager] Registered: UUID = %d, InternalIndex = %d", Object->UUID, Object->InternalIndex);
}

void UObjectManager::UnregisterObject(UObject* Object)
{
	if (!Object) return;

	auto it = std::find(GUObjectArray.begin(), GUObjectArray.end(), Object);
	if (it != GUObjectArray.end())
	{
		GUObjectArray.erase(it);
		FDebugConsole::DebugPrint("[UObjectManager] Unregistered: UUID = %d", Object->UUID);
	}
}

// Heap �޸� �Ҵ� ����
void UObjectManager::RegisterAllocation(UObject* Object, size_t Size)
{
	AllocationMap[Object] = Size;
	TotalAllocationBytes += Size;
	TotalAllocationCount++;

	FDebugConsole::DebugPrint("[UObjectManager] Allocated: %d bytes. Total Memory: %d bytes. Total Object Count: %d", Size, TotalAllocationBytes, TotalAllocationCount);
}

//  Heap �޸� ���� ����
void UObjectManager::RegisterDeallocation(UObject* Object)
{
	auto it = AllocationMap.find(Object);
	if (it != AllocationMap.end())
	{
		TotalAllocationBytes -= it->second;
		TotalAllocationCount--;
		AllocationMap.erase(it);

		FDebugConsole::DebugPrint("[UObjectManager] Total Memory: %d bytes. Total Object Count: %d", TotalAllocationBytes, TotalAllocationCount);
	}
}

void UObjectManager::PrintMemoryUsage()
{
	FDebugConsole::DebugPrint("[UObjectManager] Total Allocated Objects: %d", TotalAllocationCount);
	FDebugConsole::DebugPrint("[UObjectManager] Total Memory Used :: %d", TotalAllocationBytes);
}
