#include "ObjectFactory.h"
#include "Log/DebugConsole.h"

//UObject* UObjectFactory::ConstructObject(UClass* ClassType)
//{
//    if (!ClassType)
//    {
//        FDebugConsole::DebugPrint("[FObjectFactory] ERROR: Null UClass provided!");
//
//        return nullptr;
//    }
//
//    // UClass�� ������� �ν��Ͻ� ����
//    UObject* NewObject = ClassType->CreateInstance();
//    if (NewObject)
//    {
//        FDebugConsole::DebugPrint("[FObjectFactory] Created Object of Type: %s", ClassType->GetName());
// 
//    }
//
//    return NewObject;
//}
