#pragma once
#include "SceneComponent.h"
#include "Primitive.h"

class UPrimitiveComponent : public USceneComponent
{
public:
	UPrimitiveComponent();
	~UPrimitiveComponent();

	static UClass* GetClass();

	UClass* GetInstanceClass() const override;
};