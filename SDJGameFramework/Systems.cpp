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
	glLoadIdentity();
	for (auto& s : shapesList)
	{
		glPushMatrix();
		Object* ancester = OM.Get(s.owner);
		float mat[16];
		while (ancester)
		{
			glGetFloatv(GL_MODELVIEW_MATRIX, mat);
			glLoadIdentity();
			glTranslatef(ancester->position.x, ancester->position.y, ancester->position.z);
			glMultMatrixf(mat);
			ancester = OM.Get(ancester->parent);
		}

		glColor3fv(s.color);
		switch (s.type)
		{
			case Shape::Type::CUBE:
				glutWireCube(s.cubeSize);
				break;
			case Shape::Type::SPHERE:
				glutWireSphere(s.sphereRadius, s.sphereSlice, s.sphereStack);
				break;
			case Shape::Type::CONE:
				glutWireCone(s.coneBase, s.coneHeight, s.coneSlice, s.coneStack);
				break;
			case Shape::Type::TORUS:
				glutWireTorus(s.torusInnerRadius, s.torusOuterRadius, s.torusSide, s.torusRing);
				break;
			case Shape::Type::TEAPOT:
				glutWireTeapot(s.teapotSize);
				break;
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
		sol::protected_function fn = c.env["Update"];
		if (fn)
		{
			auto ret = fn(time);
			if (!ret.valid())
				perror(ret.get<string>().c_str());
		}
	}
}
