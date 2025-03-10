#include "Object.h"

UObject::UObject()
{    // Manager¿¡ °´Ã¼ µî·Ï
    UObjectManager::GetInstance().RegisterObject(this);
}

UObject::~UObject()
{
}

UClass* UObject::GetClass()
{
     static UClass BaseObjectClass("UObject", []() -> UObject* { return new UObject(); });
     return &BaseObjectClass;
}

void* UObject::operator new(size_t Size)
{
    void* Ptr = ::operator new(Size);
    UObject* NewObj = static_cast<UObject*>(Ptr);

    UObjectManager::GetInstance().RegisterAllocation(NewObj, Size);

    return Ptr;
}

void UObject::operator delete(void* Ptr)
{
    if (!Ptr) return;

    UObject* Obj = static_cast<UObject*>(Ptr);
    UObjectManager::GetInstance().UnregisterObject(Obj);
    UObjectManager::GetInstance().RegisterDeallocation(Obj);


    ::operator delete(Ptr);
}
