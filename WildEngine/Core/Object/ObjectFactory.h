#pragma once
#include "Object.h"
#include <unordered_map>
#include <functional>
#include "Interface/ISingleton.h"

class UObjectFactory : public ISingleton<UObjectFactory>
{
public:

    // UClass ��� ��� (���� ���� �⺻ ������)
    template <typename T>
    void RegisterClass()
    {
        ClassMap[T::GetClass()] = []() -> UObject* { return new T(); };
    }

    // UClass ��� ��� (���ڸ� �޴� ������ ����)
    template <typename T, typename... Args>
    void RegisterClassWithArgs()
    {
        ClassMapWithArgs[T::GetClass()] = [](void* ArgsPtr) -> UObject*
            {
                auto ArgsTuple = static_cast<std::tuple<Args...>*>(ArgsPtr);
                return CreateWithArgs<T>(*ArgsTuple);
            };
    }

    // ���ø� ������� ��ȯ Ÿ���� �����Ͽ� ������ ĳ���� ����
    template <typename T>
    T* ConstructObject(UClass* ClassType)
    {
        auto it = ClassMap.find(ClassType);
        if (it != ClassMap.end())
        {
            return static_cast<T*>(it->second());
        }
        return nullptr;
    }

    // ���ڰ� �ִ� ��ü ���� (���ø� ����)
    template <typename T, typename... Args>
    T* ConstructObject(UClass* ClassType, Args... args)
    {
        auto it = ClassMapWithArgs.find(ClassType);
        if (it != ClassMapWithArgs.end())
        {
            std::tuple<Args...> ArgsTuple(args...);
            return static_cast<T*>(it->second(&ArgsTuple));
        }
        return nullptr;
    }

private:
    using CreateFunc = std::function<UObject* ()>;
    using CreateFuncWithArgs = std::function<UObject* (void*)>;

    TMap<UClass*, CreateFunc> ClassMap;
    TMap<UClass*, CreateFuncWithArgs> ClassMapWithArgs;

    // ���� ���� ���� (tuple Ȱ��)
    template <typename T, typename... Args>
    static T* CreateWithArgs(std::tuple<Args...>& args)
    {
        return CreateHelper<T>(args, std::index_sequence_for<Args...>{});
    }

    template <typename T, typename Tuple, size_t... I>
    static T* CreateHelper(Tuple& tuple, std::index_sequence<I...>)
    {
        return new T(std::get<I>(tuple)...);
    }
};