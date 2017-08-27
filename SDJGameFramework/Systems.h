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
	
	void Render();	// Shape���� �����͸� �̿��ؼ� ȭ�鿡 ������
};

struct GameLogic : public BaseSystem
{
	virtual void Init();
	void Update(double time);

	CompoList<LuaComponent> scriptList;
};