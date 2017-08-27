#pragma once

#include "ComponentManager.h"

struct BaseSystem
{
	virtual void Init() {};
};

struct RenderSystem : public BaseSystem
{
	virtual void Init();

	CompoList<Shape> shapesList;
	
	void Render();	// Shape들의 데이터를 이용해서 화면에 렌더링
};