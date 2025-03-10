#pragma once

class UObject;

class UClass
{
public:
    // UObject를 생성할 함수 포인터 타입
    using ConstructorType = UObject * (*)();

    UClass(const char* InName, ConstructorType InConstructor)
        : Name(InName), Constructor(InConstructor) {
    }

    const char* GetName() const { return Name; }
    UObject* CreateInstance() const { return Constructor ? Constructor() : nullptr; }



private:
    const char* Name;
    ConstructorType Constructor;
};