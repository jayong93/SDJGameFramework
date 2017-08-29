#include "stdafx.h"
#include "Systems.h"
#include "HandleManagers.h"
#include "Framework.h"

using namespace std;

void RenderSystem::Init()
{
	CM.RegisterComponentList(this->shapesList);
}

void RenderSystem::Render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (auto& s : shapesList)
	{
		Object* owner = OM.Get(s.owner);
		glPushMatrix();
		{
			glTranslatef(owner->position.x, owner->position.y, owner->position.z);
			switch (s.shapeType)
			{
				case Shape::CUBE:
					glutWireCube(s.drawParam[0]);
					break;
				case Shape::SPHERE:
					glutWireSphere(s.drawParam[0], s.drawParam[1], s.drawParam[2]);
					break;
				case Shape::CONE:
					glutWireCone(s.drawParam[0], s.drawParam[1], s.drawParam[2], s.drawParam[3]);
					break;
				case Shape::TORUS:
					glutWireTorus(s.drawParam[0], s.drawParam[1], s.drawParam[2], s.drawParam[3]);
					break;
				case Shape::TEAPOT:
					glutWireTeapot(s.drawParam[0]);
					break;
			}
		}
		glPopMatrix();
	}
	glutSwapBuffers();
}

void GameLogic::Init()
{
	CM.RegisterComponentList(this->scriptList);
}

void GameLogic::Update(double time)
{
	auto& lua = FW.lua;
	for (auto& c : this->scriptList)
	{
		sol::table instance = lua["Component"]["instance"][uint64_t(c.handle)];
		if (instance)
		{
			sol::protected_function fn = instance["Update"];
			if (fn)
			{
				auto ret = fn(time);
				if (!ret.valid())
					perror(ret.get<string>().c_str());
			}
		}
	}
}
