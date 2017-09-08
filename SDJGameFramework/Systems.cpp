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
		switch (s.shapeType)
		{
			case Shape::CUBE:
				glutWireCube(s.cube.size);
				break;
			case Shape::SPHERE:
				glutWireSphere(s.sphere.radius, s.sphere.slice, s.sphere.stack);
				break;
			case Shape::CONE:
				glutWireCone(s.cone.base, s.cone.height, s.cone.slice, s.cone.stack);
				break;
			case Shape::TORUS:
				glutWireTorus(s.torus.innerRadius, s.torus.outerRadius, s.torus.side, s.torus.ring);
				break;
			case Shape::TEAPOT:
				glutWireTeapot(s.teapot.size);
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
		sol::table instance = lua["components"][uint64_t(c.handle)];
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
