#pragma once

#include "ComponentManager.h"

struct Message;

struct BaseSystem
{
	virtual void Init() {};
};

struct RenderSystem : public BaseSystem
{
	virtual void Init();

	CompoList<Shape> shapesList;
	
	void Render();	// Shape���� �����͸� �̿��ؼ� ȭ�鿡 ������
};