#pragma once

class UObject;

class UClass
{
public:
    // UObject�� ������ �Լ� ������ Ÿ��
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