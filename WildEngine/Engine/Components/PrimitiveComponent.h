#pragma once
#include "SceneComponent.h"

class UPrimitiveComponent : public USceneComponent
{
public:
	UPrimitiveComponent();

	virtual void Render() = 0;

	FMatrix GetModelMatrix() const;
	FMatrix GetMVPMatrix() const;
	void SetMVPMatrix(const FMatrix& MVP);

protected:
	FMatrix ModelMatrix;
	FMatrix MVPMatrix;
};
